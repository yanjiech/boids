//
//  CrazyShoot.h
//  Boids
//
//  Created by Yanjie Chen on 3/20/15.
//
//

#ifndef __Boids__CrazyShoot__
#define __Boids__CrazyShoot__

#include "SkillNode.h"

class CrazyShoot : public SkillNode {
private:
    float _base_damage;
    float _elapse;
    float _interval;
    float _speed;
    float _range;
    int _count;
    int _current_count;
    
public:
    CrazyShoot();
    virtual ~CrazyShoot();
    
    static CrazyShoot* create(  UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init(  UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__CrazyShoot__) */
