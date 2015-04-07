#ifndef __Boids_Terrain__
#define __Boids_Terrain__

#include "Singleton.h"
#include <string>
#include "BoidsPolygon.h"
#include <vector>
#include "TmxObject.h"
#include "NavMesh.h"
#include "GrassMonitor.h"
#include "BoostGeometryDef.h"

typedef bgi::rtree< segment, bgi::rstar<16, 4> > rtree_seg;

class Terrain : public Singleton<Terrain>
{
	friend class NavMesh;

protected:
	std::vector<BoidsPolygon*> barriers, sight_blockers;
	rtree_seg rtree_sight_blockers;
	float map_width, map_height, movable_x, movable_y, movable_width, movable_height;
	std::map<float, NavMesh*> meshes;
	std::map<std::string, BoidsPolygon*> potential_barriers;
	std::string hash_value_hex;
	std::vector<BoidsPolygon*> narrows;
	std::map<std::string, std::pair<cocos2d::Point, cocos2d::Point>> narrows_dir;

	void getHashValue(const std::string& tmx_file_content, const std::set<float>& all_collide_radius);
	bool parseFromBinary(FILE* f);
	bool loadTmx(const std::string& tmx_content, class GrassMonitor& grass_monitor);
	void insertSightBlocker(polygon::ring_type& ring);
	void mergeSightBlockers();
	void insertSightBlocker(BoidsPolygon* p);
	void removeSightBlocker(BoidsPolygon* p);
	void generateMeshAndSaveBinary(const std::set<float>& all_collide_radius, FILE* f = nullptr);

	void addCollidablePolygon(BoidsPolygon* p);

	std::vector<BoidsPolygon*> extra_polygons_to_draw;
	void drawSketchOn(cocos2d::DrawNode* node);

public:
	bool parseMap(const std::string& tmx_file_content, const std::set<float>& all_collide_radius);
	void release();

	std::string getHashValueHex()
	{
		return hash_value_hex;
	}

	GrassMonitor grass_monitor;

	void paintDebugLayer(cocos2d::Node* node_parent = nullptr);

	bool isBlocked(cocos2d::Point src, cocos2d::Point dst);

	NavMesh* getMeshByUnitRadius(float radius);
    
    std::map<float, NavMesh*>& getMeshes() { return meshes; }

	void addCollidablePolygon(const std::string& name, const std::vector<float>& points_x, const std::vector<float>& points_y);
	void addCollidablePolygon(const std::string& name);
	bool removeCollidablePolygon(const std::string& name);

	bool isInNarrowPlace(const cocos2d::Point& pt);
};

#endif