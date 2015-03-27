//
//  UnitNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__UnitNode__
#define __Boids__UnitNode__

#include "../interface/Updatable.h"
#include "TargetNode.h"
#include "BehaviorBase.h"
#include "spine/spine-cocos2dx.h"
#include "Skill.h"
#include "Buff.h"
#include "UnitNodeComponent.h"
#include "../AI/Collidable.h"
#include "../behavior/BehaviorBase.h"
#include "HpBar.h"
#include "./skill/SkillNode.h"

#define DEFAULT_RELAX_FRAMES 45

enum eUnitCamp {
    Unknown_Camp = 0,
    Player = 1,
    Ally = 2,
    Enemy = 3,
    Wild = 4,
    NPC = 5
};

enum eUnitState {
    Unknown_Unit_State = 0,
    Idle = 1,
    Walking = 2,
    Attacking = 3,
    Casting = 4,
    UnderControl = 5,
    Dying = 6,
    Disappear = 7,
    Dead = 8
};

enum eUnitFace {
    Front = 1,
    Back = 2
};

class Path;

class UnitData : public cocos2d::Ref {
public:
    UnitData();
    virtual ~UnitData();
    
    static UnitData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    void setAttribute( const std::string& key, const std::string& value );
   
    int unit_id;
    std::string name;
    std::string display_name;
    int level;
    float hp;
    float current_hp;
    float atk;
    float def;
    float move_speed;
    float atk_speed;
    float collide;
    float critical;
    float tenacity;
    float hit;
    float dodge;
    
    float guard_radius;
    float atk_range;
    
    float recover;
    float scale;
    
    std::string role;
    
    std::string bullet_name;
    
    bool is_melee;
    bool is_double_face;
    int default_face_dir;
    
    std::vector<std::string> skill_names;
};

class BattleLayer;

class UnitNode : public TargetNode, public boids::Updatable, public Collidable {
private:
    BattleLayer* _battle_layer;
    
    UnitData* _unit_data;
    
    int _deploy_id;
    eUnitCamp _camp;
    eUnitState _state;
    eUnitState _next_state;
    eUnitFace _face;
    std::string _sight_group;
    cocos2d::Point _born_position;
    
    bool _show_hp;
    bool _is_boss;
    
    bool _unit_state_changed;
    
    spine::SkeletonAnimation* _front;
    spine::SkeletonAnimation* _back;
    
    spine::SkeletonAnimation* _current_skeleton;
    
    cocos2d::Sprite* _shadow;
    
    cocos2d::Point _direction;
    
    //effects on the node
    cocos2d::Map<std::string, UnitNodeComponent*> _components;
    
    //ai related
    cocos2d::Map<std::string, BehaviorBase*> _behaviors;
    
    TargetNode* _chasing_target;
    
    cocos2d::Map<std::string, Buff*> _buffs;
    
    int _priority;
    
    int _hesitate_frame;
    
    int _same_dir_frame_count;
    
    bool _should_catch_up;
    
    std::unique_ptr<Path> _walk_path;
    
    bool _is_concentrate_on_walk;
    
    bool _is_movable;
    
    int _relax_frames;
    
    float _wander_radius;
    
    HpBar* _hp_bar;
    
    cocos2d::Vector<Skill*> _skills;
    
    cocos2d::ValueMap _using_skill_params;
    
    cocos2d::ValueVector _unit_tags;
    
    SkillNode* _using_skill_node;
    
private:
    void updateComponents( float delta );
    void updateBuffs( float delta );
    void updateSkills( float delta );
    void checkToUseSkills();
    
public:
    UnitNode();
    virtual ~UnitNode();
    
    static eUnitCamp getCampByString( const std::string& camp_string );
    
    static UnitNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& unit_data );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& unit_data );
    
    virtual void updateFrame( float delta );
    
    void onSkeletonAnimationStart( int track_index );
    void onSkeletonAnimationEnded( int track_index );
    void onSkeletonAnimationCompleted( int track_index );
    void onSkeletonAnimationEvent( int track_index, spEvent* event );
    
    BattleLayer* getBattleLayer() { return _battle_layer; }
    
    int getDeployId() { return _deploy_id; }
    void setDeployId( int deploy_id ) { _deploy_id = deploy_id; }
    
    eUnitCamp getUnitCamp() { return _camp; }
    void setUnitCamp( eUnitCamp camp ) { _camp = camp; }
    
    eUnitState getUnitState() { return _state; }
    void setUnitState( eUnitState state ) { _state = state; }
    
    eUnitState getNextUnitState() { return _next_state; }
    void setNextUnitState( eUnitState state ) { _next_state = state; }
    
    eUnitFace getUnitFace() { return _face; }
    void setUnitFace( eUnitFace face ) { _face = face; }
    
    const std::string& getSightGroup() { return _sight_group; }
    void setSightGroup( const std::string& sight_group ) { _sight_group = sight_group; }
    
    const cocos2d::Point& getBornPos() { return _born_position; }
    void setBornPos( const cocos2d::Point& pos ) { _born_position = pos; }
    
    spine::SkeletonAnimation* getCurrentSkeleton() { return _current_skeleton; }
    void setCurrentSkeleton( spine::SkeletonAnimation* skeleton ) { _current_skeleton = skeleton; }
    
    const cocos2d::Point& getUnitDirection() { return _direction; }
    void setUnitDirection( const cocos2d::Point& direction ) { _direction = direction; }
    
    TargetNode* getChasingTarget() { return _chasing_target; }
    void setChasingTarget( TargetNode* target ) { _chasing_target = target; }
    
    void changeUnitState( eUnitState new_state, bool force = false );
    void applyUnitState();
    
    void changeUnitDirection( const cocos2d::Point& new_dir );
    void changeFace( eUnitFace face );
    
    cocos2d::Point getLocalHitPos();
    cocos2d::Point getHitPos();
    cocos2d::Point getEmitPos();
    cocos2d::Point getLocalHeadPos();
    
    void appear();
    void disappear();
    void onDisappearEnd();
    void onDying();
    
    UnitData* getUnitData() { return _unit_data; }
    
    void takeDamage( const cocos2d::ValueMap& result, int source_id );
    void takeDamage( float amount, bool is_cri, bool is_miss, int source_id );
    void takeHeal( float amount, bool is_cri, int source_id );
    
    void riseup( float duration, float height );
    void falldown();
    
    void setGLProgrameState( const std::string& name );
    
    bool addUnitComponent( UnitNodeComponent* component, const std::string& key, eComponentLayer layer_type );
    void removeUnitComponent( const std::string& key );
    
    void adjustAllUnitComponents();
    void removeAllUnitComponents();
    
    void showHP();
    void hideHP();
    
    bool isBoss() { return _is_boss; }
    void setBoss( bool b ) { _is_boss = b; }
    
    void applyCustomChange( const std::string& content_string );
    
    void addBuff( const std::string& buff_id, Buff* buff );
    void removeBuff( const std::string& buff_id );
    bool hasBuff( const std::string& buff_id );
    void removeAllBuffs();
    
    void useSkill( int skill_id, const cocos2d::Point& dir, float range_per );
    void endSkill();
    void endCast();
    
    virtual bool willCollide( cocos2d::Point pos, float radius );
    
    bool willCollide( UnitNode* unit) ;
    
    virtual bool willCollide( UnitNode* unit, cocos2d::Point unit_new_pos );
    
    virtual bool getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir) ;
    
    virtual int getPriority() const { return _priority; }
    void setPriority( int priority ) { _priority = priority; }
    
    int getHesitateFrame() { return _hesitate_frame; }
    void setHesitateFrame( int hesitate_frame ) { _hesitate_frame = hesitate_frame; }
    
    void addBehavior( const std::string& key, BehaviorBase* behavior );
    void removeBehavior( const std::string& key );
    
    void walkTo( const cocos2d::Point& new_pos );
    void walkAlongPath( float distance );
    
    bool isUnderControl();
    bool isCasting();
    bool isAttacking();
    bool isWalking();
    bool isDying();
    
    bool isOscillate( const cocos2d::Point& new_dir );
    
    bool shouldCatchUp() { return _should_catch_up; }
    void setShouldCatchUp( bool b ) { _should_catch_up = b; }
    void evaluateCatchUp();
    
    void findPathToPosition( const cocos2d::Point& pos, int validate_frames = INT_MAX );
    
    bool isConcentrateOnWalk() { return _is_concentrate_on_walk; }
    void setConcentrateOnWalk( bool b ) { _is_concentrate_on_walk = b; }
    
    bool isHarmless();
    
    TargetNode* getAttackTarget();
    bool canAttack( TargetNode* target_node );
    
    void attack( TargetNode* unit );
    void onCharging( int i );
    void onAttackBegan();
    void onAttacking();
    void onCasting();
    
    bool isUnitInDirectView( UnitNode* unit );
    
    void setUnitTags( const std::string& tag_string );
    bool hasUnitTag( const std::string& tag_name );
    
    cocos2d::ValueVector getUnitTags() { return _unit_tags; }
    
    bool isMovable() { return _is_movable; }
    void setMovable( bool b ) { _is_movable = b; }
    
    bool isFoeOfCamp( eUnitCamp opponent_camp );
    
    bool needRelax();
    
    cocos2d::Point getNextWanderPos();
    
    float getWanderRadius() { return _wander_radius; }
    void setWanderRadius( float radius ) { _wander_radius = radius; }
    
    void setWalkPath( const Path& path );
    
    void jumpNumber( float amount, const std::string& type, bool is_critical, const std::string& name );
    
    const cocos2d::Vector<Skill*>& getSkills() { return _skills; }
    
    std::string getSkillHintTypeById( int sk_id );
    float getSkillRadiusById( int sk_id );
    float getSkillRangeById( int sk_id );
    float getSkillMinRangeById( int sk_id );
    float getSkillMaxRangeById( int sk_id );
    float getSkillCDById( int sk_id );
    bool isSkillReadyById( int sk_id );
    
    void setUsingSkillNode( SkillNode* skill_node ) { _using_skill_node = nullptr; }
    SkillNode* getSkillNode() { return _using_skill_node; }
};

#endif /* defined(__Boids__UnitNode__) */
