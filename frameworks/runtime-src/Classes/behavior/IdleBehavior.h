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
    IdleBehavior( UnitNode* unit_node );
    virtual ~IdleBehavior();
    
    static IdleBehavior* create( UnitNode* unit_node );
    virtual bool init();
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__IdleBehavior__) */
