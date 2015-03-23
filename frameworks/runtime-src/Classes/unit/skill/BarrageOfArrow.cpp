//
//  BarrageOfArrow.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/20/15.
//
//

#include "BarrageOfArrow.h"
#include "../../scene/BattleLayer.h"
#include "../../constant/BoidsConstant.h"
#include "../../Utils.h"

using namespace cocos2d;

BarrageOfArrow::BarrageOfArrow() {
    
}

BarrageOfArrow::~BarrageOfArrow() {

}

BarrageOfArrow* BarrageOfArrow::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    BarrageOfArrow* ret = new BarrageOfArrow();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_RELEASE( ret );
        return nullptr;
    }
}

bool BarrageOfArrow::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _base_damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = _interval;
    _damage_elapse = _interval / 2;
    _speed = data.at( "speed" ).asFloat();
    _waves = data.at( "waves" ).asInt();
    _current_wave = 0;
    _count_per_wave = data.at( "count_per_wave" ).asInt();
    _angle = data.at( "angle" ).asFloat();
    _range = data.at( "range" ).asFloat();
    _bullet_length = data.at( "bullet_length" ).asFloat();
    _dir_angle = _owner->getUnitDirection().getAngle();
    
    return true;
}

void BarrageOfArrow::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        _damage_elapse += delta;
        if( _damage_elapse >= _interval ) {
            _damage_elapse = 0;
            BattleLayer* battle_layer = _owner->getBattleLayer();
            cocos2d::Vector<UnitNode*> units = battle_layer->getAliveOpponentsInSector( _owner->getUnitCamp(), _owner->getEmitPos(), _owner->getUnitDirection(), _range, _angle );
            DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_BARRAGE_OF_ARROW, _base_damage );
            for( auto u : units ) {
                ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getUnitData(), u->getUnitData() );
                u->takeDamage( result.at( "damage").asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), _owner->getDeployId() );
                //add hit effect
                std::string resource = "effects/bullets/blue_ball_hit";
                std::string name = Utils::stringFormat( "%s_hit_%d", SKILL_NAME_BARRAGE_OF_ARROW, BulletNode::getNextBulletId() );
                spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
                UnitNodeSpineComponent* hit_component = UnitNodeSpineComponent::create( skeleton, name, true );
                hit_component->setPosition( u->getLocalHitPos() );
                u->addUnitComponent( hit_component, name, eComponentLayer::OverObject );
                hit_component->setAnimation( 0, "animation", false );
            }
        }
        
        if( _elapse >= _interval ) {
            if( _current_wave < _waves ) {
                _elapse = 0;
                
                BattleLayer* battle_layer = _owner->getBattleLayer();
                
                //add shoot effect
                std::string resource = "effects/vanhelsing_skill_1/shoot";
                std::string name = Utils::stringFormat( "%s_shoot_%d", SKILL_NAME_BARRAGE_OF_ARROW, BulletNode::getNextBulletId() );
                spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
                skeleton->setRotation( -CC_RADIANS_TO_DEGREES( _dir_angle ) );
                UnitNodeSpineComponent* shoot_component = UnitNodeSpineComponent::create( skeleton, name, true );
                battle_layer->addToEffectLayer( shoot_component, _owner->getEmitPos(), 1 );
                shoot_component->setAnimation( 0, "animation", false );
                
                //add arrow components
                std::string bullet_resource = "effects/bullets/vanhelsing_bullet_body";
                for( int i = 0; i < _count_per_wave; ++i ) {
                    std::string bullet_name = SKILL_NAME_BARRAGE_OF_ARROW + Utils::stringFormat( "_%d", BulletNode::getNextBulletId() );
                    skeleton = ArmatureManager::getInstance()->createArmature( bullet_resource );
                    float angle = ( ( i - ( _count_per_wave - 1 ) / 2.0f ) ) * ( CC_DEGREES_TO_RADIANS( _angle ) / ( _count_per_wave - 1 ) ) + _dir_angle;
                    Point dir = Point( cosf( angle ), sinf( angle ) );
                    skeleton->setRotation( -CC_RADIANS_TO_DEGREES( angle ) );
                    UnitNodeDirectionalSpineComponent* component = UnitNodeDirectionalSpineComponent::create( dir, _speed, _range / _speed, skeleton, bullet_name, true );
                    battle_layer->addToEffectLayer( component, _owner->getEmitPos(), 0 );
                    component->setAnimation( 0, "animation", true );
                }
            }
            
            if( ++_current_wave == _waves ) {
                this->end();
            }
        }
    }
}

void BarrageOfArrow::begin() {
    
}

void BarrageOfArrow::end() {
    _owner->endCast();
    this->setShouldRecycle( true );
}