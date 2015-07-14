//
//  SenilityCurse.h
//  Boids
//
//  Created by chenyanjie on 7/14/15.
//
//

#ifndef __Boids__SenilityCurse__
#define __Boids__SenilityCurse__

#include "SkillNode.h"

class SenilityCurse : public SkillNode {
private:
    float _duration;
    float _range;
    float _atk_per;
    float _def_per;
    float _mov_per;
    
public:
    SenilityCurse();
    virtual ~SenilityCurse();
    
    static SenilityCurse* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__SenilityCurse__) */
