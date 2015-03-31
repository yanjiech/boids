//
//  NaturesAttendants.h
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#ifndef __Boids__NaturesAttendants__
#define __Boids__NaturesAttendants__

#include "SkillNode.h"

class NaturesAttendants : public SkillNode {
public:
    NaturesAttendants();
    virtual ~NaturesAttendants();
    
    static NaturesAttendants* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__NaturesAttendants__) */
