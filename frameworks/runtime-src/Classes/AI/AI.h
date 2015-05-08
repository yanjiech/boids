#ifndef __Boids__AI__
#define __Boids__AI__

#include <set>
#include <string>
#include "cocos2d.h"
#include "AIUnit.h"
#include "../Macro.h"

class AI
{
	friend class AIUnit;
	friend class Border;
	friend class Terrain;
	friend class NavMesh;
	friend class Path;
	friend class ReplayToSave;
	friend class ReplayToPlay;
	friend class EditMode;

private:
	static AI* _instance;

	std::map<int, class AIUnit*> _units;
	int _BUILDING_PRIORITY;
	float _MINIMAL_SPLIT_DISTANCE;
	float _STOP_CATCHUP_DISTANCE;
	float _MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW;
	float _STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW;
	bool _PAINT_PATH;
	int _PAINT_MESH;
	bool _PAINT_STEP;
	int _TICK;
	bool _NOT_LOAD_NAV;
	bool _ENABLE_PUSH;

	cocos2d::DrawNode* _step_draw_node;
	std::pair<cocos2d::Point, cocos2d::Point> _narrow_dir;

//	std::vector<AIUnit*> extra_collidables;

public:
	AI();
	static AI* getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new AI();
		}
		return _instance;
	}

	void initialize(cocos2d::ValueMap constants);

	bool parseMap(std::string tmx_file_content, std::vector<float> all_collide_radius); //将所有可能的碰撞半径传入
	void releaseCurrentMap();

	BOIDS_SYNTHESIZE_READONLY(int, _current_frame)
	BOIDS_SYNTHESIZE_READONLY(std::vector<int>, _lead_group) //领先组
	BOIDS_SYNTHESIZE_READONLY(std::vector<int>, _fell_behind_group) //落后组
	BOIDS_SYNTHESIZE_READONLY(bool, _in_narrow)
	BOIDS_SYNTHESIZE_READONLY(cocos2d::Point, _catch_up_position)

	void reset() //再玩一次
	{
		_current_frame = 0;

		//释放那些Unit
//		_units.clear();
	}

	void updateFrame();

//	bool isPositionOK(cocos2d::Point pos, float radius);
//	
//	cocos2d::ValueVector getAvailablePosition(std::vector<float> radius, cocos2d::Rect region, int group_count = 1);

	void addCollidablePolygon(std::string name, std::vector<float> points_x, std::vector<float> points_y);
	void addCollidablePolygon(std::string name);
	bool removeCollidablePolygon(std::string name);

	void paintDebugLayer(); //要在MapLayer有了之后才能调

	bool splitIntoTwoGroups(std::string player_camp, float dir_x, float dir_y); //调了之后再分别get_group_a和b；如果都挨得还算近就返回false

	//测试代码
	void test_c_whole(int test_id);
	cocos2d::Point test1_pos(int t);
	int test2_int(int t);
};

#endif
