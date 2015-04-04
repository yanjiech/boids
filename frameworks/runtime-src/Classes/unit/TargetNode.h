//
//  TargetNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__TargetNode__
#define __Boids__TargetNode__

#include "cocos2d.h"

class TargetNode : public cocos2d::Node {
private:
    bool _is_attackable;
    
public:
    TargetNode();
    virtual ~TargetNode();
    
    virtual bool init();
    
    void setAttackable( bool b ) { _is_attackable = b; }
    bool isAttackable() { return _is_attackable; }
};

#endif /* defined(__Boids__TargetNode__) */
