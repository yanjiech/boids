//
//  ShadowAssault.h
//  Boids
//
//  Created by chenyanjie on 7/14/15.
//
//

#ifndef __Boids__ShadowAssault__
#define __Boids__ShadowAssault__

#include "SkillNode.h"

class ShadowAssault : public SkillNode {
private:
    float _damage;
    
public:
    ShadowAssault();
    virtual ~ShadowAssault();
    
    static ShadowAssault* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__ShadowAssault__) */
