//
//  BonesRain.cpp
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#include "BonesRain.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../../manager/ResourceManager.h"
#include "../../AI/Terrain.h"

using namespace cocos2d;

BonesRain::BonesRain() {
    
}

BonesRain::~BonesRain() {
    
}

BonesRain* BonesRain::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    BonesRain* ret = new BonesRain();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BonesRain::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _radius = data.at( "radius" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = _interval;
    _count = data.at( "count" ).asInt();
    _current_count = 0;
    _dir = Point( params.at( "dir_x" ).asFloat(), params.at( "dir_y" ).asFloat() );
    _init_pos = _owner->getPosition();
    
    return true;
}

void BonesRain::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse >= _interval ) {
            Point pos = _init_pos + _dir * ( ( _current_count + 1 ) * _radius * 2 );
            if( Terrain::getInstance()->isBlocked( _init_pos, pos ) ) {
                this->end();
            }
            std::string bullet_name = Utils::stringFormat( "skeleton_king_bullet_%d", Utils::randomNumber( 2 ) );
            
            ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( bullet_name );
            bullet_data["damage_radius"] = Value( _radius );
            bullet_data["will_miss"] = Value( false );
            
            DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_BONESRAIN, _damage );
            FixedPosBulletNode* bullet = FixedPosBulletNode::create( _owner, bullet_data, calculator, ValueMap() );
            bullet->shootAtPosition( pos );
            
            std::string resource = "effects/skeleton_king_skill_1/cross";
            std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_BONESRAIN, BulletNode::getNextBulletId() );
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
            TimeLimitSpineComponent* component = TimeLimitSpineComponent::create( 1.0f, skeleton, name, true );
            _owner->getBattleLayer()->addToOnGroundLayer( component, pos, 0 );
            component->setAnimation( 0, "animation", true );
            
            _elapse = 0;
            if( ++_current_count >= _count ) {
                this->end();
            }
        }
    }
}

void BonesRain::begin() {
    SkillNode::begin();
}

void BonesRain::end() {
    SkillNode::end();
}