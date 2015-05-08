//
//  GuardMoveBehavior.h
//  Boids
//
//  Created by chenyanjie on 4/22/15.
//
//

#ifndef __Boids__GuardMoveBehavior__
#define __Boids__GuardMoveBehavior__

#include "BehaviorBase.h"

class GuardMoveBehavior : public BehaviorBase {
public:
    GuardMoveBehavior();
    virtual ~GuardMoveBehavior();
    
    static GuardMoveBehavior* create( class TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__GuardMoveBehavior__) */
