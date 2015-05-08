//
//  GuardAttackBehavior.h
//  Boids
//
//  Created by chenyanjie on 4/22/15.
//
//

#ifndef __Boids__GuardAttackBehavior__
#define __Boids__GuardAttackBehavior__

#include "BehaviorBase.h"

class GuardAttackBehavior : public BehaviorBase {
public:
    GuardAttackBehavior();
    virtual ~GuardAttackBehavior();
    
    static GuardAttackBehavior* create( class TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__GuardAttackBehavior__) */
