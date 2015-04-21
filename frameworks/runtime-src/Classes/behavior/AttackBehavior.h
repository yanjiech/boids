//
//  AttackBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__AttackBehavior__
#define __Boids__AttackBehavior__

#include "BehaviorBase.h"

class AttackBehavior : public BehaviorBase {
public:
    AttackBehavior();
    virtual ~AttackBehavior();
    
    static AttackBehavior* create( TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta );
};

#endif /* defined(__Boids__AttackBehavior__) */
