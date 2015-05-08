//
//  LightShield.h
//  Boids
//
//  Created by chenyanjie on 3/25/15.
//
//

#ifndef __Boids__LightShield__
#define __Boids__LightShield__

#include "SkillNode.h"

class LightShield : public SkillNode {
private:
    float _duration;
    float _absorb;
    
public:
    LightShield();
    virtual ~LightShield();
    
    static LightShield* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__LightShield__) */
