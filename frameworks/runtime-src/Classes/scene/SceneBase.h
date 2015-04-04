//
//  SceneBase.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__SceneBase__
#define __Boids__SceneBase__

#include "cocos2d.h"

class SceneBase : public cocos2d::Scene {
public:
    SceneBase();
    virtual ~SceneBase();
    
    static SceneBase* create();
    
    virtual bool init();
};

#endif /* defined(__Boids__SceneBase__) */
