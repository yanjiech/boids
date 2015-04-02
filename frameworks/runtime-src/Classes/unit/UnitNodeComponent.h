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
    BelowObject = 3,
    Object = 4,
    OverObject = 5,
    Stencil = 6,
    Float = 7
};

class DamageCalculate;
class UnitNode;

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

class TimeLimitComponent : public UnitNodeComponent {
protected:
    float _duration;
    float _elapse;
    
public:
    TimeLimitComponent();
    virtual ~TimeLimitComponent();
    
    static TimeLimitComponent* create( float duration, cocos2d::Node* node, const std::string& name, bool auto_recycle );
    virtual bool init( float duration, cocos2d::Node* node, const std::string& name, bool auto_recycle );
    
    virtual void updateFrame( float delta );
};

class UnitNodeSpineComponent : public UnitNodeComponent {
public:
    UnitNodeSpineComponent();
    virtual ~UnitNodeSpineComponent();
    static UnitNodeSpineComponent* create( spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    virtual bool init( spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    
    bool setAnimation( int track_index, const std::string& name, bool loop );
    void onSkeletonAnimationCompleted( int track_index );
};


class UnitNodeFollowSpineComponent : public UnitNodeSpineComponent {
private:
    UnitNode* _source_unit;
    std::string _bone_name;
    
public:
    UnitNodeFollowSpineComponent();
    virtual ~UnitNodeFollowSpineComponent();
    static UnitNodeFollowSpineComponent* create( UnitNode* source_unit, const std::string& bone_name, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    virtual bool init( UnitNode* source_unit, const std::string& bone_name, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    
    virtual void updateFrame( float delta );
    
    void setSourceUnit( UnitNode* source_unit );
};


class UnitNodeSpineDamageComponent : public UnitNodeSpineComponent {
private:
    UnitNode* _source_unit;
    UnitNode* _target_unit;
    DamageCalculate* _damage_calculator;
    
public:
    UnitNodeSpineDamageComponent();
    virtual ~UnitNodeSpineDamageComponent();
    
    static UnitNodeSpineDamageComponent* create( UnitNode* source_unit, UnitNode* target_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle, DamageCalculate* calculator );
    virtual bool init( UnitNode* source_unit, UnitNode* target_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle, DamageCalculate* calculator );
    
    void onSkeletonAnimationEvent( int track_index, spEvent* event );
    
    void setSourceUnit( UnitNode* source_unit );
    void setTargetUnit( UnitNode* target_unit );
    void setDamageCalculator( DamageCalculate* calculator );
};

class TimeLimitSpineComponent : public UnitNodeComponent {
protected:
    float _duration;
    float _elapse;
    
public:
    TimeLimitSpineComponent();
    virtual ~TimeLimitSpineComponent();
    
    static TimeLimitSpineComponent* create( float duration, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    virtual bool init( float duration, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    
    virtual void updateFrame( float delta );
    
    bool setAnimation( int track_index, const std::string& name, bool loop );
    
    void setDuration( float duration ) { _duration = duration; }
};

class TimeLimitWanderSpineComponent : public TimeLimitSpineComponent {
protected:
    float _init_speed;
    float _accelarate;
    cocos2d::Point _speed;
    cocos2d::Point _gravity_point;
    float _range;
    UnitNode* _source_unit;
    float _accel_change_interval;
    float _accel_change_elapse;
    
public:
    TimeLimitWanderSpineComponent();
    virtual ~TimeLimitWanderSpineComponent();
    
    static TimeLimitWanderSpineComponent* create( const cocos2d::ValueMap& data, UnitNode* source_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    virtual bool init( const cocos2d::ValueMap& data, UnitNode* source_unit, spine::SkeletonAnimation* skeleton, const std::string& name, bool auto_recycle );
    
    virtual void updateFrame( float delta );
    
    void setSourceUnit( UnitNode* source_unit );
};


class UnitNodeFadeoutComponent : public UnitNodeComponent {
public:
    UnitNodeFadeoutComponent();
    virtual ~UnitNodeFadeoutComponent();
    
    static UnitNodeFadeoutComponent* create( cocos2d::Node* node, const std::string& name, float duration, int fadeto, bool auto_recycle );
    virtual bool init( cocos2d::Node* node, const std::string& name, float duration, int fadeto, bool auto_recycle );
    
    void onFadeoutEnd();
};

#endif /* defined(__Boids__UnitNodeComponent__) */
