//
//  LightningChain.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#include "LightningChain.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../../constant/BoidsConstant.h"

using namespace cocos2d;

LightningChain::LightningChain() :
_target_node( nullptr )
{
    
}

LightningChain::~LightningChain() {
    CC_SAFE_RELEASE( _target_node );
}

LightningChain* LightningChain::create( UnitNode* owner, const cocos2d::ValueMap& data, UnitNode* target_node ) {
    LightningChain* ret = new LightningChain();
    if( ret && ret->init( owner, data, target_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool LightningChain::init( UnitNode* owner, const cocos2d::ValueMap& data, UnitNode* target_node ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    _range = data.at( "range" ).asFloat();
    _length = data.at( "length" ).asFloat();
    int level = data.at( "level" ).asInt();
    _base_damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _count = data.at( "count" ).asInt();
    _current_count = 0;
    _interval = data.at( "interval" ).asFloat();
    _elapse = _interval;
    this->setTargetNode( target_node );
    _last_pos = _owner->getEmitPos();
    return true;
}

UnitNode* LightningChain::getTargetNode() {
    return _target_node;
}

void LightningChain::setTargetNode( UnitNode* target_node ) {
    CC_SAFE_RELEASE( _target_node );
    _target_node = target_node;
    CC_SAFE_RETAIN( _target_node );
}

void LightningChain::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse >= _interval ) {
            BattleLayer* battle_layer = _owner->getBattleLayer();
            Vector<UnitNode*> target_units;
            if( _target_node ) {
                target_units.pushBack( _target_node );
                this->setTargetNode( nullptr );
            }
            else {
                target_units = battle_layer->getAliveOpponentsInRange( _owner->getUnitCamp(), _last_pos, _range );
            }
            for( auto u : target_units ) {
                bool is_excluded = false;
                int uid = u->getDeployId();
                for( auto i : _exclude_targets ) {
                    if( i.asInt() == uid ) {
                        is_excluded = true;
                        break;
                    }
                }
                if( !is_excluded ) {
                    Point target_pos = u->getHitPos();
                    float distance = target_pos.distance( _last_pos );
                    Point dir = target_pos - _last_pos;
                    float angle = -dir.getAngle() * 180.0f / M_PI;
                    
                    std::string resource = "effects/zeus_skill_2/chain";
                    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
                    skeleton->setScaleX( distance / _length );
                    skeleton->setRotation( angle );
                    std::string name = Utils::stringFormat( "lighting_chain_%d", BulletNode::getNextBulletId() );
                    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
                    battle_layer->addToEffectLayer( component, _last_pos, 0 );
                    component->setAnimation( 0, "animation", false );
                    
                    _last_pos = target_pos;
                    
                    name = name + "_hit";
                    resource = "effects/zeus_skill_2/hit";
                    skeleton = ArmatureManager::getInstance()->createArmature( resource );
                    UnitNodeSpineComponent* hit_component = UnitNodeSpineComponent::create( skeleton, name, true );
                    hit_component->setPosition( u->getLocalHeadPos() );
                    u->addUnitComponent( hit_component, name, eComponentLayer::OverObject );
                    hit_component->setAnimation( 0, "animation", false );
                    
                    DamageCalculate* damage_calculator = DamageCalculate::create( SKILL_NAME_LIGHTNING_CHAIN, _base_damage );
                    ValueMap result = damage_calculator->calculateDamage( _owner->getUnitData(), u->getUnitData() );
                    u->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), _owner->getDeployId() );
                    
                    _exclude_targets.push_back( Value( uid ) );
                    break;
                }
            }
            
            _elapse = 0;
            if( ++_current_count >= _count ) {
                this->end();
            }
        }
    }
}

void LightningChain::begin() {
    SkillNode::begin();
}

void LightningChain::end() {
    this->setTargetNode( nullptr );
    SkillNode::end();
}