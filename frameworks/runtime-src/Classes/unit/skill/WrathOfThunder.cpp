//
//  WrathOfThunder.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#include "WrathOfThunder.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../../constant/BoidsConstant.h"

WrathOfThunder::WrathOfThunder() {
    
}

WrathOfThunder::~WrathOfThunder() {
    
}

WrathOfThunder* WrathOfThunder::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    WrathOfThunder* ret = new WrathOfThunder();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool WrathOfThunder::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    _range = data.at( "range" ).asFloat();
    int level = data.at( "level" ).asInt();
    _base_damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _count = data.at( "count" ).asInt();
    
    return true;
}

void WrathOfThunder::updateFrame( float delta ) {
}

void WrathOfThunder::begin() {
    BattleLayer* battle_layer = _owner->getBattleLayer();
    cocos2d::Vector<UnitNode*> units = battle_layer->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _range );
    int c = 0;
    for( auto u : units ) {
        std::string resource = "effects/zeus_skill_1";
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        std::string name = Utils::stringFormat( "wrath_of_thunder_%d", BulletNode::getNextBulletId() );
        DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_WRATH_OF_THUNDER, _base_damage );
        UnitNodeSpineDamageComponent* component = UnitNodeSpineDamageComponent::create( _owner, u, skeleton, name, true, calculator );
        u->addUnitComponent( component, name, eComponentLayer::OverObject );
        component->setAnimation( 0, "animation", false );
        if( ++c >= _count ) {
            break;
        }
    }
}

void WrathOfThunder::end() {
}