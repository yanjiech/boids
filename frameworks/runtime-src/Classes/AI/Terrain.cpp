#include "Terrain.h"
#include "TmxMap.h"
#include "TmxObjectGroup.h"
#include "cocos2d.h"
#include <algorithm>
#include "LuaFunc.h"
#include "Utils.h"
#include "Trace.h"
//#include "AI.h"
#include "BoidsPolygon.h"
#include "TmxPolyline.h"

USING_NS_CC;

bool Terrain::loadTmx(const std::string& tmx_content, class GrassMonitor& grass_monitor)
{
	Tmx::Map map;
	map.ParseText(tmx_content);
	if (map.HasError())
	{
		return false;
	}

	map_width = map.GetTileWidth() * map.GetWidth();
	map_height = map.GetTileHeight() * map.GetHeight();

	bool movable_found = false;
	
	const std::vector< Tmx::ObjectGroup* > & groups = map.GetObjectGroups();
	for (auto group : groups)
	{
		if (group->GetName() == "physics")
		{
			const std::vector< Tmx::Object* > & objects = group->GetObjects();
			for (auto ob : objects)
			{
				if (ob->GetType() == "movable")
				{
					if (!movable_found)
					{
						movable_found = true;

						movable_width = ob->GetWidth();
						movable_height = ob->GetHeight();
						movable_x = ob->GetX();
						movable_y = map_height - ob->GetY() - movable_height;
					}
					else
					{
						log("more than one movable.");
						return false;
					}
				}
				else if (ob->GetType() == "collide" || ob->GetType() == "no-go")
				{
					barriers.push_back(new BoidsPolygon());
					barriers.back()->loadFromTmxObject(ob, map_height);

					if (ob->GetType() == "collide")
					{
						sight_blockers.push_back(barriers.back());
					}
				}
				else if (ob->GetType() == "dynamic")
				{
					auto p = new BoidsPolygon();
					p->loadFromTmxObject(ob, map_height);
					potential_barriers[p->name] = p;	
					p->makeSureAnticlockwise();
				}
				else
				{
					log("[ERROR] unrecognized physics type: [%s]", ob->GetType().c_str());
				}
			}
		}
		else if (group->GetName() == "tips")
		{
			const std::vector< Tmx::Object* > & objects = group->GetObjects();
			for (auto ob : objects)
			{
				if (ob->GetType() == "narrow")
				{
					auto poly = new BoidsPolygon();
					poly->loadFromTmxObject(ob, map_height);
					narrows.push_back(poly);
				}
				else if (ob->GetType() == "narrow_dir")
				{
					std::string name = ob->GetName();
					float base_x = ob->GetX();
					float base_y = ob->GetY();
					auto polyline = ob->GetPolyline();
					if (polyline)
					{
						if (polyline->GetNumPoints() > 2)
						{
							cocos2d::log("[ERROR] narrow_dir should only contains two points. name: %s", name.c_str());
						}

						cocos2d::Point ccpt[2];
						for (int i = 0; i < 2; ++i)
						{
							const Tmx::Point& pt = polyline->GetPoint(i);
							ccpt[i].set(base_x + pt.x, map_height - (base_y + pt.y));
						}
						if (!narrows_dir.emplace(name, std::make_pair(ccpt[0], ccpt[1])).second)
						{
							cocos2d::log("[ERROR] two narrow_dir have a same name: %s", name.c_str());
						}
					}
					else
					{
						cocos2d::log("[ERROR] narrow_dir should be a polyline. name: %s", name.c_str());
					}
				}
				else if (ob->GetType() == "grass")
				{
					auto poly = new BoidsPolygon();
					poly->loadFromTmxObject(ob, map_height);
					grass_monitor.add(poly);
				}
				else
				{
					log("[ERROR] unrecognized tips type: [%s]", ob->GetType().c_str());
				}
			}
		}
	}

	if (!movable_found)
	{
		log("movable not found, use the whole map: %f, %f", map_width, map_height);
		movable_width = map_width;
		movable_height = map_height;
		movable_x = 0;
		movable_y = 0;
	}

	log("totally %d barriers, %d sight_blockers.", barriers.size(), sight_blockers.size());

	return true;
}

void Terrain::insertSightBlocker(polygon::ring_type& ring)
{
	unsigned ring_size = ring.size();
	for (unsigned i = 0; i != ring_size; ++i)
	{
		rtree_sight_blockers.insert(segment(ring[(i + 1) % ring_size], ring[i])); //存着的顺序是逆时针序(外圈)，我们需要的是顺时针序，所以要反一反
	}
}

void Terrain::mergeSightBlockers()
{
	/*boost::geometry::model::multi_polygon<polygon> mp;
	for (unsigned int i = 0; i < sight_blockers.size(); ++i)
	{
		polygon to_merge;
		initializeBoostPolygon(*sight_blockers[i], to_merge);

		boost::geometry::model::multi_polygon<polygon> mp_output;
		boost::geometry::union_(mp, to_merge, mp_output);
		mp = mp_output;
	}
	log("blockers count after union: %d", mp.size());

	for (unsigned i = 0; i != mp.size(); ++i)
	{
		insertSightBlocker(mp[i].outer());

		//处理还在new_blocker中心的可行区域
		auto& boost_inner_rings = mp[i].inners();
		if (boost_inner_rings.size())
		{
			std::vector<BoidsPolygonEx*> inner_rings;
			for (unsigned int j = 0; j != boost_inner_rings.size(); ++j)
			{
				insertSightBlocker(boost_inner_rings[j]);
			}
		}
	}*/

	//为了加快debug版的运行速度，不对sight_blocker进行合并，应该相交的也不是太多
	for (unsigned int i = 0; i < sight_blockers.size(); ++i)
	{
		insertSightBlocker(sight_blockers[i]);
	}
}

void Terrain::insertSightBlocker(BoidsPolygon* p)
{
	p->foreachelement([=](PolygonPointPtr ptr) {
		rtree_sight_blockers.insert(toBoostSeg2(ptr)); //存着的顺序是逆时针序，我们需要的是顺时针序，所以要反一反，调用2号方法
	});
}

void Terrain::removeSightBlocker(BoidsPolygon* p)
{
	p->foreachelement([=](PolygonPointPtr ptr) {
		rtree_sight_blockers.remove(toBoostSeg2(ptr)); //存着的顺序是逆时针序，我们需要的是顺时针序，所以要反一反，调用2号方法
	});
}

void Terrain::getHashValue(const std::string& tmx_file_content, const std::set<float>& all_collide_radius)
{
#define HASH_VALUE_LENGTH (32)
	unsigned char hash_value[HASH_VALUE_LENGTH];
	memset(hash_value, 0, sizeof(hash_value));
	unsigned i = 0;
	for (; i + HASH_VALUE_LENGTH <= tmx_file_content.size(); i += HASH_VALUE_LENGTH)
	{
		for (unsigned j = 0; j != HASH_VALUE_LENGTH; ++j)
		{
			hash_value[j] ^= tmx_file_content[i + j];
		}
	}
	unsigned j = 0;
	for (; i + j < tmx_file_content.size(); ++j)
	{
		hash_value[j] ^= tmx_file_content[i + j];
	}
	for (auto radius : all_collide_radius)
	{
		const char* r = (const char*)(&radius);
		for (unsigned t = 0; t != sizeof(float); ++t)
		{
			if (j == HASH_VALUE_LENGTH) j = 0;
			hash_value[j++] ^= r[t];
		}
	}
	
	hash_value_hex = Utils::encodeHex(hash_value, HASH_VALUE_LENGTH);
}

bool Terrain::parseFromBinary(FILE* f)
{
	uint32_t mesh_count;
	if (fread(&mesh_count, 4, 1, f) == 1)
	{
		for (unsigned i = 0; i != mesh_count; ++i)
		{
			auto mesh = new NavMesh();
			if (!mesh->parseFromBinary(f))
			{
				delete mesh;
				return false;
			}
			meshes[mesh->_unit_collide_radius] = mesh;
		}
		return true;
	}
	return false;
}

bool Terrain::parseMap(const std::string& tmx_file_content, const std::set<float>& all_collide_radius)
{
	Trace t;
	grass_monitor.clear();
	if (!loadTmx(tmx_file_content, grass_monitor))
	{
		return false;
	}
	t.trace("loadTmx");

	mergeSightBlockers(); //时间高的话也挪到后面去，存二进制
	t.trace("mergeSightBlockers");

	getHashValue(tmx_file_content, all_collide_radius);

	std::string file_path = FileUtils::getInstance()->getWritablePath() + hash_value_hex + ".nav";

	log("file path: %s", file_path.c_str());

	bool ret = false;
	FILE* f = fopen(file_path.c_str(), "rb");

//	if (!AI::getInstance()->_NOT_LOAD_NAV && f)
    if( f )
	{
		ret = parseFromBinary(f);
		fclose(f);

		if (!ret)
		{
			log("%s.nav exsit but parse failed", hash_value_hex.c_str());
		}
	}

	if (!ret)
	{
		f = fopen(file_path.c_str(), "wb");
		generateMeshAndSaveBinary(all_collide_radius, f);
		fclose(f);
	}

	return true;
}

void Terrain::generateMeshAndSaveBinary(const std::set<float>& all_collide_radius, FILE* f)
{	
	if (f)
	{
		uint32_t mesh_count = all_collide_radius.size();
		fwrite(&mesh_count, 4, 1, f);
	}

	//根据不同的半径生成不同的NavMesh
	for (float radius : all_collide_radius)
	{
		BoidsPolygon shrinked_movable;	//根据半径收缩movable的边界
		shrinked_movable.initializeAsRectangle(movable_x + radius, movable_y + radius, movable_width - radius - radius, movable_height - radius - radius);
		auto mesh = new NavMesh(shrinked_movable, barriers, radius, f);
		meshes[radius] = mesh;
	}
}

void Terrain::release()
{
	for (unsigned int i = 0; i < barriers.size(); ++i)
	{
		delete barriers[i];
	}
	barriers.clear();

	for (auto key_and_poly : potential_barriers)
	{
		delete key_and_poly.second;
	}
	potential_barriers.clear();
	sight_blockers.clear();

	for (auto kvp : meshes)
	{
		delete kvp.second;
	}

	for (auto narrow : narrows)
	{
		delete narrow;
	}
	narrows.clear();
}

NavMesh* Terrain::getMeshByUnitRadius(float radius)
{
	auto it = meshes.find(radius);
	assert(it != meshes.end());

	return it->second;
}

void Terrain::drawSketchOn(DrawNode* node)
{
	for (unsigned int i = 0; i < barriers.size(); ++i)
	{
		barriers[i]->drawSketchOn(node);
	}
}

void Terrain::paintDebugLayer(cocos2d::Node* node_parent)
{
	auto draw_node = DrawNode::create();
	if (node_parent)
	{
		node_parent->addChild(draw_node);
	}
	else
	{
		Utils::getCurrentMapLayer()->addChild(draw_node);
	}

	//drawSketchOn(draw_node);

	//看需要画指定半径的NavMesh
//	if (AI::getInstance()->_PAINT_MESH)
//	{
		getMeshByUnitRadius(30.0)->drawSketchOn(draw_node);
//	}

	/*BoidsPolygon p1("(5730,30) (5730,1103.48) (5197.12,641.176) (5197.59,634.087) (5005.83,467.648)");
	extra_polygons_to_draw.push_back(&p1);*/
	//extra_polygons_to_draw.push_back(&p2);

	for (auto extra_poly : extra_polygons_to_draw)
	{
		extra_poly->drawSketchOn(draw_node, cocos2d::Color4F::MAGENTA);
	}
	extra_polygons_to_draw.clear();	
}

bool Terrain::isBlocked(cocos2d::Point src, cocos2d::Point dst)
{
	for (rtree_seg::const_query_iterator it = rtree_sight_blockers.qbegin(
		bgi::intersects(Geometry::getBoundingBox(src, dst))); it != rtree_sight_blockers.qend(); ++it)
	{
		cocos2d::Point seg_a = Utils::toCocosPt(it->first), seg_b = Utils::toCocosPt(it->second);

		if (cocos2d::Vec2::isSegmentIntersect(seg_a, seg_b, src, dst) || cocos2d::Vec2::isSegmentOverlap(seg_a, seg_b, src, dst))
		{
			return true;
		}
	}
	return false;
}

void Terrain::addCollidablePolygon(BoidsPolygon* poly)
{
	for (auto radius_and_mesh : meshes)
	{
		auto enlarged_poly = poly->getEnlargedPolygon(radius_and_mesh.first);
		enlarged_poly->reverse(); //对于内部的多边形，是要反过来插入的；即多边形实质上得是顺时针顺序的。得enlarge完再reverse，否则不对
		radius_and_mesh.second->addCollidablePolygon(*enlarged_poly);

		//extra_polygons_to_draw.push_back(enlarged_poly);
		delete enlarged_poly;
	}

	insertSightBlocker(poly);
}

void Terrain::addCollidablePolygon(const std::string& name, const std::vector<float>& points_x, const std::vector<float>& points_y)
{
	BoidsPolygon* poly = new BoidsPolygon();
	poly->name = name;
	for (unsigned i = 0; i != points_x.size(); ++i)
	{
		poly->addNewVertex(points_x[i], points_y[i]);
	}
	poly->makeSureAnticlockwise();

	potential_barriers[name] = poly;

	addCollidablePolygon(poly);
}

void Terrain::addCollidablePolygon(const std::string& name)
{
	auto it_poly = potential_barriers.find(name);
	if (it_poly != potential_barriers.end())
	{
		addCollidablePolygon(it_poly->second);
	}
	else
	{
		cocos2d::log("when add, dynamic polygon %s not found", name.c_str());
	}
}

bool Terrain::removeCollidablePolygon(const std::string& name)
{
	bool ret = true;
	for (auto radius_and_mesh : meshes)
	{
		if (!radius_and_mesh.second->removeCollidablePolygon(name + BoidsPolygon::_enlarged_name_suffix))
		{
			ret = false;
		}
	}

	auto it_poly = potential_barriers.find(name);
	if (it_poly != potential_barriers.end())
	{
		removeSightBlocker(it_poly->second);
	}
	else
	{
		cocos2d::log("when remove, dynamic polygon %s not found", name.c_str());
		return false;
	}

	return ret;
}

bool Terrain::isInNarrowPlace(const cocos2d::Point& pt)
{
	for (auto narrow : narrows)
	{
		if (narrow->coversPoint(pt))
		{
//			auto it_dir = narrows_dir.find(narrow->name);
//			if (it_dir != narrows_dir.end())
//			{
//				AI::getInstance()->_narrow_dir = it_dir->second;
//			}
//			else
//			{
//				cocos2d::log("[ERROR] not found corresponding narrow_dir: %s", narrow->name.c_str());
//				AI::getInstance()->_narrow_dir.first.set(cocos2d::Point::ZERO);
//			}
			return true;
		}
	}
	return false;
}