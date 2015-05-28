//
//  DarkIllusion.h
//  Boids
//
//  Created by chenyanjie on 5/27/15.
//
//

#ifndef __Boids__DarkIllusion__
#define __Boids__DarkIllusion__

#include "SkillNode.h"
#include "../UnitNodeComponent.h"

class DarkIllusion : public SkillNode {
private:
    float _damage;
    float _range;
    float _radius;
    float _duration;
    float _elase;
    float _speed;
    float _stun_duration;
    cocos2d::ValueVector _exclude_targets;
    
    TimeLimitSpineComponent* _component;
    
    cocos2d::Point _dir;
    
public:
    DarkIllusion();
    virtual ~DarkIllusion();
    
    static DarkIllusion* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__DarkIllusion__) */
