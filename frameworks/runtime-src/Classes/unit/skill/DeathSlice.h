//
//  DeathSlice.h
//  Boids
//
//  Created by chenyanjie on 4/8/15.
//
//

#ifndef __Boids__DeathSlice__
#define __Boids__DeathSlice__

#include "SkillNode.h"

class DeathSlice : public SkillNode {
private:
    float _damage;
    float _duration;
    float _interval;
    float _range;
    float _damage_radius;
    float _speed;
    float _slow_percent;
    
public:
    DeathSlice();
    virtual ~DeathSlice();
    
    static DeathSlice* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__DeathSlice__) */
