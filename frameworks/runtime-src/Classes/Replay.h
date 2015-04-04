#ifndef __Boids_Replay_h_
#define __Boids_Replay_h_

#include <vector>
#include <map>
#include <string>
#include "cocos2d.h"
#include <cstdint>

//现在录像假设是固定帧率

class SnapshotUnit : public cocos2d::Point
{
public:
	double hp;
	SnapshotUnit(const cocos2d::Point& pt) : cocos2d::Point(pt), hp(-1.0)
	{
	}
	SnapshotUnit() 
	{
	}
};

class Snapshot : public std::map<uint16_t, SnapshotUnit>
{
public:
	unsigned random_i;

	std::vector<std::string> logs;
};

typedef std::tuple<uint32_t, double, double> tJoystick;
typedef std::tuple<uint32_t, uint16_t, double, double, double> tSkill;

class ReplayData
{
public:
	unsigned _frameRate, _snapshotInterval, _finishFrame;
	std::string _terrainHashHex;
	std::vector<tJoystick> joysticks; //因为数据来自lua，为避免和第一遍玩的时候不一致所以就直接存double
	std::vector<tSkill> skills;
	std::vector<Snapshot*> snapshots;

	void resetData();
};

//存这些key的目的：以后录像格式有微调时，只要这些key不变，那么以前的录像说不定还能放。
//当然放出来行为估计不一样，因为逻辑代码一直在改。但有时候也许不一定为了行为一样，可能只是为了测试，那么老录像只要还能起到测试作用，行为稍有变化也不需要重录
#define _REPLAY_FRAME_RATE (1)
#define _REPLAY_TERRAIN_HASH_HEX (2)
#define _REPLAY_SNAPSHOT_INTERVAL (3)
#define _REPLAY_FINISH_FRAME (4)

#define _REPLAY_META_DATA_COUNT (4)

class ReplayToSave : protected ReplayData
{
	friend class Utils;

private:
	unsigned _currentFrame;
	FILE* f;

	void saveMetaInt(uint16_t meta_key, uint32_t meta_value);
	void saveMetaString(uint16_t meta_key, const std::string& meta_value);
	void saveMetaValues();

	void log(const std::string& s);

	void saveSnapshot();

	std::vector<std::string> _logs_to_save;

public:
	ReplayToSave(int frame_rate, int snapshot_interval);
	void reset();
	void updateFrame();

	void setJoystick(double x, double y);
	void setSkill(int unit_id, double dir_x, double dir_y, double range);
	void setUnitHpToSnapshot(int unit_id, double hp);

	void save(std::string replay_name = "");
};

class ReplayToPlay : private ReplayData
{
	friend class Utils;
	friend class ReplayComparer;

private:
	bool _checkSnapshot;
	unsigned _currentFrame;
	FILE* f;

	unsigned readMetaInt();
	void readMetaString(std::string& str);
	void readMetaValues();

	std::vector<tJoystick>::const_iterator joysticks_iter;
	std::vector<tSkill>::const_iterator skills_iter;
	std::vector<Snapshot*>::const_iterator snapshots_iter;

	void log(const std::string& s);

	void compareSnapshot();

	std::map<int, double> _units_hp_to_check;
	std::vector<std::string> _logs_to_check;

public:
	ReplayToPlay(std::string replay_path, bool check_snapshot);
	void reset();
	void updateFrame();

	int getFrameRate();

	double getJoystickX();
	double getJoystickY();

	int getSkillPlayer();
	double getSkillDirX();
	double getSkillDirY();
	double getSkillRange();

	void checkUnitHpWithSnapshot(int unit_id, double hp);
};

#endif