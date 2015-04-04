//
//  BatSwarm.h
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#ifndef __Boids__BatSwarm__
#define __Boids__BatSwarm__

#include "SkillNode.h"

class BatSwarm : public SkillNode {
public:
    BatSwarm();
    virtual ~BatSwarm();
    
    static BatSwarm* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__BatSwarm__) */
