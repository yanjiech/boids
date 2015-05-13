#include "BoidsPolygonEx.h"
#include "Utils.h"
#include <sstream>
#include "../BoidsMath.h"

using namespace cocos2d;

int BoidsPolygonEx::global_id = 0;

BoidsPolygonEx::BoidsPolygonEx() : _id(global_id++)
{
}

BoidsPolygonEx::BoidsPolygonEx(const std::string& polygon_serialization) : BoidsPolygon(polygon_serialization), _id(global_id++)
{
}

BoidsPolygonEx::BoidsPolygonEx(const polygon::ring_type& ring) : _id(global_id++)
{
	for (unsigned int i = 0; i < ring.size(); ++i)
	{
		addNewVertex(ring[i].get<0>(), ring[i].get<1>());
	}
}

BoidsPolygonEx::~BoidsPolygonEx()
{
}

//更新outer（的[begin, end)这一段）和inner这个polygon之间的联系
void BoidsPolygonEx::updateConnection(ShapeConnection& conn, BoidsPolygonEx* inner_ring, PolygonPointPtr outer_begin, PolygonPointPtr outer_end)
{
	//找最近距离不用判相交，因为一旦相交了，肯定另有边(相交的那条边)更近

	inner_ring->foreachelement2([&](PolygonPointPtr inner_pt_ptr, PolygonPointPtr inner_pt2_ptr)
	{
		for (PolygonPointPtr outer_pt_ptr = outer_begin;;) //outer_begin和outer_end相等的时候表示遍历一圈，因此不能在for循环头上判断
		{
			//inner点和outer边的最近距离
			cocos2d::Point& inner_pt1 = inner_pt_ptr->d;
			cocos2d::Point& outer_pt1 = outer_pt_ptr->d;
			cocos2d::Point& outer_pt2 = outer_pt_ptr->next->d;
			{
				cocos2d::Vec2 perpendicular_vec = -Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Vec2(outer_pt1, outer_pt2));
				float s, t;
				if (cocos2d::Vec2::isLineIntersect(inner_pt1, inner_pt1 + perpendicular_vec, outer_pt1, outer_pt2, &s, &t))
				{
					if (Fuzzy::_equalGreater(s, 0.0f) && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f))
					{
						cocos2d::Point intersection_point = inner_pt1 + s * perpendicular_vec;
						float distance_sqr = inner_pt1.distanceSquared(intersection_point);
						if (distance_sqr < conn.distance_sqr)
						{
							conn.inner_pt = inner_pt_ptr;
							conn.outer_pt = outer_pt_ptr;
							conn.connection_type = ShapeConnectionType::connInnerPointToOuterEdge;
							conn.intersection_point = intersection_point;
							conn.distance_sqr = distance_sqr;
						}
					}
				}
			}

			//outer点和inner边的最近距离；一开始以为只要处理outer点在反射角上的就够了，后来发现不够
			cocos2d::Point& inner_pt2 = inner_pt2_ptr->d;
			{
				//虽然inner_ring是顺时针，垂线也是要取反的
				cocos2d::Vec2 perpendicular_vec = -Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Vec2(inner_pt1, inner_pt2));
				float s, t;
				if (cocos2d::Vec2::isLineIntersect(outer_pt1, outer_pt1 + perpendicular_vec, inner_pt1, inner_pt2, &s, &t))
				{
					if (Fuzzy::_equalGreater(s, 0.0f) && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f))
					{
						cocos2d::Point intersection_point = outer_pt1 + s * perpendicular_vec;
						float distance_sqr = outer_pt1.distanceSquared(intersection_point);
						if (distance_sqr < conn.distance_sqr)
						{
							conn.outer_pt = outer_pt_ptr;
							conn.inner_pt = inner_pt2_ptr; //根据我Connection的定义，这里返回的是inner_pt2
							conn.connection_type = ShapeConnectionType::connOuterPointToInnerEdge;
							conn.intersection_point = intersection_point;
							conn.distance_sqr = distance_sqr;
						}
					}
				}
			}

			//outer点到inner点的最近距离
			float distance_sqr = outer_pt1.distanceSquared(inner_pt1);
			if (distance_sqr < conn.distance_sqr)
			{
				conn.outer_pt = outer_pt_ptr;
				conn.inner_pt = inner_pt_ptr;
				conn.connection_type = ShapeConnectionType::connOuterPointToInnerPoint;
				conn.distance_sqr = distance_sqr;
			}

			outer_pt_ptr = outer_pt_ptr->next;
			if (outer_pt_ptr == outer_end) break;
		}
	});

	assert(conn.distance_sqr != FLT_MAX); //不可能没找到边的
	conn.destination_poly = inner_ring;
}

int debug_connection_type[3] = { 0, 0, 0 };

void BoidsPolygonEx::applyConnection(const ShapeConnection& conn, PolygonPointPtr& affected_begin, PolygonPointPtr& affected_end)
{
	debug_connection_type[conn.connection_type] ++;

	if (conn.connection_type == ShapeConnectionType::connInnerPointToOuterEdge) //内部ring的点连到外部的边
	{
		affected_begin = conn.outer_pt;
		PolygonPointPtr prev_element = insert(conn.intersection_point, conn.outer_pt); //将(外部边上的)交点插在(外部)边的起点之后
		prev_element = insert(conn.inner_pt, conn.inner_pt, prev_element); //内部ring整体插一遍
		prev_element = insert(conn.inner_pt->d, prev_element); //内部ring的第一个点也就是最后一个再用一遍
		affected_end = insert(conn.intersection_point, prev_element)->next; //交点再用一遍（下一个点是边的终点）
	}
	else if (conn.connection_type == ShapeConnectionType::connOuterPointToInnerEdge)//外部的点连到内部的边
	{
		affected_begin = conn.outer_pt;
		PolygonPointPtr prev_element = insert(conn.intersection_point, conn.outer_pt); //将(内部边上的)交点插在外部ring的端点之后
		prev_element = insert(conn.inner_pt, conn.inner_pt, prev_element); //内部ring整体插一遍
		prev_element = insert(conn.intersection_point, prev_element); //将(内部边上的)交点再用一遍
		affected_end = insert(conn.outer_pt->d, prev_element); //将外部ring的端点再用一遍
	}
	else if (conn.connection_type == ShapeConnectionType::connOuterPointToInnerPoint) //从外部的点到内部的点
	{
		affected_begin = conn.outer_pt;
		PolygonPointPtr prev_element = insert(conn.inner_pt, conn.inner_pt, conn.outer_pt); //内部ring整体插一遍
		prev_element = insert(conn.inner_pt->d, prev_element); //就内部ring的第一个点也就是最后一个点再插一遍
		affected_end = insert(conn.outer_pt->d, prev_element); //将外部端点再用一遍
	}
}

void BoidsPolygonEx::processInnerRings(const std::vector<BoidsPolygonEx*>& inner_rings)
{
	//相当于是个Prim算法。按照先连短边的顺序，应该能保证新连的边不重叠

	std::map<BoidsPolygonEx*, ShapeConnection> connections;
	for (unsigned int i = 0; i < inner_rings.size(); ++i)
	{
		updateConnection(connections[inner_rings[i]], inner_rings[i], ap, ap);
	}

	for (unsigned int t = 0; t < inner_rings.size(); ++t)
	{
		//选一个距离最小的connection
		const ShapeConnection* nearest_conn = nullptr;
		for (const auto& kvp : connections)
		{
			if (nearest_conn == nullptr || kvp.second.distance_sqr < nearest_conn->distance_sqr)
			{
				nearest_conn = &(kvp.second);
			}
		}

		assert(nearest_conn);
		PolygonPointPtr affected_begin, affected_end;
		applyConnection(*nearest_conn, affected_begin, affected_end); //应用connection

		//要把涉及到相关的connection给重置下
		for (auto& poly_and_conn : connections)
		{
			auto& conn = poly_and_conn.second;
			if (&conn == nearest_conn) continue;

			if (conn.outer_pt == nearest_conn->outer_pt)
			{
				conn.distance_sqr = FLT_MAX;
			}
		}

		connections.erase(nearest_conn->destination_poly); //将连上的inner_ring去掉

		for (auto& kvp : connections) //这里有遍历以指针为Key的map的行为，理论上多次运行有可能遍历的具体顺序不一样，导致结果可能不同。但只要我们的.nav是只生成一遍然后发布出去的，就不会有问题
		{
			updateConnection(kvp.second, kvp.first, affected_begin, affected_end); //outer_ring只有部分点是新增的，会影响剩下的这些连接到多边形的距离
		}
	}
}

BoidsPolygonEx* BoidsPolygonEx::split(PolygonPointPtr split_start, PolygonPointPtr split_end)
{
	BoidsPolygonEx* new_poly = new BoidsPolygonEx();
	new_poly->push_back(split_end->d); //先插入一个点
	new_poly->insert(split_start, split_end, new_poly->ap); //把那一段插入这个点的后面
	erase(split_start, split_end);
	return new_poly;
}

//有很多特别扁长的三角形，打算换个方法写
void BoidsPolygonEx::convexification(std::vector<BoidsPolygonEx*>& output)
{
	bool convex = true;

	//开始找对面的端点，使得：距离/割下来的形状的周长 最小。但要确保新连的边不和现有边相交（判相交以后可以优化）
	float best_ratio = FLT_MAX;
	PolygonPointPtr best_ratio_start_ptr = nullptr, best_ratio_opposite_ptr = nullptr;

	foreachelement2([&, this](PolygonPointPtr ptr1, PolygonPointPtr ptr2)
	{
		cocos2d::Point& pt1 = ptr1->d;
		cocos2d::Point& pt2 = ptr2->d;
		cocos2d::Point& pt3 = ptr2->next->d;
		if (Fuzzy::_less(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(pt2, pt3)), 0.0f)) //反射角
		{   
			convex = false;

			//这里两点间距离可能会反复计算，以后再优化
			float perimeter = pt2.distance(pt3);
			for (PolygonPointPtr before_opposite_ptr = ptr2->next, opposite_ptr = before_opposite_ptr->next; opposite_ptr != ptr1; before_opposite_ptr = before_opposite_ptr->next, opposite_ptr = opposite_ptr->next)
			{
				cocos2d::Point& opposite_pt = opposite_ptr->d;
				perimeter += before_opposite_ptr->d.distance(opposite_pt);

				if (Fuzzy::_greater(cocos2d::Vec2(pt2, pt3).cross(cocos2d::Vec2(pt2, opposite_pt)), 0.0f)) //(pt2, opposite_pt)一定要在(pt2, pt3)的逆时针方向
				{
					float new_edge_distance = pt2.distance(opposite_pt);
					float ratio = new_edge_distance / perimeter;

					if (ratio < best_ratio)
					{
						bool intersect = false;
						//且(pt2, opposite_pt)不能和多边形的任意一条边相交及相切
						foreachelement2_with_break([=, &intersect](PolygonPointPtr to_check_ptr1, PolygonPointPtr to_check_ptr2) -> bool
						{
							if (!(to_check_ptr1 == ptr2 || to_check_ptr2 == ptr2 || to_check_ptr1 == opposite_ptr || to_check_ptr2 == opposite_ptr))
							{
								float s, t;
								if (cocos2d::Vec2::isLineIntersect(ptr2->d, opposite_ptr->d, to_check_ptr1->d, to_check_ptr2->d, &s, &t))
								{
									if (s >= 0.0f && s <= 1.0f && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f)) //这里到底用不用fuzzy的判很尴尬。。用fuzzy的话有个test会过不了
									{
										intersect = true;
										return true;
									}
								}
							}
							return false;
						});

						if (!intersect)
						{
							best_ratio = ratio;
							best_ratio_start_ptr = ptr2;
							best_ratio_opposite_ptr = opposite_ptr;
						}
					}
				}
			}
		}
	});

	if (convex)
	{
		output.push_back(this);
	}
	else
	{
		assert(best_ratio_opposite_ptr && "convexification failed. polygon has convex but can't split. contact weiyuemin.");
		BoidsPolygonEx* new_poly = split(best_ratio_start_ptr, best_ratio_opposite_ptr);
		new_poly->convexification(output);
		this->convexification(output);
	}
}

void BoidsPolygonEx::convexification2(std::vector<BoidsPolygonEx*>& output)
{
	foreachelement2([&, this](PolygonPointPtr ptr1, PolygonPointPtr ptr2)
	{
		cocos2d::Point& pt1 = ptr1->d;
		cocos2d::Point& pt2 = ptr2->d;
		cocos2d::Point& pt3 = ptr2->next->d;
		if (Fuzzy::_less(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(pt2, pt3)), 0.0f)) //反射角
		{
			cocos2d::Vec2 vec_to_inner = cocos2d::Vec2(pt3, pt2).getNormalized() + cocos2d::Vec2(pt1, pt2).getNormalized(); //求反射角向内的那个向量

			//找交点
			float smallest_s = FLT_MAX, corresponding_t;
			PolygonPointPtr opposite_edge_a = nullptr;
			for (PolygonPointPtr to_check_pt1 = ptr2->next, to_check_pt2 = to_check_pt1->next; to_check_pt2 != ptr2; to_check_pt1 = to_check_pt1->next, to_check_pt2 = to_check_pt2->next)
			{
				float s, t;
				if (cocos2d::Vec2::isLineIntersect(pt2, pt2 + vec_to_inner, to_check_pt1->d, to_check_pt2->d, &s, &t))
				{
					if (Fuzzy::_greater(s, 0.0f) && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f))
					{
						if (s < smallest_s)
						{
							smallest_s = s;
							corresponding_t = t;
							opposite_edge_a = to_check_pt1;
						}
					}
				}
			}
			assert(opposite_edge_a); //不可能没有交点的

			if (Fuzzy::_equal(corresponding_t, 0.0f))
			{
				BoidsPolygonEx* new_poly = split(ptr2, opposite_edge_a);
				ptr1 = ptr2; //改循环变量的位置（可以少检查一个角）

				new_poly->convexification2(output);
			}
			else if (Fuzzy::_equal(corresponding_t, 1.0f))
			{
				BoidsPolygonEx* new_poly = split(ptr2, opposite_edge_a->next);
				ptr1 = ptr2; //改循环变量的位置（可以少检查一个角）

				new_poly->convexification2(output);
			}
			else
			{
				cocos2d::Point intersection_point = pt2 + smallest_s * vec_to_inner;
				BoidsPolygonEx* new_poly = new BoidsPolygonEx();
				new_poly->push_back(intersection_point); //先插入一个点
				new_poly->insert(ptr2, opposite_edge_a->next, new_poly->ap); //把[ptr2, opposite_edge_a->next)，即[ptr2, opposite_edge_a]，插入这个点的后面
				erase(ptr2, opposite_edge_a->next);
				insert(intersection_point, ptr2);
				ptr1 = ptr2;

				new_poly->convexification2(output);
			}
		}
	});

	output.push_back(this);
}

//检查范围：[from, to)之内的所有线段。即第一条线段是(from, from->next)，最后一条不到(to->prev, to)
PolygonPointPtr BoidsPolygonEx::checkInnerIntersectIfConnect(cocos2d::Point pt1, cocos2d::Point pt2, PolygonPointPtr from, PolygonPointPtr to, float s_constraint, float& corresponding_t)
{
	float smallest_s = FLT_MAX;
	PolygonPointPtr opposite_edge_src = nullptr;
	for (PolygonPointPtr to_check_pt1 = from, to_check_pt2 = to_check_pt1->next; to_check_pt2 != to; to_check_pt1 = to_check_pt1->next, to_check_pt2 = to_check_pt2->next)
	{
		//只判断从多边形内穿到外的情况。不加这个判断的话，遇到两条方向相反但重合的边，有可能会连错
		//因为是逆时针，所以只要判断下面这个叉积就可以了
		if (Fuzzy::_greater(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(to_check_pt1->d, to_check_pt2->d)), 0.0f))
		{
			float s, t;
			if (cocos2d::Vec2::isLineIntersect(pt1, pt2, to_check_pt1->d, to_check_pt2->d, &s, &t))
			{
				//s和s_constriant的判断要equalLess，因为连交点之后的判断，如果遇到多条边在一个点的，那么这些边都是等于s_constraint的
				//因为前面判向量的关系，t的两边都得要闭区间，否则会漏掉一些相切的情况（切点所在的两条边可能只有一条符合前面的向量条件）
				if (Fuzzy::_greater(s, 0.0f) && Fuzzy::_equalLess(s, s_constraint) && Fuzzy::_equalGreater(t, 0.0f) && Fuzzy::_equalLess(t, 1.0f))
				{
					if (Fuzzy::_equal(s, smallest_s)) //遇到s相等，那么八成是撞到个端点，且端点上面有多条边的。这时候真正最近的那条边判起来就比较复杂，应该是顺时针顺序第一个遇到的
					{
						cocos2d::Point endpoint_to_compare;
						if (Fuzzy::_equal(corresponding_t, 0.0f))
						{
							endpoint_to_compare = opposite_edge_src->next->d;
						}
						else if (Fuzzy::_equal(corresponding_t, 1.0f))
						{
							endpoint_to_compare = opposite_edge_src->d;
						}
						else
						{
							log("unexpected. intersected with multi-borders but what to compare is not an endpoint. id: %d, corresponding_t: %f", _id, corresponding_t);
							continue;
						}

						if (Fuzzy::_equal(t, 0.0f))
						{
							//to_check_pt1和origin(pt2)重合了
							if (Geometry::clockwiseCloser(pt2, pt1, to_check_pt2->d, endpoint_to_compare))
							{
								corresponding_t = 0.0f;
								opposite_edge_src = to_check_pt1;
							}
						}
						else if (Fuzzy::_equal(t, 1.0f))
						{
							//to_check_pt2和origin(pt2)重合了
							if (Geometry::clockwiseCloser(pt2, pt1, to_check_pt1->d, endpoint_to_compare))
							{
								corresponding_t = 1.0f;
								opposite_edge_src = to_check_pt1;
							}
						}
						else
						{
							log("unexpected. intersected with multi-borders but not at the endpoint. id: %d, t: %f", _id, t);
						}
					}
					else if (s < smallest_s)
					{
						smallest_s = s;
						corresponding_t = t;
						opposite_edge_src = to_check_pt1;
					}
				}
			}
		}
	}
	return opposite_edge_src;
}

void BoidsPolygonEx::convexification3(std::vector<BoidsPolygonEx*>& output)
{
	foreachelement2([&, this](PolygonPointPtr ptr1, PolygonPointPtr ptr2)
	{
		cocos2d::Point& pt1 = ptr1->d;
		cocos2d::Point& pt2 = ptr2->d;
		cocos2d::Point& pt3 = ptr2->next->d;
		if (Fuzzy::_less(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(pt2, pt3)), 0.0f)) //反射角
		{
			cocos2d::Vec2 vec_to_inner = cocos2d::Vec2(pt3, pt2).getNormalized() + cocos2d::Vec2(pt1, pt2).getNormalized(); //求反射角向内的那个向量

			//找交点
			float corresponding_t;
			PolygonPointPtr opposite_edge_src = checkInnerIntersectIfConnect(pt2, pt2 + vec_to_inner, ptr2->next, ptr2, FLT_MAX, corresponding_t);
			assert(opposite_edge_src); //不可能没有交点的

			for (;;)
			{
				//和端点连个边不一定总是能用的：有可能连完之后还是反射角。且这种情况下可能都没有产生新边，导致不停地递归
				bool can_use_edge_src = Fuzzy::_equalGreater(cocos2d::Vec2(pt1, pt2).cross(cocos2d::Vec2(pt2, opposite_edge_src->d)), 0.0f);
				bool can_use_edge_dst = Fuzzy::_equalGreater(cocos2d::Vec2(opposite_edge_src->next->d, pt2).cross(cocos2d::Vec2(pt2, pt3)), 0.0f);

				bool use_edge_src;
				if (can_use_edge_src && can_use_edge_dst)
				{
					use_edge_src = Fuzzy::_equalLess(corresponding_t, 0.5f); //两边都可以的话，如果靠近edge_src，就连edge_src
				}
				else
				{
					if (can_use_edge_src)
					{
						use_edge_src = true;
					}
					else if (can_use_edge_dst)
					{
						use_edge_src = false;
					}
					else //两边都不能用，那就只能用原始的那个交点了
					{
						cocos2d::Point intersection_point = opposite_edge_src->d + corresponding_t * (opposite_edge_src->next->d - opposite_edge_src->d);
						BoidsPolygonEx* new_poly = new BoidsPolygonEx();
						new_poly->push_back(intersection_point); //先插入一个点
						new_poly->insert(ptr2, opposite_edge_src->next, new_poly->ap); //把[ptr2, opposite_edge_src->next)，即[ptr2, opposite_edge_src]，插入这个点的后面
						erase(ptr2, opposite_edge_src->next);
						insert(intersection_point, ptr2);

						new_poly->convexification3(output);

						ptr1 = ptr2; //下一个角不用检查了
						break;
					}
				}

				PolygonPointPtr new_opposite_edge_src;
				if (use_edge_src)
				{
					new_opposite_edge_src = checkInnerIntersectIfConnect(pt2, opposite_edge_src->d, ptr2->next, opposite_edge_src, 1.0f, corresponding_t);
				}
				else
				{
					opposite_edge_src = opposite_edge_src->next; //这时候src其实是原来的dst，移动了个点
					new_opposite_edge_src = checkInnerIntersectIfConnect(pt2, opposite_edge_src->d, opposite_edge_src->next, ptr2, 1.0f, corresponding_t);
				}

				if (new_opposite_edge_src == nullptr) //如果没有交点，那就可以连了；否则将待连接的边设置为新的交点所在的边
				{
					BoidsPolygonEx* new_poly = split(ptr2, opposite_edge_src);
					new_poly->convexification3(output);

					ptr1 = ptr2; //下一个角不用检查了
					break;
				}
				else
				{
					opposite_edge_src = new_opposite_edge_src;
				}
			}
		}
	});

	output.push_back(this);
}

void BoidsPolygonEx::serializeToBinary(FILE* f)
{
	uint32_t vertex_count = size();
	fwrite(&vertex_count, 4, 1, f);

	foreach([=](cocos2d::Point pt) {
		Utils::serializeToBinary(f, pt);
	});
}

bool BoidsPolygonEx::parseFromBinary(FILE* f)
{
	uint32_t vertex_count;
	if (fread(&vertex_count, 4, 1, f) != 1) return false;

	for (unsigned i = 0; i != vertex_count; ++i)
	{
		cocos2d::Point pt;
		if (!Utils::parseFromBinary(f, pt))
		{
			log("parse %dth vertex failed.", i);
			return false;
		}
		push_back(pt);
	}

	return true;
}

void testProcessInnerRings()
{
	{ //外部点连内部边
		BoidsPolygonEx outer("(0,0) (10,-2) (10,25) (-10,25) (-10,-2)");

		std::vector<BoidsPolygonEx*> inner_rings;
		inner_rings.push_back(new BoidsPolygonEx("(-4,1) (-4,2) (4,2) (4,1)"));

		outer.processInnerRings(inner_rings);
		std::string s = outer.serialize();

		BoidsPolygonEx correct("(0,0) (0,1) (-4,1) (-4,2) (4,2) (4,1) (0,1) (0,0) (10,-2) (10,25) (-10,25) (-10,-2)");
		assert(outer == correct);

		delete inner_rings.back();
	}

	{ //内部点连外部点
		BoidsPolygonEx outer("(0,0) (10,-2) (10,25) (-10,25) (-10,-2)");

		std::vector<BoidsPolygonEx*> inner_rings;
		inner_rings.push_back(new BoidsPolygonEx("(-1,3) (0,5) (1,3) (0,1)"));

		outer.processInnerRings(inner_rings);
		std::string s = outer.serialize();

		BoidsPolygonEx correct("(0,0) (0,1) (-1,3) (0,5) (1,3) (0,1) (0,0) (10,-2) (10,25) (-10,25) (-10,-2)");
		assert(outer == correct);

		delete inner_rings.back();
	}

	{ //内部点连外部边 及外部点连内部点
		BoidsPolygonEx outer("(0,0) (4,0) (4,3) (0,3)");

		std::vector<BoidsPolygonEx*> inner_rings;
		inner_rings.push_back(new BoidsPolygonEx("(2,1) (2,1.8) (3,1.8) (3,1)"));
		inner_rings.push_back(new BoidsPolygonEx("(1,2.5) (1.5,2.5) (1.5,2) (1,2)"));

		outer.processInnerRings(inner_rings);
		std::string s = outer.serialize();

		BoidsPolygonEx correct("(0,0) (4,0) (4,3) (1,3) (1,2.5) (1.5,2.5) (1.5,2) (2,1.8) (3,1.8) (3,1) (2,1) (2,1.8) (1.5,2) (1,2) (1,2.5) (1,3) (0,3)");
		assert(outer == correct);

		delete inner_rings.back();
	}

	{ //两个内部环连同一条外部边
		BoidsPolygonEx outer("(0,0) (100,0) (100,50) (0,50)");

		std::vector<BoidsPolygonEx*> inner_rings;
		inner_rings.push_back(new BoidsPolygonEx("(30,20) (30,22) (31,22) (31,21)"));
		inner_rings.push_back(new BoidsPolygonEx("(60,20) (60,22) (61,22) (61,21)"));

		outer.processInnerRings(inner_rings);
		std::string s = outer.serialize();

		BoidsPolygonEx correct("(0,0) (30,0) (30,20) (30,22) (31,22) (31,21) (30,20) (30,0) (60,0) (60,20) (60,22) (61,22) (61,21) (60,20) (60,0) (100,0) (100,50) (0,50)");
		assert(outer == correct);

		delete inner_rings.back();
	}

	{ //内部环连外部点,另一内部环连新产生的边
		BoidsPolygonEx outer("(0,0) (50,-80) (50,300) (-50,300) (-50,-80)");

		std::vector<BoidsPolygonEx*> inner_rings;
		inner_rings.push_back(new BoidsPolygonEx("(-10,10) (-10,11) (-9,11) (-9,10)"));
		inner_rings.push_back(new BoidsPolygonEx("(10,10) (10,11) (11,11) (11,10)"));

		outer.processInnerRings(inner_rings);
		std::string s = outer.serialize();

		BoidsPolygonEx correct("(0,0) (-9,10) (-10,10) (-10,11) (-9,11) (-9,10) (-0.497237,0.552486) (10,10) (10,11) (11,11) (11,10) (10,10) (-0.497237,0.552486) (0,0) (50,-80) (50,300) (-50,300) (-50,-80)");
		assert(outer == correct);

		delete inner_rings.back();
	}
}

void checkConvexificationCorrectness(const std::vector<BoidsPolygonEx*>& output)
{
	for (unsigned int i = 0; i != output.size(); ++i)
	{
		assert(output[i]->isConvex());
	}
}

void _testConvexification(const std::string& polygon_serialization)
{
	std::vector<BoidsPolygonEx*> output;
	BoidsPolygonEx p(polygon_serialization);
	p.convexification(output);
	checkConvexificationCorrectness(output);
}

void testConvexification()
{
	_testConvexification("(2,0) (1,3) (0,2) (-2,2) (-2,0)");

	//直接连端点会有问题
	_testConvexification("(0,100) (0,0) (150,0) (150,101) (-1,101)");

	//产生一个新交点比较优
	_testConvexification("(0,0) (100,-100) (100,2) (-90,2) (-90,-100)"); 

	//_testConvexification("(3207.51,106.044) (3207.51,40.0001) (2954.31,40.0001) (2937.76,83.6628) (2970.36,96.1989)");

	//里面两个洞的
	_testConvexification("(0,0) (2,0) (2,1) (2,1.8) (3,1.8) (3,1) (2,1) (2,0) (4,0) (4,3) (1,3) (1,2.5) (1.5,2.5) (1.5,2) (1,2) (1,2.5) (1,3) (0,3)");

	//里面一个洞；反射角向内射线可能会交到双向的边上
	_testConvexification("(0,0) (100,0) (100,30) (60,50) (100,70) (140,50) (100,30) (100,0) (110,0) (170,45) (100,100) (10,50) (30,20)");

	//和远处的边相交的
	_testConvexification("(82.7673,1937.14) (162.645,1157) (265.355,1157) (280,1142.36) (280,1121.75) (304.227,1130.67) (376.805,1173.61) (458.135,1190.83) (459.308,1192.08) (456.972,1206.1) (492.272,1235.26) (540.457,1265.92) (594.493,1282.37) (687.762,1273.16) (725.85,1243) (729.416,1206.46) (739.665,1202.28) (881.003,1181.08) (934.613,1152.29) (1062.18,1137.6) (1142.15,1172.41) (1171.65,1181.93) (1172,1182.14) (1172,1223.88) (1190.43,1238.89) (1132.71,1313.53) (1067.1,1262.79) (813.35,1384.45) (750.353,1351.26) (551.665,1449.63) (551.869,1434.77) (429.825,1366.62) (430.319,1364.89) (294.45,1294.38) (256.292,1317.27) (257.769,1347.19) (326.672,1379.78) (249,1418.13) (249,1458.02) (272.067,1472.77) (391.844,1417.39) (173.049,1954.34)");
	_testConvexification("(265.355,1157) (456.972,1206.1) (492.272,1235.26) (540.457,1265.92) (594.493,1282.37) (687.762,1273.16) (725.85,1243) (729.416,1206.46) (739.665,1202.28) (881.003,1181.08) (934.613,1152.29) (1062.18,1137.6) (1142.15,1172.41) (1171.65,1181.93) (1172,1182.14) (1172,1223.88) (1190.43,1238.89) (1132.71,1313.53) (1067.1,1262.79) (813.35,1384.45) (750.353,1351.26) (551.665,1449.63) (551.869,1434.77) (429.825,1366.62) (430.319,1364.89) (294.45,1294.38) (256.292,1317.27) (257.769,1347.19) (326.672,1379.78) (249,1418.13) (249,1458.02) (272.067,1472.77) (391.844,1417.39) (451.822,1450.8) (411.37,1471.03) (417.082,1511.01) (440.119,1522.82) (453.037,1516.06) (453.335,1530.4) (173.049,1954.34) (82.7673,1937.14) (30.9999,1934.04) (162.645,1157)");
	_testConvexification("(1372.78, 1192.93) (1393.22, 692.492) (1545.98, 600.832) (1546.02, 572.565) (1444.39, 511.237) (1292.9, 592.563) (1292.02, 606.889) (1263.74, 592.473) (1228.72, 611.15) (1229.29, 641.783) (1296.02, 674.235) (1216.33, 716.252) (1087.79, 657.056) (1086.12, 629.288) (1010.92, 588.635) (964.408, 621.464) (952.015, 603.908) (987.507, 575.071) (979.7, 523.025) (979.285, 522.76) (979.155, 522.785) (1024.04, 488.458) (997.587, 443.104) (933.846, 433.298) (840.48, 418.556) (823.61, 449.528) (834.623, 461.837) (819.452, 458.267) (747.803, 443.11) (695.447, 440.898) (684.734, 456.968) (714.861, 402.031) (683.14, 349.785) (625.941, 292.586) (553.879, 258.521) (514.036, 208.986) (514.744, 191.996) (523.847, 174.701) (575.817, 119.922) (587.459, 60.0486) (579.449, 17.9999) (1953.63, 18) (1953.63, 266.369) (1869.66, 347.444) (1874, 373.387) (1957.89, 412.538) (2053.23, 368.953) (2056.06, 340.342) (1953.63, 266.369) (1953.63, 18) (2769.41, 18) (2769.41, 180.419) (2736.32, 226.734) (2744.13, 250.728) (2790.75, 266.27) (2825.39, 231.628) (2821.29, 206.363) (2769.41, 180.419) (2769.41, 18) (1420.48, 1215.38)");

	//调了好久的case, 从交点移到端点之后会遇到一个连了好多边的点。但这个case convexification3可以，2还过不了
	_testConvexification("(1626.84,1333.84) (1420.85,1246.69) (1420.48,1215.38) (1372.78,1192.93) (1318.68,1219.45) (1331.91,1169.19) (1291.41,1130.1) (1228.92,1081.37) (1166.8,1054) (1132.88,1040.03) (1092.39,1021.85) (1124.68,1007.5) (1120.12,961.914) (1074.86,941.172) (1057.08,933.441) (1060.03,921.905) (1027.68,882.619) (966.401,870.049) (887.916,858.077) (840.615,847.643) (860.734,837.584) (885.076,803.505) (874.716,772.424) (844.967,748.625) (779.943,730.478) (697.684,726.978) (690.668,742.119) (637.796,715.264) (615.753,709.072) (648.513,707.512) (738.657,683.567) (803.165,652.849) (842.863,591.249) (856.88,581.904) (849.151,595.429) (863.603,620.333) (909.309,620.333) (952.015,603.908) (964.408,621.464) (897.513,668.684) (896.527,693.315) (960.746,746.831) (1087.79,657.056) (1216.33,716.252) (1216.33,752.726) (1240.36,767.333) (1389.61,689.943) (1393.22,692.492) (1545.98,600.832) (1546.02,572.565) (1444.39,511.237) (1292.9,592.563) (1292.02,606.889) (1263.74,592.473) (1228.72,611.15) (1229.29,641.783) (1296.02,674.235) (1216.33,716.252) (1087.79,657.056) (1086.12,629.288) (1010.92,588.635) (964.408,621.464) (952.015,603.908) (987.507,575.071) (979.7,523.025) (979.285,522.76) (979.155,522.785) (1024.04,488.458) (997.587,443.104) (933.846,433.298) (840.48,418.556) (823.61,449.528) (834.623,461.837) (819.452,458.267) (747.803,443.11) (695.447,440.898) (684.734,456.968) (714.861,402.031) (683.14,349.785) (625.941,292.586) (553.879,258.521) (514.036,208.986) (514.744,191.996) (523.847,174.701) (575.817,119.922) (587.459,60.0486) (579.449,17.9999) (1953.63,18) (1953.63,266.369) (1869.66,347.444) (1874,373.387) (1957.89,412.538) (2053.23,368.953) (2056.06,340.342) (1953.63,266.369) (1953.63,18) (2769.41,18) (2769.41,180.419) (2736.32,226.734) (2744.13,250.728) (2790.75,266.27) (2825.39,231.628) (2821.29,206.363) (2769.41,180.419) (2769.41,18) (3816,18) (3816,369.028) (3675.38,369.028) (3644.3,337.945) (3569.71,335.929) (3549.53,372.926) (3562.66,420.189) (3616.15,435.472) (3673.83,391.713) (3675.38,369.028) (3816,369.028) (3816,1473.58) (3741.31,1478.82) (3658.82,1462.07) (3638.14,1458.71) (3622.63,1449.85) (3588.46,1448.9) (3557.04,1458) (3534.25,1469.76) (3499.87,1487.33) (3469.74,1522.63) (3456.75,1569.16) (3466.76,1581.33) (3457.59,1586.22) (3458.43,1617.14) (3514.67,1643.3) (3514.67,1694.19) (3451.99,1809.9) (3396.67,1779.93) (3318.49,1817.57) (3315.75,1845.56) (3345.34,1867.76) (3400.81,1891.88) (3452.41,1883.28) (3474.98,1822.35) (3451.99,1809.9) (3514.67,1694.19) (3560.93,1705.75) (3585.08,1723.19) (3612.44,1729.63) (3712.98,1754.77) (3730.04,1744.53) (3770.09,1741.77) (3801.68,1718.07) (3816,1717.63) (3816,2155.67) (3783.83,2155.67) (3770.83,2167.22) (3764.72,2167.8) (3751.4,2150.49) (3685.28,2156.69) (3672.91,2170.44) (3598.04,2180.42) (3543.83,2206.7) (3520.14,2245.46) (3539.42,2336.18) (3419.92,2355.49) (3324.15,2195.86) (3333.16,2190.46) (3330.68,2159.55) (3269.56,2135.1) (3153.99,2206.71) (3156.14,2236.85) (3213.8,2262.08) (3324.15,2195.86) (3419.92,2355.49) (3396.07,2409.71) (3425.7,2508.14) (3393.06,2506.1) (3347.97,2519.77) (3295.98,2521.17) (3290.79,2524) (2528.22,2524) (2520.04,2520.28) (2460.72,2517.16) (2449.23,2514.29) (2498.71,2495.05) (2562.8,2465.91) (2581.29,2425.86) (2579.52,2392.16) (2590.17,2380.54) (2662.44,2355.4) (2694.77,2294.96) (2682.82,2251.96) (2681.19,2220.29) (2715.16,2216.59) (2772.98,2198.38) (2824.63,2174.81) (2880.83,2117.27) (2863.34,2068.7) (2798.34,2032.59) (2742.38,2018.17) (2746.53,2010.45) (2779.21,1984.84) (2830.69,1946.23) (2871.79,1917.35) (2901.19,1867.51) (2899.73,1813.77) (2854.18,1765.61) (2768.55,1733.62) (2770.38,1730.34) (2836.94,1692.84) (2886.43,1653.95) (2904.54,1598.13) (2895.27,1584.38) (3009.1,1661.96) (3232.59,1549.17) (3239.91,1553.57) (3329.38,1568.21) (3347,1553.23) (3347,1529.4) (3374.16,1489.7) (3424.69,1459.85) (3496.55,1443.51) (3501.03,1408.91) (3453.83,1384.28) (3390.57,1358.7) (3359.64,1344.53) (3330.57,1329.45) (3317.36,1307) (3357,1267.35) (3355.56,1244.8) (3311.68,1210.89) (3218.23,1200.98) (3138.6,1182.02) (3081.83,1153.63) (3042.41,1106.58) (2978.65,1053.21) (2888.32,1033.63) (2854.34,1038.27) (2825.04,1042.96) (2769.68,1032.41) (2719.26,1037.86) (2672.62,1033.08) (2625.09,1017.23) (2592.11,982.876) (2598.99,972.442) (2667.6,1017.71) (2909.97,932.297) (2922.3,772.101) (2907.99,756.183) (2539.05,745.267) (2464.49,829.636) (2459.4,806.738) (2441.09,795.781) (2443.5,776.989) (2412.9,757.788) (2413.77,756.4) (2455.05,735.758) (2564.09,682.755) (2565.82,653.548) (2472.68,593.667) (2418.04,593.667) (2323.9,653.717) (2322.21,678.925) (2413.77,756.4) (2412.9,757.788) (2344.81,715.066) (2279.45,782.47) (2283.1,806.919) (2322.87,828.861) (2257.1,847.342) (2248.15,870.915) (2300.26,951.969) (2468.65,917.02) (2480.01,899.453) (2478.57,892.987) (2598.99,972.442) (2592.11,982.876) (2528.09,968.824) (2399.6,961.353) (2384.58,977.73) (2276.71,975.099) (2235.67,1011.82) (2235.67,1061.31) (2264.92,1066.47) (2240.47,1079.63) (2221.55,1080.53) (2161.09,1066.79) (2013.98,1095.13) (1986.53,1092.85) (1911.17,1079.62) (1863.9,1073.24) (1746.66,1031.86) (1716.01,1037.6) (1664.26,1059.78) (1655.11,1102.49) (1685.19,1141.15) (1685.62,1141.31) (1685.35,1142.01) (1543.63,1208.97) (1536.9,1229.31) (1559.03,1270.81) (1631.87,1276.72) (1737.1,1192.53) (1733.89,1165.01) (1685.35,1142.01) (1685.62,1141.31) (1735.66,1160.69) (1805.06,1170.45) (1808.2,1193.99) (1811.43,1203.68) (1805.86,1200.83) (1627.16,1305.58)");

	//150127遇到的
	_testConvexification("(5730,30) (5730,1103.48) (5197.12,641.176) (5197.59,634.087) (5005.83,467.648)");
}

void BoidsPolygonEx::runTest()
{
	testProcessInnerRings();
	testConvexification();
}