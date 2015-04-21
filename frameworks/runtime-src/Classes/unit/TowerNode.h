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
#include "spine/spine-cocos2dx.h"

enum eTowerState {
    TowerStateUnknown = 0,
    TowerStateIdle = 1,
    TowerStateAttack = 2,
    TowerStateDie = 3
};

class TowerNode : public TargetNode {
private:
    spine::SkeletonAnimation* _skeleton;
    
    float _elapse;
    
    eTowerState _state;
    
    bool _is_bullet_loaded;
    
private:
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
    
    virtual bool getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir );
    
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
};

#endif /* defined(__Boids__TowerNode__) */
