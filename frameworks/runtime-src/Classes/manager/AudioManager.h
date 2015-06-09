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
    
    cocos2d::ValueMap _audio_ids;
    
public:
    ~AudioManager();
    
    static AudioManager* getInstance();
    static void destroy();
    
    bool playMusic( const std::string& resource, bool loop = false );
    bool playEffect( const std::string& resource, bool loop = false );
};

#endif /* defined(__Boids__AudioManager__) */
