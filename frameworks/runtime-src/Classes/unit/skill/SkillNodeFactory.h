//
//  SkillNodeFactory.h
//  Boids
//
//  Created by Yanjie Chen on 3/19/15.
//
//

#ifndef __Boids__SkillNodeFactory__
#define __Boids__SkillNodeFactory__

#include "WrathOfThunder.h"
#include "LightningChain.h"
#include "BarrageOfArrow.h"
#include "CrazyShoot.h"
#include "GiantSword.h"
#include "LightShield.h"
#include "DarkPierce.h"
#include "BatSwarm.h"
#include "NaturesAttendants.h"
#include "Impetus.h"
#include "BonesRain.h"
#include "MagicPulse.h"
#include "MissileShowers.h"
#include "AtomFart.h"
#include "FatalThrow.h"
#include "FireWhirl.h"
#include "DeathSlice.h"
#include "PenetrateSpear.h"
#include "HuntingIntuition.h"
#include "GasBomb.h"
#include "CrazyScratch.h"
#include "NinthLife.h"

class SkillNodeFactory {
public:
    static SkillNode* createSkillNode( const std::string& name, UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
};

#endif /* defined(__Boids__SkillNodeFactory__) */
