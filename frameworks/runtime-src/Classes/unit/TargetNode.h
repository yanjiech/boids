//
//  TargetNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__TargetNode__
#define __Boids__TargetNode__

#include "cocos2d.h"
#include "ElementData.h"
#include "UnitNodeComponent.h"
#include "../behavior/BehaviorHeader.h"
#include "../interface/Updatable.h"
#include "../AI/Collidable.h"

class BattleLayer;

enum eTargetCamp {
    Unknown_Camp = 0,
    Player = 1,
    Ally = 2,
    Enemy = 3,
    Wild = 4,
    NPC = 5
};

class TargetNode : public cocos2d::Node, public boids::Updatable, public Collidable {
private:
    bool _is_attackable;
    
    cocos2d::Map<std::string, UnitNodeComponent*> _components;
    
    cocos2d::Map<std::string, BehaviorBase*> _behaviors;
    
protected:
    int _deploy_id;
    ElementData* _target_data;
    BattleLayer* _battle_layer;
    eTargetCamp _camp;
    
protected:
    virtual void updateComponents( float delta );
    
public:
    TargetNode();
    virtual ~TargetNode();
    
    virtual bool init();
    
    virtual void updateFrame( float delta );
    
    void setAttackable( bool b ) { _is_attackable = b; }
    bool isAttackable() { return _is_attackable; }
    
    BattleLayer* getBattleLayer() { return _battle_layer; }
    
    ElementData* getTargetData();
    void setTargetData( ElementData* data );
    
    int getDeployId() { return _deploy_id; }
    void setDeployId( int deploy_id ) { _deploy_id = deploy_id; }
    
    bool addUnitComponent( UnitNodeComponent* component, const std::string& key, eComponentLayer layer_type );
    void removeUnitComponent( const std::string& key );
    
    void adjustAllUnitComponents();
    void removeAllUnitComponents();
    
    void addBehavior( const std::string& key, BehaviorBase* behavior );
    void removeBehavior( const std::string& key );
    
    eTargetCamp getTargetCamp() { return _camp; }
    void setTargetCamp( eTargetCamp camp ) { _camp = camp; }
    
    virtual bool isDying();
    virtual bool isAlive();
    
    virtual cocos2d::Point getHitPos() { return cocos2d::Point::ZERO; }
    virtual cocos2d::Point getLocalHitPos() { return cocos2d::Point::ZERO; }
    virtual cocos2d::Point getEmitPos() { return cocos2d::Point::ZERO; }
    virtual cocos2d::Point getBonePos( const std::string& bone_name ) { return cocos2d::Point::ZERO; }
    virtual cocos2d::Point getLocalBonePos( const std::string& bone_name ) { return cocos2d::Point::ZERO; }
    
    virtual void takeDamage( const cocos2d::ValueMap& result, int source_id ) {}
    virtual void takeDamage( float amount, bool is_cri, bool is_miss, int source_id ) {}
    
    virtual bool canAttack( TargetNode* target_node ) { return false; }
    
    virtual bool willCollide( cocos2d::Point pos, float radius );
    virtual bool willCollide( TargetNode* unit);
    virtual bool willCollide( TargetNode* unit, cocos2d::Point unit_new_pos );
    virtual bool getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir ) { return false; }
    virtual int getPriority() const { return 0; }
    
    bool isFoeOfCamp( eTargetCamp opponent_camp );
};

#endif /* defined(__Boids__TargetNode__) */
