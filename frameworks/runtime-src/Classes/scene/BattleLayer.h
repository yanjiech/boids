//
//  BattleLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__BattleLayer__
#define __Boids__BattleLayer__

#include "../logic/MapLogic.h"
#include "../MapData.h"
#include "UIBattleLayer.h"
#include "UIControlLayer.h"
#include "UIBattleMenuLayer.h"
#include "UIBattleFloatLayer.h"
#include "../unit/UnitNode.h"
#include "../unit/BuildingNode.h"
#include "../unit/BulletNode.h"
#include <map>

enum eBattleSubLayer {
    MapLayer = 1,
    OnGroundLayer = 2,
    BelowObjectLayer = 3,
    ObjectLayer = 4,
    OverObjectLayer = 5,
    EffectLayer = 6,
    FloatLayer = 7,
    ToastLayer = 8,
    BattleUILayer = 9,
    BattleMenuLayer = 10,
    ControlLayer = 11
};

enum eBattleState {
    Win = 1,
    Lose = 2,
    Prepare = 3,
    Paused = 4,
    Running = 5
};

enum eCameraMode {
    CameraModeFree = 1,
    CameraModeFollow = 2,
    CameraModeFixed = 3
};

typedef std::map<int, UnitNode*> UnitMap;
typedef std::map<int, BulletNode*> BulletMap;

class BattleLayer : public cocos2d::Layer, public boids::Updatable {
private:
    MapLogic* _map_logic;
    MapData* _map_data;
    
    cocos2d::Point _max_map_position;
    cocos2d::Point _min_map_position;
    
    eCameraMode _camera_mode;
    
    cocos2d::Layer* _on_ground_layer;
    cocos2d::Layer* _below_object_layer;
    cocos2d::Layer* _object_layer;
    cocos2d::Layer* _over_object_layer;
    cocos2d::Layer* _effect_layer;
    UIBattleFloatLayer* _float_layer;
    cocos2d::Layer* _toast_layer;
    UIBattleLayer* _ui_layer;
    UIBattleMenuLayer* _battle_menu_layer;
    UIControlLayer* _control_layer;
    cocos2d::TMXTiledMap* _tmx_map;
    
    eBattleState _state;
    
    UnitMap _player_units;
    UnitMap _alive_units;
    UnitMap _dead_units;
    
    BulletMap _bullets;
    
    int _next_deploy_id;
    
    float _game_time;
private:
    void parseMapObjects();
    
    int zorderForPositionOnObjectLayer( const cocos2d::Point& pos );
    
    void reorderObjectLayer();
    
public:
    BattleLayer();
    virtual ~BattleLayer();
    
    static BattleLayer* create( MapData* map_data, bool is_pvp );
    
    virtual bool init( MapData* map_data, bool is_pvp );
    
    void setup();
    void reset();
    
    virtual void updateFrame( float delta );
    
    void startBattle();
    void pauseBattle();
    void resumeBattle();
    void restartBattle();
    void quitBattle();
    
    void setMapLogic( MapLogic* map_logic );
    
    eBattleState getState() { return _state; }
    void setState( eBattleState new_state ) { _state = new_state; }
    
    void changeState( eBattleState new_state );
    
    MapData* getMapData() { return _map_data; }
    
    UIControlLayer* getControlLayer() { return _control_layer; }
    
    const UnitMap& getPlayerUnits() { return _player_units; }
    const UnitMap& getAliveUnits() { return _alive_units; }
    const UnitMap& getDeadUnits() { return _dead_units; }
    
    UnitNode* getLeaderUnit();
    
    std::list<UnitNode*> getAliveOpponents( eUnitCamp camp );
    std::list<UnitNode*> getAliveUnitsByCamp( eUnitCamp camp );
    std::list<UnitNode*> getAliveUnitsByTag( const std::string& tag );
    std::list<UnitNode*> getAliveUnitsByName( const std::string& name );
    
    std::list<UnitNode*> getAliveOpponentsInRange( eUnitCamp camp, const cocos2d::Point& center, float radius );
    
    std::list<UnitNode*> getAliveUnitsByCampAndSightGroup( eUnitCamp camp, const std::string& sight_group );
    
    UnitNode* getAliveUnitByDeployId( int deploy_id );
    
    bool addBullet( int key, BulletNode* bullet );
    void removeBullet( int key );
    
    void onUnitAppear( UnitNode* unit );
    void onUnitDying( UnitNode* unit );
    void onUnitDead( UnitNode* unit );
    
    bool isPositionInVision( const cocos2d::Point& pos );
    
    void addToObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, const cocos2d::Point& center );
    
    void addToEffectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToBelowObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToOverObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToOnGroundLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToFloatLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    
    void deployUnit( UnitNode* unit, const cocos2d::Point& pos, const std::string& sight_group );
    void deployUnits( const std::list<UnitNode*>& units, const cocos2d::Rect& area, const std::string& sight_group );
    
    void adjustCamera();
    void centerCameraToPosition( const cocos2d::Point& pos );
    
    int getNextDeployId();
    
    float getGameTime() { return _game_time; }
    void setGameTime( float time ) { _game_time = time; }
    
    cocos2d::Point getAvailablePosition( float radius, const cocos2d::Rect& region );
    bool isPositionOK( cocos2d::Point pos, float radius );
    
    void clearChasingTarget( TargetNode* unit );
};

#endif /* defined(__Boids__BattleLayer__) */