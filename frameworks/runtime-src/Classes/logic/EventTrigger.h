//
//  EventTrigger.h
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#ifndef __Boids__EventTrigger__
#define __Boids__EventTrigger__

#include "cocos2d.h"

#define EVENT_TRIGGER_TYPE_MAP_INIT "map_init"
#define EVENT_TRIGGER_TYPE_UNIT_CHANGE "unit_change"
#define EVENT_TRIGGER_TYPE_EVENT_CHANGE "event_change"
#define EVENT_TRIGGER_TYPE_GAME_CHANGE "game_change"
#define EVENT_TRIGGER_TYPE_VISION_CHANGE "vision_change"
#define EVENT_TRIGGER_TYPE_CONVERSATION_CHANGE "conversation_change"
#define EVENT_TRIGGER_TYPE_CUSTOM "custom"

#define EVENT_STATE_UNSTRART "event_unstart"
#define EVENT_STATE_TRIGGER "event_trigger"
#define EVENT_STATE_FINISH "event_finish"
#define EVENT_STATE_ENABLE "event_enable"

class EventTrigger : public cocos2d::Ref {
private:
    int _trigger_index;
    int _total_trigger_count;
    bool _is_enabled;
    bool _is_repeat;
    std::string _relation;
    
    cocos2d::ValueMap _logic_event;
    
public:
    EventTrigger();
    virtual ~EventTrigger();
    
    static EventTrigger* create( const cocos2d::ValueMap& event );
    virtual bool init( const cocos2d::ValueMap& event );
    
    int getTotalTriggerCount() { return _total_trigger_count; }
    
    void setEnabled( bool b );
    
    bool canFire();
    
    bool isValid();
    
    bool moveOn();
    
    void activateTriggerByConditions( const cocos2d::ValueMap& conditions, class MapLogic* map_logic, class UnitNode* unit_node );
    
    void activateTriggerByUnit( class MapLogic* map_logic, class UnitNode* unit, const std::string& unit_state, const cocos2d::ValueMap& area );
    
    void onSingleTriggerPass( int index, class MapLogic* map_logic, class UnitNode* unit_node );
    
    const cocos2d::ValueMap& getLogicEvent() { return _logic_event; }
};

#endif /* defined(__Boids__EventTrigger__) */
