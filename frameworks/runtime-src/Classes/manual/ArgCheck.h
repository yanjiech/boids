#ifndef __Boids_Arg_Check_h_
#define __Boids_Arg_Check_h_

#define LUA_FUNC(NAME, ARGC) \
int NAME(lua_State *L) \
{ \
	if (!L) \
	{ \
		return 0; \
	} \
	\
	int argc = lua_gettop(L); \
	if (argc != ARGC) \
	{ \
		cocos2d::log("'%s' function has wrong number of arguments: %d, was expecting %d\n", #NAME, argc, ARGC); \
		return 0; \
	} \
	int __arg_index = 0;

#define END_FUNC \
}

#define LUA_STACK(NAME) \
	auto NAME = cocos2d::LuaEngine::getInstance()->getLuaStack()
	//auto NAME = cocos2d::LuaStack::attach(L)

#define ARG_FUNC(NAME) \
	++__arg_index; \
	int NAME = toluafix_ref_function(L, __arg_index, 0);

#define ARG_STR(NAME) { \
	++__arg_index; \
	tolua_Error tolua_err; \
	if (!tolua_iscppstring(L, __arg_index, 0, &tolua_err)) \
	{ \
		cocos2d::log("[ERROR] #%d's arg(%s) is not a string.", __arg_index, #NAME); \
		/*luaval_to_native_err(L, "#ferror:", &tolua_err, #NAME);*/ \
		return 0; \
	} \
} \
std::string NAME = tolua_tocppstring(L, __arg_index, NULL);


#endif