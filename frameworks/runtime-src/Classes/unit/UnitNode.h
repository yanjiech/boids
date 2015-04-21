//
//  UnitNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__UnitNode__
#define __Boids__UnitNode__

#include "TargetNode.h"
#include "spine/spine-cocos2dx.h"
#include "Skill.h"
#include "Buff.h"
#include "HpBar.h"
#include "./skill/SkillNode.h"

#define DEFAULT_RELAX_FRAMES 45

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

class UnitData : public ElementData {
public:
    UnitData();
    virtual ~UnitData();
    
    static UnitData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const std::string& value );
    
    float guard_radius;
    
    std::string role;
    
    bool is_melee;
    bool is_double_face;
    int default_face_dir;
    
    std::vector<std::string> skill_names;
};

class UnitNode : public TargetNode {
private:
    eUnitState _state;
    eUnitState _next_state;
    eUnitFace _face;
    bool _unit_state_changed;
    
    //share sight
    std::string _sight_group;
    
    cocos2d::Point _born_position;
    
    bool _show_hp;
    bool _is_boss;
    
    spine::SkeletonAnimation* _front;
    spine::SkeletonAnimation* _back;
    
    spine::SkeletonAnimation* _current_skeleton;
    
    cocos2d::Sprite* _shadow;
    
    cocos2d::Point _direction;
    
    TargetNode* _chasing_target;
    
    cocos2d::Map<std::string, Buff*> _buffs;
    
    int _priority;
    
    int _hesitate_frame;
    
    int _same_dir_frame_count;
    
    bool _should_catch_up;
    
    Path* _walk_path;
    
    Path* _tour_path;

    int _relax_frames;
    
    float _wander_radius;
    
    HpBar* _hp_bar;
    
    cocos2d::Vector<Skill*> _skills;
    
    cocos2d::ValueMap _using_skill_params;
    
    UnitNode* _guard_target;
    
    bool _is_concentrate_on_walk;
    
    bool _is_charging;
    TimeLimitSpineComponent* _charging_effect;
    
private:
    void updateBuffs( float delta );
    void updateSkills( float delta );
    void checkToUseSkills();
    
public:
    UnitNode();
    virtual ~UnitNode();
    
    static eTargetCamp getCampByString( const std::string& camp_string );
    
    static UnitNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& unit_data );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& unit_data );
    
    virtual void updateFrame( float delta );
    
    void onSkeletonAnimationStart( int track_index );
    void onSkeletonAnimationEnded( int track_index );
    void onSkeletonAnimationCompleted( int track_index );
    void onSkeletonAnimationEvent( int track_index, spEvent* event );
    
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
    
    TargetNode* getChasingTarget();
    void setChasingTarget( TargetNode* target );
    
    void changeUnitState( eUnitState new_state, bool force = false );
    void applyUnitState();
    
    void changeUnitDirection( const cocos2d::Point& new_dir );
    void changeFace( eUnitFace face );
    
    cocos2d::Point getLocalHitPos();
    virtual cocos2d::Point getHitPos();
    virtual cocos2d::Point getEmitPos();
    cocos2d::Point getLocalEmitPos();
    cocos2d::Point getLocalHeadPos();
    
    virtual cocos2d::Point getBonePos( const std::string& bone_name );
    virtual cocos2d::Point getLocalBonePos( const std::string& bone_name );
    
    void appear();
    void disappear();
    void onDisappearEnd();
    void onDying();
    
    UnitData* getUnitData();
    
    virtual void takeDamage( const cocos2d::ValueMap& result, TargetNode* atker );
    virtual void takeDamage( float amount, bool is_cri, bool is_miss, TargetNode* atker );
    
    void takeHeal( const cocos2d::ValueMap& result, int source_id );
    void takeHeal( float amount, bool is_cri, int source_id );
    
    void setGLProgrameState( const std::string& name );
    
    void showHP();
    void hideHP();
    
    bool isBoss() { return _is_boss; }
    void setBoss( bool b ) { _is_boss = b; }
    
    void applyCustomChange( const std::string& content_string );
    
    void addBuff( const std::string& buff_id, Buff* buff, bool replace = false );
    void removeBuff( const std::string& buff_id );
    bool hasBuff( const std::string& buff_id );
    bool hasBuffOfType( const std::string& buff_type );
    Buff* getBuffOfType( const std::string& buff_type );
    void removeAllBuffs();
    
    void useSkill( int skill_id, const cocos2d::Point& dir, float range_per, float duration = 0 );
    void endSkill();
    void endCast();
    
    virtual bool getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir );
    
    virtual int getPriority() const { return _priority; }
    void setPriority( int priority ) { _priority = priority; }
    
    int getHesitateFrame() { return _hesitate_frame; }
    void setHesitateFrame( int hesitate_frame ) { _hesitate_frame = hesitate_frame; }
    
    void walkTo( const cocos2d::Point& new_pos );
    
    void walkAlongPath( Path* path, float distance );
    void walkAlongWalkPath( float distance );
    void walkAlongTourPath( float distance );
    
    cocos2d::Point pushToward( const cocos2d::Point& dir, float distance );
    
    bool isUnderControl();
    bool willCast();
    bool isCasting();
    bool isAttacking();
    bool isWalking();
    virtual bool isDying();
    virtual bool isAlive();
    
    bool isOscillate( const cocos2d::Point& new_dir );
    
    bool shouldCatchUp() { return _should_catch_up; }
    void setShouldCatchUp( bool b ) { _should_catch_up = b; }
    void evaluateCatchUp();
    
    void findPathToPosition( const cocos2d::Point& pos, int validate_frames = INT_MAX );
    
    bool isHarmless();
    
    virtual bool canAttack( TargetNode* target_node );
    
    void attack( TargetNode* unit );
    
    void onCharging();
    void onAttackBegan();
    void onAttacking();
    void onCasting();
    
    bool isUnitInDirectView( UnitNode* unit );
    
    bool needRelax();
    
    cocos2d::Point getNextWanderPos();
    
    float getWanderRadius() { return _wander_radius; }
    void setWanderRadius( float radius ) { _wander_radius = radius; }
    
    Path* getWalkPath() { return _walk_path; }
    void setWalkPath( Path* path );
    
    Path* getTourPath() { return _tour_path; }
    void setTourPath( Path* path );
    
    void jumpNumber( float amount, const std::string& type, bool is_critical, const std::string& name );
    
    const cocos2d::Vector<Skill*>& getSkills() { return _skills; }
    
    Skill* getSkill( int sk_id );
    
    std::string getSkillHintTypeById( int sk_id );
    float getSkillRadiusById( int sk_id );
    float getSkillRangeById( int sk_id );
    float getSkillMinRangeById( int sk_id );
    float getSkillMaxRangeById( int sk_id );
    float getSkillCDById( int sk_id );
    bool isSkillReadyById( int sk_id );
    bool shouldSkillCastOnTouchDown( int sk_id );
    
    void riseup( float _duration, float delta_height );
    void falldown( float _duration, float delta_height );
    
    void makeSpeech( const std::string& content, float duration );
    
    UnitNode* getGuardTarget() { return _guard_target; }
    void setGuardTarget( UnitNode* guard_target );
    
    cocos2d::Point getGuardCenter();
    
    bool isConcentrateOnWalk() { return _is_concentrate_on_walk; }
    void setConcentrateOnWalk( bool b );
    
    bool isCharging() { return _is_charging; }
    void setCharging( bool b, std::string effect_resource = "" );
    
    //debug
    cocos2d::DrawNode* _custom_draw;
    void drawDirection();
    cocos2d::DrawNode* _new_dir_draw;
};

#endif /* defined(__Boids__UnitNode__) */
