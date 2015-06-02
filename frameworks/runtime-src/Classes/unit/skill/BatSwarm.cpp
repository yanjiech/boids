//
//  BatSwarm.cpp
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#include "BatSwarm.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

BatSwarm::BatSwarm() {
    
}

BatSwarm::~BatSwarm() {
    
}

BatSwarm* BatSwarm::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    BatSwarm* ret = new BatSwarm();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BatSwarm::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _radius = data.at( "radius" ).asFloat();
    _range = data.at( "range" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    
    return true;
}

void BatSwarm::updateFrame( float delta ) {
}

void BatSwarm::begin() {
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_BAT_SWARM, _damage );
    ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "bat" );
    bullet_data["damage_radius"] = Value( _radius );
    bullet_data["duration"] = Value( _duration );
    bullet_data["speed"] = Value( M_PI * 2.0f / _duration );
    bullet_data["will_miss"] = Value( false );
    Point unit_dir = _owner->getUnitDirection();
    Point dir = Geometry::anticlockwiseRotate60( unit_dir );
    for( int i = 0; i < 6; i++ ) {
        CircleBulletNode* bullet = CircleBulletNode::create( _owner, bullet_data, calculator, ValueMap() );
        bullet->shootToward( dir, _range / 2.0f );
        dir = Geometry::anticlockwiseRotate60( dir );
    }
}

void BatSwarm::end() {
}