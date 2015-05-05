//
//  DarkPierce.h
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#ifndef __Boids__DarkPierce__
#define __Boids__DarkPierce__

#include "SkillNode.h"

class DarkPierce : public SkillNode {
private:
    int _count;
    float _damage;
    float _range;
    float _duration;
    
public:
    DarkPierce();
    virtual ~DarkPierce();
    
    static DarkPierce* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__DarkPierce__) */
