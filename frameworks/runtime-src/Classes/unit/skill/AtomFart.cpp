//
//  AtomFart.cpp
//  Boids
//
//  Created by chenyanjie on 4/7/15.
//
//

#include "AtomFart.h"
#include "../../scene/BattleLayer.h"

using namespace cocos2d;

AtomFart::AtomFart() {
    
}

AtomFart::~AtomFart() {
    
}

AtomFart* AtomFart::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    AtomFart* ret = new AtomFart();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool AtomFart::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _range = data.at( "range" ).asFloat();
    _buff_damage = data.at( "buff_damage" ).asValueVector().at( level - 1 ).asFloat();
    _buff_duration = data.at( "buff_duration" ).asValueVector().at( level - 1 ).asFloat();
    
    return true;
}

void AtomFart::updateFrame( float delta ) {
    
}

void AtomFart::begin() {
    std::string resource = "effects/human_king_skill_1";
    std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_ATOM_FART, BulletNode::getNextBulletId() );
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    skeleton->setScale( 1.5f );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( skeleton, name, true );
    Point dir = _owner->getUnitDirection();
    Point fart_pos = _owner->getBonePos( "bone11" );
    component->setAnimation( 0, "animation", false );
    _owner->getBattleLayer()->addToEffectLayer( component, fart_pos, 0 );
    
    ValueMap buff_data;
    buff_data["duration"] = Value( _buff_duration );
    buff_data["damage"] = Value( _buff_damage );
    buff_data["buff_type"] = Value( BUFF_TYPE_POISON );
    buff_data["buff_name"] = Value( "AtomFart" );
    
    DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_ATOM_FART, _damage );
    
    Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getUnitCamp(), fart_pos, _range );
    for( auto itr = candidates.begin(); itr != candidates.end(); ++itr ) {
        UnitNode* unit = *itr;
        PoisonBuff* buff = PoisonBuff::create( unit, buff_data );
        unit->addBuff( buff->getBuffId(), buff );
        
        ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getUnitData(), unit->getUnitData() );
        unit->takeDamage( result, _owner->getDeployId() );
        
        Point push_dir = unit->getPosition() - _owner->getPosition();
        push_dir.normalize();
        float push_distance = _range * 1.2f - unit->getPosition().distance( _owner->getPosition() );
        unit->pushToward( push_dir, push_distance );
    }
}

void AtomFart::end() {
    
}