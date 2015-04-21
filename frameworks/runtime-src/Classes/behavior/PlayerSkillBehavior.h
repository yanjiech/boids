//
//  PlayerSkillBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/11/15.
//
//

#ifndef __Boids__PlayerSkillBehavior__
#define __Boids__PlayerSkillBehavior__

#include "BehaviorBase.h"

class PlayerSkillBehavior : public BehaviorBase {
private:
    float _elapse;
    
public:
    PlayerSkillBehavior();
    virtual ~PlayerSkillBehavior();
    
    static PlayerSkillBehavior* create( class TargetNode* unit_node );
    virtual bool init( TargetNode* unit_node );
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__PlayerSkillBehavior__) */
