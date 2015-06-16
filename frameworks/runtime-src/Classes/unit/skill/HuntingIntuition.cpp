//
//  HuntingIntuition.cpp
//  Boids
//
//  Created by chenyanjie on 4/15/15.
//
//

#include "HuntingIntuition.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"

using namespace cocos2d;

HuntingIntuition::HuntingIntuition() {
    
}

HuntingIntuition::~HuntingIntuition() {
    
}

HuntingIntuition* HuntingIntuition::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    HuntingIntuition* ret = new HuntingIntuition();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool HuntingIntuition::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _buff_duration = data.at( "buff_duration" ).asFloat();
    _cri_up = data.at( "cri" ).asValueVector().at( level - 1 ).asFloat();
    _hit_up = data.at( "hit" ).asValueVector().at( level - 1 ).asFloat();
    
    return true;
}

void HuntingIntuition::updateFrame( float delta ) {
    
}

void HuntingIntuition::begin() {
    ValueMap data;
    data["duration"] = Value( _buff_duration );
    data["buff_type"] = Value( BUFF_TYPE_ATTRIBUTE );
    data["buff_name"] = Value( "HuntingIntuition" );
    
    ElementData* unit_data = _owner->getTargetData();
    
    ValueMap attributes;
    attributes["cri"] = Value( unit_data->critical * _cri_up );
    attributes["hit"] = Value( unit_data->hit * _hit_up );
    data["attributes"] = Value( attributes );
    
    AttributeBuff* buff = AttributeBuff::create( _owner, data );
    _owner->addBuff( buff->getBuffId(), buff );
    buff->begin();
    
    std::string resource = "effects/hippolyta_skill_2";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_HUNTING_INTUITION, BulletNode::getNextBulletId() );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
    component->setPosition( Point::ZERO );
    component->setAnimation( 0, "animation", false );
    _owner->addUnitComponent( component, name, eComponentLayer::OverObject );
}

void HuntingIntuition::end() {
}