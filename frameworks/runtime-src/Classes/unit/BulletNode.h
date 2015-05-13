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
    
    float _speed;
    
    class TargetNode* _source_unit;
    
    DamageCalculate* _damage_calculator;
    
    cocos2d::ValueMap _buff_data;
    
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
    
    bool _show_hit_effect;
    std::string _hit_name;
    std::string _hit_type;
    
    bool _show_bomb_effect;
    std::string _bomb_name;
    std::string _bomb_type;
    
    float _hit_scale;
    
    bool _will_miss;
    
protected:
    void shoot( int emit_pos_type = 0 );
    
    bool doesHitTarget( const cocos2d::Point& source_pos, const cocos2d::Point& target_pos, float delta );
    
public:
    BulletNode();
    virtual ~BulletNode();
    
    static int getNextBulletId();
    
    static BulletNode* create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    virtual bool init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    
    virtual void shootAt( class TargetNode* target, int emit_pos_type = 0 );
    virtual void shootAtPosition( const cocos2d::Point& pos );
    
    virtual void updateFrame( float delta );
    
    int getBulletId() { return _bullet_id; }
    void setBulletId( int bullet_id ) { _bullet_id = bullet_id; }
    
    int getTargetId() { return _target_id; }
    void setTargetId( int target_id ) { _target_id = target_id; }
    
    void setSourceUnit( class TargetNode* source_unit );
    void setDamageCalculator( DamageCalculate* calculator );
    
    void setShouldRecycle( bool b );
    bool shouldRecycle() { return _should_recycle; }
    
    float getDuration() { return _duration; }
    void setDuration( float duration ) { _duration = duration; }
    
    virtual void hitTarget( class TargetNode* target_unit, bool with_buff = false );
};

class DirectionalBulletNode : public BulletNode {
private:
    cocos2d::Point _dir;
    cocos2d::ValueMapIntKey _excluded_targets;
    
public:
    DirectionalBulletNode();
    virtual ~DirectionalBulletNode();
    
    static DirectionalBulletNode* create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    virtual bool init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    
    void shootAlong( const cocos2d::Point& dir, float duration );
    
    virtual void updateFrame( float delta );
};

class DirectionalLastingBulletNode : public BulletNode {
protected:
    cocos2d::Point _dir;
    cocos2d::Point _to_pos;
    float _last_distance_to_pos;
    
    bool _reach_to_pos;
    
    float _damage_interval;
    float _damage_elapse;
    
    float _lasting_damage;
    
    cocos2d::ValueMapIntKey _excluded_targets;
    
public:
    DirectionalLastingBulletNode();
    virtual ~DirectionalLastingBulletNode();
    
    static DirectionalLastingBulletNode* create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    virtual bool init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    
    void shootTo( const cocos2d::Point& from_pos, const cocos2d::Point& to_pos );
    
    virtual void updateFrame( float delta );
};

class FixedPosBulletNode : public BulletNode {
public:
    FixedPosBulletNode();
    virtual ~FixedPosBulletNode();
    
    static FixedPosBulletNode* create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data);
    virtual bool init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    
    virtual void shootAtPosition( const cocos2d::Point& pos );
    virtual void updateFrame( float delta );
    
    void onSkeletonAnimationEvent( int track_index, spEvent* event );
    void onSkeletonAnimationCompleted( int track_index );
};

class BombBulletNode : public DirectionalLastingBulletNode {
public:
    BombBulletNode();
    virtual ~BombBulletNode();
    
    static BombBulletNode* create( class TargetNode* shooter, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    virtual bool init( class TargetNode* shooter, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data );
    
    virtual void updateFrame( float delta );
};

#endif /* defined(__Boids__BulletNode__) */
