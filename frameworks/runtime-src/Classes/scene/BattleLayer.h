//
//  BattleLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__BattleLayer__
#define __Boids__BattleLayer__

#include "UIBattleLayer.h"
#include "UIControlLayer.h"
#include "UIBattleMenuLayer.h"
#include "UIBattleFloatLayer.h"
#include "../unit/UnitNode.h"
#include "../unit/BuildingNode.h"
#include "../unit/BulletNode.h"
#include "../unit/skill/SkillNode.h"
#include "../unit/BlockNode.h"
#include "../unit/TowerNode.h"
#include "UIStoryLayer.h"
#include "../MapData.h"
#include "../logic/MapLogic.h"
#include "../Utils.h"
#include "../unit/Item.h"

enum eBattleSubLayer {
    MapLayer = 1,
    OnGroundLayer = 2,
    BelowObjectLayer = 3,
    ObjectLayer = 4,
    OverObjectLayer = 5,
    EffectLayer = 6,
    FogLayer = 7,
    FloatLayer = 8,
    ToastLayer = 9,
    ControlLayer = 10,
    BattleUILayer = 11,
    BattleStoryLayer = 12,
    BattleMenuLayer = 13
};

enum eBattleState {
    UnknownBattleState = 1,
    BattleWin = 2,
    BattleLose = 3,
    BattlePrepare = 4,
    BattlePaused = 5,
    BattleRunning = 6,
    BattleStory = 7
};

enum eCameraMode {
    CameraModeFree = 1,
    CameraModeFollow = 2,
    CameraModeFixed = 3
};

typedef cocos2d::Map<std::string, UnitNode*> UnitMap;
typedef cocos2d::Map<std::string, BulletNode*> BulletMap;
typedef cocos2d::Map<int, TowerNode*> TowerMap;
typedef cocos2d::Map<int, BlockNode*> BlockMap;
typedef cocos2d::Map<int, BuildingNode*> BuildingMap;
typedef cocos2d::Vector<DropItem*> ItemVector;

class BattleLayer : public cocos2d::Layer {
private:
    MapData* _map_data;
    MapLogic* _map_logic;
    
    std::string _level_id;
    
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
    UIBattleLayer* _skill_ui_layer;
    UIBattleMenuLayer* _battle_menu_layer;
    UIControlLayer* _control_layer;
    cocos2d::TMXTiledMap* _tmx_map;
    UIStoryLayer* _story_layer;
    
    eBattleState _state;
    
    UnitMap _player_units;
    UnitMap _alive_units;
    UnitMap _dead_units;
    
    BulletMap _bullets;
    
    TowerMap _towers;
    BuildingMap _buildings;
    BlockMap _block_nodes;
    
    ItemVector _drop_items;
    
    int _next_deploy_id;
    
    float _game_time;
    
    bool _should_show_fog;
    
    cocos2d::Node* _fog_node;
    int _fog_x_tile_count;
    int _fog_y_tile_count;
    
    cocos2d::Texture2D* _fog_texture;
    
    unsigned char *_fog_tile_visible_array;
    
    //debug
    cocos2d::DrawNode* _draw_node;
private:
    void parseMapObjects();
    void parseMapElementWithData( const cocos2d::TMXObjectGroup* group, const cocos2d::Value& data, eBattleSubLayer layer );
    
    int zorderForPositionOnObjectLayer( const cocos2d::Point& pos );
    
    void reorderObjectLayer();
    
    void updateTowers( float delta );
    void updateBlocks( float delta );
    void updateBuildings( float delta );
    void updateBullets( float delta );
    
    void updateFogSprite();
    
public:
    //debug
    cocos2d::DrawNode* getDrawNode() { return _draw_node; }
    
    static eBattleState getBattleStateFromString( const std::string& str );
    
    BattleLayer();
    virtual ~BattleLayer();
    
    static BattleLayer* create( MapData* map_data, const std::string& level_id, bool is_pvp );
    
    virtual bool init( MapData* map_data, const std::string& level_id, bool is_pvp );
    
    const std::string& getLevelId() { return _level_id; }
    void setLevelId( const std::string& level_id ) { _level_id = level_id; }
    
    void setup();
    void reset();
    
    virtual void updateFrame( float delta );
    
    void startBattle();
    void pauseBattle();
    void resumeBattle();
    void restartBattle();
    void quitBattle();
    
    void setMapData( MapData* map_data );
    void setMapLogic( MapLogic* map_logic );
    
    eBattleState getState() { return _state; }
    void setState( eBattleState new_state ) { _state = new_state; }
    
    void changeState( eBattleState new_state );
    bool checkState();
    
    MapData* getMapData() { return _map_data; }
    MapLogic* getMapLogic() { return _map_logic; }
    
    UIControlLayer* getControlLayer() { return _control_layer; }
    
    const UnitMap& getPlayerUnits() const { return _player_units; }
    const UnitMap& getAliveUnits() const { return _alive_units; }
    const UnitMap& getDeadUnits() const { return _dead_units; }
    
    UnitNode* getLeaderUnit();
    
    cocos2d::Vector<UnitNode*> getAliveUnitsByCondition( eTargetCamp camp, const std::vector<std::string>& tags, const cocos2d::Point& center, float range );
    
    cocos2d::Vector<UnitNode*> getAliveOpponents( eTargetCamp camp );
    cocos2d::Vector<UnitNode*> getAliveUnitsByCamp( eTargetCamp camp );
    
    cocos2d::Vector<UnitNode*> getAliveUnitsInRange( const cocos2d::Point& center, float range );
    cocos2d::Vector<UnitNode*> getAliveUnitsInRoundRange( const cocos2d::Point& center, float radius );
    
    cocos2d::Vector<UnitNode*> getAliveAllyInRange( eTargetCamp camp, const cocos2d::Point& center, float radius );
    cocos2d::Vector<UnitNode*> getAliveUnitsByTag( const std::string& tag );
    cocos2d::Vector<UnitNode*> getAliveUnitsByName( const std::string& name );
    cocos2d::Vector<UnitNode*> getAliveOpponentsInRange( eTargetCamp camp, const cocos2d::Point& center, float radius );
    cocos2d::Vector<UnitNode*> getAliveOpponentsInRange( eTargetCamp camp, const cocos2d::Point& init_pos, const cocos2d::Point& center, float radius );
    cocos2d::Vector<UnitNode*> getAliveOpponentsInRoundRange( eTargetCamp camp, const cocos2d::Point& init_pos, const cocos2d::Point& center, float radius );
    cocos2d::Vector<UnitNode*> getAliveOpponentsInSector( eTargetCamp camp, const cocos2d::Point& center, const cocos2d::Point& dir, float radius, float angle );
    cocos2d::Vector<UnitNode*> getAliveUnitsByCampAndSightGroup( eTargetCamp camp, const std::string& sight_group );
    
    const TowerMap& getAllTowers() { return _towers; }
    cocos2d::Vector<TowerNode*> getAliveTowersInRange( eTargetCamp camp, const cocos2d::Point& center, float range );
    cocos2d::Vector<TowerNode*> getAliveTowersInRange( eTargetCamp camp, const cocos2d::Point& init_pos, const cocos2d::Point& center, float range );
    
    const BlockMap& getBlockNodes() { return _block_nodes; }
    void addBlockNode( BlockNode* block_node, eBattleSubLayer layer );
    void removeBlockNode( BlockNode* block_node );
    BlockNode* getBlockNode( const std::string& name );
    
    TargetNode* getAliveTargetByDeployId( int deploy_id );
    
    bool addBullet( int key, BulletNode* bullet, eBattleSubLayer layer = eBattleSubLayer::EffectLayer );
    void removeBullet( int key );
    
    void onUnitAppear( UnitNode* unit );
    void onUnitDying( UnitNode* unit );
    void onUnitDisappear( UnitNode* unit );
    void onUnitMoved( UnitNode* unit );
    
    bool isPositionInVision( const cocos2d::Point& pos );
    
    void addToObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    
    void addToEffectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToBelowObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToOverObjectLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToOnGroundLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    void addToFloatLayer( cocos2d::Node* node, const cocos2d::Point& pos, int local_zorder );
    
    void addToLayer( cocos2d::Node* node, eBattleSubLayer layer, const cocos2d::Point& pos, int local_zorder );
    
    void deployUnit( UnitNode* unit, const cocos2d::Point& pos, const std::string& sight_group );
    void deployUnits( const cocos2d::Vector<UnitNode*>& units, const cocos2d::Rect& area, const std::string& sight_group );
    
    void deployTower( TowerNode* tower, const cocos2d::Point& pos, eBattleSubLayer layer );
    
    void deployBuilding( BuildingNode* building, const cocos2d::Point& pos, eBattleSubLayer layer );
    
    void adjustCamera();
    void centerCameraToPosition( const cocos2d::Point& pos );
    
    int getNextDeployId();
    
    float getGameTime() { return _game_time; }
    void setGameTime( float time ) { _game_time = time; }
    
    cocos2d::Point getAvailablePosition( float radius, const cocos2d::Rect& region );
    bool isPositionOK( cocos2d::Point pos, float radius );
    
    void clearChasingTarget( TargetNode* target );
    
    void startStory( const cocos2d::ValueMap& story_data );
    void endStory( UIStoryLayer* story );
    
    bool shouldShowFog() { return _should_show_fog; }
    void setShouldShowFog( bool b ) { _should_show_fog = b; }
    
    UIBattleLayer* getUIBattleLayer() { return _skill_ui_layer; }
    
    void dropItem( DropItem* item, const cocos2d::Point& pos, eBattleSubLayer layer );
};

#endif /* defined(__Boids__BattleLayer__) */
