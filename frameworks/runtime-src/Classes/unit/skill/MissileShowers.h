//
//  MissileShowers.h
//  Boids
//
//  Created by chenyanjie on 4/1/15.
//
//

#ifndef __Boids__MissileShowers__
#define __Boids__MissileShowers__

#include "SkillNode.h"

class MissileShowers : public SkillNode {
private:
    float _damage;
    float _range;
    float _radius;
    int _count;
    int _current_count;
    float _interval;
    float _elapse;
    
public:
    MissileShowers();
    ~MissileShowers();
    
    static MissileShowers* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__MissileShowers__) */
