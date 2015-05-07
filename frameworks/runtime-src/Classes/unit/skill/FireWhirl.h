//
//  FireWhirl.h
//  Boids
//
//  Created by chenyanjie on 4/8/15.
//
//

#ifndef __Boids__FireWhirl__
#define __Boids__FireWhirl__

#include "SkillNode.h"

class FireWhirl : public SkillNode {
private:
    float _damage;
    float _duration;
    float _interval;
    float _elapse;
    float _damage_elapse;
    float _range;
    
    class TimeLimitSpineComponent* _effect_node;
    
public:
    FireWhirl();
    virtual ~FireWhirl();
    
    static FireWhirl* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__FireWhirl__) */
