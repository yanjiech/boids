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
#include "../AI/Collidable.h"

enum eTargetCamp {
    Unknown_Camp = 0,
    Player = 1,
    Ally = 2,
    Enemy = 3,
    Wild = 4,
    NPC = 5
};

class BattleLayer;

class TargetNode : public cocos2d::Node, public Collidable {
protected:
    int _deploy_id;
    
    ElementData* _target_data;
    eTargetCamp _camp;
    bool _is_attackable;
    cocos2d::ValueMap _unit_tags;
    cocos2d::Map<std::string, BehaviorBase*> _behaviors;
    cocos2d::Map<std::string, UnitNodeComponent*> _components;
    
    bool _is_collidable;
    
    BattleLayer* _battle_layer;

    int _priority;
    
protected:
    void updateComponents( float delta );
    
public:
    TargetNode();
    virtual ~TargetNode();
    
    virtual bool init( BattleLayer* battle_layer );
    
    virtual void updateFrame( float delta );
    
    virtual void setAttackable( bool b ) { _is_attackable = b; }
    bool isAttackable() { return _is_attackable; }
    
    BattleLayer* getBattleLayer() { return _battle_layer; }
    
    ElementData* getTargetData();
    void setTargetData( ElementData* data );
    
    int getDeployId() { return _deploy_id; }
    void setDeployId( int deploy_id ) { _deploy_id = deploy_id; }
    
    bool addUnitComponent( UnitNodeComponent* component, const std::string& key, eComponentLayer layer_type );
    void removeUnitComponent( const std::string& key );
    
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
    
    virtual void takeDamage( const cocos2d::ValueMap& result, TargetNode* atker ) {}
    virtual void takeDamage( float amount, bool is_cri, bool is_miss, TargetNode* atker ) {}
    
    virtual bool canAttack( TargetNode* target_node ) { return false; }
    
    virtual bool willCollide( cocos2d::Point pos, float radius );
    virtual bool willCollide( TargetNode* unit);
    virtual bool willCollide( TargetNode* unit, cocos2d::Point unit_new_pos );
    virtual bool getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir ) { return false; }
    virtual int getPriority() const { return _priority; }
    
    bool isFoeOfCamp( eTargetCamp opponent_camp );
    
    const cocos2d::ValueMap& getUnitTags() { return _unit_tags; }
    void addUnitTag( const std::string& tag );
    void removeUnitTag( const std::string& tag );
    void setUnitTags( const std::string& tag_string );
    bool hasUnitTag( const std::string& tag_name );
    
    virtual bool isCollidable() { return _is_collidable; }
    virtual void setCollidable( bool b ) { _is_collidable = b; }
};

#endif /* defined(__Boids__TargetNode__) */
