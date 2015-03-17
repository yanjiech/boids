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
#include <map>

class BattleLayer;

#define GAME_TASK_STATE_UNSTARTED "task_unstart"
#define GAME_TASK_STATE_STARTED "task_start"
#define GAME_TASK_FAILED "task_failed"
#define GAME_TASK_FINISHED "task_finished"

#define GAME_STATE_UNSTART "game_unstart"
#define GAME_STATE_START "game_start"
#define GAME_STATE_LOSE "game_lose"
#define GAME_STATE_WIN "game_win"

class GameTask {
public:
    GameTask( const std::string& name, const std::string& desc, const std::string& state );
    GameTask( const GameTask& other );
    
    GameTask& operator= ( const GameTask& other );
    
    std::string task_name;
    std::string task_desc;
    std::string task_state;
};

class MapLogic : public cocos2d::Ref, public boids::Updatable {
private:
    BattleLayer* _battle_layer;
    
    cocos2d::Vector<EventTrigger*> _triggers;
    
    std::map<std::string, GameTask> _game_tasks;
    
    cocos2d::ValueVector _conditions;
    
    cocos2d::Map<std::string, EventAction*> _event_actions;
    
    cocos2d::ValueMap _unit_appear_count_by_camp;
    cocos2d::ValueMap _unit_disappear_count_by_camp;
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
    
    void deployPlayerUnits();
    
    void onMapInit();
    
    void executeLogicEvent( EventTrigger* trigger, UnitNode* unit_node );
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
    
    void checkGameState();
    
    void onTargetNodeAppear( class TargetNode* target_node );
    void onTargetNodeDisappear( class TargetNode* target_node );
    
    int getUnitAppearCountByCamp( const std::string& camp );
    int getUnitDisappearCountByCamp( const std::string& camp );
    
    int getUnitAppearCountByTag( const std::string& tag );
    int getUnitDisappearCountByTag( const std::string& tag );
    
    int getUnitAppearCountByName( const std::string& name );
    int getUnitDisappearCountByName( const std::string& name );
};

#endif /* defined(__Boids__MapLogic__) */
