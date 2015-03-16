//
//  PlayerInfo.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__PlayerInfo__
#define __Boids__PlayerInfo__

#include "external/json/document.h"

class PlayerInfo {
private:
    rapidjson::Document _player_info;
    
private:
    static PlayerInfo* _instance;
    
    PlayerInfo();
    
public:
    ~PlayerInfo();
    
    static PlayerInfo* getInstance();
    
    void loadPlayerInfo();
    
    const rapidjson::Value& getPlayerUnitsInfo();
};

#endif /* defined(__Boids__PlayerInfo__) */
