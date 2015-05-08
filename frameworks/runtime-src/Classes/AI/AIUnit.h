#ifndef __Boids__AIUnit__
#define __Boids__AIUnit__

#include "AI.h"
#include "Collidable.h"
#include <memory>
#include "Path.h"
#include "../Macro.h"

enum MoveType
{
	typeBePushed = -1,
	typeStay = 0,
	typeInitiative = 1,
	typeStuck = 2 //此时_dir_want_to_go有值
};

class AIUnit : public Collidable
{
private:
	cocos2d::Point _position;
	int _hesitate_frame;
	cocos2d::Vec2 _last_walk_unit_dir;
	int _last_walk_faceto;
	int _last_faceto_frame;
	int _last_last_faceto; //上上个朝向。可以随意转向，但不能转到了一个朝向很快又转回来。相当于是有cd

	std::vector<AIUnit*> pushed_units; //推了谁都得记下来，如果最后没走成，那么所有相关的推操作都得回退。A推B，B又推了C，那么C记在B上。只是每帧有效
	std::vector<cocos2d::Point> position_before_pushed;
	bool tryBePushed(cocos2d::Point be_pushed_by_position, float be_pushed_by_radius);
	void restorePush(bool restore_self);
	void confirmPush();
	bool _tryWalk(cocos2d::Point& dest, float max_walk_length, bool enable_push, cocos2d::Vec2 original_dir, std::function<bool(cocos2d::Vec2, cocos2d::Vec2)> is_angle_ok, std::function<float(cocos2d::Vec2)> get_new_dir_length = nullptr);
	bool tryWalk(cocos2d::Point& dest, cocos2d::Vec2 original_dir);
	bool walkPos(cocos2d::Point dest);
	bool isOscillate(int faceto);
	int _already_hesitated;

	std::unique_ptr<class Path> path; //path只是个短期的事，长期的目的地还得另存(lua)

	static std::string serializeUnitsVector(const std::vector<AIUnit*>& vec);

	void paintPath();
	
	int _during_catchup; //会存发起catchup的帧号，析构path的时候如果发现能对得上，就置为0，否则不动
	
	void raw_findPath(cocos2d::Point dest, int path_validity_frame = INT_MAX);

	std::vector<std::pair<int, cocos2d::Point>> _pos_history; //调试用，本来是要包ifdef里的，但为了方便把define挪到cpp，这个就不管了，反正空vector也不怎么浪费

	//TODO 草的这部分还没和lua接上
	std::vector<std::string> grass_stepped_on;
	cocos2d::ValueVector getGrassStatus(); //就是grass_stepped_on包了下，且刚踩进草丛和离开草丛要标出来

	bool inRangeAndNotBlocked(AIUnit* other, float range);
	bool buildingInRangeAndNotBlocked(float building_x, float building_y, float building_radius, float range);
	
	//===实现Collidable的方法===
	int getPriority() const;
	bool willCollide(cocos2d::Point pos, float radius);
	bool willCollide(AIUnit* unit, cocos2d::Point unit_new_pos);
	bool getAdvisedNewDir(AIUnit* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir);

public:
	AIUnit();
	virtual ~AIUnit();

	void initialize(int unit_id);
	void deleteUnit();

	bool hasPath();
	void resetPath();
	bool isPathOutdated();
	bool isPathEnd();
	
	bool walkFrameOnPath(); //外部一般调的是这个，即在事先的路径上走一帧的路
	bool walkStep(float step_x, float step_y); //也可能调这个，即根据摇杆的方向走

	void findPath(float dest_x, float dest_y, int path_validity_frame = INT_MAX);
	void findPathWithDestRadius(float dest_x, float dest_y, float dest_radius, int path_validity_frame = INT_MAX);
	void findPathForTarget(AIUnit* target, int path_validity_frame = INT_MAX);
	void findPathForTargetWithRadius(AIUnit* target, float dest_radius, int path_validity_frame = INT_MAX);

	void resetLastWalk();
	void set_hesitate_frame(int hesitate_frame)
	{
		if (hesitate_frame == 0)
		{
			resetLastWalk();
		}
		_hesitate_frame = hesitate_frame;
	}

	void catchup(float dest_x, float dest_y);

    bool inDirectView(AIUnit* other);
	bool canAttack(AIUnit* other);
	bool buildingInDirectView(float building_x, float building_y, float building_radius);
	bool canAttackBuilding(float building_x, float building_y, float building_radius);

	//供lua直接访问的成员变量（设置进来就不变的）
	BOIDS_SYNTHESIZE_READONLY(int, _unit_id) // != 0的一个id

	//供lua直接访问的成员变量（有可能会变化的，于是要有set方法）
	cocos2d::Point get_position() const
	{
		return _position;
	}
	void set_position(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}
	BOIDS_SYNTHESIZE(float, _collide_radius)
	BOIDS_SYNTHESIZE(int, _priority) //lua那边每帧开始的时候会设置
	BOIDS_SYNTHESIZE(int, _priority2) //lua那边每帧开始的时候会设置
	BOIDS_SYNTHESIZE(float, _max_walk_length)
	BOIDS_SYNTHESIZE(std::string, _unit_camp)
	BOIDS_SYNTHESIZE(MoveType, _move_type)
	BOIDS_SYNTHESIZE_READONLY(cocos2d::Point, _dir_want_to_go)
	BOIDS_SYNTHESIZE_WRITEONLY(float, _guard_radius)
	BOIDS_SYNTHESIZE_WRITEONLY(float, _attack_radius)
	
	bool is_during_catchup()
	{
		return _during_catchup > 0;
	}
};

#endif
