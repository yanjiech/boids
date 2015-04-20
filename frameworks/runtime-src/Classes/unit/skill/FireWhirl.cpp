//
//  FireWhirl.cpp
//  Boids
//
//  Created by chenyanjie on 4/8/15.
//
//

#include "FireWhirl.h"
#include "../../scene/BattleLayer.h"
#include "../UnitNodeComponent.h"

using namespace cocos2d;

FireWhirl::FireWhirl() :
_effect_node( nullptr )
{
    
}

FireWhirl::~FireWhirl() {
    
}

FireWhirl* FireWhirl::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    FireWhirl* ret = new FireWhirl();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool FireWhirl::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _range = data.at( "range" ).asFloat();
    _elapse = 0;
    _damage_elapse = 0;
    
    return true;
}

void FireWhirl::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->end();
        }
        else {
            _damage_elapse += delta;
            if( _damage_elapse >= _interval ) {
                _damage_elapse = 0;
                
                DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_FIRE_WHIRL, _damage );
                
                Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _range );
                
                for( auto itr = candidates.begin(); itr != candidates.end(); ++itr ) {
                    UnitNode* target_unit = *itr;
                    ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getUnitData(), target_unit->getUnitData() );
                    target_unit->takeDamage( result, _owner );
                }
            }
        }
    }
}

void FireWhirl::begin() {
    SkillNode::begin();
    
    std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_FIRE_WHIRL, BulletNode::getNextBulletId() );
    std::string resource = "effects/women_fire_wizard_skill_1";
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    _effect_node = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
    
    _owner->addUnitComponent( _effect_node, name, eComponentLayer::OverObject );
    _effect_node->setAnimation( 0, "animation", true );
}

void FireWhirl::end() {
    if( _effect_node ) {
        _effect_node->setDuration( 0 );
    }
    SkillNode::end();
}