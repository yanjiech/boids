//
//  CrazyScratch.h
//  Boids
//
//  Created by chenyanjie on 5/5/15.
//
//

#ifndef __Boids__CrazyScratch__
#define __Boids__CrazyScratch__

#include "SkillNode.h"

class CrazyScratch : public SkillNode {
private:
    float _duration;
    float _elapse;
    float _range;
    float _damage;
    float _damage_interval;
    float _damage_elapse;
    cocos2d::Point _dir;
    
public:
    CrazyScratch();
    virtual ~CrazyScratch();
    
    static CrazyScratch* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    virtual void refreshDamage();
};

#endif /* defined(__Boids__CrazyScratch__) */
