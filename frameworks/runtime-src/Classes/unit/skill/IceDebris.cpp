//
//  IceDebris.cpp
//  Boids
//
//  Created by chenyanjie on 5/29/15.
//
//

#include "IceDebris.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

IceDebris::IceDebris() :
_calculator( nullptr )
{
    
}

IceDebris::~IceDebris() {
    CC_SAFE_RELEASE( _calculator );
}

IceDebris* IceDebris::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    IceDebris* ret = new IceDebris();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool IceDebris::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    TargetNode* target_node = _owner->getChasingTarget();
    if( target_node != nullptr ) {
        _target_pos = target_node->getPosition();
    }
    else {
        _target_pos = _owner->getPosition() + _owner->getUnitDirection() * data.at( "range" ).asFloat();
    }
    
    int level = data.at( "level" ).asInt();
    float damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_ICE_DEBRIS, damage );
    this->setDamageCalculator( calculator );
    
    float slow = data.at( "slow" ).asFloat();
    float buff_duration = data.at( "buff_duration" ).asFloat();
    _buff_data["buff_type"] = Value( BUFF_TYPE_SLOW );
    _buff_data["buff_name"] = Value( SKILL_NAME_ICE_DEBRIS );
    _buff_data["duration"] = Value( buff_duration );
    _buff_data["slow_percent"] = Value( slow );
    
    return true;
}

void IceDebris::updateFrame( float delta ) {
    
}

void IceDebris::begin() {
    ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "ice" );
    bullet_data["will_miss"] = Value( false );
    
    FixedPosBulletNode* bullet = FixedPosBulletNode::create( _owner, bullet_data, _calculator, _buff_data );
    bullet->shootAtPosition( _target_pos, (int)eBattleSubLayer::OnGroundLayer );
}

void IceDebris::end() {
    
}

class DamageCalculate* IceDebris::getDamageCaculator() {
    return _calculator;
}

void IceDebris::setDamageCalculator( class DamageCalculate* calculator ) {
    CC_SAFE_RETAIN( calculator );
    CC_SAFE_RELEASE( _calculator );
    _calculator = calculator;
}