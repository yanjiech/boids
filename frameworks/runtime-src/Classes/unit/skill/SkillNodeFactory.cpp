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
    else if( name == SKILL_NAME_ATOM_FART ) {
        ret = AtomFart::create( owner, data );
    }
    else if( name == SKILL_NAME_FATAL_THROW ) {
        ret = FatalThrow::create( owner, data );
    }
    else if( name == SKILL_NAME_FIRE_WHIRL ) {
        ret = FireWhirl::create( owner, data );
    }
    else if( name == SKILL_NAME_DEATH_SLICE ) {
        ret = DeathSlice::create( owner, data );
    }
    else if( name == SKILL_NAME_PENETRATE_SPEAR ) {
        ret = PenetrateSpear::create( owner, data, params );
    }
    else if( name == SKILL_NAME_HUNTING_INTUITION ) {
        ret = HuntingIntuition::create( owner, data );
    }
    
    return ret;
}