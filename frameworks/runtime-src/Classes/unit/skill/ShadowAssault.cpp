//
//  ShadowAssault.cpp
//  Boids
//
//  Created by chenyanjie on 7/14/15.
//
//

#include "ShadowAssault.h"
#include "../../scene/BattleLayer.h"
#include "../../AI/Terrain.h"

using namespace cocos2d;

ShadowAssault::ShadowAssault() {
    
}

ShadowAssault::~ShadowAssault() {
    
}

ShadowAssault* ShadowAssault::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    ShadowAssault* ret = new ShadowAssault();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ShadowAssault::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    
    return true;
}

void ShadowAssault::updateFrame( float delta ) {
    
}

void ShadowAssault::begin() {
    UnitNode* chasing_unit = dynamic_cast<UnitNode*>( _owner->getChasingTarget() );
    if( chasing_unit ) {
        float collide = _owner->getTargetData()->collide;
        Vec2 dir = chasing_unit->getUnitDirection();
        Point pos = chasing_unit->getPosition();
        Point new_pos;
        do {
            float radius = chasing_unit->getTargetData()->collide + 3;
            new_pos = pos - dir * radius;
            BattleLayer* battle_layer = _owner->getBattleLayer();
            if( battle_layer->isPositionOK( new_pos, collide ) ) {
                break;
            }
            
            new_pos = pos + Geometry::anticlockwisePerpendicularVecToLine( dir ) * radius;
            if( battle_layer->isPositionOK( new_pos, collide ) ) {
                break;
            }
            
            new_pos = pos + Geometry::clockwisePerpendicularVecToLine( dir ) * radius;
            if( battle_layer->isPositionOK( new_pos, collide ) ) {
                break;
            }
            
            new_pos = pos;
            break;
        }while( true );
        
        _owner->setPosition( new_pos );
        
        DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_SHADOW_ASSAULT, _damage );
        ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getTargetData(), chasing_unit->getTargetData() );
        if( !result["cri"].asBool() ) {
            result["cri"] = Value( true );
            result["damage"] = Value( result["damage"].asFloat() * 2 );
            chasing_unit->takeDamage( result, _owner );
        }
    }
}

void ShadowAssault::end() {
    
}