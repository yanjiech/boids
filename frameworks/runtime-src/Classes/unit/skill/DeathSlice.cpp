//
//  DeathSlice.cpp
//  Boids
//
//  Created by chenyanjie on 4/8/15.
//
//

#include "DeathSlice.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

DeathSlice::DeathSlice() {
    
}

DeathSlice::~DeathSlice() {
    
}

DeathSlice* DeathSlice::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    DeathSlice* ret = new DeathSlice();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DeathSlice::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _range = data.at( "range" ).asFloat();
    _damage_radius = data.at( "radius" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _speed = data.at( "speed" ).asFloat();
    _slow_percent = data.at( "slow_percent" ).asFloat();
    
    return true;
}

void DeathSlice::updateFrame( float delta ) {
    
}

void DeathSlice::begin() {
    ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "saucer" );
    bullet_data["speed"] = Value( _speed );
    bullet_data["damage_radius"] = Value( _damage_radius );
    bullet_data["duration"] = Value( _duration );
    bullet_data["interval"] = Value( _interval );
    bullet_data["will_miss"] = Value( false );
    
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_DEATH_SLICE, _damage );
    
    ValueMap buff_data;
    buff_data["duration"] = Value( _interval );
    buff_data["buff_type"] = Value( BUFF_TYPE_SLOW );
    buff_data["buff_name"] = Value( "DeathSlice" );
    buff_data["slow_percent"] = Value( _slow_percent );
    
    Point from_pos = _owner->getEmitPos();
    Point dir;
    Point to_pos;
    TargetNode* chasing_target = _owner->getChasingTarget();
    if( chasing_target && Math::isPositionInRange( chasing_target->getPosition(), _owner->getPosition(), _range ) ) {
        Point target_pos = chasing_target->getPosition();
        dir = from_pos - target_pos;
        dir.normalize();
        to_pos = target_pos;
    }
    else {
        dir = _owner->getUnitDirection();
        to_pos = from_pos + dir * _range;
    }
    
    DirectionalLastingBulletNode* bullet = DirectionalLastingBulletNode::create( _owner, bullet_data, calculator, buff_data );
    bullet->shootTo( from_pos, to_pos );
}

void DeathSlice::end() {
    
}