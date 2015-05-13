#include "Net.h"
#include "NetForExport.h"
#include "base/CCDirector.h"
#include "EngineRelated.h"
#include "boids.pb.h"
#include "battle.pb.h"
#include "cocos2d.h"
#include "NetDef.h"
#include "NetworkAdapter.h"
#include "ArgCheck.h"
#include "Profile.h"
#include "TcpClient.h"

TcpClient tcp_client;

//===直接导出到lua里的方法===
void Net::initialize(std::string server_ip, int server_port)
{
	initialize_profile();

	tcp_client.init(server_ip, server_port);
	tcp_client.connect();

	cocos2d::Director::getInstance()->set_battle_irrelevant_update(battle_irrelevant_update);
}

std::string Net::getUserId()
{
	return user_id;
}

//===收到包的处理函数===
int autoMatchCallback;

void processGameInit(const boids::GameInitData& game_init_data, cocos2d::LuaStack* stack)
{
	cocos2d::LuaValueDict lua_force;
	for (int i = 0; i < game_init_data.forces_size(); ++i)
	{
		const auto& force = game_init_data.forces(i);
		cocos2d::log("force_id: %d, user_id: %s", force.force_id(), force.user_id().c_str());

		cocos2d::LuaValueDict force_units;
		for (int j = 0; j < force.units_size(); ++j)
		{
			const auto& unit = force.units(j);
			cocos2d::log("unit name: %s", unit.unit_name().c_str());
			force_units.emplace(unit.unit_name(), cocos2d::LuaValue::intValue(unit.unit_level()));
		}

		cocos2d::LuaValueDict lua_force_data;
		lua_force_data.emplace("force_id", cocos2d::LuaValue::intValue(force.force_id()));
		lua_force_data.emplace("units", cocos2d::LuaValue::dictValue(force_units));

		lua_force.emplace(force.user_id(), cocos2d::LuaValue::dictValue(lua_force_data));
	}
	stack->pushLuaValueDict(lua_force);
}

void autoMatchResponse(const boids::MatchResponse& response)
{
	auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
	stack->pushInt(response.ret_value());
	stack->pushString(response.ret_info().c_str());
	
	if (response.ret_value() == boids::MatchResponse_Value_Success)
	{
		cocos2d::log("autoMatch success. ip: %s, port: %d", response.game_server_ip().c_str(), response.game_server_port());
		if (NetworkAdapter::getInstance()->init(response.game_server_ip(), response.game_server_port()))
		{
			processGameInit(response.game_init_data(), stack);
			stack->executeFunctionByHandler(autoMatchCallback, 3);
		}
		else
		{
			cocos2d::log("[ERROR] udp init failed !");
		}
	}
	else
	{
		cocos2d::log("autoMatch error: %d %s", response.ret_value(), response.ret_info().c_str());
		stack->executeFunctionByHandler(autoMatchCallback, 2);
	}
}

enum ReceivingStage
{
	to_receive_size,
	to_receive_data
};
ReceivingStage stage = to_receive_size;

unsigned char* receive_data;
uint32_t data_size;

float accumulated_time = 0.0f;
float time_to_send_heart_beat = 1.0f; //60s发一个心跳包

void tcpUpdate(float dt)
{
	accumulated_time += dt;
	if (accumulated_time >= time_to_send_heart_beat)
	{
		//发送心跳包
		uint32_t empty = 4;
		tcp_client.send((unsigned char*)&empty, 4);

		accumulated_time -= time_to_send_heart_beat;
	}

	if (stage == to_receive_size)
	{
		if (tcp_client.receive((unsigned char*)&data_size, 4) == 0)
		{
			return;
		}
		data_size -= 4; //后面数据的具体长度是要减去头的4的
		receive_data = new unsigned char[data_size];
		stage = to_receive_data;
	}

	if (stage == to_receive_data)
	{
		if (tcp_client.receive(receive_data, data_size) == 0)
		{
			return;
		}

		boids::BoidsMessageHeader __msg;
		if (__msg.ParseFromArray(receive_data, data_size))
		{
			switch (__msg.type())
			{
				HANDLE_MSG(boids::AUTO_MATCH_RESPONSE, boids::MatchResponse, autoMatchResponse);
			default:
				cocos2d::log("[ERROR] unrecognized msg type: %d", __msg.type());
				break;
			}
		}
		else
		{
			cocos2d::log("[ERROR] receive a packet but not a BoidsMessageHeader. data_len: %d", data_size);
		}

		delete receive_data;
		stage = to_receive_size;
	}
}

template<typename P>
void SendMsg(boids::MessageType msg_type, P& proto)
{
	boids::BoidsMessageHeader proto_with_header;
	proto_with_header.set_type(msg_type);
	proto.SerializeToString(proto_with_header.mutable_data());

	int size = proto_with_header.ByteSize();
	size += 4;

	unsigned char* send_buf = new unsigned char[size];
	*((uint32_t*)send_buf) = size;

	proto_with_header.SerializeWithCachedSizesToArray((google_lalune::protobuf::uint8*)(send_buf + 4));
	tcp_client.send(send_buf, size);
}

//===主动发请求的函数(通常是手动导到lua)===
LUA_FUNC(autoMatch, 2)
	ARG_FUNC(_autoMatchCallback);
	ARG_STR(map_name);
	autoMatchCallback = _autoMatchCallback;
	boids::MatchRequest req;
	req.set_user_id(user_id);
	req.set_map_name(map_name);
	SendMsg(boids::AUTO_MATCH_REQUEST, req);
	return 0;
END_FUNC

void extendNet(lua_State *L)
{
	tolua_beginmodule(L, "Net");
	tolua_function(L, "autoMatch", autoMatch);
	tolua_endmodule(L);
}