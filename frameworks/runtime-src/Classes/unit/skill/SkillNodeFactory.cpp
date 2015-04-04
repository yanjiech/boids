//
//  SkillNodeFactory.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#include "SkillNodeFactory.h"
#include "../UnitNode.h"
#include "../../constant/BoidsConstant.h"

SkillNode* SkillNodeFactory::createSkillNode( const std::string& name, UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    SkillNode* ret = nullptr;
    if( name == SKILL_NAME_WRATH_OF_THUNDER ) {
        ret = WrathOfThunder::create( owner, data );
    }
    else if( name == SKILL_NAME_LIGHTNING_CHAIN ) {
        ret = LightningChain::create( owner, data, dynamic_cast<UnitNode*>( owner->getChasingTarget() ) );
    }
    else if( name == SKILL_NAME_BARRAGE_OF_ARROW ) {
        ret = BarrageOfArrow::create( owner, data );
    }
    else if( name == SKILL_NAME_CRAZYSHOOT ) {
        ret = CrazyShoot::create( owner, data );
    }
    else if( name == SKILL_NAME_GIANT_SWORD ) {
        ret = GiantSword::create( owner, data, params );
    }
    else if( name == SKILL_NAME_LIGHTSHIELD ) {
        ret = LightShield::create( owner, data );
    }
    else if( name == SKILL_NAME_DARK_PIERCE ) {
        ret = DarkPierce::create( owner, data );
    }
    else if( name == SKILL_NAME_BAT_SWARM ) {
        ret = BatSwarm::create( owner, data );
    }
    else if( name == SKILL_NAME_NATURESATTENDANTS ) {
        ret = NaturesAttendants::create( owner, data );
    }
    else if( name == SKILL_NAME_IMPETUS ) {
        ret = Impetus::create( owner, data );
    }
    else if( name == SKILL_NAME_BONESRAIN ) {
        ret = BonesRain::create( owner, data, params );
    }
    else if( name == SKILL_NAME_MAGIC_PULSE ) {
        ret = MagicPulse::create( owner, data );
    }
    else if( name == SKILL_NAME_MISSILE_SHOWERS ) {
        ret = MissileShowers::create( owner, data );
    }
    
    return ret;
}