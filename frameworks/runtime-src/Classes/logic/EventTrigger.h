//
//  EventTrigger.h
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#ifndef __Boids__EventTrigger__
#define __Boids__EventTrigger__

#include "Trigger.h"

class EventTrigger : public cocos2d::Ref {
private:
    class MapLogic* _map_logic;
    
    bool _should_recycle;
    int _current_trigger_index;
    int _total_trigger_count;
    bool _is_enabled;
    bool _is_repeat;
    std::string _relation;
    
    cocos2d::ValueMap _event_data;
    
    cocos2d::Vector<Trigger*> _triggers;
    
public:
    EventTrigger();
    virtual ~EventTrigger();
    
    static EventTrigger* create( class MapLogic* map_logic, const cocos2d::ValueMap& event_data );
    virtual bool init( class MapLogic* map_logic, const cocos2d::ValueMap& event_data );
    
    virtual void updateFrame( float delta );
    
    void setMapLogic( class MapLogic* map_logic );
    
    bool shouldRecycle() { return _should_recycle; }
    void setShouldRecycle( bool b ) { _should_recycle = b; }
    
    int getTotalTriggerCount() { return _total_trigger_count; }
    void setTotalTriggerCount( int count ) { _total_trigger_count = count; }
    
    void setEnabled( bool b );
    
    bool canFire();
    
    bool isEnabled();
    
    void moveOn();
    
    void trigger( class UnitNode* unit_node );
    
    void activateTriggerByConditions( const cocos2d::ValueMap& conditions, class UnitNode* unit_node );
    
    void activateTriggerByUnit( class UnitNode* unit, const std::string& unit_state, const cocos2d::ValueMap& area );
    
    void checkTriggerPass( class UnitNode* unit_node = nullptr );
    
    const cocos2d::ValueMap& getEventData() { return _event_data; }
    
    const cocos2d::Vector<Trigger*>& getTriggers() { return _triggers; }
    
    void addTrigger( Trigger* trigger );
};

#endif /* defined(__Boids__EventTrigger__) */
