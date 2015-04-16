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
#include "../BulletNode.h"
#include "../../manager/ResourceManager.h"

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
    _speed = data.at( "speed" ).asFloat();
    _waves = data.at( "waves" ).asInt();
    _current_wave = 0;
    _count_per_wave = data.at( "count_per_wave" ).asInt();
    _angle = data.at( "angle" ).asFloat();
    _range = data.at( "range" ).asFloat();
    _dir_angle = _owner->getUnitDirection().getAngle();
    
    return true;
}

void BarrageOfArrow::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        
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
                ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "vanhelsing_skill_bullet" );
                bullet_data["will_miss"] = Value( false );
                for( int i = 0; i < _count_per_wave; ++i ) {
                    float angle = ( ( i - ( _count_per_wave - 1 ) / 2.0f ) ) * ( CC_DEGREES_TO_RADIANS( _angle ) / ( _count_per_wave - 1 ) ) + _dir_angle;
                    Point dir = Point( cosf( angle ), sinf( angle ) );
                    DamageCalculate* calculator = DamageCalculate::create( "BarrageOfArrow", _base_damage );
                    DirectionalBulletNode* bullet = DirectionalBulletNode::create( _owner, bullet_data, calculator, ValueMap() );
                    battle_layer->addBullet( bullet->getBulletId(), bullet );
                    bullet->shootAlong( dir, _range / _speed, _owner );
                }
            }
            
            if( ++_current_wave >= _waves ) {
                this->end();
            }
        }
    }
}

void BarrageOfArrow::begin() {
    SkillNode::begin();
}

void BarrageOfArrow::end() {
    _owner->endCast();
    SkillNode::end();
}