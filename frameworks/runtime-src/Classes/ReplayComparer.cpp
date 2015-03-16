#include "ReplayComparer.h"
#include "Replay.h"
#include "Utils.h"

bool compareSnapshot(unsigned frame, const Snapshot* a, const Snapshot* b)
{
	bool ret = true;
	if (a->logs.size() != b->logs.size())
	{
		cocos2d::log("[%d] replay inconsistent. a->logs_count: %d, b->logs_count: %d", frame, a->logs.size(), b->logs.size());
		ret = false;
		for (unsigned i = 0; i != a->logs.size(); ++i)
		{
			if (i < b->logs.size())
			{
				cocos2d::log("#%d, a->log: %s; b->log: %s", i, a->logs[i].c_str(), b->logs[i].c_str());
			}
			else
			{
				cocos2d::log("log #%d in snapshot_a: %s", i, a->logs[i].c_str());
			}
		}
		for (unsigned i = a->logs.size(); i < b->logs.size(); ++i)
		{
			cocos2d::log("log #%d in snapshot_b: %s", i, b->logs[i].c_str());
		}
	}
	else
	{
		for (unsigned i = 0; i != a->logs.size(); ++i)
		{
			if (a->logs[i] != b->logs[i])
			{
				ret = false;
				cocos2d::log("[%d] #%d, a->log: %s; b->log: %s", frame, i, a->logs[i].c_str(), b->logs[i].c_str());
			}
			else
			{
				cocos2d::log("[%d] (same) #%d: %s", frame, i, a->logs[i].c_str());
			}
		}
	}

	if (a->size() != b->size())
	{
		ret = false;
		cocos2d::log("[%d] replay inconsistent. a_unit_count: %d, b_unit_count: %d", frame, a->size(), b->size());
	}

	if (a->random_i != b->random_i)
	{
		ret = false;
		cocos2d::log("[%d] replay inconsistent. a_random_i: %d, b_random_i: %d", frame, a->random_i, b->random_i);
	}

	for (auto id_u_in_a : *a) //取snapshot_a中的元素，和b中的对比
	{
		const auto& it_unit_in_b = b->find(id_u_in_a.first);
		if (it_unit_in_b == b->end())
		{
			ret = false;
			cocos2d::log("[%d] replay inconsistent. unit %d in a not found in b", frame, id_u_in_a.first);
		}
		else
		{
			const auto& unit_in_a = id_u_in_a.second;
			const auto& unit_in_b = it_unit_in_b->second;
			if (!unit_in_b.fuzzyEquals(unit_in_a, 1e-4f))
			{
				ret = false;
				cocos2d::log("[%d] replay inconsistent. unit_id : %d, pos_in_a : %.6f, %.6f, pos_in_b : %.6f, %.6f",
					frame, id_u_in_a.first, unit_in_a.x, unit_in_a.y, unit_in_b.x, unit_in_b.y);
			}
		}
	}
	return ret;
}

bool ReplayComparer::compare(const char* replay1, const char* replay2)
{
	ReplayToPlay r1(std::string(replay1) + ".rep", true);
	ReplayToPlay r2(std::string(replay2) + ".rep", true);
	cocos2d::log("comparing %s(%d) and %s(%d).", replay1, r1._finishFrame, replay2, r2._finishFrame);
	Utils::replay_to_play = nullptr; //必须把这个变量赋回去，否则之后logr的时候会出错

	bool ret = true;

	if (r1._snapshotInterval && r2._snapshotInterval && r1._snapshotInterval == r2._snapshotInterval)
	{
		for (unsigned _currentFrame = 1; _currentFrame <= r1._finishFrame && _currentFrame <= r2._finishFrame; ++_currentFrame)
		{
			if (r1.snapshots_iter != r1.snapshots.end() && r2.snapshots_iter != r2.snapshots.end() && _currentFrame % r1._snapshotInterval == 0)
			{
				if (!compareSnapshot(_currentFrame, *r1.snapshots_iter, *r2.snapshots_iter))
				{
					ret = false;
				}
				r1.snapshots_iter++;
				r2.snapshots_iter++;
			}
		}
	}
	else
	{
		cocos2d::log("[ERROR] snapshotInterval not match. %d %d", r1._snapshotInterval, r2._snapshotInterval);
	}
	return ret;
}