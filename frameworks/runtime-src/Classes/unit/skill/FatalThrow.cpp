//
//  FatalThrow.cpp
//  Boids
//
//  Created by chenyanjie on 4/7/15.
//
//

#include "FatalThrow.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"
#include "../../behavior/BehaviorHeader.h"

using namespace cocos2d;

FatalThrow::FatalThrow() :
_flamingo( nullptr ),
_hint_effect( nullptr )
{
    
}

FatalThrow::~FatalThrow() {
    CC_SAFE_RELEASE( _flamingo );
}

FatalThrow* FatalThrow::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    FatalThrow* ret = new FatalThrow();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool FatalThrow::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    for( int i = 0; i < 2; i++ ) {
        _intervals[i] = data.at( "interval" ).asValueVector().at( i ).asFloat();
    }
    _range = data.at( "range" ).asFloat();
    
    _left_or_right = 0;
    _throw_elapse = 0;
    return true;
}

void FatalThrow::updateFrame( float delta ) {
    if( !_should_recycle ) {
        if( !_flamingo || !_flamingo->isAlive() ) {
            this->end();
        }
        else {
            _throw_elapse += delta;
            if( _throw_elapse >= _intervals[_left_or_right] ) {
                _left_or_right = ( _left_or_right + 1 ) % 2;
                _throw_elapse = 0;
                //throw something
                std::string bullet_name;
                int rand = Utils::randomNumber( 2 );
                switch ( rand ) {
                    case 1:
                        bullet_name = "flamingo";
                        break;
                    case 2:
                        bullet_name = "crown";
                        break;
                    default:
                        break;
                }
                
                Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _range );
                
                int count = (int)candidates.size();
                if( count > 0 ) {
                    int rand_candidate = Utils::randomNumber( count ) - 1;
                    UnitNode* target_unit = candidates.at( rand_candidate );
                    
                    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_FATAL_THROW, _damage );
                    ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( bullet_name );
                    bullet_data["will_miss"] = Value( false );
                    BulletNode* bullet = BulletNode::create( _owner, bullet_data, calculator, ValueMap() );
                    bullet->shootAt( target_unit, _left_or_right + 1 );
                }
            }
        }
    }
}

void FatalThrow::begin() {
    SkillNode::begin();
    _owner->setAttackable( false );
    
    //invoke flamingo
    Vector<UnitNode*> deploy_units;
    float guard_radius = _owner->getUnitData()->guard_radius;
    Rect deploy_area = Rect( _owner->getPosition().x - guard_radius, _owner->getPosition().y - guard_radius, guard_radius * 2, guard_radius * 2 );
    
    BattleLayer* battle_layer = _owner->getBattleLayer();
    ValueMap unit_data;
    unit_data["name"] = Value( "flamingo" );
    unit_data["level"] = Value( _owner->getUnitData()->level );
    
    UnitNode* flamingo = UnitNode::create( battle_layer, unit_data );
    flamingo->setGuardTarget( _owner );
    flamingo->setGuardRange( _range );
    flamingo->addBehavior( BEHAVIOR_NAME_ATTACK, GuardAttackBehavior::create( flamingo ) );
    flamingo->addBehavior( BEHAVIOR_NAME_MOVE, GuardMoveBehavior::create( flamingo ) );
    flamingo->addBehavior( BEHAVIOR_NAME_IDLE, IdleBehavior::create( flamingo ) );
    
    if( _owner->getTargetCamp() == eTargetCamp::Player ) {
        flamingo->setTargetCamp( eTargetCamp::Ally );
    }
    else {
        flamingo->setTargetCamp( _owner->getTargetCamp() );
    }
    
    deploy_units.pushBack( flamingo );
    battle_layer->deployUnits( deploy_units, deploy_area, _owner->getSightGroup() );
    
    this->setFlamingo( flamingo );
    
    _hint_effect = Sprite::create( "ui/skillcircle.png" );
    _hint_effect->setScale( _range / 200.0f );
    battle_layer->addToOnGroundLayer( _hint_effect, _owner->getPosition(), 0 );
    
    battle_layer->clearChasingTarget( _owner );
}

void FatalThrow::end() {
    if( _hint_effect ) {
        _hint_effect->removeFromParent();
        _hint_effect = nullptr;
    }
    _owner->endCast();
    _owner->setAttackable( true );
    SkillNode::end();
}

void FatalThrow::setFlamingo( UnitNode* unit_node ) {
    CC_SAFE_RELEASE( _flamingo );
    _flamingo = unit_node;
    CC_SAFE_RETAIN( _flamingo );
}