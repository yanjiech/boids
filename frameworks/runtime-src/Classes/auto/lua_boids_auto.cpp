#include "lua_boids_auto.hpp"
#include "ArmatureManager.h"
#include "BattleLayer.h"
#include "UnitNode.h"
#include "Trace.h"
#include "MapData.h"
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clearArmatureData",argc, 0);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->loadArmatureData(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "loadArmatureData",argc, 1);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->unloadArmatureData(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unloadArmatureData",argc, 1);
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

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getBonePosition(arg0, arg1);
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBonePosition",argc, 2);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        spine::SkeletonAnimation* ret = cobj->createArmature(arg0);
        object_to_luaval<spine::SkeletonAnimation>(tolua_S, "sp.SkeletonAnimation",(spine::SkeletonAnimation*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "createArmature",argc, 1);
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

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getSlotPosition(arg0, arg1);
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSlotPosition",argc, 2);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_ArmatureManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_boids_ArmatureManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ArmatureManager)");
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

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cocos2d::TMXTiledMap* ret = cobj->generateTiledMapWithFlags(arg0);
        object_to_luaval<cocos2d::TMXTiledMap>(tolua_S, "cc.TMXTiledMap",(cocos2d::TMXTiledMap*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "generateTiledMapWithFlags",argc, 1);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMetaData",argc, 0);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeImagesFromCache",argc, 0);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMapData",argc, 0);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dumpMetaData(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpMetaData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_dumpMetaData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getMapJson(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getMapJson'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::ValueMap& ret = cobj->getMapJson();
        ccvaluemap_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMapJson",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getMapJson'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getAreasVectorByTag(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getAreasVectorByTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cocos2d::ValueVector ret = cobj->getAreasVectorByTag(arg0);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAreasVectorByTag",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getAreasVectorByTag'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getMetaJson(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getMetaJson'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::ValueMap& ret = cobj->getMetaJson();
        ccvaluemap_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMetaJson",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getMetaJson'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getAreaMapByName(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getAreaMapByName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cocos2d::ValueMap ret = cobj->getAreaMapByName(arg0);
        ccvaluemap_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAreaMapByName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getAreaMapByName'.",&tolua_err);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "loadImagesToCache",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_loadImagesToCache'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_MapData_getAreaMapByPosition(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_MapData_getAreaMapByPosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cocos2d::ValueMap ret = cobj->getAreaMapByPosition(arg0);
        ccvaluemap_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAreaMapByPosition",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_getAreaMapByPosition'.",&tolua_err);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj = new MapData(arg0);
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"MapData");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "MapData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_MapData_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_MapData_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (MapData)");
    return 0;
}

int lua_register_boids_MapData(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"MapData");
    tolua_cclass(tolua_S,"MapData","MapData","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"MapData");
        tolua_function(tolua_S,"new",lua_boids_MapData_constructor);
        tolua_function(tolua_S,"generateTiledMapWithFlags",lua_boids_MapData_generateTiledMapWithFlags);
        tolua_function(tolua_S,"getMetaData",lua_boids_MapData_getMetaData);
        tolua_function(tolua_S,"removeImagesFromCache",lua_boids_MapData_removeImagesFromCache);
        tolua_function(tolua_S,"getMapData",lua_boids_MapData_getMapData);
        tolua_function(tolua_S,"dumpMetaData",lua_boids_MapData_dumpMetaData);
        tolua_function(tolua_S,"getMapJson",lua_boids_MapData_getMapJson);
        tolua_function(tolua_S,"getAreasVectorByTag",lua_boids_MapData_getAreasVectorByTag);
        tolua_function(tolua_S,"getMetaJson",lua_boids_MapData_getMetaJson);
        tolua_function(tolua_S,"getAreaMapByName",lua_boids_MapData_getAreaMapByName);
        tolua_function(tolua_S,"loadImagesToCache",lua_boids_MapData_loadImagesToCache);
        tolua_function(tolua_S,"getAreaMapByPosition",lua_boids_MapData_getAreaMapByPosition);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(MapData).name();
    g_luaType[typeName] = "MapData";
    g_typeCast["MapData"] = "MapData";
    return 1;
}

int lua_boids_UnitNode_getBornPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getBornPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::Point& ret = cobj->getBornPos();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBornPos",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getBornPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setCurrentSkeleton(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setCurrentSkeleton'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        spine::SkeletonAnimation* arg0;

        ok &= luaval_to_object<spine::SkeletonAnimation>(tolua_S, 2, "sp.SkeletonAnimation",&arg0);
        if(!ok)
            return 0;
        cobj->setCurrentSkeleton(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setCurrentSkeleton",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setCurrentSkeleton'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_removeUnitComponent(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_removeUnitComponent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->removeUnitComponent(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeUnitComponent",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_removeUnitComponent'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSkills(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSkills'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::Vector<Skill *>& ret = cobj->getSkills();
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSkills",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSkills'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_findPathToPosition(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_findPathToPosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->findPathToPosition(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        cocos2d::Point arg0;
        int arg1;

        ok &= luaval_to_point(tolua_S, 2, &arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        if(!ok)
            return 0;
        cobj->findPathToPosition(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "findPathToPosition",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_findPathToPosition'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_init(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        BattleLayer* arg0;
        cocos2d::ValueMap arg1;

        ok &= luaval_to_object<BattleLayer>(tolua_S, 2, "BattleLayer",&arg0);

        ok &= luaval_to_ccvaluemap(tolua_S, 3, &arg1);
        if(!ok)
            return 0;
        bool ret = cobj->init(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_init'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_walkTo(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_walkTo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->walkTo(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "walkTo",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_walkTo'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_addBehavior(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_addBehavior'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        BehaviorBase* arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_object<BehaviorBase>(tolua_S, 3, "BehaviorBase",&arg1);
        if(!ok)
            return 0;
        cobj->addBehavior(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addBehavior",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_addBehavior'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_addBuff(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_addBuff'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        Buff* arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_object<Buff>(tolua_S, 3, "Buff",&arg1);
        if(!ok)
            return 0;
        cobj->addBuff(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addBuff",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_addBuff'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isBoss(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isBoss'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isBoss();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isBoss",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isBoss'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSkillMaxRangeById(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSkillMaxRangeById'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        double ret = cobj->getSkillMaxRangeById(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSkillMaxRangeById",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSkillMaxRangeById'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSkillCDById(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSkillCDById'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        double ret = cobj->getSkillCDById(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSkillCDById",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSkillCDById'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_changeUnitDirection(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_changeUnitDirection'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->changeUnitDirection(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "changeUnitDirection",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_changeUnitDirection'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_addUnitComponent(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_addUnitComponent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        UnitNodeComponent* arg0;
        std::string arg1;
        eComponentLayer arg2;

        ok &= luaval_to_object<UnitNodeComponent>(tolua_S, 2, "UnitNodeComponent",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        bool ret = cobj->addUnitComponent(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addUnitComponent",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_addUnitComponent'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isCasting(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isCasting'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isCasting();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isCasting",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isCasting'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setUnitFace(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setUnitFace'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitFace arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setUnitFace(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setUnitFace",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setUnitFace'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_removeBehavior(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_removeBehavior'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->removeBehavior(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeBehavior",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_removeBehavior'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setWanderRadius(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setWanderRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setWanderRadius(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setWanderRadius",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setWanderRadius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isHarmless(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isHarmless'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isHarmless();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isHarmless",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isHarmless'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setHesitateFrame(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setHesitateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setHesitateFrame(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setHesitateFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setHesitateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getUnitState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getUnitState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getUnitState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getUnitState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getUnitState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_shouldCatchUp(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_shouldCatchUp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->shouldCatchUp();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "shouldCatchUp",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_shouldCatchUp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isSkillReadyById(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isSkillReadyById'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->isSkillReadyById(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isSkillReadyById",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isSkillReadyById'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSkillHintTypeById(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSkillHintTypeById'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        std::string ret = cobj->getSkillHintTypeById(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSkillHintTypeById",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSkillHintTypeById'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_canAttack(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_canAttack'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        TargetNode* arg0;

        ok &= luaval_to_object<TargetNode>(tolua_S, 2, "TargetNode",&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->canAttack(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "canAttack",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_canAttack'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_hasBuff(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_hasBuff'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->hasBuff(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "hasBuff",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_hasBuff'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_hasUnitTag(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_hasUnitTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->hasUnitTag(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "hasUnitTag",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_hasUnitTag'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getCurrentSkeleton(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getCurrentSkeleton'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        spine::SkeletonAnimation* ret = cobj->getCurrentSkeleton();
        object_to_luaval<spine::SkeletonAnimation>(tolua_S, "sp.SkeletonAnimation",(spine::SkeletonAnimation*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getCurrentSkeleton",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getCurrentSkeleton'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isOscillate(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isOscillate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        bool ret = cobj->isOscillate(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isOscillate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isOscillate'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_useSkill(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_useSkill'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        cocos2d::Point arg1;
        double arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_number(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->useSkill(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "useSkill",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_useSkill'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isFoeOfCamp(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isFoeOfCamp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitCamp arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->isFoeOfCamp(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isFoeOfCamp",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isFoeOfCamp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setNextUnitState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setNextUnitState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitState arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setNextUnitState(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setNextUnitState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setNextUnitState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onDying(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onDying'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->onDying();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onDying",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onDying'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getDeployId(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getDeployId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getDeployId();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getDeployId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getDeployId'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_hideHP(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_hideHP'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->hideHP();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "hideHP",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_hideHP'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getHesitateFrame(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getHesitateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getHesitateFrame();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getHesitateFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getHesitateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_falldown(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_falldown'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->falldown();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "falldown",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_falldown'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onAttacking(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onAttacking'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->onAttacking();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onAttacking",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onAttacking'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onCharging(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onCharging'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->onCharging(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onCharging",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onCharging'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setUnitDirection(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setUnitDirection'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->setUnitDirection(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setUnitDirection",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setUnitDirection'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_removeBuff(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_removeBuff'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->removeBuff(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeBuff",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_removeBuff'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setUnitState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setUnitState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitState arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setUnitState(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setUnitState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setUnitState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSkillRangeById(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSkillRangeById'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        double ret = cobj->getSkillRangeById(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSkillRangeById",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSkillRangeById'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_adjustAllUnitComponents(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_adjustAllUnitComponents'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->adjustAllUnitComponents();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "adjustAllUnitComponents",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_adjustAllUnitComponents'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getChasingTarget(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getChasingTarget'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        TargetNode* ret = cobj->getChasingTarget();
        object_to_luaval<TargetNode>(tolua_S, "TargetNode",(TargetNode*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getChasingTarget",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getChasingTarget'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setDeployId(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setDeployId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setDeployId(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setDeployId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setDeployId'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isWalking(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isWalking'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isWalking();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isWalking",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isWalking'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_walkAlongPath(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_walkAlongPath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->walkAlongPath(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "walkAlongPath",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_walkAlongPath'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_applyUnitState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_applyUnitState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->applyUnitState();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "applyUnitState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_applyUnitState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onSkeletonAnimationEnded(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onSkeletonAnimationEnded'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->onSkeletonAnimationEnded(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onSkeletonAnimationEnded",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onSkeletonAnimationEnded'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_endCast(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_endCast'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->endCast();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "endCast",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_endCast'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getEmitPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getEmitPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getEmitPos();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getEmitPos",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getEmitPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_evaluateCatchUp(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_evaluateCatchUp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->evaluateCatchUp();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "evaluateCatchUp",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_evaluateCatchUp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getLocalHeadPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getLocalHeadPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getLocalHeadPos();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getLocalHeadPos",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getLocalHeadPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_jumpNumber(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_jumpNumber'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        double arg0;
        std::string arg1;
        bool arg2;
        std::string arg3;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_boolean(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->jumpNumber(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "jumpNumber",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_jumpNumber'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onSkeletonAnimationStart(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onSkeletonAnimationStart'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->onSkeletonAnimationStart(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onSkeletonAnimationStart",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onSkeletonAnimationStart'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_riseup(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_riseup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->riseup(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "riseup",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_riseup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isUnderControl(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isUnderControl'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isUnderControl();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isUnderControl",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isUnderControl'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getUnitCamp(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getUnitCamp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getUnitCamp();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getUnitCamp",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getUnitCamp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onSkeletonAnimationEvent(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onSkeletonAnimationEvent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        spEvent* arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        #pragma warning NO CONVERSION TO NATIVE FOR spEvent*;
        if(!ok)
            return 0;
        cobj->onSkeletonAnimationEvent(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onSkeletonAnimationEvent",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onSkeletonAnimationEvent'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setPriority(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setPriority'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setPriority(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setPriority",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setPriority'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setConcentrateOnWalk(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setConcentrateOnWalk'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setConcentrateOnWalk(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setConcentrateOnWalk",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setConcentrateOnWalk'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setUnitTags(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setUnitTags'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setUnitTags(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setUnitTags",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setUnitTags'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onCasting(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onCasting'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->onCasting();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onCasting",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onCasting'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getUnitDirection(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getUnitDirection'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::Point& ret = cobj->getUnitDirection();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getUnitDirection",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getUnitDirection'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isUnitInDirectView(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isUnitInDirectView'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        UnitNode* arg0;

        ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->isUnitInDirectView(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isUnitInDirectView",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isUnitInDirectView'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_attack(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_attack'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        TargetNode* arg0;

        ok &= luaval_to_object<TargetNode>(tolua_S, 2, "TargetNode",&arg0);
        if(!ok)
            return 0;
        cobj->attack(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "attack",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_attack'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isAttacking(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isAttacking'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isAttacking();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isAttacking",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isAttacking'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setBornPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setBornPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->setBornPos(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setBornPos",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setBornPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_removeAllUnitComponents(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_removeAllUnitComponents'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->removeAllUnitComponents();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeAllUnitComponents",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_removeAllUnitComponents'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onAttackBegan(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onAttackBegan'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->onAttackBegan();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onAttackBegan",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onAttackBegan'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setGLProgrameState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setGLProgrameState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setGLProgrameState(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setGLProgrameState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setGLProgrameState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isConcentrateOnWalk(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isConcentrateOnWalk'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isConcentrateOnWalk();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isConcentrateOnWalk",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isConcentrateOnWalk'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_willCollide(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_willCollide'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            UnitNode* arg0;
            ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);

            if (!ok) { break; }
            bool ret = cobj->willCollide(arg0);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            cocos2d::Point arg0;
            ok &= luaval_to_point(tolua_S, 2, &arg0);

            if (!ok) { break; }
            double arg1;
            ok &= luaval_to_number(tolua_S, 3,&arg1);

            if (!ok) { break; }
            bool ret = cobj->willCollide(arg0, arg1);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            UnitNode* arg0;
            ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);

            if (!ok) { break; }
            cocos2d::Point arg1;
            ok &= luaval_to_point(tolua_S, 3, &arg1);

            if (!ok) { break; }
            bool ret = cobj->willCollide(arg0, arg1);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "willCollide",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_willCollide'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getLocalHitPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getLocalHitPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getLocalHitPos();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getLocalHitPos",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getLocalHitPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onDisappearEnd(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onDisappearEnd'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->onDisappearEnd();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onDisappearEnd",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onDisappearEnd'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_applyCustomChange(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_applyCustomChange'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->applyCustomChange(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "applyCustomChange",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_applyCustomChange'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_endSkill(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_endSkill'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->endSkill();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "endSkill",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_endSkill'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_removeAllBuffs(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_removeAllBuffs'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->removeAllBuffs();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeAllBuffs",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_removeAllBuffs'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_updateFrame(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_updateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->updateFrame(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "updateFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_updateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getPriority(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getPriority'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getPriority();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getPriority",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getPriority'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getNextWanderPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getNextWanderPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getNextWanderPos();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getNextWanderPos",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getNextWanderPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isDying(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isDying'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isDying();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isDying",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isDying'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_takeHeal(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_takeHeal'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        double arg0;
        bool arg1;
        int arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->takeHeal(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "takeHeal",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_takeHeal'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_showHP(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_showHP'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->showHP();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "showHP",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_showHP'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getUnitData(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getUnitData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        UnitData* ret = cobj->getUnitData();
        object_to_luaval<UnitData>(tolua_S, "UnitData",(UnitData*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getUnitData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getUnitData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_disappear(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_disappear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->disappear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "disappear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_disappear'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_needRelax(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_needRelax'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->needRelax();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "needRelax",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_needRelax'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setMovable(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setMovable'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setMovable(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setMovable",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setMovable'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_changeUnitState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_changeUnitState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitState arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->changeUnitState(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "changeUnitState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_changeUnitState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setBoss(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setBoss'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setBoss(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setBoss",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setBoss'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getNextUnitState(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getNextUnitState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getNextUnitState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getNextUnitState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getNextUnitState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_isMovable(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_isMovable'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isMovable();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isMovable",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_isMovable'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getHitPos(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getHitPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getHitPos();
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getHitPos",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getHitPos'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setChasingTarget(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setChasingTarget'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        TargetNode* arg0;

        ok &= luaval_to_object<TargetNode>(tolua_S, 2, "TargetNode",&arg0);
        if(!ok)
            return 0;
        cobj->setChasingTarget(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setChasingTarget",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setChasingTarget'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getBattleLayer(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getBattleLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        BattleLayer* ret = cobj->getBattleLayer();
        object_to_luaval<BattleLayer>(tolua_S, "BattleLayer",(BattleLayer*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBattleLayer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getBattleLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getUnitFace(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getUnitFace'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getUnitFace();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getUnitFace",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getUnitFace'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getAdvisedNewDir(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getAdvisedNewDir'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        UnitNode* arg0;
        cocos2d::Vec2 arg1;
        cocos2d::Vec2 arg2;

        ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);

        ok &= luaval_to_vec2(tolua_S, 3, &arg1);

        ok &= luaval_to_vec2(tolua_S, 4, &arg2);
        if(!ok)
            return 0;
        bool ret = cobj->getAdvisedNewDir(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAdvisedNewDir",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getAdvisedNewDir'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getAttackTarget(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getAttackTarget'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        TargetNode* ret = cobj->getAttackTarget();
        object_to_luaval<TargetNode>(tolua_S, "TargetNode",(TargetNode*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAttackTarget",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getAttackTarget'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setShouldCatchUp(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setShouldCatchUp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setShouldCatchUp(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setShouldCatchUp",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setShouldCatchUp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSkillMinRangeById(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSkillMinRangeById'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        double ret = cobj->getSkillMinRangeById(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSkillMinRangeById",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSkillMinRangeById'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_appear(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_appear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->appear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "appear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_appear'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getWanderRadius(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getWanderRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getWanderRadius();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getWanderRadius",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getWanderRadius'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setUnitCamp(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setUnitCamp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitCamp arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setUnitCamp(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setUnitCamp",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setUnitCamp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_changeFace(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_changeFace'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitFace arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->changeFace(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "changeFace",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_changeFace'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_takeDamage(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_takeDamage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        double arg0;
        bool arg1;
        bool arg2;
        int arg3;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);

        ok &= luaval_to_boolean(tolua_S, 4,&arg2);

        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3);
        if(!ok)
            return 0;
        cobj->takeDamage(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "takeDamage",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_takeDamage'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_setSightGroup(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_setSightGroup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setSightGroup(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setSightGroup",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_setSightGroup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_onSkeletonAnimationCompleted(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_onSkeletonAnimationCompleted'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->onSkeletonAnimationCompleted(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onSkeletonAnimationCompleted",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_onSkeletonAnimationCompleted'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_getSightGroup(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnitNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_UnitNode_getSightGroup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getSightGroup();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSightGroup",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getSightGroup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_UnitNode_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        BattleLayer* arg0;
        cocos2d::ValueMap arg1;
        ok &= luaval_to_object<BattleLayer>(tolua_S, 2, "BattleLayer",&arg0);
        ok &= luaval_to_ccvaluemap(tolua_S, 3, &arg1);
        if(!ok)
            return 0;
        UnitNode* ret = UnitNode::create(arg0, arg1);
        object_to_luaval<UnitNode>(tolua_S, "UnitNode",(UnitNode*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_create'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_UnitNode_getCampByString(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"UnitNode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        int ret = (int)UnitNode::getCampByString(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getCampByString",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_getCampByString'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_UnitNode_constructor(lua_State* tolua_S)
{
    int argc = 0;
    UnitNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new UnitNode();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"UnitNode");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "UnitNode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_UnitNode_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_UnitNode_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (UnitNode)");
    return 0;
}

int lua_register_boids_UnitNode(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"UnitNode");
    tolua_cclass(tolua_S,"UnitNode","UnitNode","TargetNode",nullptr);

    tolua_beginmodule(tolua_S,"UnitNode");
        tolua_function(tolua_S,"new",lua_boids_UnitNode_constructor);
        tolua_function(tolua_S,"getBornPos",lua_boids_UnitNode_getBornPos);
        tolua_function(tolua_S,"setCurrentSkeleton",lua_boids_UnitNode_setCurrentSkeleton);
        tolua_function(tolua_S,"removeUnitComponent",lua_boids_UnitNode_removeUnitComponent);
        tolua_function(tolua_S,"getSkills",lua_boids_UnitNode_getSkills);
        tolua_function(tolua_S,"findPathToPosition",lua_boids_UnitNode_findPathToPosition);
        tolua_function(tolua_S,"init",lua_boids_UnitNode_init);
        tolua_function(tolua_S,"walkTo",lua_boids_UnitNode_walkTo);
        tolua_function(tolua_S,"addBehavior",lua_boids_UnitNode_addBehavior);
        tolua_function(tolua_S,"addBuff",lua_boids_UnitNode_addBuff);
        tolua_function(tolua_S,"isBoss",lua_boids_UnitNode_isBoss);
        tolua_function(tolua_S,"getSkillMaxRangeById",lua_boids_UnitNode_getSkillMaxRangeById);
        tolua_function(tolua_S,"getSkillCDById",lua_boids_UnitNode_getSkillCDById);
        tolua_function(tolua_S,"changeUnitDirection",lua_boids_UnitNode_changeUnitDirection);
        tolua_function(tolua_S,"addUnitComponent",lua_boids_UnitNode_addUnitComponent);
        tolua_function(tolua_S,"isCasting",lua_boids_UnitNode_isCasting);
        tolua_function(tolua_S,"setUnitFace",lua_boids_UnitNode_setUnitFace);
        tolua_function(tolua_S,"removeBehavior",lua_boids_UnitNode_removeBehavior);
        tolua_function(tolua_S,"setWanderRadius",lua_boids_UnitNode_setWanderRadius);
        tolua_function(tolua_S,"isHarmless",lua_boids_UnitNode_isHarmless);
        tolua_function(tolua_S,"setHesitateFrame",lua_boids_UnitNode_setHesitateFrame);
        tolua_function(tolua_S,"getUnitState",lua_boids_UnitNode_getUnitState);
        tolua_function(tolua_S,"shouldCatchUp",lua_boids_UnitNode_shouldCatchUp);
        tolua_function(tolua_S,"isSkillReadyById",lua_boids_UnitNode_isSkillReadyById);
        tolua_function(tolua_S,"getSkillHintTypeById",lua_boids_UnitNode_getSkillHintTypeById);
        tolua_function(tolua_S,"canAttack",lua_boids_UnitNode_canAttack);
        tolua_function(tolua_S,"hasBuff",lua_boids_UnitNode_hasBuff);
        tolua_function(tolua_S,"hasUnitTag",lua_boids_UnitNode_hasUnitTag);
        tolua_function(tolua_S,"getCurrentSkeleton",lua_boids_UnitNode_getCurrentSkeleton);
        tolua_function(tolua_S,"isOscillate",lua_boids_UnitNode_isOscillate);
        tolua_function(tolua_S,"useSkill",lua_boids_UnitNode_useSkill);
        tolua_function(tolua_S,"isFoeOfCamp",lua_boids_UnitNode_isFoeOfCamp);
        tolua_function(tolua_S,"setNextUnitState",lua_boids_UnitNode_setNextUnitState);
        tolua_function(tolua_S,"onDying",lua_boids_UnitNode_onDying);
        tolua_function(tolua_S,"getDeployId",lua_boids_UnitNode_getDeployId);
        tolua_function(tolua_S,"hideHP",lua_boids_UnitNode_hideHP);
        tolua_function(tolua_S,"getHesitateFrame",lua_boids_UnitNode_getHesitateFrame);
        tolua_function(tolua_S,"falldown",lua_boids_UnitNode_falldown);
        tolua_function(tolua_S,"onAttacking",lua_boids_UnitNode_onAttacking);
        tolua_function(tolua_S,"onCharging",lua_boids_UnitNode_onCharging);
        tolua_function(tolua_S,"setUnitDirection",lua_boids_UnitNode_setUnitDirection);
        tolua_function(tolua_S,"removeBuff",lua_boids_UnitNode_removeBuff);
        tolua_function(tolua_S,"setUnitState",lua_boids_UnitNode_setUnitState);
        tolua_function(tolua_S,"getSkillRangeById",lua_boids_UnitNode_getSkillRangeById);
        tolua_function(tolua_S,"adjustAllUnitComponents",lua_boids_UnitNode_adjustAllUnitComponents);
        tolua_function(tolua_S,"getChasingTarget",lua_boids_UnitNode_getChasingTarget);
        tolua_function(tolua_S,"setDeployId",lua_boids_UnitNode_setDeployId);
        tolua_function(tolua_S,"isWalking",lua_boids_UnitNode_isWalking);
        tolua_function(tolua_S,"walkAlongPath",lua_boids_UnitNode_walkAlongPath);
        tolua_function(tolua_S,"applyUnitState",lua_boids_UnitNode_applyUnitState);
        tolua_function(tolua_S,"onSkeletonAnimationEnded",lua_boids_UnitNode_onSkeletonAnimationEnded);
        tolua_function(tolua_S,"endCast",lua_boids_UnitNode_endCast);
        tolua_function(tolua_S,"getEmitPos",lua_boids_UnitNode_getEmitPos);
        tolua_function(tolua_S,"evaluateCatchUp",lua_boids_UnitNode_evaluateCatchUp);
        tolua_function(tolua_S,"getLocalHeadPos",lua_boids_UnitNode_getLocalHeadPos);
        tolua_function(tolua_S,"jumpNumber",lua_boids_UnitNode_jumpNumber);
        tolua_function(tolua_S,"onSkeletonAnimationStart",lua_boids_UnitNode_onSkeletonAnimationStart);
        tolua_function(tolua_S,"riseup",lua_boids_UnitNode_riseup);
        tolua_function(tolua_S,"isUnderControl",lua_boids_UnitNode_isUnderControl);
        tolua_function(tolua_S,"getUnitCamp",lua_boids_UnitNode_getUnitCamp);
        tolua_function(tolua_S,"onSkeletonAnimationEvent",lua_boids_UnitNode_onSkeletonAnimationEvent);
        tolua_function(tolua_S,"setPriority",lua_boids_UnitNode_setPriority);
        tolua_function(tolua_S,"setConcentrateOnWalk",lua_boids_UnitNode_setConcentrateOnWalk);
        tolua_function(tolua_S,"setUnitTags",lua_boids_UnitNode_setUnitTags);
        tolua_function(tolua_S,"onCasting",lua_boids_UnitNode_onCasting);
        tolua_function(tolua_S,"getUnitDirection",lua_boids_UnitNode_getUnitDirection);
        tolua_function(tolua_S,"isUnitInDirectView",lua_boids_UnitNode_isUnitInDirectView);
        tolua_function(tolua_S,"attack",lua_boids_UnitNode_attack);
        tolua_function(tolua_S,"isAttacking",lua_boids_UnitNode_isAttacking);
        tolua_function(tolua_S,"setBornPos",lua_boids_UnitNode_setBornPos);
        tolua_function(tolua_S,"removeAllUnitComponents",lua_boids_UnitNode_removeAllUnitComponents);
        tolua_function(tolua_S,"onAttackBegan",lua_boids_UnitNode_onAttackBegan);
        tolua_function(tolua_S,"setGLProgrameState",lua_boids_UnitNode_setGLProgrameState);
        tolua_function(tolua_S,"isConcentrateOnWalk",lua_boids_UnitNode_isConcentrateOnWalk);
        tolua_function(tolua_S,"willCollide",lua_boids_UnitNode_willCollide);
        tolua_function(tolua_S,"getLocalHitPos",lua_boids_UnitNode_getLocalHitPos);
        tolua_function(tolua_S,"onDisappearEnd",lua_boids_UnitNode_onDisappearEnd);
        tolua_function(tolua_S,"applyCustomChange",lua_boids_UnitNode_applyCustomChange);
        tolua_function(tolua_S,"endSkill",lua_boids_UnitNode_endSkill);
        tolua_function(tolua_S,"removeAllBuffs",lua_boids_UnitNode_removeAllBuffs);
        tolua_function(tolua_S,"updateFrame",lua_boids_UnitNode_updateFrame);
        tolua_function(tolua_S,"getPriority",lua_boids_UnitNode_getPriority);
        tolua_function(tolua_S,"getNextWanderPos",lua_boids_UnitNode_getNextWanderPos);
        tolua_function(tolua_S,"isDying",lua_boids_UnitNode_isDying);
        tolua_function(tolua_S,"takeHeal",lua_boids_UnitNode_takeHeal);
        tolua_function(tolua_S,"showHP",lua_boids_UnitNode_showHP);
        tolua_function(tolua_S,"getUnitData",lua_boids_UnitNode_getUnitData);
        tolua_function(tolua_S,"disappear",lua_boids_UnitNode_disappear);
        tolua_function(tolua_S,"needRelax",lua_boids_UnitNode_needRelax);
        tolua_function(tolua_S,"setMovable",lua_boids_UnitNode_setMovable);
        tolua_function(tolua_S,"changeUnitState",lua_boids_UnitNode_changeUnitState);
        tolua_function(tolua_S,"setBoss",lua_boids_UnitNode_setBoss);
        tolua_function(tolua_S,"getNextUnitState",lua_boids_UnitNode_getNextUnitState);
        tolua_function(tolua_S,"isMovable",lua_boids_UnitNode_isMovable);
        tolua_function(tolua_S,"getHitPos",lua_boids_UnitNode_getHitPos);
        tolua_function(tolua_S,"setChasingTarget",lua_boids_UnitNode_setChasingTarget);
        tolua_function(tolua_S,"getBattleLayer",lua_boids_UnitNode_getBattleLayer);
        tolua_function(tolua_S,"getUnitFace",lua_boids_UnitNode_getUnitFace);
        tolua_function(tolua_S,"getAdvisedNewDir",lua_boids_UnitNode_getAdvisedNewDir);
        tolua_function(tolua_S,"getAttackTarget",lua_boids_UnitNode_getAttackTarget);
        tolua_function(tolua_S,"setShouldCatchUp",lua_boids_UnitNode_setShouldCatchUp);
        tolua_function(tolua_S,"getSkillMinRangeById",lua_boids_UnitNode_getSkillMinRangeById);
        tolua_function(tolua_S,"appear",lua_boids_UnitNode_appear);
        tolua_function(tolua_S,"getWanderRadius",lua_boids_UnitNode_getWanderRadius);
        tolua_function(tolua_S,"setUnitCamp",lua_boids_UnitNode_setUnitCamp);
        tolua_function(tolua_S,"changeFace",lua_boids_UnitNode_changeFace);
        tolua_function(tolua_S,"takeDamage",lua_boids_UnitNode_takeDamage);
        tolua_function(tolua_S,"setSightGroup",lua_boids_UnitNode_setSightGroup);
        tolua_function(tolua_S,"onSkeletonAnimationCompleted",lua_boids_UnitNode_onSkeletonAnimationCompleted);
        tolua_function(tolua_S,"getSightGroup",lua_boids_UnitNode_getSightGroup);
        tolua_function(tolua_S,"create", lua_boids_UnitNode_create);
        tolua_function(tolua_S,"getCampByString", lua_boids_UnitNode_getCampByString);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(UnitNode).name();
    g_luaType[typeName] = "UnitNode";
    g_typeCast["UnitNode"] = "UnitNode";
    return 1;
}

int lua_boids_BattleLayer_addToEffectLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addToEffectLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        cocos2d::Point arg1;
        int arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->addToEffectLayer(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addToEffectLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addToEffectLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_addToOnGroundLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addToOnGroundLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        cocos2d::Point arg1;
        int arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->addToOnGroundLayer(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addToOnGroundLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addToOnGroundLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveUnitsByTag(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveUnitsByTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cocos2d::Vector<UnitNode *> ret = cobj->getAliveUnitsByTag(arg0);
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveUnitsByTag",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveUnitsByTag'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_resumeBattle(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_resumeBattle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->resumeBattle();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "resumeBattle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_resumeBattle'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_isPositionInVision(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_isPositionInVision'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        bool ret = cobj->isPositionInVision(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isPositionInVision",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_isPositionInVision'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveUnits(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveUnits'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::Map<std::string, UnitNode *>& ret = cobj->getAliveUnits();
        ccmap_string_key_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveUnits",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveUnits'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_clearChasingTarget(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_clearChasingTarget'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        TargetNode* arg0;

        ok &= luaval_to_object<TargetNode>(tolua_S, 2, "TargetNode",&arg0);
        if(!ok)
            return 0;
        cobj->clearChasingTarget(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clearChasingTarget",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_clearChasingTarget'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getPlayerUnits(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getPlayerUnits'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::Map<std::string, UnitNode *>& ret = cobj->getPlayerUnits();
        ccmap_string_key_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getPlayerUnits",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getPlayerUnits'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_onUnitDying(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_onUnitDying'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        UnitNode* arg0;

        ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);
        if(!ok)
            return 0;
        cobj->onUnitDying(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onUnitDying",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_onUnitDying'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_deployUnit(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_deployUnit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        UnitNode* arg0;
        cocos2d::Point arg1;
        std::string arg2;

        ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->deployUnit(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "deployUnit",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_deployUnit'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveUnitsByName(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveUnitsByName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cocos2d::Vector<UnitNode *> ret = cobj->getAliveUnitsByName(arg0);
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveUnitsByName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveUnitsByName'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getNextDeployId(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getNextDeployId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getNextDeployId();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getNextDeployId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getNextDeployId'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_setState(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_setState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eBattleState arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setState(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_setState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_deployUnits(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_deployUnits'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Vector<UnitNode *> arg0;
        cocos2d::Rect arg1;
        std::string arg2;

        ok &= luaval_to_ccvector(tolua_S, 2, &arg0);

        ok &= luaval_to_rect(tolua_S, 3, &arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->deployUnits(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "deployUnits",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_deployUnits'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_pauseBattle(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_pauseBattle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->pauseBattle();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "pauseBattle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_pauseBattle'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getDeadUnits(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getDeadUnits'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const cocos2d::Map<std::string, UnitNode *>& ret = cobj->getDeadUnits();
        ccmap_string_key_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getDeadUnits",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getDeadUnits'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_adjustCamera(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_adjustCamera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->adjustCamera();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "adjustCamera",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_adjustCamera'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAvailablePosition(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAvailablePosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        cocos2d::Rect arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_rect(tolua_S, 3, &arg1);
        if(!ok)
            return 0;
        cocos2d::Point ret = cobj->getAvailablePosition(arg0, arg1);
        point_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAvailablePosition",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAvailablePosition'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_init(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        MapData* arg0;
        bool arg1;

        ok &= luaval_to_object<MapData>(tolua_S, 2, "MapData",&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        bool ret = cobj->init(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_init'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_changeState(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_changeState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eBattleState arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->changeState(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "changeState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_changeState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_quitBattle(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_quitBattle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->quitBattle();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "quitBattle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_quitBattle'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveUnitsByCampAndSightGroup(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveUnitsByCampAndSightGroup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        eUnitCamp arg0;
        std::string arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cocos2d::Vector<UnitNode *> ret = cobj->getAliveUnitsByCampAndSightGroup(arg0, arg1);
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveUnitsByCampAndSightGroup",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveUnitsByCampAndSightGroup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_startBattle(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_startBattle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->startBattle();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "startBattle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_startBattle'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_addToObjectLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addToObjectLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        cocos2d::Point arg1;
        cocos2d::Point arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_point(tolua_S, 4, &arg2);
        if(!ok)
            return 0;
        cobj->addToObjectLayer(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addToObjectLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addToObjectLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveUnitsByCamp(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveUnitsByCamp'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitCamp arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cocos2d::Vector<UnitNode *> ret = cobj->getAliveUnitsByCamp(arg0);
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveUnitsByCamp",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveUnitsByCamp'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_setGameTime(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_setGameTime'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setGameTime(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setGameTime",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_setGameTime'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_addToOverObjectLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addToOverObjectLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        cocos2d::Point arg1;
        int arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->addToOverObjectLayer(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addToOverObjectLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addToOverObjectLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveUnitByDeployId(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveUnitByDeployId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        UnitNode* ret = cobj->getAliveUnitByDeployId(arg0);
        object_to_luaval<UnitNode>(tolua_S, "UnitNode",(UnitNode*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveUnitByDeployId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveUnitByDeployId'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getState(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getState'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_addBullet(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addBullet'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        BulletNode* arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_object<BulletNode>(tolua_S, 3, "BulletNode",&arg1);
        if(!ok)
            return 0;
        bool ret = cobj->addBullet(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addBullet",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addBullet'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveOpponentsInRange(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveOpponentsInRange'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        eUnitCamp arg0;
        cocos2d::Point arg1;
        double arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_number(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cocos2d::Vector<UnitNode *> ret = cobj->getAliveOpponentsInRange(arg0, arg1, arg2);
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveOpponentsInRange",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveOpponentsInRange'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_centerCameraToPosition(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_centerCameraToPosition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Point arg0;

        ok &= luaval_to_point(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->centerCameraToPosition(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "centerCameraToPosition",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_centerCameraToPosition'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getAliveOpponents(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getAliveOpponents'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        eUnitCamp arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cocos2d::Vector<UnitNode *> ret = cobj->getAliveOpponents(arg0);
        ccvector_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAliveOpponents",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getAliveOpponents'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getControlLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getControlLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        UIControlLayer* ret = cobj->getControlLayer();
        object_to_luaval<UIControlLayer>(tolua_S, "UIControlLayer",(UIControlLayer*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getControlLayer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getControlLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_addToFloatLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addToFloatLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        cocos2d::Point arg1;
        int arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->addToFloatLayer(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addToFloatLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addToFloatLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_setMapLogic(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_setMapLogic'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        MapLogic* arg0;

        ok &= luaval_to_object<MapLogic>(tolua_S, 2, "MapLogic",&arg0);
        if(!ok)
            return 0;
        cobj->setMapLogic(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setMapLogic",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_setMapLogic'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getLeaderUnit(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getLeaderUnit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        UnitNode* ret = cobj->getLeaderUnit();
        object_to_luaval<UnitNode>(tolua_S, "UnitNode",(UnitNode*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getLeaderUnit",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getLeaderUnit'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getGameTime(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getGameTime'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getGameTime();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getGameTime",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getGameTime'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_reset(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_reset'", nullptr);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "reset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_reset'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_isPositionOK(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_isPositionOK'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Point arg0;
        double arg1;

        ok &= luaval_to_point(tolua_S, 2, &arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        bool ret = cobj->isPositionOK(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isPositionOK",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_isPositionOK'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_removeBullet(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_removeBullet'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->removeBullet(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeBullet",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_removeBullet'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_setup(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_setup'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->setup();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setup",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_setup'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_getMapData(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_getMapData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        MapData* ret = cobj->getMapData();
        object_to_luaval<MapData>(tolua_S, "MapData",(MapData*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMapData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_getMapData'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_addToBelowObjectLayer(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_addToBelowObjectLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        cocos2d::Point arg1;
        int arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_point(tolua_S, 3, &arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->addToBelowObjectLayer(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addToBelowObjectLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_addToBelowObjectLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_restartBattle(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_restartBattle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->restartBattle();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "restartBattle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_restartBattle'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_onUnitDead(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_onUnitDead'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        UnitNode* arg0;

        ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);
        if(!ok)
            return 0;
        cobj->onUnitDead(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onUnitDead",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_onUnitDead'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_onUnitAppear(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_onUnitAppear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        UnitNode* arg0;

        ok &= luaval_to_object<UnitNode>(tolua_S, 2, "UnitNode",&arg0);
        if(!ok)
            return 0;
        cobj->onUnitAppear(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onUnitAppear",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_onUnitAppear'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_updateFrame(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BattleLayer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_boids_BattleLayer_updateFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->updateFrame(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "updateFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_updateFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_boids_BattleLayer_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"BattleLayer",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        MapData* arg0;
        bool arg1;
        ok &= luaval_to_object<MapData>(tolua_S, 2, "MapData",&arg0);
        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        BattleLayer* ret = BattleLayer::create(arg0, arg1);
        object_to_luaval<BattleLayer>(tolua_S, "BattleLayer",(BattleLayer*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_create'.",&tolua_err);
#endif
    return 0;
}
int lua_boids_BattleLayer_constructor(lua_State* tolua_S)
{
    int argc = 0;
    BattleLayer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new BattleLayer();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"BattleLayer");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "BattleLayer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_BattleLayer_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_BattleLayer_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (BattleLayer)");
    return 0;
}

int lua_register_boids_BattleLayer(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"BattleLayer");
    tolua_cclass(tolua_S,"BattleLayer","BattleLayer","cc.Layer",nullptr);

    tolua_beginmodule(tolua_S,"BattleLayer");
        tolua_function(tolua_S,"new",lua_boids_BattleLayer_constructor);
        tolua_function(tolua_S,"addToEffectLayer",lua_boids_BattleLayer_addToEffectLayer);
        tolua_function(tolua_S,"addToOnGroundLayer",lua_boids_BattleLayer_addToOnGroundLayer);
        tolua_function(tolua_S,"getAliveUnitsByTag",lua_boids_BattleLayer_getAliveUnitsByTag);
        tolua_function(tolua_S,"resumeBattle",lua_boids_BattleLayer_resumeBattle);
        tolua_function(tolua_S,"isPositionInVision",lua_boids_BattleLayer_isPositionInVision);
        tolua_function(tolua_S,"getAliveUnits",lua_boids_BattleLayer_getAliveUnits);
        tolua_function(tolua_S,"clearChasingTarget",lua_boids_BattleLayer_clearChasingTarget);
        tolua_function(tolua_S,"getPlayerUnits",lua_boids_BattleLayer_getPlayerUnits);
        tolua_function(tolua_S,"onUnitDying",lua_boids_BattleLayer_onUnitDying);
        tolua_function(tolua_S,"deployUnit",lua_boids_BattleLayer_deployUnit);
        tolua_function(tolua_S,"getAliveUnitsByName",lua_boids_BattleLayer_getAliveUnitsByName);
        tolua_function(tolua_S,"getNextDeployId",lua_boids_BattleLayer_getNextDeployId);
        tolua_function(tolua_S,"setState",lua_boids_BattleLayer_setState);
        tolua_function(tolua_S,"deployUnits",lua_boids_BattleLayer_deployUnits);
        tolua_function(tolua_S,"pauseBattle",lua_boids_BattleLayer_pauseBattle);
        tolua_function(tolua_S,"getDeadUnits",lua_boids_BattleLayer_getDeadUnits);
        tolua_function(tolua_S,"adjustCamera",lua_boids_BattleLayer_adjustCamera);
        tolua_function(tolua_S,"getAvailablePosition",lua_boids_BattleLayer_getAvailablePosition);
        tolua_function(tolua_S,"init",lua_boids_BattleLayer_init);
        tolua_function(tolua_S,"changeState",lua_boids_BattleLayer_changeState);
        tolua_function(tolua_S,"quitBattle",lua_boids_BattleLayer_quitBattle);
        tolua_function(tolua_S,"getAliveUnitsByCampAndSightGroup",lua_boids_BattleLayer_getAliveUnitsByCampAndSightGroup);
        tolua_function(tolua_S,"startBattle",lua_boids_BattleLayer_startBattle);
        tolua_function(tolua_S,"addToObjectLayer",lua_boids_BattleLayer_addToObjectLayer);
        tolua_function(tolua_S,"getAliveUnitsByCamp",lua_boids_BattleLayer_getAliveUnitsByCamp);
        tolua_function(tolua_S,"setGameTime",lua_boids_BattleLayer_setGameTime);
        tolua_function(tolua_S,"addToOverObjectLayer",lua_boids_BattleLayer_addToOverObjectLayer);
        tolua_function(tolua_S,"getAliveUnitByDeployId",lua_boids_BattleLayer_getAliveUnitByDeployId);
        tolua_function(tolua_S,"getState",lua_boids_BattleLayer_getState);
        tolua_function(tolua_S,"addBullet",lua_boids_BattleLayer_addBullet);
        tolua_function(tolua_S,"getAliveOpponentsInRange",lua_boids_BattleLayer_getAliveOpponentsInRange);
        tolua_function(tolua_S,"centerCameraToPosition",lua_boids_BattleLayer_centerCameraToPosition);
        tolua_function(tolua_S,"getAliveOpponents",lua_boids_BattleLayer_getAliveOpponents);
        tolua_function(tolua_S,"getControlLayer",lua_boids_BattleLayer_getControlLayer);
        tolua_function(tolua_S,"addToFloatLayer",lua_boids_BattleLayer_addToFloatLayer);
        tolua_function(tolua_S,"setMapLogic",lua_boids_BattleLayer_setMapLogic);
        tolua_function(tolua_S,"getLeaderUnit",lua_boids_BattleLayer_getLeaderUnit);
        tolua_function(tolua_S,"getGameTime",lua_boids_BattleLayer_getGameTime);
        tolua_function(tolua_S,"reset",lua_boids_BattleLayer_reset);
        tolua_function(tolua_S,"isPositionOK",lua_boids_BattleLayer_isPositionOK);
        tolua_function(tolua_S,"removeBullet",lua_boids_BattleLayer_removeBullet);
        tolua_function(tolua_S,"setup",lua_boids_BattleLayer_setup);
        tolua_function(tolua_S,"getMapData",lua_boids_BattleLayer_getMapData);
        tolua_function(tolua_S,"addToBelowObjectLayer",lua_boids_BattleLayer_addToBelowObjectLayer);
        tolua_function(tolua_S,"restartBattle",lua_boids_BattleLayer_restartBattle);
        tolua_function(tolua_S,"onUnitDead",lua_boids_BattleLayer_onUnitDead);
        tolua_function(tolua_S,"onUnitAppear",lua_boids_BattleLayer_onUnitAppear);
        tolua_function(tolua_S,"updateFrame",lua_boids_BattleLayer_updateFrame);
        tolua_function(tolua_S,"create", lua_boids_BattleLayer_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(BattleLayer).name();
    g_luaType[typeName] = "BattleLayer";
    g_typeCast["BattleLayer"] = "BattleLayer";
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

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->printStatistics(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "printStatistics",argc, 0);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->trace(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "trace",argc, 1);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "beginTrace",argc, 0);
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

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->accumulate(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "accumulate",argc, 1);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "beginAccumulate",argc, 0);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "beginAccumulate_static",argc, 0);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "beginTrace_static",argc, 0);
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
        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        Trace::printStatistics_static(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "printStatistics_static",argc, 0);
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
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        Trace::trace_static(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "trace_static",argc, 1);
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
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        Trace::accumulate_static(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "accumulate_static",argc, 1);
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
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "Trace",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_boids_Trace_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_boids_Trace_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Trace)");
    return 0;
}

int lua_register_boids_Trace(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Trace");
    tolua_cclass(tolua_S,"Trace","Trace","",nullptr);

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
TOLUA_API int register_all_boids(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"ue",0);
	tolua_beginmodule(tolua_S,"ue");

	lua_register_boids_MapData(tolua_S);
	lua_register_boids_BattleLayer(tolua_S);
	lua_register_boids_ArmatureManager(tolua_S);
	lua_register_boids_UnitNode(tolua_S);
	lua_register_boids_Trace(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

