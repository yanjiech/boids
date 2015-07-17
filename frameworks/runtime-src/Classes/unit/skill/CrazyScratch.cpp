//
//  CrazyScratch.cpp
//  Boids
//
//  Created by chenyanjie on 5/5/15.
//
//

#include "CrazyScratch.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

CrazyScratch::CrazyScratch() :
_skeleton( nullptr ) {
    
}

CrazyScratch::~CrazyScratch() {
    
}

CrazyScratch* CrazyScratch::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    CrazyScratch* ret = new CrazyScratch();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CrazyScratch::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = _interval;
    _range = data.at( "range" ).asFloat();
    _count = data.at( "count" ).asInt();
    _origin_pos = _owner->getPosition();
    _last_pos = _origin_pos;
    
    Point dir = _owner->getUnitDirection();
    if( dir.y > 0 ) {
        _skeleton = ArmatureManager::getInstance()->createArmature( "characters/kyle/kyle_b" );
    }
    else {
        _skeleton = ArmatureManager::getInstance()->createArmature( "characters/kyle/kyle_f" );
    }
    if( dir.x > 0 ) {
        _skeleton->getSkeleton()->flipX = 1;
    }
    else {
        _skeleton->getSkeleton()->flipX = 0;
    }
    
    _owner->getBattleLayer()->addToEffectLayer( _skeleton, _last_pos, _owner->getBattleLayer()->zorderForPositionOnObjectLayer( _last_pos ) );
    _skeleton->setAnimation( 0, "Cast_1", true );
    _skeleton->setBlendFunc( BlendFunc::ADDITIVE );
    return true;
}

void CrazyScratch::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _interval ) {
            _elapse = 0;
            if( _excluded_targets.size() >= _count ) {
                this->end();
            }
            else {
                Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _last_pos, _range );
                int count = (int)candidates.size();
                if( count <= 0 ) {
                    this->end();
                }
                else {
                    bool found_target = false;
                    while( count > 0 ) {
                        int rand = Utils::randomNumber( count ) - 1;
                        UnitNode* unit = candidates.at( rand );
                        int unit_id = unit->getDeployId();
                        if( _excluded_targets.find( unit_id ) == _excluded_targets.end() ) {
                            _excluded_targets.insert( unit_id, unit );
                            _last_pos = unit->getPosition();
                            _skeleton->setPosition( _last_pos );
                            _skeleton->setLocalZOrder( _owner->getBattleLayer()->zorderForPositionOnObjectLayer( _last_pos ) );
                            found_target = true;
                            break;
                        }
                        else {
                            candidates.erase( rand );
                            count--;
                        }
                    }
                    if( !found_target ) {
                        this->end();
                    }
                }
            }
        }
    }
}

void CrazyScratch::begin() {
    SkillNode::begin();
    _owner->setVisible( false );
    _owner->setAttackable( false );
    _owner->getBattleLayer()->clearChasingTarget( _owner );
}

void CrazyScratch::end() {
    _skeleton->removeFromParent();
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_CRAZY_SCRATCH, _damage );
    for( auto pair : _excluded_targets ) {
        UnitNode* unit = pair.second;
        if( unit->isAlive() ) {
            ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getTargetData(), unit->getTargetData() );
            unit->takeDamage( result, _owner );
        }
    }
    _owner->endCast();
    _owner->setVisible( true );
    _owner->setAttackable( true );
    SkillNode::end();
}