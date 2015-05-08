//
//  LightningChain.h
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#ifndef __Boids__LightningChain__
#define __Boids__LightningChain__

#include "SkillNode.h"

class LightningChain : public SkillNode {
private:
    UnitNode* _target_node;
    
    float _range;
    float _length;
    float _base_damage;
    int _count;
    int _current_count;
    
    float _interval;
    float _elapse;
    
    cocos2d::ValueVector _exclude_targets;
    cocos2d::Point _last_pos;
    
public:
    LightningChain();
    virtual ~LightningChain();
    
    static LightningChain* create( UnitNode* owner, const cocos2d::ValueMap& data, UnitNode* target_node );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, UnitNode* target_node );
    
    UnitNode* getTargetNode();
    void setTargetNode( UnitNode* target_node );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__LightningChain__) */
