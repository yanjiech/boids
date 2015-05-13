//
//  PlayerAttackBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/11/15.
//
//

#ifndef __Boids__PlayerAttackBehavior__
#define __Boids__PlayerAttackBehavior__

#include "BehaviorBase.h"

class PlayerAttackBehavior : public BehaviorBase {
public:
    PlayerAttackBehavior();
    virtual ~PlayerAttackBehavior();
    
    static PlayerAttackBehavior* create( class TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__PlayerAttackBehavior__) */
