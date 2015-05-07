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
    float _duration;
    float _shoot_elapse;
    float _interval;
    float _warning_duration;
    float _damage;
    float _damage_radius;
    float _gas_duratoin;
    float _gas_damage;
    float _range;
    
    cocos2d::Point _shoot_pos;
    
    int _stage; //0 load, 1 warn
    
public:
    GasBomb();
    virtual ~GasBomb();
    
    static GasBomb* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__GasBomb__) */
