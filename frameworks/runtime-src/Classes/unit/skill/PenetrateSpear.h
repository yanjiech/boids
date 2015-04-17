//
//  PenetrateSpear.h
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#ifndef __Boids__PenetrateSpear__
#define __Boids__PenetrateSpear__

#include "SkillNode.h"

class PenetrateSpear : public SkillNode {
private:
    float _damage;
    float _duration;
    float _speed;
    float _range;
    float _damage_radius;
    std::string _bullet_name;
    
public:
    PenetrateSpear();
    virtual ~PenetrateSpear();
    
    static PenetrateSpear* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__PenetrateSpear__) */
