//
//  BarrageOfArrow.h
//  Boids
//
//  Created by Yanjie Chen on 3/20/15.
//
//

#ifndef __Boids__BarrageOfArrow__
#define __Boids__BarrageOfArrow__

#include "SkillNode.h"

class BarrageOfArrow : public SkillNode {
private:
    float _base_damage;
    float _interval;
    float _elapse;
    float _speed;
    int _waves;
    int _current_wave;
    int _count_per_wave;
    float _angle;
    float _range;
    float _bullet_length;
    
    float _dir_angle;
    
public:
    BarrageOfArrow();
    virtual ~BarrageOfArrow();
    
    static BarrageOfArrow* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__BarrageOfArrow__) */
