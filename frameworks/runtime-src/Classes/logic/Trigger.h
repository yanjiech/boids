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
    
    virtual void updateFrame( float delta );
};

class MapInitTrigger : public Trigger {
public:
    MapInitTrigger();
    virtual ~MapInitTrigger();
    
    static MapInitTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

class UnitChangeTrigger : public Trigger {
protected:
    int _need_count;
    int _current_count;
    
public:
    UnitChangeTrigger();
    virtual ~UnitChangeTrigger();
    
    static UnitChangeTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class UnitAppearTrigger : public UnitChangeTrigger {
public:
    UnitAppearTrigger();
    virtual ~UnitAppearTrigger();
    
    static UnitAppearTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class UnitDisappearTrigger : public UnitChangeTrigger {
public:
    UnitDisappearTrigger();
    virtual ~UnitDisappearTrigger();
    
    static UnitDisappearTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class UnitDeadTrigger : public UnitChangeTrigger {
public:
    UnitDeadTrigger();
    virtual ~UnitDeadTrigger();
    
    static UnitDeadTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class UnitAliveTrigger : public UnitChangeTrigger {
public:
    UnitAliveTrigger();
    virtual ~UnitAliveTrigger();
    
    static UnitAliveTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
};

class UnitStayTrigger : public Trigger {
private:
    float _elapse;
    float _duration;
    
    bool _is_active;
    
public:
    UnitStayTrigger();
    virtual ~UnitStayTrigger();
    
    static UnitStayTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void updateTrigger( class MapLogic* map_logic, class UnitNode* unit_node, const std::string& unit_state );
    
    virtual void updateFrame( float delta );
    
    bool isActive() { return _is_active; }
    void setActive( bool b ) { _is_active = b; }
};

class StoryChangeTrigger : public Trigger {
public:
    StoryChangeTrigger();
    virtual ~StoryChangeTrigger();
    
    static StoryChangeTrigger* create( const cocos2d::ValueMap& data );
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
    
    virtual void updateTrigger( const cocos2d::ValueMap& update_data );
};

class CustomTrigger : public Trigger {
public:
    CustomTrigger();
    virtual ~CustomTrigger();
    
    static CustomTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

class GameChangeTrigger : public Trigger {
public:
    GameChangeTrigger();
    virtual ~GameChangeTrigger();
    
    static GameChangeTrigger* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};


#endif /* defined(__Boids__Trigger__) */
