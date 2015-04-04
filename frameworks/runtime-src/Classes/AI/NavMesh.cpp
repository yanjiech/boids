#include "NavMesh.h"
#include "Constants.h"
#include "Utils.h"
#include "Trace.h"
#include <memory>
#include <boost/heap/binomial_heap.hpp>
#include "Border.h"
//#include "AI.h"
#include "MapDebugger.h"
#include "BoidsPolygon.h"
#include "../constant/DebugMacro.h"

//待改进项
//* 出生重叠的话，互相没法推开，别人也穿不过去

typedef std::pair<cocos2d::Point, cocos2d::Point> PointPair;
PointPair makePointPair(cocos2d::Point a, cocos2d::Point b)
{
	return a < b ? std::make_pair(a, b) : std::make_pair(b, a);
}

void NavMesh::mergeAndBuildGraph(bool merge) //调试的时候可能会传false
{
	std::set<BoidsPolygonEx*> polygons_to_merge;

	//先把polygons都填到set里。主要是为了标记哪些没被删除。合并和建图过程中不从这个set遍历
	for (unsigned int i = 0; i != polygons.size(); ++i)
	{
		polygons_to_merge.insert(polygons[i]);
	}

	std::map<PointPair, PolygonBorder> borders;

	//扫所有边，对于被共用的边，看看能否合并。不能的话就建立一个portal
	for (unsigned int i = 0; i != polygons.size(); ++i)
	{
		BoidsPolygonEx* current_poly = polygons[i];
		for (PolygonPointPtr pt0 = current_poly->tail, pta = current_poly->ap, ptb = pta->next;;)
		{
			PointPair pp = makePointPair(pta->d, ptb->d);
			auto it_border = borders.find(pp);
			if (it_border == borders.end()) //没找到已经存在的边，那么先把自己存进去
			{
				borders[pp] = PolygonBorder(pt0, pta, ptb, current_poly);
			}
			else
			{
				//已经有边了，那么检查是否能合并
				PolygonBorder& old_border = it_border->second;
				BoidsPolygonEx* other_poly = old_border.belong_to;
				if (merge &&
					Fuzzy::_equalGreater(cocos2d::Vec2(pt0->d, pta->d).cross(cocos2d::Vec2(old_border.ptb->d, old_border.ptb->next->d)), 0.0f) &&
					Fuzzy::_equalGreater(cocos2d::Vec2(old_border.pt0->d, old_border.pta->d).cross(cocos2d::Vec2(ptb->d, ptb->next->d)), 0.0f))
				{
					//可以合并
					PolygonPointPtr new_edge_0 = pt0, new_edge_a = pta;
					for (PolygonPointPtr to_insert = old_border.ptb->next;;)
					{
						PolygonPointPtr new_edge_b = current_poly->insert(to_insert->d, new_edge_a);
						borders[makePointPair(new_edge_a->d, new_edge_b->d)] = PolygonBorder(new_edge_0, new_edge_a, new_edge_b, current_poly); //边插到新polygon后相关值都变了，要更换borders中存的border
						new_edge_0 = new_edge_a;
						new_edge_a = new_edge_b; //这个时候，(new_edge_0, new_edge_a)是刚更新的那条边，new_edge_a是刚插入的那个点
						to_insert = to_insert->next;
						if (to_insert == old_border.pta) break;
					}
					//刚结束循环时，还差(new_edge_a, new_edge_a的next(即ptb))没更新。插入了other_poly->size() - 2个点，但得更新other_poly->size() - 1条边
					borders[makePointPair(new_edge_a->d, new_edge_a->next->d)] = PolygonBorder(new_edge_0, new_edge_a, new_edge_a->next, current_poly);

					//顺便把新的pt0赋好值：循环结束时pt0和pta还要往前挪动的，所以他现在是new_edge_0，挪动之后他是最后一个被插入的点(new_edge_a)，pta就会是现在的ptb，正好
					pt0 = new_edge_0;					
					
					//将已有的portal移交
					for (unsigned int j = 0; j != other_poly->portals.size(); ++j)
					{
						current_poly->portals.push_back(other_poly->portals[j]);

						//该portal对面的polygon的portals数组也要改
						BoidsPolygonEx* portal_dest = other_poly->portals[j].to;

						bool find_corresponding_portal = false;
						for (auto& portal_of_dest : portal_dest->portals)
						{
							if (portal_of_dest.to == other_poly)
							{
								portal_of_dest.to = current_poly;
								find_corresponding_portal = true;
							}
						}
						assert(find_corresponding_portal);
					}

					//从polygons_to_merge里移除并释放被合并的polygon
					polygons_to_merge.erase(other_poly);
					delete other_poly;

					borders.erase(it_border); //去掉map里的border，合并完成，这个border彻底不需要了
				}
				else
				{
					//不能合并，那么建portal
					PolygonPortal portal1;
					portal1.a = pta->d;
					portal1.b = ptb->d;
					portal1.to = other_poly;
					current_poly->portals.push_back(portal1);

					PolygonPortal portal2; //portal中存的a,b点是带顺序的，portal2的时候要反过来
					portal2.a = ptb->d;
					portal2.b = pta->d;
					portal2.to = current_poly;
					other_poly->portals.push_back(portal2);
				}
			}
			pt0 = pt0->next;
			pta = pt0->next;
			if (pta == current_poly->ap) break; //之所以要拿pta来判，是因为pt0是会被改变的。pta虽然重新赋值了，但其实指向的点是接下来正应该遍历的那个店。同样tail也可能会改变，而ap不会被改变
			ptb = pta->next;
		}
	}

	polygons.clear();
	for (auto poly : polygons_to_merge)
	{
		//portal的from之前都是不赋值的，因为赋了也可能改。到最后再赋
		for (unsigned int i = 0; i != poly->portals.size(); ++i)
		{
			poly->portals[i].from = poly;
		}
		polygons.push_back(poly);
	}
}

void NavMesh::createRTree()
{
	for (unsigned i = 0; i < polygons.size(); ++i)
	{
		// calculate polygon bounding box, insert new value
		rtree.insert(std::make_pair(polygons[i]->getBoundingBox(), polygons[i]));
	}
}

void NavMesh::insertBorders(polygon::ring_type& ring, FILE* serializeTo)
{
	uint32_t ring_size = ring.size();
	assert(ring_size > 1);
	if (serializeTo)
	{
		fwrite(&ring_size, 4, 1, serializeTo);
		for (unsigned i = 0; i != ring_size; ++i)
		{
			Utils::serializeToBinary(serializeTo, Utils::toCocosPt(ring[i]));
		}
	}
	for (unsigned i = 0; i != ring_size; ++i)
	{
		Border* b = new Border(Utils::toCocosPt(ring[i]), Utils::toCocosPt(ring[(i + 1) % ring_size]), Utils::toCocosPt(ring[(i + 2) % ring_size]));
		borders.push_back(b);
		rtree_borders.insert(std::make_pair(b->getBoostSeg(), b));
	}
}

NavMesh::NavMesh(BoidsPolygon& movable, const std::vector<BoidsPolygon*>& barriers, float unit_collide_radius, FILE* serializeTo)
{
	Trace t;

	releaseData(); //万一有之前parse失败的残余，得先清空一下

	_unit_collide_radius = unit_collide_radius;

	if (serializeTo)
	{
		fwrite(&_unit_collide_radius, sizeof(_unit_collide_radius), 1, serializeTo);
	}

	//根据半径扩充障碍物
	std::vector<BoidsPolygon*> new_barriers;
	for (unsigned int i = 0; i < barriers.size(); ++i)
	{
		new_barriers.push_back(barriers[i]->getEnlargedPolygon(unit_collide_radius));
	}
	t.trace("NavMesh_enlarge");
	
	//去掉障碍物
	std::vector<polygon> new_movables;
	new_movables.push_back(polygon());
	initializeBoostPolygon(movable, new_movables.back());
	for (unsigned int i = 0; i < new_barriers.size(); ++i)
	{
		polygon to_minus;
		initializeBoostPolygon(*new_barriers[i], to_minus);

		std::vector<polygon> output;
		for (auto it_movable = new_movables.begin(); it_movable != new_movables.end(); ++it_movable)
		{
			boost::geometry::difference(*it_movable, to_minus, output);
		}
		new_movables = output;
	}	
	t.trace("NavMesh_difference");
//	log("movables count after difference: %d", new_movables.size());
	assert(new_movables.size());

	//如果要存二进制的话，得先存下有多少个ring
	if (serializeTo)
	{
		uint32_t total_rings = 0;
		for (unsigned int i = 0; i != new_movables.size(); ++i)
		{
			total_rings += new_movables[i].inners().size() + 1;
		}
		fwrite(&total_rings, 4, 1, serializeTo);
	}

	t.beginAccumulate();

	rtree_borders.clear(); //万一有之前parse失败的残余，得先清空一下
	std::vector<BoidsPolygonEx*> polygons_no_inner_ring;
	for (unsigned int i = 0; i != new_movables.size(); ++i)
	{
		insertBorders(new_movables[i].outer(), serializeTo);
		t.accumulate("rtree_borders");

		BoidsPolygonEx* outer_ring = new BoidsPolygonEx(new_movables[i].outer());

		//处理还在movable中心的障碍物(inner_ring)
		auto& boost_inner_rings = new_movables[i].inners();
		if (boost_inner_rings.size())
		{
			std::vector<BoidsPolygonEx*> inner_rings;
			for (unsigned int j = 0; j != boost_inner_rings.size(); ++j)
			{
				insertBorders(boost_inner_rings[j], serializeTo);

				BoidsPolygonEx* inner_ring = new BoidsPolygonEx(boost_inner_rings[j]);
				inner_rings.push_back(inner_ring);
			}

			t.accumulate("rtree_borders");
			outer_ring->processInnerRings(inner_rings);
			t.accumulate("inner_rings");
		}
		polygons_no_inner_ring.push_back(outer_ring);
	}
	t.printStatistics();
	t.trace("NavMesh_processInnerRings_and_create_RTree_borders");

	//debug, show process_inner_rings_result without convexification
	//for (auto p : polygons_no_inner_ring)
	//{
	//	polygons.push_back(p);
	//	p->foreach([](const cocos2d::Point& pt) {
	//		map_dbg.addPoint(pt);
	//	});
	//}
	//return;

	extern int debug_connection_type[3];
	log("shapeConnection type count: %d %d %d", debug_connection_type[0], debug_connection_type[1], debug_connection_type[2]);
	
	for (unsigned int i = 0; i != polygons_no_inner_ring.size(); ++i)
	{
		polygons_no_inner_ring[i]->convexification(polygons);
	}
	t.trace("NavMesh_convexification");
	log("polygons count after convexification: %d", polygons.size());
	assert(polygons.size());

	mergeAndBuildGraph();
	t.trace("NavMesh_mergeAndBuildGraph");
	log("polygons count after merge: %d", polygons.size());

	if (serializeTo)
	{
		uint32_t polygons_count = polygons.size();
		fwrite(&polygons_count, 4, 1, serializeTo);

		for (unsigned i = 0; i != polygons_count; ++i)
		{
			polygons[i]->serializeToBinary(serializeTo);
		}

		t.trace("NavMesh_savePolygons");
	}

	createRTree();
	t.trace("NavMesh_createRTree");
}

void NavMesh::releaseData()
{
	for (auto p : polygons)
	{
		delete p;
	}
	polygons.clear();

	for (auto b : borders)
	{
		delete b;
	}
	borders.clear();

	for (auto name_and_vector : additional_borders)
	{
		for (auto b : *name_and_vector.second)
		{
			delete b;
		}
		delete name_and_vector.second;
	}
	additional_borders.clear();
}

NavMesh::~NavMesh()
{
	releaseData();
}

void NavMesh::addCollidablePolygon(BoidsPolygon& poly)
{
	auto it_borders_list = additional_borders.find(poly.name);
	if (it_borders_list == additional_borders.end())
	{
		it_borders_list = additional_borders.insert(std::make_pair(poly.name, new std::vector<Border*>())).first;
	}
	std::vector<Border*>& borders_list = *it_borders_list->second;

	poly.foreachelement([&](PolygonPointPtr ptr) {
		Border* b = new Border(ptr->d, ptr->next->d, ptr->next->next->d);
		borders_list.push_back(b);
		rtree_borders.insert(std::make_pair(b->getBoostSeg(), b));
	});
}

bool NavMesh::removeCollidablePolygon(const std::string& name)
{
	auto it_borders_list = additional_borders.find(name);
	if (it_borders_list == additional_borders.end())
	{
		Utils::logAI("try to remove polygon %s but not found", name.c_str());
		return false;
	}

	std::vector<Border*>& borders_list = *it_borders_list->second;
	for (auto b : borders_list)
	{
		rtree_borders.remove(std::make_pair(b->getBoostSeg(), b));
	}

	delete it_borders_list->second;
	additional_borders.erase(it_borders_list);

	return true;
}

BoidsPolygonEx* NavMesh::findPolygonByPoint(cocos2d::Point pt)
{
	for (rtree_poly_ex::const_query_iterator it = rtree.qbegin(bgi::intersects(point(pt.x, pt.y))); it != rtree.qend(); ++it)
	{
		if (it->second->coversPoint(pt))
		{
			return it->second;
		}
	}

	return nullptr;
}

void NavMesh::getNearbyBorders(cocos2d::Point pos, float box_radius, std::vector<Collidable*>& output)
{
	for (rtree_border::const_query_iterator it = rtree_borders.qbegin(
		bgi::intersects(box(point(pos.x - box_radius, pos.y - box_radius), point(pos.x + box_radius, pos.y + box_radius)))); it != rtree_borders.qend(); ++it)
	{
		output.push_back(it->second);
	}
}

bool NavMesh::hasPath(cocos2d::Point from, cocos2d::Point to)
{
	//TODO
	return true;
}

class AStarNode
{
public:
	PolygonPortal *portal;
	float f;	
};

bool operator < (const AStarNode& a, const AStarNode& b)
{
	return a.f > b.f; //因为用的堆是大根堆，所以这里的小于号实现得反一反
}

typedef boost::heap::binomial_heap<AStarNode> AStarHeap;

class PathLog
{
public:
	PolygonPortal *come_from;
	AStarHeap::handle_type handle;

	PathLog()
	{
	}

	PathLog(PolygonPortal* _come_from, AStarHeap::handle_type _handle) : come_from(_come_from), handle(_handle)
	{
	}
};

void NavMesh::findCorners(cocos2d::Point start, cocos2d::Point goal, std::vector<PolygonPortal*>& portals, std::vector<cocos2d::Point>& corners)
{
	assert(portals.size()); //没经过portals的应该肯定不会进到这里

	//视野左侧和右侧，即ppt上的last_vis_left, last_vis_right。本来应该a b反一反的，但因为我们的start和goal本质上也是反着来的，所以。。
	cocos2d::Vec2 visible_left(start, portals.front()->a), visible_right(start, portals.front()->b);
	int visible_left_portal_i = 0, visible_right_portal_i = 0; 

	//将goal作为虚拟的portals加进来。否则会写重复代码，因为goal一样会导致turn
	portals.push_back(new PolygonPortal());
	portals.back()->a = portals.back()->b = goal;

	for (unsigned int i = 1; i != portals.size(); ++i) //按ppt的说法，循环里可以<2次叉积..但反正不是瓶颈所在，先不优化了
	{
		const PolygonPortal& current_portal = *portals[i];
		cocos2d::Vec2 new_left(start, current_portal.a), new_right(start, current_portal.b);

		bool turned = false;
		if (Fuzzy::_less(visible_right.cross(new_left), 0.0f)) //如果new_left到right的右边去了，那说明right是一个拐点
		{
			corners.push_back(start + visible_right);
			i = visible_right_portal_i + 1; //将当前portal移到拐点的下一个portal上
			turned = true;
		}
		else if (Fuzzy::_greater(visible_left.cross(new_right), 0.0f)) //new_right到left的左边去了，说明left是一个拐点
		{
			corners.push_back(start + visible_left);
			i = visible_left_portal_i + 1; //将当前portal移到拐点的下一个portal上
			turned = true;
		}

		if (turned)
		{
			start = corners.back(); //将起点移到拐点上
			visible_left_portal_i = visible_right_portal_i = i;  //重新设置visible区域
			visible_left = portals[visible_left_portal_i]->a - start;
			visible_right = portals[visible_right_portal_i]->b - start;
		}
		else
		{
			if (Fuzzy::_equalLess(visible_left.cross(new_left), 0.0f)) //new_left如果到了left的右边，那么要更新left。
								//包括如果和left共线，也要更新。共线主要发生在一个拐点被多个portal共用。这时候更新的主要意义在于更新visible_left_portal_i
			{
				visible_left = new_left;
				visible_left_portal_i = i;
			}
			if (Fuzzy::_equalGreater(visible_right.cross(new_right), 0.0f)) //new_right如果到了right的左边，要更新right。共线也要更新
			{
				visible_right = new_right;
				visible_right_portal_i = i;
			}
		}
	}
}

std::unique_ptr<Path> NavMesh::findPath(cocos2d::Point from, cocos2d::Point to, int path_validity_frame)
{
	//Trace t;

    auto path = std::unique_ptr<Path>(new Path(path_validity_frame)); //应该根据path的需要来设定有限时间，再改
	path->can_reach_final_destination = true; //还是得判一下能不能到的 再改
	path->steps.push_back(to); //目的地点是肯定要塞进去的

	BoidsPolygonEx* polygon_start = findPolygonByPoint(from);
	BoidsPolygonEx* polygon_end = findPolygonByPoint(to);

	//t.trace("findPolygon");

	if (polygon_start == nullptr)
	{
		path->can_reach_final_destination = false;
		//Utils::logAI("unexpected. start point not in a polygon. %f,%f", from.x, from.y);
		return path;
	}

	if (polygon_end == nullptr)
	{
		path->can_reach_final_destination = false;
		return path;
	}

	if (polygon_start == polygon_end) //在同一个多边形，不用寻了
	{
		return path;
	}

	AStarHeap heap;
	std::map<BoidsPolygonEx*, PathLog> logs;
	for (unsigned int i = 0; i < polygon_start->portals.size(); ++i)
	{
		AStarNode node;
		node.portal = &polygon_start->portals[i];
		cocos2d::Point center = node.portal->center();
		node.f = from.distance(center) + center.distance(to);
		AStarHeap::handle_type handle = heap.push(node);
		logs[node.portal->to] = PathLog(node.portal, handle);
	}

	while (heap.size())
	{
		AStarNode current = heap.top();
		heap.pop();

		BoidsPolygonEx* current_polygon = current.portal->to;

		if (current_polygon == polygon_end)
		{
			//到达目的地点，取出来，将中间点存到path里
			std::vector<PolygonPortal*> portals;
			PolygonPortal* current_portal = current.portal;
			for (;;)
			{
				portals.push_back(current_portal);
				if (current_portal->from == polygon_start) break;
				auto it = logs.find(current_portal->from);
				assert(it != logs.end());
				current_portal = it->second.come_from;
			}

			if (PAINT_PATH)
			{
				Utils::clearDrawNode();
				auto node = Utils::getDrawNode();
				if (node)
				{
					node->drawLine(from, portals.back()->center(), cocos2d::Color4F::WHITE);
					for (unsigned int i = 1; i < portals.size(); ++i)
					{
						node->drawLine(portals[i - 1]->center(), portals[i]->center(), cocos2d::Color4F::WHITE);
					}
					node->drawLine(portals.front()->center(), to, cocos2d::Color4F::WHITE);
				}
			}

			//将曲线平滑
			findCorners(to, from, portals, path->steps); //这里是从目的地开始找，反过来的。反正我们要的path->steps也正好是反过来的

			if (PAINT_PATH)
			{
				//输出调试用路线
				Utils::clearDrawNode();
				auto node = Utils::getDrawNode();
				if (node)
				{
					cocos2d::Color4F c(Utils::randomFloat2(), Utils::randomFloat2(), Utils::randomFloat2(), 1.0f);
					path->paint(from, node, c);
				}
			}

			//t.trace("findPath");
			return path;
		}

		for (unsigned int i = 0; i < current_polygon->portals.size(); ++i)
		{
			PolygonPortal& candidate_portal = current_polygon->portals[i];
			BoidsPolygonEx* dest_poly = candidate_portal.to;
			if (dest_poly == current.portal->from) continue; //不往回走
			
			const cocos2d::Point candidate_portal_center = candidate_portal.center();
			float new_f = current.f + current.portal->center().distance(candidate_portal_center) + candidate_portal_center.distance(to); //计算新f: 已知距离 + 启发函数（直线距离）

			//看目的地多边形是否已在；
			auto it = logs.find(dest_poly);
			if (it != logs.end())
			{
				//在的话看是否要更新
				AStarHeap::handle_type& handle = it->second.handle;
				if (new_f < (*handle).f)
				{
					(*handle).portal = &candidate_portal;
					(*handle).f = new_f;
					heap.increase(handle); //f的值改小了，但对于heap来说，他是increase了
				}
			}
			else //不存在的话插入堆和map
			{
				AStarNode node;
				node.portal = &candidate_portal;
				node.f = new_f;
				AStarHeap::handle_type handle = heap.push(node);
				logs[dest_poly] = PathLog(node.portal, handle);
			}
		}
	}

	path->can_reach_final_destination = false;
	//找不到路的话是返回离目的地最近的可以到的地方。再改

	//t.trace("findPath");
	return path;
}

bool NavMesh::parseBordersFromBinary(FILE* f)
{
	uint32_t rings_count;
	if (fread(&rings_count, 4, 1, f) != 1) return false;

	for (unsigned i = 0; i != rings_count; ++i)
	{
		uint32_t ring_size;
		if (fread(&ring_size, 4, 1, f) != 1)
		{
			return false;
		}

		std::vector<cocos2d::Point> points;
		points.resize(ring_size);
		for (unsigned i = 0; i != ring_size; ++i)
		{
			if (!Utils::parseFromBinary(f, points[i]))
			{
				return false;
			}
		}
		for (unsigned i = 0; i != ring_size; ++i)
		{
			Border* b = new Border(points[i], points[(i + 1) % ring_size], points[(i + 2) % ring_size]);
			borders.push_back(b);
			rtree_borders.insert(std::make_pair(b->getBoostSeg(), b));
		}
	}

	return true;
}

bool NavMesh::parseFromBinary(FILE* f) //主要就存了3个东西：单位碰撞半径、Border的Rings、PolygonEx(合并后，但不含Portal)
{
	Trace t;

	if (fread(&_unit_collide_radius, sizeof(float), 1, f) != 1) return false;

	if (!parseBordersFromBinary(f)) return false;
	t.trace("load_borders");

	uint32_t polygons_count;
	if (fread(&polygons_count, sizeof(float), 1, f) != 1) return false;

	for (unsigned i = 0; i != polygons_count; ++i)
	{
		auto poly = new BoidsPolygonEx();
		if (!poly->parseFromBinary(f))
		{
			log("parse %dth polygon failed.", i);
			delete poly;
			return false;
		}
		polygons.push_back(poly);
	}
	t.trace("load_polygons");
	log("polygons count : %d", polygons.size());
	assert(polygons.size());

	mergeAndBuildGraph();
	t.trace("NavMesh_mergeAndBuildGraph");
	log("polygons count after merge: %d", polygons.size());
	
	createRTree();
	t.trace("NavMesh_createRTree");

	return true;
}

void NavMesh::runTest()
{
	testMergeAndBuildGraph();
	testFindPath();
}

void NavMesh::testMergeAndBuildGraph()
{
	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (0,-4) (0,4) (-2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (-2,2) (-4,4) (-4,-4)"));
		m.mergeAndBuildGraph();
		assert(m.polygons.size() == 2);
		assert(m.polygons[0]->portals.back().to == m.polygons[1]);
		assert(m.polygons[1]->portals.back().to == m.polygons[0]);
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (0,-4) (0,4) (-2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (0,-4) (2,-2) (2,2)"));
		m.mergeAndBuildGraph();
		assert(m.polygons.size() == 1);
		assert(m.polygons.back()->serialize() == "(0,4) (-2,2) (-2,-2) (0,-4) (2,-2) (2,2)");
}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (0,-4) (0,4) (-2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (0,-4) (2,-2) (2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (2,2) (2,5)"));
		m.mergeAndBuildGraph();
		assert(m.polygons.size() == 1);
		assert(m.polygons.back()->serialize() == "(0,4) (-2,2) (-2,-2) (0,-4) (2,-2) (2,2) (2,5)");
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (0,-4) (0,4) (-2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (0,-4) (2,-2) (2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (-2,2) (-4,4) (-4,-4)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (2,2) (2,5)"));
		m.mergeAndBuildGraph();
		assert(m.polygons.size() == 2);
		std::string s0 = m.polygons[0]->serialize();
		std::string s1 = m.polygons[1]->serialize();
		assert(m.polygons[0]->portals.back().to == m.polygons[1]);
		assert(m.polygons[1]->portals.back().to == m.polygons[0]);
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(-2,-2) (0,-4) (0,4) (-2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (0,-4) (2,-2) (2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (2,2) (2,5)"));
		m.polygons.push_back(new BoidsPolygonEx("(2,-2) (0,-4) (2,-5)"));
		m.mergeAndBuildGraph();
		assert(m.polygons.size() == 1);
		assert(m.polygons.back()->serialize() == "(2,-2) (2,2) (2,5) (0,4) (-2,2) (-2,-2) (0,-4) (2,-5)");
	}
}

void NavMesh::testFindPath()
{
	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(0,0) (1,-1) (0,1)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,0) (0,1) (-1,0)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,0) (-1,-1) (1,-1)"));
		m.mergeAndBuildGraph();
		m.createRTree();
		assert(m.polygons.size() == 3);
		std::shared_ptr<Path> path = m.findPath(cocos2d::Point(-0.25f, 0.5f), cocos2d::Point(-0.5f, -0.75f));
		assert(*path == Path("(-0.5,-0.75) (0.0,0.0)")); //要序列化的时候别加f在数字末尾，sscanf不认的。。
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(0,4) (1,2) (2,2) (2,6) (1,6)"));
		m.polygons.push_back(new BoidsPolygonEx("(2,1) (3,0) (5,0) (5,2) (2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(2,7) (2,6) (5,6) (5,8) (3,8)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,7) (1,6) (2,6) (2,7) (1,8)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,1) (1,0) (2,1) (2,2) (1,2)"));
		m.mergeAndBuildGraph();
		m.createRTree();
		assert(m.polygons.size() == 5);
		std::shared_ptr<Path> path = m.findPath(cocos2d::Point(3.0f, 1.0f), cocos2d::Point(3.0f, 7.0f));
		assert(*path == Path("(3,7) (2,6) (2,2)"));
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(2,4) (2,2) (4,2) (4,4)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,0) (4,0) (4,2) (2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,6) (2,4) (4,4) (4,6)"));
		m.mergeAndBuildGraph();
		m.createRTree();
		assert(m.polygons.size() == 3);
		std::shared_ptr<Path> path = m.findPath(cocos2d::Point(1.0f, 0.5f), cocos2d::Point(1.0f, 5.5f));
		assert(*path == Path("(1,5.5) (2,4) (2,2)"));
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(2,4) (2,2) (4,2) (4,4)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,0) (1,0) (4,0) (4,2) (2,2)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,6) (2,4) (4,4) (4,6)"));
		m.polygons.push_back(new BoidsPolygonEx("(-2,2) (-2,-3) (-1,0)"));
		m.polygons.push_back(new BoidsPolygonEx("(-1,0) (-2,-3) (0,-3) (1,0) (0,0)"));
		m.mergeAndBuildGraph();
		m.createRTree();
		assert(m.polygons.size() == 5);
		std::shared_ptr<Path> path = m.findPath(cocos2d::Point(-1.5f, 0.5f), cocos2d::Point(1.0f, 5.5f));
		assert(*path == Path("(1,5.5) (2,4) (2,2) (0,0) (-1,0)"));
	}

	{
		NavMesh m;
		m.polygons.push_back(new BoidsPolygonEx("(0,0) (0,-2) (4,-2) (5,0)"));
		m.polygons.push_back(new BoidsPolygonEx("(0,-2) (1,-4) (5,-4) (4,-2)"));
		m.polygons.push_back(new BoidsPolygonEx("(1,-4) (9,-7) (5,-4)"));
		m.polygons.push_back(new BoidsPolygonEx("(5,-4) (9,-7) (8,-4)"));
		m.polygons.push_back(new BoidsPolygonEx("(8,-4) (9,-7) (9,-4)"));
		m.polygons.push_back(new BoidsPolygonEx("(-2,3) (0,0) (5,0) (7,3)"));
		m.mergeAndBuildGraph(false);
		m.createRTree();
		assert(m.polygons.size() == 6);
		std::shared_ptr<Path> path = m.findPath(cocos2d::Point(4.0f, 1.0f), cocos2d::Point(8.5f, -5.0f));
		assert(*path == Path("(8.5,-5) (5,-4) (4,-2)"));
	}
}

void NavMesh::drawBoostRing(polygon::ring_type& ring, cocos2d::Color4F color, cocos2d::DrawNode* node)
{
	if (ring.size() > 1)
	{
		for (unsigned int i = 1; i < ring.size(); ++i)
		{
			Utils::drawVec(Utils::toCocosPt(ring[i - 1]), Utils::toCocosPt(ring[i]), node, color);
		}
		Utils::drawVec(Utils::toCocosPt(ring.back()), Utils::toCocosPt(ring.front()), node, color);
	}
}

void NavMesh::drawBoostPolygonOn(polygon& poly, cocos2d::DrawNode* node)
{
	drawBoostRing(poly.outer(), Color4F::RED, node);
	for (unsigned int i = 0; i < poly.inners().size(); ++i)
	{
		drawBoostRing(poly.inners()[i], Color4F::BLUE, node);
	}
}

void NavMesh::drawSketchOn(cocos2d::DrawNode* node)
{
	for (unsigned int i = 0; i < polygons.size(); ++i)
	{
		polygons[i]->drawSketchOn(node);
	}
}