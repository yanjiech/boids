//
//  Trigger.h
//  Boids
//
//  Created by chenyanjie on 3/26/15.
//
//

#ifndef __Boids__Trigger__
#define __Boids__Trigger__

#include "cocos2d.h"
#include "../constant/BoidsConstant.h"

class Trigger : public cocos2d::Ref {
protected:
    std::string _trigger_type;
    bool _could_trigger;
    cocos2d::ValueMap _trigger_data;
    
public:
    Trigger();
    virtual ~Trigger();
    
    static Trigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    const std::string& getTriggerType() { return _trigger_type; }
    void setTriggerType( const std::string& type ) { _trigger_type = type; }
    
    bool couldTrigger() { return _could_trigger; }
    void setCouldTrigger( bool b ) { _could_trigger = b; }
    
    virtual void updateTrigger( const cocos2d::ValueMap& update_data );
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class MapInitTrigger : public Trigger {
public:
    MapInitTrigger();
    virtual ~MapInitTrigger();
    
    static MapInitTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

class UnitChangeTrigger : public Trigger {
public:
    UnitChangeTrigger();
    virtual ~UnitChangeTrigger();
    
    static UnitChangeTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class ConversationChangeTrigger : public Trigger {
public:
    ConversationChangeTrigger();
    virtual ~ConversationChangeTrigger();
    
    static ConversationChangeTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

class EventChangeTrigger : public Trigger {
public:
    EventChangeTrigger();
    virtual ~EventChangeTrigger();
    
    static EventChangeTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

class VisionChangeTrigger : public Trigger {
public:
    VisionChangeTrigger();
    virtual ~VisionChangeTrigger();
    
    static VisionChangeTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

class CustomTrigger : public Trigger {
public:
    CustomTrigger();
    virtual ~CustomTrigger();
    
    static CustomTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};


#endif /* defined(__Boids__Trigger__) */
