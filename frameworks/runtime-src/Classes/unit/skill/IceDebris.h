//
//  IceDebris.h
//  Boids
//
//  Created by chenyanjie on 5/29/15.
//
//

#ifndef __Boids__IceDebris__
#define __Boids__IceDebris__

#include "SkillNode.h"

class IceDebris : public SkillNode {
public:
    IceDebris();
    virtual ~IceDebris();
    
    static IceDebris* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__IceDebris__) */
