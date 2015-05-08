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

CrazyScratch::CrazyScratch() {
    
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
    _elapse = 0;
    _damage_interval = data.at( "interval" ).asFloat();
    _damage_elapse = 0;
    _range = data.at( "range" ).asFloat();
    _dir = owner->getUnitDirection();
    
    return true;
}

void CrazyScratch::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->end();
        }
        else {
            _damage_elapse += delta;
            if( _damage_elapse > _damage_interval ) {
                _damage_elapse = 0;
                
                Point dir;
                if( _dir.x < 0 ) {
                    dir = Point( -1.0f, 0 );
                }
                else {
                    dir = Point( 1.0f, 0 );
                }
                
                DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_CRAZY_SCRATCH, _damage );
                
                Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInSector( _owner->getTargetCamp(), _owner->getPosition(), dir, _range, 180.0f );
                for( auto unit : candidates ) {
                    ValueMap result = calculator->calculateDamage( _owner->getTargetData(), unit->getTargetData() );
                    unit->takeDamage( result, _owner );
                }
            }
        }
    }
}

void CrazyScratch::begin() {
    SkillNode::begin();
}

void CrazyScratch::end() {
    _owner->endCast();
    SkillNode::end();
}

void CrazyScratch::refreshDamage() {
    _damage_elapse = _damage_interval;
}