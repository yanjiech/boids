//
//  Buff.h
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#ifndef __Boids__Buff__
#define __Boids__Buff__

#include "cocos2d.h"
#include "../constant/BoidsConstant.h"

class UnitNode;
class TargetNode;

class Buff : public cocos2d::Ref {
protected:
    static int _global_buff_id;
    
    std::string _buff_id;
    std::string _buff_type;
    bool _should_recycle;
    
    float _duration;
    float _elapse;
    
    UnitNode* _owner;
    
    cocos2d::ValueMap _data;
    
public:
    static int getNextBuffId();
    
    Buff();
    virtual ~Buff();
    
    virtual Buff* clone();
    
    static Buff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    virtual float filterDamage( float damage, TargetNode* atker ) { return damage; }
    
    bool isInfinite() { return _duration < 0; }
    
    bool shouldRecycle() { return _should_recycle; }
    void setShouldRecycle( bool b ) { _should_recycle = b; }
    
    float getDuration() { return _duration; }
    void setDuration( float duration ) { _duration = duration; }
    
    UnitNode* getOwner();
    void setOwner( UnitNode* owner );
    
    const std::string& getBuffId() { return _buff_id; }
    void setBuffId( const std::string& new_id ) { _buff_id = new_id; }
    
    const std::string& getBuffType() { return _buff_type; }
    void setBuffType( const std::string& buff_type ) { _buff_type = buff_type; }
    
    const cocos2d::ValueMap& getBuffData() { return _data; }
    void setBuffData( const cocos2d::ValueMap& data ) { _data = data; }
};

class ElementData;

class AttributeBuff : public Buff {
private:
    ElementData* _unit_data;
    
public:
    AttributeBuff();
    virtual ~AttributeBuff();
    
    static AttributeBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    ElementData* getElementData();
    void setElementData( ElementData* data );
};

class StunBuff : public Buff {
public:
    StunBuff();
    virtual ~StunBuff();
    
    static StunBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

class BurnBuff : public Buff {
public:
    BurnBuff();
    virtual ~BurnBuff();
};

class PoisonBuff : public Buff {
public:
    PoisonBuff();
    virtual ~PoisonBuff();
    
    static PoisonBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

class SlowBuff : public Buff {
private:
    float _slow_percent;
    
public:
    SlowBuff();
    virtual ~SlowBuff();
    
    static SlowBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

class ShieldBuff : public Buff {
private:
    int _absorb;
    
public:
    ShieldBuff();
    virtual ~ShieldBuff();
    
    static ShieldBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    virtual float filterDamage( float damage, TargetNode* atker );
    
    int getAbsorb() { return _absorb; }
    void setAbsorb( float value ) { _absorb = value; }
};

class UndeadBuff : public Buff {
public:
    UndeadBuff();
    virtual ~UndeadBuff();
    
    static UndeadBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    virtual float filterDamage( float damage, TargetNode* atker );
};

class PierceBuff : public Buff {
public:
    PierceBuff();
    virtual ~PierceBuff();
    
    static PierceBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

class TagBuff : public Buff {
protected:
    std::string _tag;
    class TimeLimitSpineComponent* _component;
    
public:
    TagBuff();
    virtual ~TagBuff();
    
    static TagBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

class TagProtectBuff : public TagBuff {
public:
    TagProtectBuff();
    virtual ~TagProtectBuff();
    
    static TagProtectBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    virtual float filterDamage( float damage, TargetNode* atker );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__Buff__) */
