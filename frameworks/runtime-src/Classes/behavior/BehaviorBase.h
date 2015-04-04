//
//  BehaviorBase.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__BehaviorBase__
#define __Boids__BehaviorBase__

#include "cocos2d.h"

#define BEHAVIOR_NAME_MOVE "move"
#define BEHAVIOR_NAME_ATTACK "attack"
#define BEHAVIOR_NAME_SKILL "skill"
#define BEHAVIOR_NAME_IDLE "idle"

class UnitNode;

class BehaviorBase : public cocos2d::Ref {
protected:
    UnitNode* _unit_node;
    
public:
    BehaviorBase( UnitNode* unit_node );
    virtual ~BehaviorBase();
    
    virtual bool init();
    virtual bool behave( float delta = 0 );
    
    UnitNode* getUnitNode() { return _unit_node; }
    void setUnitNode( UnitNode* unit_node ) { _unit_node = unit_node; }
};

#endif /* defined(__Boids__BehaviorBase__) */
