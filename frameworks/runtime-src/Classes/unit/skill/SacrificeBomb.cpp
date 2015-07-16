//
//  SacrificeBomb.cpp
//  Boids
//
//  Created by chenyanjie on 6/4/15.
//
//

#include "SacrificeBomb.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/AudioManager.h"

using namespace cocos2d;

SacrificeBomb::SacrificeBomb() :
_calculator( nullptr ),
_sp_range( nullptr )
{
    
}

SacrificeBomb::~SacrificeBomb() {
    CC_SAFE_RELEASE( _calculator );
}

SacrificeBomb* SacrificeBomb::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    SacrificeBomb* ret = new SacrificeBomb();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SacrificeBomb::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    float damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_SACRIFICE_BOMB, damage );
    this->setDamageCalculator( calculator );
    
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    _range = data.at( "range" ).asFloat();
    
    return true;
}

void SacrificeBomb::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            //add bomb effect
            AudioManager::getInstance()->playEffect( "common/explode.mp3" );
            
            BattleLayer* battle_layer = _owner->getBattleLayer();
            
            std::string resource = "effects/bomb_robot_skill_1";
            std::string name = Utils::stringFormat( "%s_effect", SKILL_NAME_SACRIFICE_BOMB );
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
            UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
            component->setAnimation( 0, "animation", false );
            battle_layer->addToOnGroundLayer( component, _owner->getPosition(), battle_layer->zorderForPositionOnObjectLayer( _owner->getPosition() ) );
            //calculate damage
            
            Vector<UnitNode*> candidates = battle_layer->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _owner->getPosition(), _range );
            for( auto unit : candidates ) {
                ValueMap result = _calculator->calculateDamage( _owner->getTargetData(), unit->getTargetData() );
                unit->takeDamage( result, _owner );
            }
            
            _owner->changeUnitState( eUnitState::Dying, true );
            this->end();
        }
    }
}

void SacrificeBomb::begin() {
    SkillNode::begin();
    _owner->setAttackable( false );
    _sp_range = Sprite::createWithSpriteFrameName( "skillcircle.png" );
    _sp_range->setScale( _range / 215.0f );
    BattleLayer* battle_layer = _owner->getBattleLayer();
    battle_layer->addToOnGroundLayer( _sp_range, _owner->getPosition(), battle_layer->zorderForPositionOnObjectLayer( _owner->getPosition() ) );
}

void SacrificeBomb::end() {
    SkillNode::end();
    if( _sp_range ) {
        _sp_range->removeFromParent();
        _sp_range = nullptr;
    }
}

class DamageCalculate* SacrificeBomb::getDamageCaculator() {
    return _calculator;
}

void SacrificeBomb::setDamageCalculator( class DamageCalculate* calculator ) {
    CC_SAFE_RETAIN( calculator );
    CC_SAFE_RELEASE( _calculator );
    _calculator = calculator;
}