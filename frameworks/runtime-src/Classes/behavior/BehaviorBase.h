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

class TargetNode;
class UnitNode;
class TowerNode;

class BehaviorBase : public cocos2d::Ref {
protected:
    TargetNode* _target_node;
    
    bool _is_enabled;
    
public:
    BehaviorBase();
    virtual ~BehaviorBase();
    
    virtual bool init( TargetNode* target_node );
    virtual bool behave( float delta = 0 );
    
    TargetNode* getTargetNode() { return _target_node; }
    void setTargetNode( TargetNode* target_node ) { _target_node = target_node; }
    
    bool isEnabled() { return _is_enabled; }
    void setEnabled( bool b ) { _is_enabled = b; }
};

#endif /* defined(__Boids__BehaviorBase__) */
