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
#include "spine/spine-cocos2dx.h"

class CrazyScratch : public SkillNode {
private:
    float _duration;
    float _range;
    float _damage;
    float _interval;
    float _elapse;
    int _count;
    cocos2d::Point _origin_pos;
    cocos2d::Point _last_pos;
    
    cocos2d::Map<int,UnitNode*> _excluded_targets;
    
    spine::SkeletonAnimation* _skeleton;
    
public:
    CrazyScratch();
    virtual ~CrazyScratch();
    
    static CrazyScratch* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
};

#endif /* defined(__Boids__CrazyScratch__) */
