#include "lua_boids_auto.hpp"
#include "ArmatureManager.h"
#include "AI.h"
#include "AIUnit.h"
#include "Replay.h"
#include "NetworkAdapter.h"
#include "Trace.h"
#include "LuaUtils.h"
#include "CmdLineDebuggerLua.h"
#include "EditMode.h"
#include "LuaPlatform.h"
#include "MapData.h"
#include "NetForExport.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_boids_ArmatureManager_clearArmatureData(lua_State* tolua_S)
{
    int argc = 0;
    ArmatureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ArmatureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ArmatureManager_clearArmatureData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clearArmatureData();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ArmatureManager:clearArmatureData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_clearArmatureData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ArmatureManager_loadArmatureData(lua_State* tolua_S)
{
    int argc = 0;
    ArmatureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ArmatureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ArmatureManager_loadArmatureData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ArmatureManager:loadArmatureData");
        if(!ok)
            return 0;
        cobj->loadArmatureData(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ArmatureManager:loadArmatureData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_loadArmatureData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ArmatureManager_unloadArmatureData(lua_State* tolua_S)
{
    int argc = 0;
    ArmatureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ArmatureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ArmatureManager_unloadArmatureData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ArmatureManager:unloadArmatureData");
        if(!ok)
            return 0;
        cobj->unloadArmatureData(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ArmatureManager:unloadArmatureData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_unloadArmatureData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ArmatureManager_getBonePosition(lua_State* tolua_S)
{
    int argc = 0;
    ArmatureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ArmatureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ArmatureManager_getBonePosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        spine::SkeletonAnimation* arg0;
        std::string arg1;

        ok &= luaval_to_object<spine::SkeletonAnimation>(tolua_S, 2, "sp.SkeletonAnimation",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "ArmatureManager:getBonePosition");
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getBonePosition(arg0, arg1);
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ArmatureManager:getBonePosition",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_getBonePosition'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ArmatureManager_createArmature(lua_State* tolua_S)
{
    int argc = 0;
    ArmatureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ArmatureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ArmatureManager_createArmature'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ArmatureManager:createArmature");
        if(!ok)
            return 0;
        spine::SkeletonAnimation* ret = cobj->createArmature(arg0);
        object_to_luaval<spine::SkeletonAnimation>(tolua_S, "sp.SkeletonAnimation",(spine::SkeletonAnimation*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ArmatureManager:createArmature",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_createArmature'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ArmatureManager_getSlotPosition(lua_State* tolua_S)
{
    int argc = 0;
    ArmatureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ArmatureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ArmatureManager_getSlotPosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        spine::SkeletonAnimation* arg0;
        std::string arg1;

        ok &= luaval_to_object<spine::SkeletonAnimation>(tolua_S, 2, "sp.SkeletonAnimation",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "ArmatureManager:getSlotPosition");
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getSlotPosition(arg0, arg1);
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ArmatureManager:getSlotPosition",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_getSlotPosition'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ArmatureManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ArmatureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        ArmatureManager* ret = ArmatureManager::getInstance();
        object_to_luaval<ArmatureManager>(tolua_S, "ArmatureManager",(ArmatureManager*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "ArmatureManager:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_ArmatureManager_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (ArmatureManager)");
    return 0;
}

int lua_register_boids_ArmatureManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ArmatureManager");
    tolua_cclass(tolua_S,"ArmatureManager","ArmatureManager","",nullptr);

    tolua_beginmodule(tolua_S,"ArmatureManager");
        tolua_function(tolua_S,"clearArmatureData",lua_boids_ArmatureManager_clearArmatureData);
        tolua_function(tolua_S,"loadArmatureData",lua_boids_ArmatureManager_loadArmatureData);
        tolua_function(tolua_S,"unloadArmatureData",lua_boids_ArmatureManager_unloadArmatureData);
        tolua_function(tolua_S,"getBonePosition",lua_boids_ArmatureManager_getBonePosition);
        tolua_function(tolua_S,"createArmature",lua_boids_ArmatureManager_createArmature);
        tolua_function(tolua_S,"getSlotPosition",lua_boids_ArmatureManager_getSlotPosition);
        tolua_function(tolua_S,"getInstance", lua_boids_ArmatureManager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ArmatureManager).name();
    g_luaType[typeName] = "ArmatureManager";
    g_typeCast["ArmatureManager"] = "ArmatureManager";
    return 1;
}

int lua_boids_AIUnit_resetPath(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_resetPath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->resetPath();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:resetPath",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_resetPath'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_buildingInDirectView(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_buildingInDirectView'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        double arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:buildingInDirectView");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:buildingInDirectView");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AIUnit:buildingInDirectView");
        if(!ok)
            return 0;
        bool ret = cobj->buildingInDirectView(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:buildingInDirectView",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_buildingInDirectView'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_catchup(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_catchup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:catchup");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:catchup");
        if(!ok)
            return 0;
        cobj->catchup(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:catchup",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_catchup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_move_type(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_move_type'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->get_move_type();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_move_type",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_move_type'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_canAttack(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_canAttack'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        AIUnit* arg0;

        ok &= luaval_to_object<AIUnit>(tolua_S, 2, "AIUnit",&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->canAttack(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:canAttack",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_canAttack'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_findPathWithDestRadius(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_findPathWithDestRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        double arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:findPathWithDestRadius");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:findPathWithDestRadius");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AIUnit:findPathWithDestRadius");
        if(!ok)
            return 0;
        cobj->findPathWithDestRadius(arg0, arg1, arg2);
        return 0;
    }
    if (argc == 4) 
    {
        double arg0;
        double arg1;
        double arg2;
        int arg3;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:findPathWithDestRadius");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:findPathWithDestRadius");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AIUnit:findPathWithDestRadius");

        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "AIUnit:findPathWithDestRadius");
        if(!ok)
            return 0;
        cobj->findPathWithDestRadius(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:findPathWithDestRadius",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_findPathWithDestRadius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_move_type(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_move_type'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        MoveType arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AIUnit:set_move_type");
        if(!ok)
            return 0;
        cobj->set_move_type(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_move_type",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_move_type'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_priority(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_priority'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AIUnit:set_priority");
        if(!ok)
            return 0;
        cobj->set_priority(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_priority",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_priority'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_position(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_position'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:set_position");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:set_position");
        if(!ok)
            return 0;
        cobj->set_position(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_position",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_position'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_hasPath(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_hasPath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->hasPath();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:hasPath",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_hasPath'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_is_during_catchup(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_is_during_catchup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->is_during_catchup();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:is_during_catchup",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_is_during_catchup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_max_walk_length(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_max_walk_length'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:set_max_walk_length");
        if(!ok)
            return 0;
        cobj->set_max_walk_length(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_max_walk_length",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_max_walk_length'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_priority2(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_priority2'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AIUnit:set_priority2");
        if(!ok)
            return 0;
        cobj->set_priority2(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_priority2",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_priority2'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_priority2(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_priority2'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->get_priority2();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_priority2",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_priority2'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_findPathForTarget(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_findPathForTarget'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        AIUnit* arg0;

        ok &= luaval_to_object<AIUnit>(tolua_S, 2, "AIUnit",&arg0);
        if(!ok)
            return 0;
        cobj->findPathForTarget(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        AIUnit* arg0;
        int arg1;

        ok &= luaval_to_object<AIUnit>(tolua_S, 2, "AIUnit",&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "AIUnit:findPathForTarget");
        if(!ok)
            return 0;
        cobj->findPathForTarget(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:findPathForTarget",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_findPathForTarget'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_collide_radius(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_collide_radius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:set_collide_radius");
        if(!ok)
            return 0;
        cobj->set_collide_radius(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_collide_radius",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_collide_radius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_deleteUnit(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_deleteUnit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->deleteUnit();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:deleteUnit",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_deleteUnit'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_guard_radius(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_guard_radius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:set_guard_radius");
        if(!ok)
            return 0;
        cobj->set_guard_radius(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_guard_radius",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_guard_radius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_position(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_position'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->get_position();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_position",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_position'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_hesitate_frame(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_hesitate_frame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AIUnit:set_hesitate_frame");
        if(!ok)
            return 0;
        cobj->set_hesitate_frame(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_hesitate_frame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_hesitate_frame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_findPathForTargetWithRadius(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_findPathForTargetWithRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        AIUnit* arg0;
        double arg1;

        ok &= luaval_to_object<AIUnit>(tolua_S, 2, "AIUnit",&arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:findPathForTargetWithRadius");
        if(!ok)
            return 0;
        cobj->findPathForTargetWithRadius(arg0, arg1);
        return 0;
    }
    if (argc == 3) 
    {
        AIUnit* arg0;
        double arg1;
        int arg2;

        ok &= luaval_to_object<AIUnit>(tolua_S, 2, "AIUnit",&arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:findPathForTargetWithRadius");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "AIUnit:findPathForTargetWithRadius");
        if(!ok)
            return 0;
        cobj->findPathForTargetWithRadius(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:findPathForTargetWithRadius",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_findPathForTargetWithRadius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_findPath(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_findPath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:findPath");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:findPath");
        if(!ok)
            return 0;
        cobj->findPath(arg0, arg1);
        return 0;
    }
    if (argc == 3) 
    {
        double arg0;
        double arg1;
        int arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:findPath");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:findPath");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "AIUnit:findPath");
        if(!ok)
            return 0;
        cobj->findPath(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:findPath",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_findPath'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_isPathEnd(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_isPathEnd'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isPathEnd();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:isPathEnd",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_isPathEnd'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_unit_camp(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_unit_camp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::string ret = cobj->get_unit_camp();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_unit_camp",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_unit_camp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_initialize(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_initialize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AIUnit:initialize");
        if(!ok)
            return 0;
        cobj->initialize(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:initialize",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_initialize'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_walkFrameOnPath(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_walkFrameOnPath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->walkFrameOnPath();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:walkFrameOnPath",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_walkFrameOnPath'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_canAttackBuilding(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_canAttackBuilding'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        double arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:canAttackBuilding");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:canAttackBuilding");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AIUnit:canAttackBuilding");
        if(!ok)
            return 0;
        bool ret = cobj->canAttackBuilding(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:canAttackBuilding",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_canAttackBuilding'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_inDirectView(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_inDirectView'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        AIUnit* arg0;

        ok &= luaval_to_object<AIUnit>(tolua_S, 2, "AIUnit",&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->inDirectView(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:inDirectView",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_inDirectView'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_unit_camp(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_unit_camp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AIUnit:set_unit_camp");
        if(!ok)
            return 0;
        cobj->set_unit_camp(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_unit_camp",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_unit_camp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_priority(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_priority'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->get_priority();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_priority",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_priority'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_isPathOutdated(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_isPathOutdated'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isPathOutdated();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:isPathOutdated",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_isPathOutdated'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_unit_id(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_unit_id'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->get_unit_id();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_unit_id",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_unit_id'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_collide_radius(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_collide_radius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->get_collide_radius();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_collide_radius",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_collide_radius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_max_walk_length(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_max_walk_length'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->get_max_walk_length();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_max_walk_length",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_max_walk_length'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_walkStep(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_walkStep'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:walkStep");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AIUnit:walkStep");
        if(!ok)
            return 0;
        bool ret = cobj->walkStep(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:walkStep",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_walkStep'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_get_dir_want_to_go(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_get_dir_want_to_go'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->get_dir_want_to_go();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:get_dir_want_to_go",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_get_dir_want_to_go'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_resetLastWalk(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_resetLastWalk'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->resetLastWalk();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:resetLastWalk",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_resetLastWalk'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_set_attack_radius(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AIUnit*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AIUnit_set_attack_radius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AIUnit:set_attack_radius");
        if(!ok)
            return 0;
        cobj->set_attack_radius(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:set_attack_radius",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_set_attack_radius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AIUnit_constructor(lua_State* tolua_S)
{
    int argc = 0;
    AIUnit* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new AIUnit();
        tolua_pushusertype(tolua_S,(void*)cobj,"AIUnit");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AIUnit:AIUnit",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AIUnit_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_AIUnit_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (AIUnit)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"AIUnit",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        AIUnit* self = (AIUnit*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_AIUnit(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AIUnit");
    tolua_cclass(tolua_S,"AIUnit","AIUnit","Collidable",lua_boids_AIUnit_finalize);

    tolua_beginmodule(tolua_S,"AIUnit");
        tolua_function(tolua_S,"new",lua_boids_AIUnit_constructor);
        tolua_function(tolua_S,"resetPath",lua_boids_AIUnit_resetPath);
        tolua_function(tolua_S,"buildingInDirectView",lua_boids_AIUnit_buildingInDirectView);
        tolua_function(tolua_S,"catchup",lua_boids_AIUnit_catchup);
        tolua_function(tolua_S,"get_move_type",lua_boids_AIUnit_get_move_type);
        tolua_function(tolua_S,"canAttack",lua_boids_AIUnit_canAttack);
        tolua_function(tolua_S,"findPathWithDestRadius",lua_boids_AIUnit_findPathWithDestRadius);
        tolua_function(tolua_S,"set_move_type",lua_boids_AIUnit_set_move_type);
        tolua_function(tolua_S,"set_priority",lua_boids_AIUnit_set_priority);
        tolua_function(tolua_S,"set_position",lua_boids_AIUnit_set_position);
        tolua_function(tolua_S,"hasPath",lua_boids_AIUnit_hasPath);
        tolua_function(tolua_S,"is_during_catchup",lua_boids_AIUnit_is_during_catchup);
        tolua_function(tolua_S,"set_max_walk_length",lua_boids_AIUnit_set_max_walk_length);
        tolua_function(tolua_S,"set_priority2",lua_boids_AIUnit_set_priority2);
        tolua_function(tolua_S,"get_priority2",lua_boids_AIUnit_get_priority2);
        tolua_function(tolua_S,"findPathForTarget",lua_boids_AIUnit_findPathForTarget);
        tolua_function(tolua_S,"set_collide_radius",lua_boids_AIUnit_set_collide_radius);
        tolua_function(tolua_S,"deleteUnit",lua_boids_AIUnit_deleteUnit);
        tolua_function(tolua_S,"set_guard_radius",lua_boids_AIUnit_set_guard_radius);
        tolua_function(tolua_S,"get_position",lua_boids_AIUnit_get_position);
        tolua_function(tolua_S,"set_hesitate_frame",lua_boids_AIUnit_set_hesitate_frame);
        tolua_function(tolua_S,"findPathForTargetWithRadius",lua_boids_AIUnit_findPathForTargetWithRadius);
        tolua_function(tolua_S,"findPath",lua_boids_AIUnit_findPath);
        tolua_function(tolua_S,"isPathEnd",lua_boids_AIUnit_isPathEnd);
        tolua_function(tolua_S,"get_unit_camp",lua_boids_AIUnit_get_unit_camp);
        tolua_function(tolua_S,"initialize",lua_boids_AIUnit_initialize);
        tolua_function(tolua_S,"walkFrameOnPath",lua_boids_AIUnit_walkFrameOnPath);
        tolua_function(tolua_S,"canAttackBuilding",lua_boids_AIUnit_canAttackBuilding);
        tolua_function(tolua_S,"inDirectView",lua_boids_AIUnit_inDirectView);
        tolua_function(tolua_S,"set_unit_camp",lua_boids_AIUnit_set_unit_camp);
        tolua_function(tolua_S,"get_priority",lua_boids_AIUnit_get_priority);
        tolua_function(tolua_S,"isPathOutdated",lua_boids_AIUnit_isPathOutdated);
        tolua_function(tolua_S,"get_unit_id",lua_boids_AIUnit_get_unit_id);
        tolua_function(tolua_S,"get_collide_radius",lua_boids_AIUnit_get_collide_radius);
        tolua_function(tolua_S,"get_max_walk_length",lua_boids_AIUnit_get_max_walk_length);
        tolua_function(tolua_S,"walkStep",lua_boids_AIUnit_walkStep);
        tolua_function(tolua_S,"get_dir_want_to_go",lua_boids_AIUnit_get_dir_want_to_go);
        tolua_function(tolua_S,"resetLastWalk",lua_boids_AIUnit_resetLastWalk);
        tolua_function(tolua_S,"set_attack_radius",lua_boids_AIUnit_set_attack_radius);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(AIUnit).name();
    g_luaType[typeName] = "AIUnit";
    g_typeCast["AIUnit"] = "AIUnit";
    return 1;
}

int lua_boids_AI_reset(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_reset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->reset();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:reset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_reset'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_isPositionOK(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_isPositionOK'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Point arg0;
        double arg1;

        ok &= luaval_to_point(tolua_S, 2, &arg0, "AI:isPositionOK");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AI:isPositionOK");
        if(!ok)
            return 0;
        bool ret = cobj->isPositionOK(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:isPositionOK",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_isPositionOK'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_releaseCurrentMap(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_releaseCurrentMap'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->releaseCurrentMap();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:releaseCurrentMap",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_releaseCurrentMap'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_addCollidablePolygon(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (AI*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_addCollidablePolygon'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AI:addCollidablePolygon");

            if (!ok) { break; }
            cobj->addCollidablePolygon(arg0);
            return 0;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AI:addCollidablePolygon");

            if (!ok) { break; }
            std::vector<float, std::allocator<float> > arg1;
            ok &= luaval_to_std_vector_float(tolua_S, 3, &arg1, "AI:addCollidablePolygon");

            if (!ok) { break; }
            std::vector<float, std::allocator<float> > arg2;
            ok &= luaval_to_std_vector_float(tolua_S, 4, &arg2, "AI:addCollidablePolygon");

            if (!ok) { break; }
            cobj->addCollidablePolygon(arg0, arg1, arg2);
            return 0;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n",  "AI:addCollidablePolygon",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_addCollidablePolygon'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_test2_int(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_test2_int'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AI:test2_int");
        if(!ok)
            return 0;
        int ret = cobj->test2_int(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:test2_int",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_test2_int'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_get_fell_behind_group(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_get_fell_behind_group'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::vector<int> ret = cobj->get_fell_behind_group();
        ccvector_int_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:get_fell_behind_group",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_get_fell_behind_group'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_get_catch_up_position(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_get_catch_up_position'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->get_catch_up_position();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:get_catch_up_position",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_get_catch_up_position'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_splitIntoTwoGroups(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_splitIntoTwoGroups'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AI:splitIntoTwoGroups");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AI:splitIntoTwoGroups");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AI:splitIntoTwoGroups");
        if(!ok)
            return 0;
        bool ret = cobj->splitIntoTwoGroups(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:splitIntoTwoGroups",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_splitIntoTwoGroups'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_test1_pos(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_test1_pos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AI:test1_pos");
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->test1_pos(arg0);
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:test1_pos",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_test1_pos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_updateFrame(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_updateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->updateFrame();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:updateFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_updateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_getAvailablePosition(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_getAvailablePosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::vector<float, std::allocator<float> > arg0;
        cocos2d::Rect arg1;

        ok &= luaval_to_std_vector_float(tolua_S, 2, &arg0, "AI:getAvailablePosition");

        ok &= luaval_to_rect(tolua_S, 3, &arg1, "AI:getAvailablePosition");
        if(!ok)
            return 0;
        cocos2d::ValueVector ret = cobj->getAvailablePosition(arg0, arg1);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    if (argc == 3) 
    {
        std::vector<float, std::allocator<float> > arg0;
        cocos2d::Rect arg1;
        int arg2;

        ok &= luaval_to_std_vector_float(tolua_S, 2, &arg0, "AI:getAvailablePosition");

        ok &= luaval_to_rect(tolua_S, 3, &arg1, "AI:getAvailablePosition");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "AI:getAvailablePosition");
        if(!ok)
            return 0;
        cocos2d::ValueVector ret = cobj->getAvailablePosition(arg0, arg1, arg2);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:getAvailablePosition",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_getAvailablePosition'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_get_current_frame(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_get_current_frame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->get_current_frame();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:get_current_frame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_get_current_frame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_paintDebugLayer(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_paintDebugLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->paintDebugLayer();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:paintDebugLayer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_paintDebugLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_get_in_narrow(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_get_in_narrow'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->get_in_narrow();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:get_in_narrow",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_get_in_narrow'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_removeCollidablePolygon(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_removeCollidablePolygon'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AI:removeCollidablePolygon");
        if(!ok)
            return 0;
        bool ret = cobj->removeCollidablePolygon(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:removeCollidablePolygon",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_removeCollidablePolygon'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_initialize(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_initialize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "AI:initialize");
        if(!ok)
            return 0;
        cobj->initialize(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:initialize",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_initialize'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_get_lead_group(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_get_lead_group'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::vector<int> ret = cobj->get_lead_group();
        ccvector_int_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:get_lead_group",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_get_lead_group'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_parseMap(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_parseMap'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::vector<float, std::allocator<float> > arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AI:parseMap");

        ok &= luaval_to_std_vector_float(tolua_S, 3, &arg1, "AI:parseMap");
        if(!ok)
            return 0;
        bool ret = cobj->parseMap(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:parseMap",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_parseMap'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_test_c_whole(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AI*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_AI_test_c_whole'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AI:test_c_whole");
        if(!ok)
            return 0;
        cobj->test_c_whole(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:test_c_whole",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_test_c_whole'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_AI_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AI",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        AI* ret = AI::getInstance();
        object_to_luaval<AI>(tolua_S, "AI",(AI*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "AI:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_getInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_AI_constructor(lua_State* tolua_S)
{
    int argc = 0;
    AI* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new AI();
        tolua_pushusertype(tolua_S,(void*)cobj,"AI");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "AI:AI",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_AI_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_AI_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (AI)");
    return 0;
}

int lua_register_boids_AI(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AI");
    tolua_cclass(tolua_S,"AI","AI","",nullptr);

    tolua_beginmodule(tolua_S,"AI");
        tolua_function(tolua_S,"new",lua_boids_AI_constructor);
        tolua_function(tolua_S,"reset",lua_boids_AI_reset);
        tolua_function(tolua_S,"isPositionOK",lua_boids_AI_isPositionOK);
        tolua_function(tolua_S,"releaseCurrentMap",lua_boids_AI_releaseCurrentMap);
        tolua_function(tolua_S,"addCollidablePolygon",lua_boids_AI_addCollidablePolygon);
        tolua_function(tolua_S,"test2_int",lua_boids_AI_test2_int);
        tolua_function(tolua_S,"get_fell_behind_group",lua_boids_AI_get_fell_behind_group);
        tolua_function(tolua_S,"get_catch_up_position",lua_boids_AI_get_catch_up_position);
        tolua_function(tolua_S,"splitIntoTwoGroups",lua_boids_AI_splitIntoTwoGroups);
        tolua_function(tolua_S,"test1_pos",lua_boids_AI_test1_pos);
        tolua_function(tolua_S,"updateFrame",lua_boids_AI_updateFrame);
        tolua_function(tolua_S,"getAvailablePosition",lua_boids_AI_getAvailablePosition);
        tolua_function(tolua_S,"get_current_frame",lua_boids_AI_get_current_frame);
        tolua_function(tolua_S,"paintDebugLayer",lua_boids_AI_paintDebugLayer);
        tolua_function(tolua_S,"get_in_narrow",lua_boids_AI_get_in_narrow);
        tolua_function(tolua_S,"removeCollidablePolygon",lua_boids_AI_removeCollidablePolygon);
        tolua_function(tolua_S,"initialize",lua_boids_AI_initialize);
        tolua_function(tolua_S,"get_lead_group",lua_boids_AI_get_lead_group);
        tolua_function(tolua_S,"parseMap",lua_boids_AI_parseMap);
        tolua_function(tolua_S,"test_c_whole",lua_boids_AI_test_c_whole);
        tolua_function(tolua_S,"getInstance", lua_boids_AI_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(AI).name();
    g_luaType[typeName] = "AI";
    g_typeCast["AI"] = "AI";
    return 1;
}

int lua_boids_ReplayToSave_reset(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToSave*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToSave_reset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->reset();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:reset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_reset'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToSave_setSkill(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToSave*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToSave_setSkill'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        int arg0;
        double arg1;
        double arg2;
        double arg3;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "ReplayToSave:setSkill");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "ReplayToSave:setSkill");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "ReplayToSave:setSkill");

        ok &= luaval_to_number(tolua_S, 5,&arg3, "ReplayToSave:setSkill");
        if(!ok)
            return 0;
        cobj->setSkill(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:setSkill",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_setSkill'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToSave_setJoystick(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToSave*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToSave_setJoystick'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "ReplayToSave:setJoystick");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "ReplayToSave:setJoystick");
        if(!ok)
            return 0;
        cobj->setJoystick(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:setJoystick",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_setJoystick'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToSave_setUnitHpToSnapshot(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToSave*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToSave_setUnitHpToSnapshot'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        double arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "ReplayToSave:setUnitHpToSnapshot");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "ReplayToSave:setUnitHpToSnapshot");
        if(!ok)
            return 0;
        cobj->setUnitHpToSnapshot(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:setUnitHpToSnapshot",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_setUnitHpToSnapshot'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToSave_save(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToSave*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToSave_save'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->save();
        return 0;
    }
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReplayToSave:save");
        if(!ok)
            return 0;
        cobj->save(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:save",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_save'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToSave_updateFrame(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToSave*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToSave_updateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->updateFrame();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:updateFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_updateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToSave_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToSave* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "ReplayToSave:ReplayToSave");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "ReplayToSave:ReplayToSave");
        if(!ok)
            return 0;
        cobj = new ReplayToSave(arg0, arg1);
        tolua_pushusertype(tolua_S,(void*)cobj,"ReplayToSave");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToSave:ReplayToSave",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToSave_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_ReplayToSave_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (ReplayToSave)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"ReplayToSave",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        ReplayToSave* self = (ReplayToSave*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_ReplayToSave(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ReplayToSave");
    tolua_cclass(tolua_S,"ReplayToSave","ReplayToSave","ReplayData",lua_boids_ReplayToSave_finalize);

    tolua_beginmodule(tolua_S,"ReplayToSave");
        tolua_function(tolua_S,"new",lua_boids_ReplayToSave_constructor);
        tolua_function(tolua_S,"reset",lua_boids_ReplayToSave_reset);
        tolua_function(tolua_S,"setSkill",lua_boids_ReplayToSave_setSkill);
        tolua_function(tolua_S,"setJoystick",lua_boids_ReplayToSave_setJoystick);
        tolua_function(tolua_S,"setUnitHpToSnapshot",lua_boids_ReplayToSave_setUnitHpToSnapshot);
        tolua_function(tolua_S,"save",lua_boids_ReplayToSave_save);
        tolua_function(tolua_S,"updateFrame",lua_boids_ReplayToSave_updateFrame);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ReplayToSave).name();
    g_luaType[typeName] = "ReplayToSave";
    g_typeCast["ReplayToSave"] = "ReplayToSave";
    return 1;
}

int lua_boids_ReplayToPlay_reset(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_reset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->reset();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:reset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_reset'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getFrameRate(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getFrameRate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getFrameRate();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getFrameRate",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getFrameRate'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getSkillRange(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getSkillRange'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getSkillRange();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getSkillRange",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getSkillRange'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getSkillDirX(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getSkillDirX'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getSkillDirX();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getSkillDirX",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getSkillDirX'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getSkillDirY(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getSkillDirY'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getSkillDirY();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getSkillDirY",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getSkillDirY'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getJoystickX(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getJoystickX'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getJoystickX();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getJoystickX",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getJoystickX'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getJoystickY(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getJoystickY'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getJoystickY();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getJoystickY",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getJoystickY'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_getSkillPlayer(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_getSkillPlayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getSkillPlayer();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:getSkillPlayer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_getSkillPlayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_updateFrame(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_updateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->updateFrame();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:updateFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_updateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_checkUnitHpWithSnapshot(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ReplayToPlay*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_ReplayToPlay_checkUnitHpWithSnapshot'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        double arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "ReplayToPlay:checkUnitHpWithSnapshot");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "ReplayToPlay:checkUnitHpWithSnapshot");
        if(!ok)
            return 0;
        cobj->checkUnitHpWithSnapshot(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:checkUnitHpWithSnapshot",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_checkUnitHpWithSnapshot'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_ReplayToPlay_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ReplayToPlay* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        bool arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReplayToPlay:ReplayToPlay");

        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "ReplayToPlay:ReplayToPlay");
        if(!ok)
            return 0;
        cobj = new ReplayToPlay(arg0, arg1);
        tolua_pushusertype(tolua_S,(void*)cobj,"ReplayToPlay");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "ReplayToPlay:ReplayToPlay",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ReplayToPlay_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_ReplayToPlay_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (ReplayToPlay)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"ReplayToPlay",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        ReplayToPlay* self = (ReplayToPlay*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_ReplayToPlay(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ReplayToPlay");
    tolua_cclass(tolua_S,"ReplayToPlay","ReplayToPlay","ReplayData",lua_boids_ReplayToPlay_finalize);

    tolua_beginmodule(tolua_S,"ReplayToPlay");
        tolua_function(tolua_S,"new",lua_boids_ReplayToPlay_constructor);
        tolua_function(tolua_S,"reset",lua_boids_ReplayToPlay_reset);
        tolua_function(tolua_S,"getFrameRate",lua_boids_ReplayToPlay_getFrameRate);
        tolua_function(tolua_S,"getSkillRange",lua_boids_ReplayToPlay_getSkillRange);
        tolua_function(tolua_S,"getSkillDirX",lua_boids_ReplayToPlay_getSkillDirX);
        tolua_function(tolua_S,"getSkillDirY",lua_boids_ReplayToPlay_getSkillDirY);
        tolua_function(tolua_S,"getJoystickX",lua_boids_ReplayToPlay_getJoystickX);
        tolua_function(tolua_S,"getJoystickY",lua_boids_ReplayToPlay_getJoystickY);
        tolua_function(tolua_S,"getSkillPlayer",lua_boids_ReplayToPlay_getSkillPlayer);
        tolua_function(tolua_S,"updateFrame",lua_boids_ReplayToPlay_updateFrame);
        tolua_function(tolua_S,"checkUnitHpWithSnapshot",lua_boids_ReplayToPlay_checkUnitHpWithSnapshot);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ReplayToPlay).name();
    g_luaType[typeName] = "ReplayToPlay";
    g_typeCast["ReplayToPlay"] = "ReplayToPlay";
    return 1;
}

int lua_boids_NetworkAdapter_setMove(lua_State* tolua_S)
{
    int argc = 0;
    NetworkAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetworkAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetworkAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_NetworkAdapter_setMove'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "NetworkAdapter:setMove");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "NetworkAdapter:setMove");
        if(!ok)
            return 0;
        cobj->setMove(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NetworkAdapter:setMove",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_NetworkAdapter_setMove'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_NetworkAdapter_start(lua_State* tolua_S)
{
    int argc = 0;
    NetworkAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetworkAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetworkAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_NetworkAdapter_start'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->start();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NetworkAdapter:start",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_NetworkAdapter_start'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_NetworkAdapter_init(lua_State* tolua_S)
{
    int argc = 0;
    NetworkAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetworkAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetworkAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_NetworkAdapter_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        int arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "NetworkAdapter:init");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "NetworkAdapter:init");
        if(!ok)
            return 0;
        bool ret = cobj->init(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NetworkAdapter:init",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_NetworkAdapter_init'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_NetworkAdapter_stop(lua_State* tolua_S)
{
    int argc = 0;
    NetworkAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetworkAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetworkAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_NetworkAdapter_stop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->stop();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NetworkAdapter:stop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_NetworkAdapter_stop'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_NetworkAdapter_setSkill(lua_State* tolua_S)
{
    int argc = 0;
    NetworkAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetworkAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetworkAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_NetworkAdapter_setSkill'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        int arg0;
        int arg1;
        int arg2;
        int arg3;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "NetworkAdapter:setSkill");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "NetworkAdapter:setSkill");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "NetworkAdapter:setSkill");

        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "NetworkAdapter:setSkill");
        if(!ok)
            return 0;
        cobj->setSkill(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NetworkAdapter:setSkill",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_NetworkAdapter_setSkill'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_NetworkAdapter_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NetworkAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        NetworkAdapter* ret = NetworkAdapter::getInstance();
        object_to_luaval<NetworkAdapter>(tolua_S, "NetworkAdapter",(NetworkAdapter*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "NetworkAdapter:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_NetworkAdapter_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_NetworkAdapter_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (NetworkAdapter)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"NetworkAdapter",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        NetworkAdapter* self = (NetworkAdapter*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_NetworkAdapter(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NetworkAdapter");
    tolua_cclass(tolua_S,"NetworkAdapter","NetworkAdapter","",lua_boids_NetworkAdapter_finalize);

    tolua_beginmodule(tolua_S,"NetworkAdapter");
        tolua_function(tolua_S,"setMove",lua_boids_NetworkAdapter_setMove);
        tolua_function(tolua_S,"start",lua_boids_NetworkAdapter_start);
        tolua_function(tolua_S,"init",lua_boids_NetworkAdapter_init);
        tolua_function(tolua_S,"stop",lua_boids_NetworkAdapter_stop);
        tolua_function(tolua_S,"setSkill",lua_boids_NetworkAdapter_setSkill);
        tolua_function(tolua_S,"getInstance", lua_boids_NetworkAdapter_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NetworkAdapter).name();
    g_luaType[typeName] = "NetworkAdapter";
    g_typeCast["NetworkAdapter"] = "NetworkAdapter";
    return 1;
}

int lua_boids_Trace_printStatistics(lua_State* tolua_S)
{
    int argc = 0;
    Trace* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Trace*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_Trace_printStatistics'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->printStatistics();
        return 0;
    }
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "Trace:printStatistics");
        if(!ok)
            return 0;
        cobj->printStatistics(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace:printStatistics",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_printStatistics'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_Trace_trace(lua_State* tolua_S)
{
    int argc = 0;
    Trace* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Trace*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_Trace_trace'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Trace:trace");
        if(!ok)
            return 0;
        cobj->trace(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace:trace",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_trace'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_Trace_beginTrace(lua_State* tolua_S)
{
    int argc = 0;
    Trace* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Trace*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_Trace_beginTrace'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->beginTrace();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace:beginTrace",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_beginTrace'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_Trace_accumulate(lua_State* tolua_S)
{
    int argc = 0;
    Trace* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Trace*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_Trace_accumulate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Trace:accumulate");
        if(!ok)
            return 0;
        cobj->accumulate(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace:accumulate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_accumulate'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_Trace_beginAccumulate(lua_State* tolua_S)
{
    int argc = 0;
    Trace* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Trace*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_Trace_beginAccumulate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->beginAccumulate();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace:beginAccumulate",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_beginAccumulate'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_Trace_beginAccumulate_static(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Trace::beginAccumulate_static();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Trace:beginAccumulate_static",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_beginAccumulate_static'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_Trace_beginTrace_static(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Trace::beginTrace_static();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Trace:beginTrace_static",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_beginTrace_static'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_Trace_printStatistics_static(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Trace::printStatistics_static();
        return 0;
    }
    if (argc == 1)
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "Trace:printStatistics_static");
        if(!ok)
            return 0;
        Trace::printStatistics_static(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Trace:printStatistics_static",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_printStatistics_static'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_Trace_trace_static(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Trace:trace_static");
        if(!ok)
            return 0;
        Trace::trace_static(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Trace:trace_static",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_trace_static'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_Trace_accumulate_static(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Trace",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Trace:accumulate_static");
        if(!ok)
            return 0;
        Trace::accumulate_static(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Trace:accumulate_static",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_accumulate_static'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_Trace_constructor(lua_State* tolua_S)
{
    int argc = 0;
    Trace* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new Trace();
        tolua_pushusertype(tolua_S,(void*)cobj,"Trace");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace:Trace",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_Trace_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (Trace)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"Trace",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        Trace* self = (Trace*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_Trace(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Trace");
    tolua_cclass(tolua_S,"Trace","Trace","",lua_boids_Trace_finalize);

    tolua_beginmodule(tolua_S,"Trace");
        tolua_function(tolua_S,"new",lua_boids_Trace_constructor);
        tolua_function(tolua_S,"printStatistics",lua_boids_Trace_printStatistics);
        tolua_function(tolua_S,"trace",lua_boids_Trace_trace);
        tolua_function(tolua_S,"beginTrace",lua_boids_Trace_beginTrace);
        tolua_function(tolua_S,"accumulate",lua_boids_Trace_accumulate);
        tolua_function(tolua_S,"beginAccumulate",lua_boids_Trace_beginAccumulate);
        tolua_function(tolua_S,"beginAccumulate_static", lua_boids_Trace_beginAccumulate_static);
        tolua_function(tolua_S,"beginTrace_static", lua_boids_Trace_beginTrace_static);
        tolua_function(tolua_S,"printStatistics_static", lua_boids_Trace_printStatistics_static);
        tolua_function(tolua_S,"trace_static", lua_boids_Trace_trace_static);
        tolua_function(tolua_S,"accumulate_static", lua_boids_Trace_accumulate_static);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Trace).name();
    g_luaType[typeName] = "Trace";
    g_typeCast["Trace"] = "Trace";
    return 1;
}

int lua_boids_LuaUtils_c(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        bool ret = LuaUtils::c();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:c",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_c'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_logr(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaUtils:logr");
        if(!ok)
            return 0;
        LuaUtils::logr(arg0);
        return 0;
    }
    if (argc == 2)
    {
        std::string arg0;
        bool arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaUtils:logr");
        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "LuaUtils:logr");
        if(!ok)
            return 0;
        LuaUtils::logr(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:logr",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_logr'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_log(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaUtils:log");
        if(!ok)
            return 0;
        LuaUtils::log(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:log",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_log'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_randomNumber2(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaUtils:randomNumber2");
        if(!ok)
            return 0;
        int ret = LuaUtils::randomNumber2(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:randomNumber2",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_randomNumber2'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_randomFloat(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        double ret = LuaUtils::randomFloat();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:randomFloat",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_randomFloat'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_logf(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaUtils:logf");
        if(!ok)
            return 0;
        LuaUtils::logf(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:logf",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_logf'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_randomFloat2(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        double ret = LuaUtils::randomFloat2();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:randomFloat2",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_randomFloat2'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaUtils_randomNumber(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaUtils:randomNumber");
        if(!ok)
            return 0;
        int ret = LuaUtils::randomNumber(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaUtils:randomNumber",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaUtils_randomNumber'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_LuaUtils_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (LuaUtils)");
    return 0;
}

int lua_register_boids_LuaUtils(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaUtils");
    tolua_cclass(tolua_S,"LuaUtils","LuaUtils","",nullptr);

    tolua_beginmodule(tolua_S,"LuaUtils");
        tolua_function(tolua_S,"c", lua_boids_LuaUtils_c);
        tolua_function(tolua_S,"logr", lua_boids_LuaUtils_logr);
        tolua_function(tolua_S,"log", lua_boids_LuaUtils_log);
        tolua_function(tolua_S,"randomNumber2", lua_boids_LuaUtils_randomNumber2);
        tolua_function(tolua_S,"randomFloat", lua_boids_LuaUtils_randomFloat);
        tolua_function(tolua_S,"logf", lua_boids_LuaUtils_logf);
        tolua_function(tolua_S,"randomFloat2", lua_boids_LuaUtils_randomFloat2);
        tolua_function(tolua_S,"randomNumber", lua_boids_LuaUtils_randomNumber);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaUtils).name();
    g_luaType[typeName] = "LuaUtils";
    g_typeCast["LuaUtils"] = "LuaUtils";
    return 1;
}

int lua_boids_CmdLineDebuggerLua_popCmd(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"CmdLineDebuggerLua",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        std::string ret = CmdLineDebuggerLua::popCmd();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "CmdLineDebuggerLua:popCmd",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_CmdLineDebuggerLua_popCmd'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_CmdLineDebuggerLua_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (CmdLineDebuggerLua)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"CmdLineDebuggerLua",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        CmdLineDebuggerLua* self = (CmdLineDebuggerLua*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_CmdLineDebuggerLua(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"CmdLineDebuggerLua");
    tolua_cclass(tolua_S,"CmdLineDebuggerLua","CmdLineDebuggerLua","",lua_boids_CmdLineDebuggerLua_finalize);

    tolua_beginmodule(tolua_S,"CmdLineDebuggerLua");
        tolua_function(tolua_S,"popCmd", lua_boids_CmdLineDebuggerLua_popCmd);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(CmdLineDebuggerLua).name();
    g_luaType[typeName] = "CmdLineDebuggerLua";
    g_typeCast["CmdLineDebuggerLua"] = "CmdLineDebuggerLua";
    return 1;
}

int lua_boids_MapData_generateTiledMapWithFlags(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (MapData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_generateTiledMapWithFlags'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "MapData:generateTiledMapWithFlags");
        if(!ok)
            return 0;
        cocos2d::TMXTiledMap* ret = cobj->generateTiledMapWithFlags(arg0);
        object_to_luaval<cocos2d::TMXTiledMap>(tolua_S, "cc.TMXTiledMap",(cocos2d::TMXTiledMap*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:generateTiledMapWithFlags",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_generateTiledMapWithFlags'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getMetaData(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (MapData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getMetaData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getMetaData();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:getMetaData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getMetaData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_removeImagesFromCache(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (MapData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_removeImagesFromCache'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->removeImagesFromCache();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:removeImagesFromCache",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_removeImagesFromCache'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getMapData(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (MapData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getMapData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getMapData();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:getMapData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getMapData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_dumpMetaData(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (MapData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_dumpMetaData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "MapData:dumpMetaData");
        if(!ok)
            return 0;
        cobj->dumpMetaData(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:dumpMetaData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_dumpMetaData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_loadImagesToCache(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (MapData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_loadImagesToCache'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->loadImagesToCache();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:loadImagesToCache",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_loadImagesToCache'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_constructor(lua_State* tolua_S)
{
    int argc = 0;
    MapData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "MapData:MapData");
        if(!ok)
            return 0;
        cobj = new MapData(arg0);
        tolua_pushusertype(tolua_S,(void*)cobj,"MapData");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData:MapData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_MapData_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (MapData)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"MapData",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        MapData* self = (MapData*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_MapData(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"MapData");
    tolua_cclass(tolua_S,"MapData","MapData","",lua_boids_MapData_finalize);

    tolua_beginmodule(tolua_S,"MapData");
        tolua_function(tolua_S,"new",lua_boids_MapData_constructor);
        tolua_function(tolua_S,"generateTiledMapWithFlags",lua_boids_MapData_generateTiledMapWithFlags);
        tolua_function(tolua_S,"getMetaData",lua_boids_MapData_getMetaData);
        tolua_function(tolua_S,"removeImagesFromCache",lua_boids_MapData_removeImagesFromCache);
        tolua_function(tolua_S,"getMapData",lua_boids_MapData_getMapData);
        tolua_function(tolua_S,"dumpMetaData",lua_boids_MapData_dumpMetaData);
        tolua_function(tolua_S,"loadImagesToCache",lua_boids_MapData_loadImagesToCache);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(MapData).name();
    g_luaType[typeName] = "MapData";
    g_typeCast["MapData"] = "MapData";
    return 1;
}

int lua_boids_EditMode_backMain(lua_State* tolua_S)
{
    int argc = 0;
    EditMode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"EditMode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (EditMode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_EditMode_backMain'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->backMain();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "EditMode:backMain",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_EditMode_backMain'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_EditMode_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EditMode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        EditMode* ret = EditMode::getInstance();
        object_to_luaval<EditMode>(tolua_S, "EditMode",(EditMode*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "EditMode:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_EditMode_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_EditMode_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (EditMode)");
    return 0;
}

int lua_register_boids_EditMode(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"EditMode");
    tolua_cclass(tolua_S,"EditMode","EditMode","",nullptr);

    tolua_beginmodule(tolua_S,"EditMode");
        tolua_function(tolua_S,"backMain",lua_boids_EditMode_backMain);
        tolua_function(tolua_S,"getInstance", lua_boids_EditMode_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(EditMode).name();
    g_luaType[typeName] = "EditMode";
    g_typeCast["EditMode"] = "EditMode";
    return 1;
}

int lua_boids_LuaPlatform_isPlatformMac(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaPlatform",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        bool ret = LuaPlatform::isPlatformMac();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaPlatform:isPlatformMac",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaPlatform_isPlatformMac'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaPlatform_isPlatformWin32(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaPlatform",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        bool ret = LuaPlatform::isPlatformWin32();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaPlatform:isPlatformWin32",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaPlatform_isPlatformWin32'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaPlatform_isPlatformIOS(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaPlatform",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        bool ret = LuaPlatform::isPlatformIOS();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaPlatform:isPlatformIOS",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaPlatform_isPlatformIOS'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_LuaPlatform_isPlatformAndroid(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaPlatform",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        bool ret = LuaPlatform::isPlatformAndroid();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "LuaPlatform:isPlatformAndroid",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_LuaPlatform_isPlatformAndroid'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_LuaPlatform_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (LuaPlatform)");
    return 0;
}

int lua_register_boids_LuaPlatform(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaPlatform");
    tolua_cclass(tolua_S,"LuaPlatform","LuaPlatform","",nullptr);

    tolua_beginmodule(tolua_S,"LuaPlatform");
        tolua_function(tolua_S,"isPlatformMac", lua_boids_LuaPlatform_isPlatformMac);
        tolua_function(tolua_S,"isPlatformWin32", lua_boids_LuaPlatform_isPlatformWin32);
        tolua_function(tolua_S,"isPlatformIOS", lua_boids_LuaPlatform_isPlatformIOS);
        tolua_function(tolua_S,"isPlatformAndroid", lua_boids_LuaPlatform_isPlatformAndroid);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaPlatform).name();
    g_luaType[typeName] = "LuaPlatform";
    g_typeCast["LuaPlatform"] = "LuaPlatform";
    return 1;
}

int lua_boids_Net_initialize(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Net",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        int arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Net:initialize");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "Net:initialize");
        if(!ok)
            return 0;
        Net::initialize(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Net:initialize",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Net_initialize'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_Net_getUserId(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Net",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        std::string ret = Net::getUserId();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "Net:getUserId",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Net_getUserId'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_Net_finalize(lua_State* tolua_S)
{
    //printf("luabindings: finalizing LUA object (Net)");
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
    !tolua_isusertype(tolua_S,1,"Net",0,&tolua_err) ||
    !tolua_isnoobj(tolua_S,2,&tolua_err)
    )
        goto tolua_lerror;
    else
#endif
    {
        Net* self = (Net*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", nullptr);
#endif
        delete self;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
    return 0;
#endif
    return 0;
}

int lua_register_boids_Net(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Net");
    tolua_cclass(tolua_S,"Net","Net","",lua_boids_Net_finalize);

    tolua_beginmodule(tolua_S,"Net");
        tolua_function(tolua_S,"initialize", lua_boids_Net_initialize);
        tolua_function(tolua_S,"getUserId", lua_boids_Net_getUserId);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Net).name();
    g_luaType[typeName] = "Net";
    g_typeCast["Net"] = "Net";
    return 1;
}
TOLUA_API int register_all_boids(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"ue",0);
	tolua_beginmodule(tolua_S,"ue");

	lua_register_boids_ReplayToPlay(tolua_S);
	lua_register_boids_CmdLineDebuggerLua(tolua_S);
	lua_register_boids_Trace(tolua_S);
	lua_register_boids_AI(tolua_S);
	lua_register_boids_ArmatureManager(tolua_S);
	lua_register_boids_EditMode(tolua_S);
	lua_register_boids_LuaPlatform(tolua_S);
	lua_register_boids_ReplayToSave(tolua_S);
	lua_register_boids_LuaUtils(tolua_S);
	lua_register_boids_NetworkAdapter(tolua_S);
	lua_register_boids_Net(tolua_S);
	lua_register_boids_AIUnit(tolua_S);
	lua_register_boids_MapData(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

