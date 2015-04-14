//
//  GiantSword.cpp
//  Boids
//
//  Created by chenyanjie on 3/25/15.
//
//

#include "GiantSword.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

GiantSword::GiantSword() {
    
}

GiantSword::~GiantSword() {
    
}

GiantSword* GiantSword::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    GiantSword* ret = new GiantSword();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GiantSword::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _radius = data.at( "radius" ).asFloat();
    _stun = data.at( "stun" ).asFloat();
    float min_range = data.at( "min_range" ).asFloat();
    float max_range = data.at( "max_range" ).asFloat();
    _range = params.at( "range_per" ).asFloat() * ( max_range - min_range ) + min_range;
    _dir = Point( params.at( "dir_x" ).asFloat(), params.at( "dir_y" ).asFloat() );
    
    return true;
}

void GiantSword::updateFrame( float delta ) {
}

void GiantSword::begin() {
    std::string resource = "effects/saber_skill_1/down";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    UnitNodeSpineComponent* bottom_component = UnitNodeSpineComponent::create( skeleton, Utils::stringFormat( "giantsword_%d_bottom", BulletNode::getNextBulletId() ), true );
    bottom_component->setAnimation( 0, "animation", true );
    Point bottom_pos = _owner->getPosition() + _dir * _range;
    _owner->getBattleLayer()->addToOnGroundLayer( bottom_component, bottom_pos, 0 );
    
    DamageCalculate* calculator = DamageCalculate::create( "GiantSword", _damage );
    ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "saber_bullet" );
    bullet_data["will_miss"] = Value( false );
    FixedPosBulletNode* bullet = FixedPosBulletNode::create( dynamic_cast<TargetNode*>( _owner ), bullet_data, calculator, ValueMap() );
    bullet->shootAtPosition( _owner->getPosition() + _dir * _range );
}

void GiantSword::end() {
}