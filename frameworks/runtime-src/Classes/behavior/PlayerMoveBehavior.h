//
//  PlayerMoveBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/10/15.
//
//

#ifndef __Boids__PlayerMoveBehavior__
#define __Boids__PlayerMoveBehavior__

#include "BehaviorBase.h"

class PlayerMoveBehavior : public BehaviorBase {
public:
    PlayerMoveBehavior();
    virtual ~PlayerMoveBehavior();
    
    static PlayerMoveBehavior* create( UnitNode* unit_node );
    virtual bool init( UnitNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__PlayerMoveBehavior__) */
