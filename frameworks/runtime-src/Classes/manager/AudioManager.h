//
//  AudioManager.h
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#ifndef __Boids__AudioManager__
#define __Boids__AudioManager__

#include "cocos2d.h"

class AudioManager {
private:
    static AudioManager* _instance;
    
    AudioManager();
    
public:
    ~AudioManager();
    
    static AudioManager* getInstance();
    static void destroy();
};

#endif /* defined(__Boids__AudioManager__) */
