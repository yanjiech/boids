//
//  GiantSword.h
//  Boids
//
//  Created by chenyanjie on 3/25/15.
//
//

#ifndef __Boids__GiantSword__
#define __Boids__GiantSword__

#include "SkillNode.h"

class GiantSword : public SkillNode {
private:
    float _radius;
    float _range;
    float _damage;
    float _stun;
    cocos2d::Point _dir;
    float _duration;
    
public:
    GiantSword();
    virtual ~GiantSword();
    
    static GiantSword* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__GiantSword__) */
