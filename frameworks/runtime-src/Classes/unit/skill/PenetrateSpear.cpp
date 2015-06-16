//
//  PenetrateSpear.cpp
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#include "PenetrateSpear.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

PenetrateSpear::PenetrateSpear() {
    
}

PenetrateSpear::~PenetrateSpear() {
    
}

PenetrateSpear* PenetrateSpear::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    PenetrateSpear* ret = new PenetrateSpear();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool PenetrateSpear::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _speed = data.at( "bullet_speed" ).asFloat();
    _bullet_name = data.at( "bullet_name" ).asString();
    _damage_radius = data.at( "radius" ).asFloat();
    
    float max_range = data.at( "max_range" ).asFloat();
    float min_range = data.at( "min_range" ).asFloat();
    
    float max_extra = data.at( "max_extra" ).asFloat();
    float max_time = data.at( "max_time" ).asFloat();
    float touch_down_duration = params.at( "touch_down_duration" ).asFloat();
    float percent = touch_down_duration / max_time;
    if( percent > 1.0f ) {
        percent = 1.0f;
    }
    
    _damage = _damage * ( 1.0f + max_extra * percent );
    
    _range = min_range + ( max_range - min_range ) * percent;
    
    return true;
}

void PenetrateSpear::updateFrame( float delta ) {
}

void PenetrateSpear::begin() {
    ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( _bullet_name );
    bullet_data["speed"] = Value( _speed );
    bullet_data["damage_radius"] = Value( _damage_radius );
    bullet_data["will_miss"] = Value( false );
    
    Point dir = _owner->getUnitDirection();
    
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_PENETRATE_SPEAR, _damage );
    
    DirectionalBulletNode* bullet = DirectionalBulletNode::create( _owner, bullet_data, calculator, ValueMap() );
    _owner->getBattleLayer()->addBullet( bullet->getBulletId(), bullet );
    
    bullet->shootAlong( dir, _range / _speed );
}

void PenetrateSpear::end() {
    SkillNode::end();
}