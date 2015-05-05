//
//  AtomFart.h
//  Boids
//
//  Created by chenyanjie on 4/7/15.
//
//

#ifndef __Boids__AtomFart__
#define __Boids__AtomFart__

#include "SkillNode.h"

class AtomFart : public SkillNode {
private:
    float _damage;
    float _range;
    float _buff_damage;
    float _buff_duration;
    
public:
    AtomFart();
    virtual ~AtomFart();
    
    static AtomFart* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__AtomFart__) */
