//
//  TowerNode.cpp
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#include "TowerNode.h"
#include "../scene/BattleLayer.h"
#include "../ArmatureManager.h"
#include "JumpText.h"
#include "ElementData.h"
#include "../manager/ResourceManager.h"

using namespace cocos2d;

TowerNode::TowerNode() :
_is_bullet_loaded( false )
{
    
}

TowerNode::~TowerNode() {
    
}

TowerNode* TowerNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& tower_data ) {
    TowerNode* ret = new TowerNode();
    if( ret && ret->init( battle_layer, tower_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TowerNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& tower_data ) {
    if( !TargetNode::init() ) {
        return false;
    }
    
    _battle_layer = battle_layer;
    
    std::string name = tower_data.at( "name" ).asString();
    int level = tower_data.at( "level" ).asInt();
    ValueMap vm = ResourceManager::getInstance()->getTowerData( name );
    vm["name"] = Value( name );
    vm["level"] = Value( level );
    
    ElementData* target_data = ElementData::create( vm );
    this->setTargetData( target_data );
    
    _elapse = 0;
    this->setTowerState( eTowerState::TowerStateIdle );
    
    std::string resource = ResourceManager::getInstance()->getPathForResource( name, eResourceType::Tower );
    _skeleton = ArmatureManager::getInstance()->createArmature( resource );
    this->addChild( _skeleton, eComponentLayer::Object );
    Rect bounding_box = _skeleton->getBoundingBox();
    this->setContentSize( bounding_box.size );
    
    TowerAttackBehavior* attack_behavior = TowerAttackBehavior::create( this );
    this->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
    
    return true;
}

void TowerNode::updateFrame( float delta ) {
    this->reloadBullet( delta );
    TargetNode::updateFrame( delta );
}

cocos2d::Point TowerNode::getHitPos() {
    return this->getBonePos( "HitPoint" );
}

cocos2d::Point TowerNode::getLocalHitPos() {
    return this->getLocalBonePos( "HitPoint" );
}

cocos2d::Point TowerNode::getEmitPos() {
    return this->getBonePos( "EmitPoint" );
}

cocos2d::Point TowerNode::getBonePos( const std::string& bone_name ) {
    Point ret = this->getLocalBonePos( bone_name );
    ret = _skeleton->convertToWorldSpace( ret );
    ret = this->getParent()->convertToNodeSpace( ret );
    return ret;
}

cocos2d::Point TowerNode::getLocalBonePos( const std::string& bone_name ) {
    return ArmatureManager::getInstance()->getBonePosition( _skeleton, bone_name );
}

bool TowerNode::getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir ) {
    Vec2 unit_to_this = this->getPosition() - unit->getPosition();
    float unit_dir_to_center = old_dir.cross( unit_to_this );

    if( unit_dir_to_center > 0 ) {
        new_dir = Geometry::clockwisePerpendicularVecToLine( unit_to_this );
    }
    else {
        new_dir = Geometry::anticlockwisePerpendicularVecToLine( unit_to_this );
    }
    
    new_dir.normalize();
    
    return true;
}

void TowerNode::changeTowerState( eTowerState new_state ) {
    if( _state != new_state ) {
        this->setTowerState( new_state );
        
        switch( _state ) {
            case eTowerState::TowerStateDie:
                _skeleton->setAnimation( 0, "Die", false );
                break;
            default:
                break;
        }
    }
}

void TowerNode::takeDamage( const cocos2d::ValueMap& result, int source_id ) {
    this->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), source_id );
}

void TowerNode::takeDamage( float amount, bool is_cri, bool is_miss, int source_id ) {
    if( this->isAttackable() && _target_data->current_hp > 0 ) {
        _target_data->current_hp -= amount;
        
        if( _target_data->current_hp <= 0 ) {
            _target_data->current_hp = 0;
        }
        
        //jump damage number
        std::string jump_text_name = Utils::stringFormat( "damage_number_%d", BulletNode::getNextBulletId() );
        this->jumpNumber( amount, "damage", is_cri, jump_text_name );
        
        //dying
        if( _target_data->current_hp <= 0 ) {
            this->changeTowerState( eTowerState::TowerStateDie );
            _battle_layer->clearChasingTarget( this );
        }
    }
}

void TowerNode::attack( TargetNode* unit ) {
    _is_bullet_loaded = false;
    DamageCalculate* damage_calculator = DamageCalculate::create( "normal", 0 );
    BulletNode* bullet = BulletNode::create( this, ResourceManager::getInstance()->getBulletData( _target_data->bullet_name ), damage_calculator, ValueMap() );
    bullet->shootAt( unit );
}

void TowerNode::jumpNumber( float amount, const std::string& type, bool is_critical, const std::string& name ) {
    JumpText* jump_text = JumpText::create( Utils::toStr( (int)amount ), type, is_critical, _camp, name );
    this->addUnitComponent( jump_text, name, eComponentLayer::OverObject );
    jump_text->setPosition( this->getLocalBonePos( "HitPoint" ) );
    jump_text->start( is_critical );
}

bool TowerNode::isAlive() {
    return _state < eTowerState::TowerStateDie;
}

bool TowerNode::isDying() {
    return _state >= eTowerState::TowerStateDie;
}

//private methods
void TowerNode::reloadBullet( float delta ) {
    if( !_is_bullet_loaded ) {
        _elapse += delta;
        if( _elapse >= _target_data->atk_speed ) {
            _is_bullet_loaded = true;
            _elapse = 0;
        }
    }
}