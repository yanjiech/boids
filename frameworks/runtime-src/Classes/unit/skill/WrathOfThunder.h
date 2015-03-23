//
//  WrathOfThunder.h
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#ifndef __Boids__WrathOfThunder__
#define __Boids__WrathOfThunder__

#include "SkillNode.h"

class WrathOfThunder : public SkillNode {
private:
    float _range;
    int _count;
    float _base_damage;
    
public:
    WrathOfThunder();
    virtual ~WrathOfThunder();
    
    static WrathOfThunder* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__WrathOfThunder__) */
