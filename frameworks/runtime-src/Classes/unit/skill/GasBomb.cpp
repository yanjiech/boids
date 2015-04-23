//
//  GasBomb.cpp
//  Boids
//
//  Created by chenyanjie on 4/23/15.
//
//

#include "GasBomb.h"
#include "../../scene/BattleLayer.h"

using namespace cocos2d;

GasBomb::GasBomb() {
    
}

GasBomb::~GasBomb() {
    
}

GasBomb* GasBomb::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    GasBomb* ret = new GasBomb();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GasBomb::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = 0;
    
    
    return true;
}

void GasBomb::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
    }
}

void GasBomb::begin() {
    SkillNode::begin();
    _owner->setAttackable( false );
//    _owner->setCollidable( false );
}

void GasBomb::end() {
    _owner->endCast();
    _owner->setAttackable( true );
//    _owner->setCollidable( true );
    SkillNode::end();
}