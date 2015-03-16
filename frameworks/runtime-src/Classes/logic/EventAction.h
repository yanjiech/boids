//
//  EventAction.h
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#ifndef __Boids__EventAction__
#define __Boids__EventAction__

#include "cocos2d.h"

#define ACTION_TYPE_UNIT_CHANGE "unit_change"
#define ACTION_TYPE_TASK_CHANGE "task_change"
#define ACTION_TYPE_GAME_CHANGE "game_change"
#define ACTION_TYPE_EVENT_CHANGE "event_change"
#define ACTION_TYPE_VISION_CHANGE "vision_change"
#define ACTION_TYPE_WAVE_ACTION "waves_appear"
#define ACTION_TYPE_CONVERSATION_ACTION "conversation_appear"
#define ACTION_TYPE_CUSTOM "custom"

typedef std::function<void(bool)> EventActionCallback;

class EventAction : public cocos2d::Ref {
private:
    class MapLogic* _map_logic;
    class EventTrigger* _trigger;
    
    int _current_round;
    int _repeat;
    float _delay;
    float _interval;
    float _accumulator;
    
    bool _auto_unschedule;
    
    EventActionCallback _callback;
    
    std::string _action_name;
    bool _is_running;
    
    bool _should_recycle;
    
    cocos2d::ValueMap _action_data;
    cocos2d::Map<std::string, cocos2d::Ref*> _params;
    
public:
    EventAction();
    virtual ~EventAction();
    
    static EventAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger  );
    
    bool isInfinite() { return _repeat < 0; }
    
    bool start( const cocos2d::Map<std::string, cocos2d::Ref*> params, bool auto_unschedule );
    void stop();
    
    virtual void updateFrame( float delta );
    
    const std::string& getActionName() { return _action_name; }
    
    void onEventTriggered( bool finish );
    
    bool shouldRecycle() { return _should_recycle; }
};

#endif /* defined(__Boids__EventAction__) */
