//
//  UnitNodeComponent.h
//  Boids
//
//  Created by Yanjie Chen on 3/8/15.
//
//

#ifndef __Boids__UnitNodeComponent__
#define __Boids__UnitNodeComponent__

#include "cocos2d.h"
#include "../ArmatureManager.h"

enum eComponentLayer {
    MostBelow = 0,
    SelfBelow = 1,
    SelfAbove = 2,
    Object = 3,
    OverObject = 4,
    Stencil = 5,
    Float = 6
};

class UnitNodeComponent : public cocos2d::Node {
protected:
    bool _auto_recycle;
    bool _should_recycle;

    cocos2d::Node* _node;
    
public:
    UnitNodeComponent();
    virtual ~UnitNodeComponent();
    
    static UnitNodeComponent* create( cocos2d::Node* node, const std::string& name, bool auto_recycle );
    virtual bool init( cocos2d::Node* node, const std::string& name, bool auto_recycle );
    
    bool shouldRecycle() { return _should_recycle; }
    void setShouldRecycle( bool b ) { _should_recycle = b; }
    
    cocos2d::Node* getNode() { return _node; }
    
    virtual void updateFrame( float delta );
};

class UnitNodeSpineComponent : public UnitNodeComponent {
public:
    UnitNodeSpineComponent();
    virtual ~UnitNodeSpineComponent();
    static UnitNodeSpineComponent* create( spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    virtual bool init( spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    
    bool setAnimation( int track_index, const std::string& name, bool loop );
    void onSkeletonAnimationEnded( int track_index );
};

#endif /* defined(__Boids__UnitNodeComponent__) */
