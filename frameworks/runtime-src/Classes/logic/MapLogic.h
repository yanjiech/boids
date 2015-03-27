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
#include "../interface/Updatable.h"
#include "EventTrigger.h"
#include "EventAction.h"
#include "GameTask.h"
#include <map>

class UnitNode;
class TargetNode;
class BattleLayer;

#define GAME_STATE_UNSTART "game_unstart"
#define GAME_STATE_START "game_start"
#define GAME_STATE_LOSE "game_lose"
#define GAME_STATE_WIN "game_win"

class MapLogic : public cocos2d::Ref, public boids::Updatable {
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
    
public:
    MapLogic();
    ~MapLogic();
    
    static MapLogic* retainedCreate( BattleLayer* battle_layer );
    
    virtual bool init( BattleLayer* battle_layer );
    
    virtual void updateFrame( float delta );
    
    BattleLayer* getBattleLayer() { return _battle_layer; }
    
    void gameEndWithResult( const cocos2d::ValueMap& result );
    
    void deployPlayerUnits();
    
    void onMapInit();
    
    void executeUnitAction( const cocos2d::ValueMap& action_data, UnitNode* unit_node );
    void executeWaveAction( const cocos2d::ValueMap& action_data, const std::string& wave_action_tag );
    void executeConversationAction( const cocos2d::ValueMap& action_data );
    void executeSpeech( const cocos2d::ValueMap& action_data );
    void executeCustomAction( const cocos2d::ValueMap& action_data );
    
    void onEventChanged( const std::string& event_name, const std::string& event_state );
    void onTaskStateChanged( const std::string& task_name, const std::string& task_state );
    void onGameStateChanged( const std::string& game_state );
    void onVisionChanged( const cocos2d::ValueMap& action_data );
    void onCustomTrigger( const std::string& trigger_name );
    void onConversationStateChanged( const std::string& trigger_name, const std::string& trigger_state );
    
    void addEventAction( EventAction* action, const std::string& key );
    void removeEventAction( const std::string& key );
    
    void updateEventActions( float delta );
    
    void setTriggersEnabledOfName( const std::string& name, bool b );
    
    void checkGameState( float delta );
    
    void onTargetNodeAppear( class TargetNode* target_node );
    void onTargetNodeDisappear( class TargetNode* target_node );
    void onUnitMoved( class UnitNode* unit_node );
    
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
    
};

#endif /* defined(__Boids__MapLogic__) */
