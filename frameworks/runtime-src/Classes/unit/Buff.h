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
#include "ElementData.h"

class UnitNode;
class TargetNode;

typedef enum {
    BuffGroupBuff = 1,
    BuffGroupDebuff = 2
}eBuffGroup;

typedef enum {
    BuffEffectPosOrigin = 0,
    BuffEffectPosHead = 1,
    BuffEffectPosBody = 2,
    BuffEffectPosAboveHead = 3
}eBuffEffectPos;

class Buff : public cocos2d::Ref {
protected:
    static int _global_buff_id;
    
    std::string _buff_id;
    std::string _buff_type;
    std::string _buff_name;
    bool _should_recycle;
    
    float _duration;
    float _elapse;
    
    UnitNode* _owner;
    
    cocos2d::ValueMap _data;
    
    eBuffGroup _buff_group;
    
    std::string _effect_resource;
    float _effect_scale;
    float _effect_scale_x;
    float _effect_scale_y;
    cocos2d::Color3B _effect_color;
    std::string _effect_blend;
    
    eBuffEffectPos _effect_pos;
    int _effect_layer; //0 default, 1 over object, 2 below object
    
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
    
    eBuffGroup getBuffGroup() { return _buff_group; }
    void setBuffGroup( eBuffGroup group ) { _buff_group = group; }
};

//class ElementData;
//
//class AttributeBuff : public Buff {
//private:
//    ElementData* _unit_data;
//    
//public:
//    AttributeBuff();
//    virtual ~AttributeBuff();
//    
//    static AttributeBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
//    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
//    
//    virtual void updateFrame( float delta );
//    
//    virtual void begin();
//    virtual void end();
//    
//    ElementData* getElementData();
//    void setElementData( ElementData* data );
//};

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

class RecoverBuff : public Buff {
private:
    float _interval;
    float _recover_elapse;
    float _hp;
    float _mp;
    
public:
    RecoverBuff();
    virtual ~RecoverBuff();
    
    static RecoverBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

class BlessBuff : public Buff {
public:
    float atk_fix;
    float def_fix;
    float hp_fix;
    float mp_fix;
    float cri_fix;
    float ten_fix;
    float hit_fix;
    float dod_fix;
    float mov_fix;
    float range_fix;
    float atk_speed_fix;
    float guard_fix;
    float view_range_fix;
    float rec_fix;
    
    float atk_per;
    float def_per;
    float hp_per;
    float mp_per;
    float cri_per;
    float ten_per;
    float hit_per;
    float dod_per;
    float mov_per;
    float range_per;
    float atk_speed_per;
    float guard_per;
    float view_range_per;
    float rec_per;
    
public:
    BlessBuff();
    virtual ~BlessBuff();
    
    static BlessBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    virtual void apply( ElementData* data );
    virtual void unapply( ElementData* data );
};

class CurseBuff : public BlessBuff {
public:
    CurseBuff();
    virtual ~CurseBuff();
    
    static CurseBuff* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    virtual void apply( ElementData* data );
    virtual void unapply( ElementData* data );
};

#endif /* defined(__Boids__Buff__) */
