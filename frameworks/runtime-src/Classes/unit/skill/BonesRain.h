//
//  BonesRain.h
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#ifndef __Boids__BonesRain__
#define __Boids__BonesRain__

#include "SkillNode.h"

class BonesRain : public SkillNode {
private:
    float _damage;
    cocos2d::Point _dir;
    cocos2d::Point _init_pos;
    float _radius;
    float _interval;
    float _elapse;
    int _count;
    int _current_count;
    
public:
    BonesRain();
    virtual ~BonesRain();
    
    static BonesRain* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__BonesRain__) */
