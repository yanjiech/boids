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
#include "../AI/Terrain.h"

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
    if( !TargetNode::init( battle_layer ) ) {
        return false;
    }
    
    std::string name = tower_data.at( "name" ).asString();
    std::string type = tower_data.at( "type" ).asString();
    int level = tower_data.at( "level" ).asInt();
    ValueMap vm = ResourceManager::getInstance()->getTowerData( type );
    vm["name"] = Value( name );
    vm["level"] = Value( level );
    
    auto itr = tower_data.find( "boundary" );
    if( itr != tower_data.end() ) {
        const ValueMap& boundary_data = itr->second.asValueMap();
        _boundaries.loadFromValueMap( boundary_data );
        _boundaries.name = boundary_data.at( "name" ).asString();
        this->setPosition( _boundaries.center );
    }
    else {
        this->setPosition( this->getPosition() );
    }
    
    ElementData* target_data = ElementData::create( vm );
    this->setTargetData( target_data );
    
    _elapse = 0;
    this->setTowerState( eTowerState::TowerStateIdle );
    
    std::string resource = ResourceManager::getInstance()->getPathForResource( type, eResourceType::Tower );
    _skeleton = ArmatureManager::getInstance()->createArmature( resource );
    this->addChild( _skeleton, eComponentLayer::Object );
    Rect bounding_box = _skeleton->getBoundingBox();
    this->setContentSize( bounding_box.size );
    
    float hpbar_width = DEFAULT_HP_BAR_WIDTH;
    float hpbar_height = DEFAULT_HP_BAR_HEIGHT;
    
    _hp_bar = HpBar::create( Size( hpbar_width, hpbar_height ) );
    _hp_bar->setPosition( Point( 0, bounding_box.size.height ) );
    _hp_bar->setPercentage( 100.0f );
    this->addChild( _hp_bar, 5 );
    
    itr = tower_data.find( "show_hp" );
    if( itr != tower_data.end() ) {
        _hp_bar->setVisible( itr->second.asBool() );
    }
    else {
        _hp_bar->setVisible( false );
    }
    
    TowerAttackBehavior* attack_behavior = TowerAttackBehavior::create( this );
    this->addBehavior( BEHAVIOR_NAME_ATTACK, attack_behavior );
    
    this->setTargetCamp( eTargetCamp::Enemy );
    
    this->setCollidable( true );
    
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

void TowerNode::changeTowerState( eTowerState new_state ) {
    if( _state != new_state ) {
        this->setTowerState( new_state );
        
        switch( _state ) {
            case eTowerState::TowerStateAttack:
                _skeleton->setAnimation( 0, "Attack", false );
                break;
            case eTowerState::TowerStateIdle:
                _skeleton->setAnimation( 0, "Idle", true );
                break;
            case eTowerState::TowerStateDie:
                _skeleton->setAnimation( 0, "Die", false );
                break;
            default:
                break;
        }
    }
}

void TowerNode::takeDamage( const cocos2d::ValueMap& result, TargetNode* atker ) {
    this->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), atker );
}

void TowerNode::takeDamage( float amount, bool is_cri, bool is_miss, TargetNode* atker ) {
    if( this->isAttackable() && _target_data->current_hp > 0 ) {
        _target_data->current_hp -= amount;
        
        if( _target_data->current_hp <= 0 ) {
            _target_data->current_hp = 0;
        }
        
        //jump damage number
        std::string jump_text_name = Utils::stringFormat( "damage_number_%d", BulletNode::getNextBulletId() );
        this->jumpNumber( amount, "damage", is_cri, jump_text_name );
        
        if( _hp_bar ) {
            _hp_bar->setPercentage( _target_data->current_hp / _target_data->hp * 100.0f );
        }
        
        //dying
        if( _target_data->current_hp <= 0 ) {
            if( _hp_bar ) {
                _hp_bar->setVisible( false );
            }
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

void TowerNode::setCollidable( bool b ) {
    _is_collidable = b;
    if( !_is_collidable ) {
        for( auto pair : Terrain::getInstance()->getMeshes() ) {
            pair.second->removeCollidablePolygon( _boundaries.name );
        }
    }
    else {
        for( auto pair : Terrain::getInstance()->getMeshes() ) {
            pair.second->addCollidablePolygon( _boundaries );
        }
    }
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

//thorn node
ThornNode::ThornNode() {
    
}

ThornNode::~ThornNode() {
    
}

ThornNode* ThornNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& thorn_data ) {
    ThornNode* ret = new ThornNode();
    if( ret && ret->init( battle_layer, thorn_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ThornNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& thorn_data ) {
    if( !TowerNode::init( battle_layer, thorn_data ) ) {
        return false;
    }
    
    this->setAttackable( false );
    this->setCollidable( false );
    
    _skeleton->setEventListener( CC_CALLBACK_2( ThornNode::onSkeletonAnimationEvent, this ) );
    this->changeTowerState( eTowerState::TowerStateAttack );
    
    return true;
}

void ThornNode::updateFrame( float delta ) {
    if( _state == eTowerState::TowerStateIdle ) {
        this->reloadBullet( delta );
        if( isBulletLoaded() ) {
            this->changeTowerState( eTowerState::TowerStateAttack );
        }
    }
}

void ThornNode::onSkeletonAnimationEvent( int track_index, spEvent* event ) {
    spTrackEntry* entry = _skeleton->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    std::string event_name = std::string( event->data->name );
    if( ( animation_name == "Attack" || animation_name == "OnAttacking" ) ) {
        this->doAttack();
    }
}

void ThornNode::doAttack() {
    Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsInRange( eTargetCamp::Enemy, this->getPosition(), this->getTargetData()->atk_range );
    DamageCalculate* calculator = DamageCalculate::create( "normal", 0 );
    for( auto unit : candidates ) {
        ValueMap result = calculator->calculateDamage( this->getTargetData(), unit->getTargetData() );
        unit->takeDamage( result, this );
        
        std::string resource = "effects/bullets/blood_hit";
        std::string name = Utils::stringFormat( "ThornHit_%d", BulletNode::getNextBulletId() );
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
        component->setPosition( unit->getLocalHitPos() );
        unit->addUnitComponent( component, name, eComponentLayer::OverObject );
        component->setAnimation( 0, "animation", false );
    }
    
    this->setTowerState( eTowerState::TowerStateIdle );
    _is_bullet_loaded = false;
}

ElectricAlexandra::ElectricAlexandra() :
_bullet( nullptr ),
_another_part( nullptr )
{
    
}

ElectricAlexandra::~ElectricAlexandra() {
    this->unloadBullet();
}

ElectricAlexandra* ElectricAlexandra::create( BattleLayer* battle_layer, const cocos2d::ValueMap& data ) {
    ElectricAlexandra* ret = new ElectricAlexandra();
    if( ret && ret->init( battle_layer, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ElectricAlexandra::init( BattleLayer* battle_layer, const cocos2d::ValueMap& data ) {
    if( !TowerNode::init( battle_layer, data ) ) {
        return false;
    }
    
    this->setAttackable( false );
    
    return true;
}

void ElectricAlexandra::updateFrame( float delta ) {
    if( _is_enabled && _another_part ) {
        if( !_is_bullet_loaded ) {
            this->reloadBullet( delta );
        }
        if( _is_bullet_loaded ) {
            _is_bullet_loaded = false;
            //do attack
            DamageCalculate* calculator = DamageCalculate::create( "normal", 0 );
            Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsIntersectsLine( this->getTargetCamp(), this->getEmitPos(), _another_part->getEmitPos() );
            for( UnitNode* unit : candidates ) {
                ValueMap result = calculator->calculateDamage( this->getTargetData(), unit->getTargetData() );
                unit->takeDamage( result, this );
                
                //add effect
                std::string resource = "effects/zeus_skill_2/hit";
                std::string name = Utils::stringFormat( "electric_hit_%d", BulletNode::getNextBulletId() );
                spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
                UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
                unit->addUnitComponent( component, name, eComponentLayer::OverObject );
                component->setPosition( unit->getLocalHitPos() );
                component->setAnimation( 0, "animation", false );
            }
        }
    }
}

void ElectricAlexandra::setEnabled( bool b ) {
    TowerNode::setEnabled( b );
    if( _bullet ) {
        _bullet->setVisible( b );
    }
    if( b ) {
        _skeleton->setAnimation( 0, "Idle", true );
    }
    else {
        _skeleton->setAnimation( 0, "Disappear", false );
    }
}

void ElectricAlexandra::setAnotherPart( ElectricAlexandra* another ) {
    CC_SAFE_RETAIN( another );
    if( _another_part ) {
        _another_part->removeFromParent();
        _another_part->release();
    }
    _another_part = another;
    
    if( _another_part ) {
        _battle_layer->addToObjectLayer( _another_part, _another_part->getPosition(), _battle_layer->zorderForPositionOnObjectLayer( _another_part->getPosition() ) );
        
        Point emit_pos = this->getEmitPos();
        Point another_pos = _another_part->getEmitPos();
        Point dir = another_pos - emit_pos;
        _another_part->setBullet( this->getBullet() );
        _bullet->setLocalZOrder( _battle_layer->zorderForPositionOnObjectLayer( ( emit_pos + another_pos ) / 2 ) );
        _bullet->setScaleX( emit_pos.distance( another_pos ) / 260.0f );
        _bullet->setRotation( -dir.getAngle() * 180.0f / M_PI );
    }
}

void ElectricAlexandra::loadBullet() {
    if( _bullet == nullptr ) {
        std::string bullet_resource = "effects/zeus_skill_2/chain";
        _bullet = ArmatureManager::getInstance()->createArmature( bullet_resource );
        _battle_layer->addToEffectLayer( _bullet, this->getEmitPos(), 0 );
        _bullet->setAnimation( 0, "animation", true );
    }
}

void ElectricAlexandra::unloadBullet() {
    if( _bullet ) {
        _another_part->setBullet( nullptr );
        _bullet->removeFromParent();
    }
}