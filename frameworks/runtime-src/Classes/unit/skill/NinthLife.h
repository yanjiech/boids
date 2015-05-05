//
//  NinthLife.h
//  Boids
//
//  Created by chenyanjie on 5/5/15.
//
//

#ifndef __Boids__NinthLife__
#define __Boids__NinthLife__

#include "SkillNode.h"

class NinthLife : public SkillNode {
private:
    float _buff_duration;
    
public:
    NinthLife();
    virtual ~NinthLife();
    
    static NinthLife* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__NinthLife__) */
