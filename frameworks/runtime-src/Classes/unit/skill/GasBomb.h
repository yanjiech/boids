//
//  GasBomb.h
//  Boids
//
//  Created by chenyanjie on 4/23/15.
//
//

#ifndef __Boids__GasBomb__
#define __Boids__GasBomb__

#include "SkillNode.h"

class GasBomb : public SkillNode {
private:
    float _elapse;
    float _interval;
    float _damage;
    
public:
    GasBomb();
    virtual ~GasBomb();
    
    static GasBomb* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__GasBomb__) */
