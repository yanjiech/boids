//
//  MagicPulse.h
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#ifndef __Boids__MagicPulse__
#define __Boids__MagicPulse__

#include "SkillNode.h"

class TimeLimitSpineComponent;

class MagicPulse : public SkillNode {
private:
    float _damage;
    float _interval;
    float _elapse;
    float _damage_elapse;
    float _range;
    float _length;
    float _duration;
    UnitNode* _target_unit;
    
    TimeLimitSpineComponent* _component;
    TimeLimitSpineComponent* _hit_component;
    
    void adjustComponent();
    
public:
    MagicPulse();
    virtual ~MagicPulse();
    
    static MagicPulse* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    void setTargetUnit( UnitNode* targetUnit );
};

#endif /* defined(__Boids__MagicPulse__) */
