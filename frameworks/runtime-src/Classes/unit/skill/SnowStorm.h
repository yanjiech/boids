//
//  SnowStorm.h
//  Boids
//
//  Created by chenyanjie on 5/29/15.
//
//

#ifndef __Boids__SnowStorm__
#define __Boids__SnowStorm__

#include "SkillNode.h"

class SnowStorm : public SkillNode {
private:
    float _damage;
    float _radius;
    cocos2d::Point _target_pos;
    float _duration;
    float _elapse;
    
public:
    SnowStorm();
    virtual ~SnowStorm();
    
    static SnowStorm* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__SnowStorm__) */
