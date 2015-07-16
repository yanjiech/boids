//
//  SkillNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/18/15.
//
//

#ifndef __Boids__SkillNode__
#define __Boids__SkillNode__

#include "cocos2d.h"

class UnitNode;

class SkillNode : public cocos2d::Ref {
protected:
    bool _should_recycle;
    UnitNode* _owner;
    
    bool _could_interupt;
    
public:
    SkillNode();
    virtual ~SkillNode();
    
    static SkillNode* create( UnitNode* owner );
    virtual bool init( UnitNode* owner );
    
    virtual void updateFrame( float delta );
    
    bool shouldRecycle() { return _should_recycle; }
    void setShouldRecycle( bool b ) { _should_recycle = b; }
    
    void setOwner( UnitNode* owner );
    UnitNode* getOwner();
    
    virtual void begin();
    virtual void end();
    
    virtual void refreshDamage() {}
    
    bool couldInterupt() { return _could_interupt; }
    void setCouldInterupt( bool b ) { _could_interupt = b; }
};

#endif /* defined(__Boids__SkillNode__) */
