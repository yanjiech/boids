//
//  MapLogic.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__MapLogic__
#define __Boids__MapLogic__

#include "cocos2d.h"
#include "EventTrigger.h"
#include "EventAction.h"
#include "GameTask.h"

class BattleLayer;

#define GAME_STATE_UNSTART "game_unstart"
#define GAME_STATE_START "game_start"
#define GAME_STATE_LOSE "game_lose"
#define GAME_STATE_WIN "game_win"

class MapLogic : public cocos2d::Ref {
private:
    BattleLayer* _battle_layer;
    
    cocos2d::Vector<EventTrigger*> _triggers;
    
    cocos2d::Vector<GameTask*> _game_tasks;
    
    cocos2d::Map<std::string, EventAction*> _event_actions;
    
    cocos2d::ValueMapIntKey _unit_appear_count_by_camp;
    cocos2d::ValueMapIntKey _unit_disappear_count_by_camp;
    
    cocos2d::ValueMap _unit_appear_count_by_tag;
    cocos2d::ValueMap _unit_disappear_count_by_tag;
    
    cocos2d::ValueMap _unit_appear_count_by_name;
    cocos2d::ValueMap _unit_disappear_count_by_name;
    
    cocos2d::ValueMap _obtained_items;
    
private:
    void updateEventTriggers( float delta );
    void updateEventActions( float delta );
    
public:
    MapLogic();
    virtual ~MapLogic();
    
    static MapLogic* create( BattleLayer* battle_layer );
    
    virtual bool init( BattleLayer* battle_layer );
    
    virtual void updateFrame( float delta );
    
    BattleLayer* getBattleLayer() { return _battle_layer; }
    
    void gameEndWithResult( const cocos2d::ValueMap& result );
    
    void deployPlayerUnits();
    
    void onMapInit();
    
    const cocos2d::Vector<GameTask*> getGameTasks() { return _game_tasks; }
    const cocos2d::ValueMap& getDropedItems() { return _obtained_items; }
    
    void onEventChanged( const std::string& event_name, const std::string& event_state );
    void onTaskStateChanged( const std::string& task_name, const std::string& task_state, float progress );
    void onCustomTrigger( const std::string& trigger_name );
    void onConversationStateChanged( const std::string& trigger_name, const std::string& trigger_state );
    
    void onVisionChanged( const cocos2d::ValueMap& update_data );
    
    void addEventAction( EventAction* action, const std::string& key );
    void removeEventAction( const std::string& key );
    
    void setTriggerStateByName( const std::string& name, int state ); //0 enable, 1 disable, 2 recycle
    void setActionStateByName( const std::string& name, int state ); //0 enable, 1 disable, 2 recycle
    
    void onTargetNodeAppear( class TargetNode* target_node );
    void onTargetNodeDead( class TargetNode* target_node );
    void onTargetNodeDisappear( class TargetNode* target_node );
    void onUnitMoved( class UnitNode* unit_node );
    
    void onStoryChange( const std::string& story_name, const std::string& story_state );
    
    int getUnitAppearCountByCamp( int camp );
    void increaseUnitAppearCountByCamp( int count, int camp );
    
    int getUnitDisappearCountByCamp( int camp );
    void increaseUnitDisappearCountByCamp( int count, int camp );
    
    int getUnitAppearCountByTag( const std::string& tag );
    void increaseUnitAppearCountByTag( int count, const std::string& tag );
    
    int getUnitDisappearCountByTag( const std::string& tag );
    void increaseUnitDisappearCountByTag( int count, const std::string& tag );
    
    int getUnitAppearCountByName( const std::string& name );
    void increaseUnitAppearCountByName( int count, const std::string& name );
    
    int getUnitDisappearCountByName( const std::string& name );
    void increaseUnitDisappearCountByName( int count, const std::string& name );
    
    void obtainItem( const std::string& item_id, int count );
    void dropItem( UnitNode* unit );
};

#endif /* defined(__Boids__MapLogic__) */
