//
//  BulletNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/13/15.
//
//

#ifndef __Boids__BulletNode__
#define __Boids__BulletNode__

#include "cocos2d.h"
#include "../data/DamageCalculate.h"
#include <spine/spine-cocos2dx.h>

#define DEFAULT_BULLET_SPEED 500.0

class BulletNode : public cocos2d::Node {
protected:
    static int _global_bullet_id;
    
    cocos2d::ValueMap _bullet_data;
    
    class BattleLayer* _battle_layer;
    
    int _bullet_id;
    
    int _target_id;
    
    int _source_id;
    
    int _source_camp;
    
    float _speed;
    
    class UnitData* _unit_data;
    
    DamageCalculate* _damage_calculator;
    
    class Buff* _buff;
    
    cocos2d::Point _target_pos;
    
    cocos2d::Node* _bullet;
    cocos2d::MotionStreak* _streak;
    
    cocos2d::Point _init_pos;
    
    bool _is_paracurve;
    bool _track_target;
    bool _need_rotate;
    
    float _accumulator;
    float _duration;
    
    float _damage_radius;
    
    bool _should_recycle;
    
protected:
    void shoot( class UnitNode* source );
    
    bool doesHitTarget( const cocos2d::Point& source_pos, const cocos2d::Point& target_pos, float delta );
    
public:
    BulletNode();
    virtual ~BulletNode();
    
    static int getNextBulletId();
    
    static BulletNode* create( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, class Buff* buff );
    virtual bool init( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, class Buff* buff );
    
    virtual void shootAt( class UnitNode* source, class TargetNode* target );
    virtual void shootAtPosition( class UnitNode* source, const cocos2d::Point& pos );
    
    virtual void updateFrame( float delta );
    
    int getBulletId() { return _bullet_id; }
    void setBulletId( int bullet_id ) { _bullet_id = bullet_id; }
    
    int getTargetId() { return _target_id; }
    void setTargetId( int target_id ) { _target_id = target_id; }
    
    int getSourceId() { return _source_id; }
    void setSourceId( int source_id ) { _source_id = source_id; }
    
    int getSourceCamp() { return _source_camp; }
    void setSourceCamp( int camp ) { _source_camp = camp; }
    
    void setUnitData( class UnitData* unit_data );
    void setDamageCalculator( DamageCalculate* calculator );
    void setBuff( class Buff* buff );
    
    void setShouldRecycle( bool b );
    bool shouldRecycle() { return _should_recycle; }
};

class DirectionalBulletNode : public BulletNode {
private:
    cocos2d::Point _dir;
    float _duration;
    cocos2d::ValueMapIntKey _excluded_targets;
    
public:
    DirectionalBulletNode();
    virtual ~DirectionalBulletNode();
    
    static DirectionalBulletNode* create( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, class Buff* buff );
    virtual bool init( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, class Buff* buff );
    
    void shootAlong( const cocos2d::Point& dir, float duration, class UnitNode* source_unit );
    
    virtual void updateFrame( float delta );
};

class FixedPosBulletNode : public BulletNode {
public:
    FixedPosBulletNode();
    virtual ~FixedPosBulletNode();
    
    static FixedPosBulletNode* create( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, class Buff* buff );
    virtual bool init( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, class Buff* buff );
    
    virtual void shootAtPosition( const cocos2d::Point& pos );
    virtual void updateFrame( float delta );
    
    void onSkeletonAnimationEvent( int track_index, spEvent* event );
    void onSkeletonAnimationCompleted( int track_index );
};

class HealBullet : public BulletNode {
private:
    float _duration;
    
public:
    HealBullet();
    virtual ~HealBullet();
    
    static HealBullet* create( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator );
    virtual bool init( class UnitNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculator* damage_calculator );
    
    virtual void updateFrame( float delta );
};

#endif /* defined(__Boids__BulletNode__) */
