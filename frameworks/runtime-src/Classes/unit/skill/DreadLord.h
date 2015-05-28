//
//  DreadLord.h
//  Boids
//
//  Created by chenyanjie on 5/27/15.
//
//

#ifndef __Boids__DreadLord__
#define __Boids__DreadLord__

#include "SkillNode.h"

class DreadLord : public SkillNode {
private:
    float _extra_atk;
    float _extra_dod;
    float _duration;
    float _elapse;
    
public:
    DreadLord();
    virtual ~DreadLord();
    
    static DreadLord* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};


#endif /* defined(__Boids__DreadLord__) */
