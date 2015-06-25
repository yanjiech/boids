//
//  TowerNode.h
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#ifndef __Boids__TowerNode__
#define __Boids__TowerNode__

#include "TargetNode.h"
#include "../AI/BoidsPolygon.h"
#include "HpBar.h"
#include "spine/spine-cocos2dx.h"

enum eTowerState {
    TowerStateUnknown = 0,
    TowerStateIdle = 1,
    TowerStateAttack = 2,
    TowerStateDie = 3
};

class TowerNode : public TargetNode {
protected:
    spine::SkeletonAnimation* _skeleton;
    
    float _elapse;
    
    eTowerState _state;
    
    bool _is_bullet_loaded;
    
    BoidsPolygon _boundaries;
    
    bool _is_enabled;
    
    HpBar* _hp_bar;
    
protected:
    void reloadBullet( float delta );
    
public:
    TowerNode();
    virtual ~TowerNode();
    
    static TowerNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& tower_data );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& tower_data );
    
    virtual void updateFrame( float delta );
    
    virtual cocos2d::Point getHitPos();
    virtual cocos2d::Point getLocalHitPos();
    virtual cocos2d::Point getEmitPos();
    virtual cocos2d::Point getBonePos( const std::string& bone_name );
    virtual cocos2d::Point getLocalBonePos( const std::string& bone_name );
    
    eTowerState getTowerState() { return _state; }
    void setTowerState( eTowerState new_state ) { _state = new_state; }
    
    void changeTowerState( eTowerState new_state );
    
    virtual void takeDamage( const cocos2d::ValueMap& result, TargetNode* atker );
    virtual void takeDamage( float amount, bool is_cri, bool is_miss, TargetNode* atker );
    
    bool isBulletLoaded() { return _is_bullet_loaded; }

    void attack( TargetNode* unit );
    
    void jumpNumber( float amount, const std::string& type, bool is_critical, const std::string& name );
    
    virtual bool isAlive();
    virtual bool isDying();
    
    virtual void setCollidable( bool b );
    
    virtual bool isEnabled() { return _is_enabled; }
    virtual void setEnabled( bool b ) { _is_enabled = b; }
    
    virtual bool willCollide( cocos2d::Point pos, float radius ) { return false; }
    virtual bool willCollide( TargetNode* unit) { return false; }
    virtual bool willCollide( TargetNode* unit, cocos2d::Point unit_new_pos ) { return false; }
};

class ThornNode : public TowerNode {
public:
    ThornNode();
    virtual ~ThornNode();
    
    static ThornNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& thorn_data );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& thorn_data );
    
    virtual void updateFrame( float delta );
    
    virtual void onSkeletonAnimationEvent( int track_index, spEvent* event );
    
    void doAttack();
};

class ElectricAlexandra : public TowerNode {
private:
    ElectricAlexandra* _another_part;
    spine::SkeletonAnimation* _bullet;
    
public:
    ElectricAlexandra();
    virtual ~ElectricAlexandra();
    
    static ElectricAlexandra* create( BattleLayer* battle_layer, const cocos2d::ValueMap& data );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    virtual void setEnabled( bool b );
    
    ElectricAlexandra* getAnotherPart() { return _another_part; }
    void setAnotherPart( ElectricAlexandra* another );
    
    spine::SkeletonAnimation* getBullet() { return _bullet; }
    void setBullet( spine::SkeletonAnimation* bullet ) { _bullet = bullet; }
    
    void loadBullet();
    void unloadBullet();
};

#endif /* defined(__Boids__TowerNode__) */
