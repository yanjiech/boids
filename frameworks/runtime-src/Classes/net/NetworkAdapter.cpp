#include "NetworkAdapter.h"
#include "NetworkAdapterInternal.h"
#include "UdpClient.h"
#include "cocos2d.h"
#include "pvp.pb.h"
#include "UdpClient.h"
#include "CCLuaEngine.h"
#include "Profile.h"
#include "Utils.h"
#include "ArgCheck.h"
#include <queue>
#include <map>

NetworkAdaperInternal* _internal;

boids::UserOperationPackage user_op_package;
int current_op;
UdpClient udp_c;

bool during_init;
bool has_sent_msg_this_frame;

#define MILLISECS_PER_FRAME (33)

unsigned _timestamp_next_frame;
std::map<unsigned, boids::UserOperationPackage> cached_ops;

typedef std::queue<boids::UserOperation> OpsQueue;
std::map<std::string, OpsQueue> stackedMoveOps;
std::map<int, boids::UserOperation> skills;

void processUserOperations(const boids::UserOperationPackage& ops)
{
	for (int i = 0; i < ops.operations_size(); ++i)
	{
		const auto& op = ops.operations(i);
		if (op.op_type() == boids::UserOperation_OperationType_Move)
		{
			stackedMoveOps[op.user_id()].push(op);
		}
		else if (op.op_type() == boids::UserOperation_OperationType_UseSkill)
		{
			auto emplace_result = skills.emplace(op.unit_id(), op);
			if (!emplace_result.second)
			{
				Utils::logAI("warning: same skill execute multiple times in same frame");
			}
		}
		else
		{
			Utils::logAI("unrecognized op_type: %d", op.op_type());
		}
	}
}

bool NetworkAdaperInternal::tryReceive()
{
	if (!has_sent_msg_this_frame) //这帧没发过包的话要发空包
	{
		boids::GameMessage msg;
		msg.set_type(boids::GameMessage_MessageType_HeartBeat);

		std::string message_string;
		msg.SerializeToString(&message_string);
		udp_c.send((const unsigned char *)message_string.c_str(), message_string.size());
	}

	unsigned char buffer[1024];
	int recv_bytes = udp_c.receive(buffer, 1024); //receive每一帧都要调，很多事情都是在receive里面做的

	if (during_init && udp_c.isConnected())
	{
		boids::GameMessage msg;
		msg.set_type(boids::GameMessage_MessageType_UserOperation);
		auto user_op = msg.mutable_user_op();
		user_op->set_op_type(boids::UserOperation_OperationType_EnterGame);
		user_op->set_user_id(user_id);

		std::string message_string;
		msg.SerializeToString(&message_string);

		cocos2d::log("send enter game");

		udp_c.send((const unsigned char *)message_string.c_str(), message_string.size());

		during_init = false;
	}
	else
	{
		if (recv_bytes > 0)
		{
			//cocos2d::log("receive: %d data: (hidden)", recv_bytes);

			current_op = 0;

			boids::GameMessage msg;
			if (msg.ParseFromArray(buffer, recv_bytes))
			{
				if (msg.type() == boids::GameMessage_MessageType_UserOperationPackage)
				{
					cocos2d::log("[INFO] UserOperationPackage. timestamp: %d", msg.user_op_package().timestamp());
					cached_ops.emplace(msg.user_op_package().timestamp(), msg.user_op_package());
					//user_op_package = msg.user_op_package();
				}
				else if (msg.type() == boids::GameMessage_MessageType_HeartBeat)
				{
					cocos2d::log("[DEBUG] heart beat. ignored.");
				}
				else
				{
					cocos2d::log("[ERROR] unrecognized game message type: %d", msg.type());
				}
			}
			else
			{
				cocos2d::log("[ERROR] protobuf parse error");
			}
		}
	}

	auto it_ops = cached_ops.find(_timestamp_next_frame);
	if (it_ops == cached_ops.end())
	{
		cocos2d::log("missing frame, timestamp: %d", _timestamp_next_frame);
		return false;
	}
	else
	{
		cocos2d::log("consumed frame, timestamp: %d", _timestamp_next_frame);
		processUserOperations(it_ops->second);
		cached_ops.erase(it_ops);
		_timestamp_next_frame += MILLISECS_PER_FRAME;

		has_sent_msg_this_frame = false; //该函数return true意味着显示层会走一帧，于是要把has_sent_msg_this_frame置为false。相当于每一帧开始的时候赋个初值。
		//但有个小问题：这么写的话万一因故我最近一直没收到包，我也就不会发心跳包了，会不会有问题？
		//我觉得不会吧。即每次收新包都会到这儿，都会把has_sent_msg_this_frame置为false，都对应着必有一个心跳包

		return true;
	}
}

NetworkAdapter* NetworkAdapter::_instance = nullptr;

NetworkAdapter::NetworkAdapter()
{
	_internal = new NetworkAdaperInternal();
}

bool NetworkAdapter::init(std::string host_ip, int host_port)
{
	srand(time(NULL));
	for (int t = 0; t < 3; ++t)
	{
		int local_port = 30000 + rand() % 1000;
		if (udp_c.init(host_ip, host_port, local_port))
		{
			return true;
		}
		else
		{
			cocos2d::log("[ERROR] create udp failed on port: %d", local_port);
		}
	}
	return false;
}

void NetworkAdapter::start()
{
	cocos2d::Director::getInstance()->setNetworkInterface(_internal);

	during_init = true;
	has_sent_msg_this_frame = false;
	_timestamp_next_frame = 0;

	udp_c.connect();
}

void NetworkAdapter::stop()
{
	cocos2d::Director::getInstance()->setNetworkInterface(nullptr);
	_timestamp_next_frame = 0;
	cached_ops.clear();
}

void NetworkAdapter::setMove(int dir_x, int dir_y)
{
	has_sent_msg_this_frame = true;

	cocos2d::log("setMove: %d, %d", dir_x, dir_y);

	boids::GameMessage msg;
	msg.set_type(boids::GameMessage_MessageType_UserOperation);
	auto user_op = msg.mutable_user_op();
	user_op->set_op_type(boids::UserOperation_OperationType_Move);
	user_op->set_user_id(user_id);
	user_op->set_dir_x(dir_x);
	user_op->set_dir_y(dir_y);

	std::string message_string;
	msg.SerializeToString(&message_string);

	udp_c.send((const unsigned char *)message_string.c_str(), message_string.size());
}

void NetworkAdapter::setSkill(int unit_id, int dir_x, int dir_y, int range)
{
	has_sent_msg_this_frame = true;

	cocos2d::log("setSkill: %d, %d, %d", unit_id, dir_x, dir_y);

	boids::GameMessage msg;
	msg.set_type(boids::GameMessage_MessageType_UserOperation);
	auto user_op = msg.mutable_user_op();
	user_op->set_op_type(boids::UserOperation_OperationType_UseSkill);
	user_op->set_user_id(user_id);
	user_op->set_unit_id(unit_id);
	//user_op->set_skill_id(skill_id); 暂时不发id，假设手动的都是1
	user_op->set_dir_x(dir_x);
	user_op->set_dir_y(dir_y);
	user_op->set_skill_range(range);

	std::string message_string;
	msg.SerializeToString(&message_string);

	udp_c.send((const unsigned char *)message_string.c_str(), message_string.size());
}

const cocos2d::LuaValue toLuaCCP(const boids::UserOperation& op)
{
	cocos2d::LuaValueDict ccp;
	ccp.emplace("x", cocos2d::LuaValue::floatValue((float)op.dir_x() / 1e6));
	ccp.emplace("y", cocos2d::LuaValue::floatValue((float)op.dir_y() / 1e6));
	return cocos2d::LuaValue::dictValue(ccp);
}

LUA_FUNC(getOps, 0)
	LUA_STACK(stack);
	cocos2d::LuaValueDict lua_moves;
	for (auto& id_and_queue : stackedMoveOps)
	{
		if (!id_and_queue.second.empty())
		{
			lua_moves.emplace(id_and_queue.first, toLuaCCP(id_and_queue.second.front()));
			id_and_queue.second.pop();
		}
	}
	stack->pushLuaValueDict(lua_moves);

	cocos2d::LuaValueDict lua_skills;
	for (auto& unit_id_and_op : skills)
	{
		const auto& skill_op = unit_id_and_op.second;
		cocos2d::LuaValueDict skill_data;
		skill_data.emplace("dir", toLuaCCP(skill_op));
		skill_data.emplace("range", cocos2d::LuaValue::intValue(skill_op.skill_range()));
		lua_skills.emplace(Utils::toStr(unit_id_and_op.first), cocos2d::LuaValue::dictValue(skill_data));
	}
	skills.clear();
	stack->pushLuaValueDict(lua_skills);

	return 2;
END_FUNC

void extendNetworkAdapter(lua_State *L)
{
	tolua_beginmodule(L, "NetworkAdapter");
	tolua_function(L, "getOps", getOps);
	tolua_endmodule(L);
}