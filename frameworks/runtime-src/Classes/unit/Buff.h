//
//  Buff.h
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#ifndef __Boids__Buff__
#define __Boids__Buff__

#include "cocos2d.h"

class Buff : public cocos2d::Ref {
public:
    Buff();
    virtual ~Buff();
    
    static Buff* create();
    virtual bool init();
};

#endif /* defined(__Boids__Buff__) */
