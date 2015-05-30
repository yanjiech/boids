//
//  SnowStorm.cpp
//  Boids
//
//  Created by chenyanjie on 5/29/15.
//
//

#include "SnowStorm.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

SnowStorm::SnowStorm() :
_calculator( nullptr ) {
    
}

SnowStorm::~SnowStorm() {
    CC_SAFE_RELEASE( _calculator );
}

SnowStorm* SnowStorm::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    SnowStorm* ret = new SnowStorm();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SnowStorm::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }

    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _radius = data.at( "radius" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    _interval = data.at( "interval" ).asFloat();
    _damage_interval = 0;
    float min_range = data.at( "min_range" ).asFloat();
    float max_range = data.at( "max_range" ).asFloat();
    float range = params.at( "range_per" ).asFloat() * ( max_range - min_range ) + min_range;
    Point dir = Point( params.at( "dir_x" ).asFloat(), params.at( "dir_y" ).asFloat() );
    _target_pos = _owner->getPosition() + dir * range;
    
    float buff_duration = data.at( "buff_duration" ).asFloat();
    float slow = data.at( "slow" ).asFloat();
    
    _buff_data["buff_type"] = Value( BUFF_TYPE_SLOW );
    _buff_data["buff_name"] = Value( SKILL_NAME_SNOW_STORM );
    _buff_data["duration"] = Value( buff_duration );
    _buff_data["slow_percent"] = Value( slow );
    
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_SNOW_STORM, _damage );
    this->setDamageCalculator( calculator );
    
    return true;
}

void SnowStorm::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->end();
        }
        else {
            _damage_interval += delta;
            if( _damage_interval > _interval ) {
                _damage_interval = 0;
                //add storm
                Point pos = Utils::randomPositionInRange( _target_pos, _radius );
                ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "storm_bullet" );
                bullet_data["will_miss"] = Value( false );
                FixedPosBulletNode* bullet = FixedPosBulletNode::create( _owner, bullet_data, _calculator, _buff_data );
                bullet->shootAtPosition( pos );
            }
        }
    }
}

void SnowStorm::begin() {
    SkillNode::begin();
}

void SnowStorm::end() {
    _owner->endCast();
    SkillNode::end();
}

class DamageCalculate* SnowStorm::getDamageCaculator() {
    return _calculator;
}

void SnowStorm::setDamageCalculator( class DamageCalculate* calculator ) {
    CC_SAFE_RETAIN( calculator );
    CC_SAFE_RELEASE( _calculator );
    _calculator = calculator;
}