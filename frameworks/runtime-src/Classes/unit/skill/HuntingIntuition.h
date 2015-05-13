//
//  HuntingIntuition.h
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#ifndef __Boids__HuntingIntuition__
#define __Boids__HuntingIntuition__

#include "SkillNode.h"

class HuntingIntuition : public SkillNode {
private:
    float _buff_duration;
    float _cri_up;
    float _hit_up;
    
public:
    HuntingIntuition();
    virtual ~HuntingIntuition();
    
    static HuntingIntuition* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__HuntingIntuition__) */
