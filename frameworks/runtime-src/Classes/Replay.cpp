#include "Replay.h"
#include "Terrain.h"
#include "Trace.h"

using namespace cocos2d;

void ReplayData::resetData()
{
	joysticks.clear();
	skills.clear();
	for (auto snapshot_ptr : snapshots)
	{
		delete snapshot_ptr;
	}
	snapshots.clear();
}

ReplayToSave::ReplayToSave(int frame_rate, int snapshot_interval)
{
	_frameRate = frame_rate;
	_terrainHashHex = Terrain::getInstance()->getHashValueHex();
	_snapshotInterval = snapshot_interval;
	reset();

	Utils::replay_to_save = this;
}

void ReplayToSave::reset()
{
	resetData();
	_currentFrame = 0;
}

#define _SNAPSHOT_RANDOM_ (10000)

void ReplayToSave::saveSnapshot()
{
//	Snapshot* snapshot_ptr = new Snapshot();
//	snapshots.push_back(snapshot_ptr);
//
//	snapshot_ptr->random_i = Utils::random_i();
//
//	for (auto id_u : AI::getInstance()->_units)
//	{
//		snapshot_ptr->emplace(id_u.second->get_unit_id(), id_u.second->get_position());
//	}
//
//	snapshot_ptr->logs = _logs_to_save;
//	_logs_to_save.clear(); //save之后要清空本帧的logs
}

void ReplayToSave::updateFrame()
{
	_currentFrame++;

	if (_snapshotInterval && _currentFrame % _snapshotInterval == 0)
	{
		saveSnapshot();
	}
}

void ReplayToSave::setJoystick(double x, double y)
{
	assert(joysticks.empty() || std::get<0>(joysticks.back()) < _currentFrame && "reset joystick in a same frame");
	joysticks.emplace_back(_currentFrame, x, y);
}

void ReplayToSave::setSkill(int unit_id, double dir_x, double dir_y, double range)
{
	//assert(skills.empty() || std::get<0>(skills.back()) < _currentFrame && "more than one skill in a same frame");
	skills.emplace_back(_currentFrame, unit_id, dir_x, dir_y, range);
}

void ReplayToSave::setUnitHpToSnapshot(int unit_id, double hp)
{
	assert(snapshots.size() && "setUnitHpToSnapshot before we have a snapshot");
	Snapshot* snapshot_ptr = snapshots.back();
	auto it_unit = snapshot_ptr->find(unit_id);
	if (it_unit == snapshot_ptr->end())
	{
		assert(false && "some unit doesn't exist in c but exist in lua");
	}
	else
	{
		it_unit->second.hp = hp;
	}
}

void ReplayToSave::log(const std::string& s)
{
	_logs_to_save.push_back(s);
}

void ReplayToSave::saveMetaInt(uint16_t meta_key, uint32_t meta_value)
{
	fwrite(&meta_key, sizeof(meta_key), 1, f);
	fwrite(&meta_value, sizeof(meta_value), 1, f);
}

void ReplayToSave::saveMetaString(uint16_t meta_key, const std::string& meta_value)
{
	fwrite(&meta_key, sizeof(meta_key), 1, f);
	uint16_t string_size = meta_value.size();
	fwrite(&string_size, sizeof(string_size), 1, f);
	fwrite(meta_value.c_str(), sizeof(char), string_size, f);
}

void ReplayToSave::saveMetaValues()
{
	uint16_t meta_data_count = _REPLAY_META_DATA_COUNT;
	fwrite(&meta_data_count, sizeof(meta_data_count), 1, f);

	saveMetaInt(_REPLAY_FRAME_RATE, _frameRate);
	saveMetaString(_REPLAY_TERRAIN_HASH_HEX, _terrainHashHex);
	saveMetaInt(_REPLAY_SNAPSHOT_INTERVAL, _snapshotInterval);
	saveMetaInt(_REPLAY_FINISH_FRAME, _finishFrame);
}

void ReplayToSave::save(std::string replay_name)
{
	_finishFrame = _currentFrame;

	if (replay_name.empty())
	{
		replay_name = Utils::getFileNameByTime();
	}
	std::string replay_save_path = cocos2d::FileUtils::getInstance()->getWritablePath() + replay_name + ".rep";
	f = fopen(replay_save_path.c_str(), "wb");
	assert(f && "failed to open file for saving replay");

	saveMetaValues();

	uint32_t joysticks_size = joysticks.size();
	fwrite(&joysticks_size, sizeof(uint32_t), 1, f);
	for (const auto& joystick : joysticks)
	{
		fwrite(&std::get<0>(joystick), sizeof(uint32_t), 1, f);
		fwrite(&std::get<1>(joystick), sizeof(double), 1, f);
		fwrite(&std::get<2>(joystick), sizeof(double), 1, f);
	}

	uint32_t skills_size = skills.size();
	fwrite(&skills_size, sizeof(uint32_t), 1, f);
	for (const auto& skill : skills)
	{
		fwrite(&std::get<0>(skill), sizeof(uint32_t), 1, f);
		fwrite(&std::get<1>(skill), sizeof(uint16_t), 1, f);
		fwrite(&std::get<2>(skill), sizeof(double), 1, f);
		fwrite(&std::get<3>(skill), sizeof(double), 1, f);
		fwrite(&std::get<4>(skill), sizeof(double), 1, f);
	}

	uint32_t snapshots_size = snapshots.size();
	fwrite(&snapshots_size, sizeof(uint32_t), 1, f);
	for (auto snapshot_ptr : snapshots)
	{
		const auto& snapshot = *snapshot_ptr;

		fwrite(&snapshot_ptr->random_i, sizeof(uint32_t), 1, f);

		uint16_t unit_count = snapshot.size();
		fwrite(&unit_count, sizeof(uint16_t), 1, f);
		for (const auto& snapshot_unit : snapshot)
		{
			fwrite(&snapshot_unit.first, sizeof(uint16_t), 1, f);
			fwrite(&snapshot_unit.second.x, sizeof(float), 1, f);
			fwrite(&snapshot_unit.second.y, sizeof(float), 1, f);
			fwrite(&snapshot_unit.second.hp, sizeof(double), 1, f);
		}

		uint16_t logs_count = snapshot.logs.size();
		fwrite(&logs_count, sizeof(uint16_t), 1, f);
		for (const std::string& s : snapshot.logs)
		{
			uint16_t string_size = s.size();
			fwrite(&string_size, sizeof(string_size), 1, f);
			fwrite(s.c_str(), sizeof(char), string_size, f);
		}
	}

	fclose(f);

	Utils::logAI("=====replay[%s] save finish.=====", replay_name.c_str());
}

unsigned ReplayToPlay::readMetaInt()
{
	uint32_t meta_value;
	fread(&meta_value, sizeof(uint32_t), 1, f);
	return meta_value;
}

void ReplayToPlay::readMetaString(std::string& str)
{
	uint16_t string_size;
	fread(&string_size, sizeof(uint16_t), 1, f);
	str.resize(string_size);
	fread((char*)str.c_str(), sizeof(char), string_size, f);
}

void ReplayToPlay::readMetaValues()
{
	uint16_t meta_data_count;
	if (fread(&meta_data_count, sizeof(uint16_t), 1, f) != 1)
	{
		cocos2d::log("replay: read meta_data_count error");
		return;
	}

	//赋上默认值，如果录像里没读到就用默认值
	_frameRate = 30;
	_snapshotInterval = 0;
	_finishFrame = 0;
	for (unsigned i = 0; i != meta_data_count; ++i)
	{
		uint16_t meta_key;
		if (fread(&meta_key, sizeof(uint16_t), 1, f) != 1)
		{
			cocos2d::log("replay: read meta_key error");
		}
		switch (meta_key)
		{
		case _REPLAY_FRAME_RATE:
			_frameRate = readMetaInt();
			break;
		case _REPLAY_TERRAIN_HASH_HEX:
			readMetaString(_terrainHashHex);
			if (_terrainHashHex != Terrain::getInstance()->getHashValueHex())
			{
				cocos2d::log("WARNING: terrain_hash_hex not match. current: %s, in_replay: %s", Terrain::getInstance()->getHashValueHex().c_str(), _terrainHashHex.c_str());
			}
			break;
		case _REPLAY_SNAPSHOT_INTERVAL:
			_snapshotInterval = readMetaInt();
			break;
		case _REPLAY_FINISH_FRAME:
			_finishFrame = readMetaInt();
			break;
		default:
			cocos2d::log("replay: unrecognized meta_key: %d", meta_key);
			break;
		}
	}

	cocos2d::log("_frameRate: %d, _snapshotInterval: %d, _finishFrame: %d", _frameRate, _snapshotInterval, _finishFrame);
}

ReplayToPlay::ReplayToPlay(std::string replay_path, bool check_snapshot) : _checkSnapshot(check_snapshot)
{
	BEGIN_TRACE;

	f = fopen((FileUtils::getInstance()->getWritablePath() + replay_path).c_str(), "rb");
	assert(f && "failed to open file for loading replay");

	readMetaValues();

	cocos2d::log("check snapshot: %d", _checkSnapshot);

	uint32_t joysticks_size;
	fread(&joysticks_size, sizeof(uint32_t), 1, f);
	cocos2d::log("joysticks_size: %d", joysticks_size);
	for (unsigned i = 0; i != joysticks_size; ++i)
	{
		joysticks.emplace_back();
		auto& joystick = joysticks.back();
		fread(&std::get<0>(joystick), sizeof(uint32_t), 1, f);
		fread(&std::get<1>(joystick), sizeof(double), 1, f);
		fread(&std::get<2>(joystick), sizeof(double), 1, f);
	}

	uint32_t skills_size;
	fread(&skills_size, sizeof(uint32_t), 1, f);
	cocos2d::log("skills_size: %d", skills_size);
	for (unsigned i = 0; i != skills_size; ++i)
	{
		skills.emplace_back();
		auto& skill = skills.back();
		fread(&std::get<0>(skill), sizeof(uint32_t), 1, f);
		fread(&std::get<1>(skill), sizeof(uint16_t), 1, f);
		fread(&std::get<2>(skill), sizeof(double), 1, f);
		fread(&std::get<3>(skill), sizeof(double), 1, f);
		fread(&std::get<4>(skill), sizeof(double), 1, f);
	}

	if (_checkSnapshot)
	{
		uint32_t snapshots_size;
		fread(&snapshots_size, sizeof(uint32_t), 1, f);
		cocos2d::log("snapshots_size: %d", snapshots_size);
		for (unsigned i = 0; i != snapshots_size; ++i)
		{
			Snapshot* snapshot_ptr = new Snapshot();
			snapshots.push_back(snapshot_ptr);

			fread(&snapshot_ptr->random_i, sizeof(uint32_t), 1, f);

			uint16_t unit_count;
			fread(&unit_count, sizeof(uint16_t), 1, f);
			for (unsigned j = 0; j != unit_count; ++j)
			{
				uint16_t unit_id;
				fread(&unit_id, sizeof(uint16_t), 1, f);
				SnapshotUnit snap_unit;
				fread(&snap_unit.x, sizeof(float), 1, f);
				fread(&snap_unit.y, sizeof(float), 1, f);
				fread(&snap_unit.hp, sizeof(double), 1, f);

				snapshot_ptr->emplace(unit_id, snap_unit);
			}

			uint16_t logs_count;
			fread(&logs_count, sizeof(uint16_t), 1, f);
			for (unsigned j = 0; j != logs_count; ++j)
			{
				uint16_t string_size;
				fread(&string_size, sizeof(string_size), 1, f);
				std::string str(string_size, 0);
				fread((char*)str.c_str(), sizeof(char), string_size, f);
				snapshot_ptr->logs.push_back(str);
			}
		}
	}

	fclose(f);

	reset();

	Utils::replay_to_play = this;

	TRACE("loadReplay");
}

void ReplayToPlay::reset()
{
	_currentFrame = 0;
	joysticks_iter = joysticks.cbegin();
	skills_iter = skills.cbegin();
	snapshots_iter = snapshots.cbegin();
}

void ReplayToPlay::compareSnapshot()
{
//	Snapshot& snapshot = **snapshots_iter;
//
//	if (snapshot.logs.size() != _logs_to_check.size())
//	{
//		Utils::logAI("replay inconsistent. logs_count: %d, expected: %d", _logs_to_check.size(), snapshot.logs.size());
//		for (unsigned i = 0; i != snapshot.logs.size(); ++i)
//		{
//			if (i < _logs_to_check.size())
//			{
//				cocos2d::log("log #%d: %s; expected: %s", i, _logs_to_check[i].c_str(), snapshot.logs[i].c_str());
//			}
//			else
//			{
//				cocos2d::log("log #%d in snapshot: %s", i, snapshot.logs[i].c_str());
//			}
//		}
//	}
//	else
//	{
//		for (unsigned i = 0; i != snapshot.logs.size(); ++i)
//		{
//			if (snapshot.logs[i] != _logs_to_check[i])
//			{
//				cocos2d::log("log #%d in snapshot: %s", i, snapshot.logs[i].c_str());
//			}
//		}
//	}
//	_logs_to_check.clear(); //compare之后要清空本帧的logs
//
//	const auto& current_units = AI::getInstance()->_units;
//
//	if (snapshot.size() != current_units.size())
//	{
//		Utils::logAI("replay inconsistent. unit_count: %d, expected: %d", current_units.size(), snapshot.size());
//	}
//
//	unsigned random_i = Utils::random_i();
//	if (random_i != snapshot.random_i)
//	{
//		Utils::logAI("replay inconsistent. random_i: %d, expected_random_i: %d", random_i, snapshot.random_i);
//	}
//	
//	for (auto id_u : current_units)
//	{
//		const auto& it_unit_in_snapshot = snapshot.find(id_u.second->get_unit_id());
//		if (it_unit_in_snapshot == snapshot.end())
//		{
//			Utils::logAI("replay inconsistent. unit %d not found in the snapshot", id_u.second->get_unit_id());
//		}
//		else
//		{
//			const auto& unit_in_snapshot = it_unit_in_snapshot->second;
//			const auto& unit_pos = id_u.second->get_position();
//			if (!unit_in_snapshot.fuzzyEquals(unit_pos, 1e-4f))
//			{
//				Utils::logAI("replay inconsistent. unit_id : %d, pos : %.6f, %.6f, expected_pos : %.6f, %.6f",
//					id_u.second->get_unit_id(), unit_pos.x, unit_pos.y, unit_in_snapshot.x, unit_in_snapshot.y);
//
//				//DEBUG
//				auto next_snapshot_iter = snapshots_iter;
//				next_snapshot_iter++;
//				Snapshot& next_snapshot = **next_snapshot_iter;
//				auto unit_in_next_snap_iter = next_snapshot.find(id_u.second->get_unit_id());
//				if (unit_in_next_snap_iter != next_snapshot.end())
//				{
//					cocos2d::log("next_pos: %.6f, %.6f", unit_in_next_snap_iter->second.x, unit_in_next_snap_iter->second.y);
//				}
//			}
//			const auto& unit_hp = _units_hp_to_check.find(id_u.second->get_unit_id());
//			assert(unit_hp != _units_hp_to_check.end() && "unit exist in c module but not exist in lua");
//			if (fabs(unit_in_snapshot.hp - unit_hp->second) > 1e-4f)
//			{
//				Utils::logAI("replay inconsistent. unit_id : %d, hp : %.6f, expected_hp : %.6f",
//					id_u.second->get_unit_id(), unit_hp->second, unit_in_snapshot.hp);
//			}
//		}
//	}
}

void ReplayToPlay::log(const std::string& s)
{
	if (_checkSnapshot)
	{
		_logs_to_check.push_back(s);
	}
}

void ReplayToPlay::checkUnitHpWithSnapshot(int unit_id, double hp)
{
	_units_hp_to_check[unit_id] = hp;
}

void ReplayToPlay::updateFrame()
{
	_currentFrame++;

	if (_currentFrame > _finishFrame)
	{
		cocos2d::log("Replay finished.total: %d, current: %d", _finishFrame, _currentFrame);
		return;
	}

	if (joysticks_iter != joysticks.cend() && _currentFrame > std::get<0>(*joysticks_iter))
	{
		joysticks_iter++;
	}

	if (skills_iter != skills.cend() && _currentFrame > std::get<0>(*skills_iter))
	{
		skills_iter++;
	}

	if (_checkSnapshot && _snapshotInterval && snapshots_iter != snapshots.end() && _currentFrame % _snapshotInterval == 0)
	{		
		compareSnapshot();
		snapshots_iter++;
	}
}

int ReplayToPlay::getFrameRate()
{
	return _frameRate;
}

double ReplayToPlay::getJoystickX()
{
	if (joysticks_iter != joysticks.cend() && _currentFrame == std::get<0>(*joysticks_iter))
	{
		return std::get<1>(*joysticks_iter);
	}
	return 0.0;
}

double ReplayToPlay::getJoystickY()
{
	if (joysticks_iter != joysticks.cend() && _currentFrame == std::get<0>(*joysticks_iter))
	{
		return std::get<2>(*joysticks_iter);
	}
	return 0.0;
}

int ReplayToPlay::getSkillPlayer()
{
	if (skills_iter != skills.cend() && _currentFrame == std::get<0>(*skills_iter))
	{
		return std::get<1>(*skills_iter);
	}
	return 0;
}

double ReplayToPlay::getSkillDirX()
{
	if (skills_iter != skills.cend() && _currentFrame == std::get<0>(*skills_iter))
	{
		return std::get<2>(*skills_iter);
	}
	return 0.0;
}

double ReplayToPlay::getSkillDirY()
{
	if (skills_iter != skills.cend() && _currentFrame == std::get<0>(*skills_iter))
	{
		return std::get<3>(*skills_iter);
	}
	return 0.0;
}

double ReplayToPlay::getSkillRange()
{
	if (skills_iter != skills.cend() && _currentFrame == std::get<0>(*skills_iter))
	{
		return std::get<4>(*skills_iter);
	}
	return 0.0;
}