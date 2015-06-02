//
//  DarkPierce.cpp
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#include "DarkPierce.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"

using namespace cocos2d;

DarkPierce::DarkPierce() {
    
}

DarkPierce::~DarkPierce() {
    
}

DarkPierce* DarkPierce::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    DarkPierce* ret = new DarkPierce();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DarkPierce::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _count = data.at( "count" ).asInt();
    _range = data.at( "range" ).asFloat();
    _duration = data.at( "stun" ).asFloat();
    
    return true;
}

void DarkPierce::updateFrame( float delta ) {
}

void DarkPierce::begin() {
    BattleLayer* battle_layer = _owner->getBattleLayer();
    Vector<UnitNode*> candidates = battle_layer->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _owner->getPosition(), _range );
    int size = (int)candidates.size();
    for( int i = 0; i < _count && size > 0; i++ ) {
        int rand = Utils::randomNumber( size ) - 1;
        UnitNode* target_unit = candidates.at( rand );
        ValueMap buff_data;
        buff_data["buff_type"] = Value( BUFF_TYPE_PIERCE );
        buff_data["duration"] = Value( _duration );
        buff_data["buff_name"] = Value( "DarkPierce" );
        Buff* buff = Buff::create( target_unit, buff_data );
        target_unit->addBuff( buff->getBuffId(), buff );
        buff->begin();
        
        DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_DARK_PIERCE, _damage );
        ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getUnitData(), target_unit->getUnitData() );
        target_unit->takeDamage( result, _owner );
        candidates.erase( rand );
        --size;
    }
}

void DarkPierce::end() {
}