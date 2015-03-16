//
//  UnitSkill.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__UnitSkill__
#define __Boids__UnitSkill__

#include "cocos2d.h"
#include "../interface/Updatable.h"

class UnitSkill : public cocos2d::Ref, public boids::Updatable {
public:
    
    virtual void updateFrame( float delta );
};

#endif /* defined(__Boids__UnitSkill__) */
