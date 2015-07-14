//
//  TimeRemount.h
//  Boids
//
//  Created by chenyanjie on 7/14/15.
//
//

#ifndef __Boids__TimeRemount__
#define __Boids__TimeRemount__

#include "SkillNode.h"

class TimeRemount : public SkillNode {
private:
    float _duration;
    float _elapse;
    float _hp_per;
    float _mp_per;
    float _interval;
    
public:
    TimeRemount();
    virtual ~TimeRemount();
    
    static TimeRemount* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__TimeRemount__) */
