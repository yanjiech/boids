//
//  PlayerInfo.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__PlayerInfo__
#define __Boids__PlayerInfo__

#include "cocos2d.h"

class PlayerInfo {
private:
    cocos2d::ValueMap _player_info;
    
private:
    static PlayerInfo* _instance;
    
    PlayerInfo();
    
public:
    ~PlayerInfo();
    
    static PlayerInfo* getInstance();
    
    void loadPlayerInfo();
    
    cocos2d::ValueVector getPlayerDeployedUnitsInfo();
    
    cocos2d::ValueVector getPlayerDeployedUnitNames();
    
    void gainGold( int gain );
    int getGold();
    
    void gainDiamond( int gain );
    int getDiamond();
    
    void gainTeamExp( int exp );
    void setTeamLevel( int level );
    int getTeamLevel();
    
    void gainTeamSkillLevel( int lvl_up, const std::string& skill_name );
    cocos2d::ValueMap& getTeamSkill( const std::string& skill_name );
};

#endif /* defined(__Boids__PlayerInfo__) */
