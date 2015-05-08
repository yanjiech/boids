//
//  IdleBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__IdleBehavior__
#define __Boids__IdleBehavior__

#include "BehaviorBase.h"

class IdleBehavior : public BehaviorBase {
public:
    IdleBehavior();
    virtual ~IdleBehavior();
    
    static IdleBehavior* create( TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__IdleBehavior__) */
