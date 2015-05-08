//
//  MoveBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__MoveBehavior__
#define __Boids__MoveBehavior__

#include "BehaviorBase.h"

class MoveBehavior : public BehaviorBase {
public:
    MoveBehavior();
    virtual ~MoveBehavior();
    
    static MoveBehavior* create( TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__MoveBehavior__) */
