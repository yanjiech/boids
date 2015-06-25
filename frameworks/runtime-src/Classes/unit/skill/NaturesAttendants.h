//
//  NaturesAttendants.h
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#ifndef __Boids__NaturesAttendants__
#define __Boids__NaturesAttendants__

#include "SkillNode.h"

class NaturesAttendants : public SkillNode {
private:
    float _heal;
    float _range;
    float _duration;
    float _interval;
    float _elapse;
    float _heal_elapse;
    float _accel;
    float _init_speed;
    int _count;
    
    cocos2d::Vector<class TimeLimitWanderSpineComponent*> _effects;
    
public:
    NaturesAttendants();
    virtual ~NaturesAttendants();
    
    static NaturesAttendants* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__NaturesAttendants__) */
