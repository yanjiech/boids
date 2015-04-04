//
//  SkillBehavior.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__SkillBehavior__
#define __Boids__SkillBehavior__

#include "BehaviorBase.h"

class SkillBehavior : public BehaviorBase {
private:
    float _elapse;
    
public:
    SkillBehavior( UnitNode* unit_node );
    virtual ~SkillBehavior();
    
    static SkillBehavior* create( UnitNode* unit_node );
    virtual bool init();
    
    virtual bool behave( float delta = 0 );
};

#endif /* defined(__Boids__SkillBehavior__) */
