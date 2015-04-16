//
//  FatalThrow.h
//  Boids
//
//  Created by chenyanjie on 4/7/15.
//
//

#ifndef __Boids__FatalThrow__
#define __Boids__FatalThrow__

#include "SkillNode.h"

class FatalThrow : public SkillNode {
private:
    float _damage;
    float _intervals[2];
    float _throw_elapse;
    int _left_or_right; //0 left, 1 right
    float _range;
    
    UnitNode* _flamingo;
    
    cocos2d::Node* _hint_effect;
    
public:
    FatalThrow();
    virtual ~FatalThrow();
    
    static FatalThrow* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    void setFlamingo( UnitNode* unit_node );
};

#endif /* defined(__Boids__FatalThrow__) */
