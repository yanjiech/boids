//
//  PenetrateSpear.h
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#ifndef __Boids__PenetrateSpear__
#define __Boids__PenetrateSpear__

#include "SkillNode.h"

class PenetrateSpear : public SkillNode {
public:
    PenetrateSpear();
    virtual ~PenetrateSpear();
    
    static PenetrateSpear* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__PenetrateSpear__) */
