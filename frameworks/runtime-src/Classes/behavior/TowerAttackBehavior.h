//
//  TowerAttackBehavior.h
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#ifndef __Boids__TowerAttackBehavior__
#define __Boids__TowerAttackBehavior__

#include "BehaviorBase.h"

class TowerAttackBehavior : public BehaviorBase {
public:
    TowerAttackBehavior();
    virtual ~TowerAttackBehavior();
    
    static TowerAttackBehavior* create( TowerNode* tower );
    virtual bool init( TowerNode* tower );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__TowerAttackBehavior__) */
