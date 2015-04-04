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

typedef std::function<void(bool)> EventActionCallback;

class EventAction : public cocos2d::Ref {
protected:
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
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    bool isInfinite() { return _repeat < 0; }
    
    bool start( const cocos2d::Map<std::string, cocos2d::Ref*> params, bool auto_unschedule );
    void stop();
    
    virtual void updateFrame( float delta );
    
    const std::string& getActionName() { return _action_name; }
    
    virtual void onActionTriggered( bool finish );
    
    bool shouldRecycle() { return _should_recycle; }
    void setShouldRecycle( bool b ) { _should_recycle = b; }
};

class UnitChangeAction : public EventAction {
public:
    UnitChangeAction();
    virtual ~UnitChangeAction();
    
    static UnitChangeAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

class TaskChangeAction : public EventAction {
public:
    TaskChangeAction();
    virtual ~TaskChangeAction();
    
    static TaskChangeAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

class GameChangeAction : public EventAction {
public:
    GameChangeAction();
    virtual ~GameChangeAction();
    
    static GameChangeAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

class EventChangeAction : public EventAction {
public:
    EventChangeAction();
    virtual ~EventChangeAction();
    
    static EventChangeAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

class VisionChangeAction : public EventAction {
public:
    VisionChangeAction();
    virtual ~VisionChangeAction();
    
    static VisionChangeAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

class WaveAction : public EventAction {
public:
    WaveAction();
    virtual ~WaveAction();
    
    static WaveAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

class ConversationAction : public EventAction {
private:
    int _repeat_times;
    int _current_times;
    bool _is_random_order;
    float _interval;
    float _elapse;
    int _current_speech_id;
    
public:
    ConversationAction();
    virtual ~ConversationAction();
    
    static ConversationAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
    
    virtual void updateFrame( float delta );
};

class CustomAction : public EventAction {
public:
    CustomAction();
    virtual ~CustomAction();
    
    static CustomAction* create( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    virtual bool init( const cocos2d::ValueMap& action_data, class MapLogic* map_logic, class EventTrigger* trigger );
    
    virtual void onActionTriggered( bool finish );
};

#endif /* defined(__Boids__EventAction__) */
