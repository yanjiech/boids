//
//  Impetus.cpp
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#include "Impetus.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"
#include "../../Utils.h"

using namespace cocos2d;

Impetus::Impetus() {
    
}

Impetus::~Impetus() {
    
}

Impetus* Impetus::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    Impetus* ret = new Impetus();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Impetus::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _buff_damage = data.at( "buff_damage" ).asValueVector().at( level - 1 ).asFloat();
    _buff_duration = data.at( "buff_duration" ).asFloat();
    _range = data.at( "range" ).asFloat();
    
    return true;
}

void Impetus::updateFrame( float delta ) {
}

void Impetus::begin() {    
    Point init_pos = _owner->getPosition();
    
    UnitNode* target_unit = dynamic_cast<UnitNode*>( _owner->getChasingTarget() );
    if( target_unit == nullptr || target_unit->getPosition().distance( _owner->getPosition() ) > _range ) {
        Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), init_pos, init_pos, _range );
        int size = (int)candidates.size();
        if( size > 0 ) {
            int rand = Utils::randomNumber( size ) - 1;
            target_unit = candidates.at( rand );
        }
    }

    if( target_unit ) {
        ValueMap buff_data;
        buff_data["duration"] = Value( _buff_duration );
        buff_data["damage"] = Value( _buff_damage );
        buff_data["buff_name"] = Value( SKILL_NAME_IMPETUS );
        ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "enchantress_skill_javelin" );
        bullet_data["will_miss"] = Value( false );
        DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_IMPETUS, _damage );
        BulletNode* bullet = BulletNode::create( _owner, bullet_data, calculator, buff_data );
        bullet->shootAt( target_unit );
    }
}

void Impetus::end() {
}