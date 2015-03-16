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
    AttackBehavior( UnitNode* unit_node );
    virtual ~AttackBehavior();
    
    static AttackBehavior* create( UnitNode* unit_node );
    virtual bool init();
    
    virtual bool behave( float delta );
};

#endif /* defined(__Boids__AttackBehavior__) */
