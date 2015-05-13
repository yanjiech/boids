//
//  Impetus.h
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#ifndef __Boids__Impetus__
#define __Boids__Impetus__

#include "SkillNode.h"

class Impetus : public SkillNode {
private:
    float _damage;
    float _range;
    float _buff_duration;
    float _buff_damage;
    
public:
    Impetus();
    virtual ~Impetus();
    
    static Impetus* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__Impetus__) */
