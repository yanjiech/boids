//
//  NaturesAttendants.cpp
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#include "NaturesAttendants.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"

using namespace cocos2d;

NaturesAttendants::NaturesAttendants() {
    
}

NaturesAttendants::~NaturesAttendants() {
    
}

NaturesAttendants* NaturesAttendants::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    NaturesAttendants* ret = new NaturesAttendants();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool NaturesAttendants::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    return true;
}

void NaturesAttendants::updateFrame( float delta ) {
    
}

void NaturesAttendants::begin() {
    std::string resource = "effects/enchantress_skill_1/charge";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_NATURESATTENDANTS, BulletNode::getNextBulletId() );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
    component->setPosition( Point::ZERO );
    _owner->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
    component->setAnimation( 0, "animation", false );
}

void NaturesAttendants::end() {
}