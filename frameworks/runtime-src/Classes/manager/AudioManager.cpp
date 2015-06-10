//
//  AudioManager.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#include "AudioManager.h"
#include "audio/include/SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace experimental;

AudioManager* AudioManager::_instance = nullptr;

AudioManager::AudioManager() {

}

AudioManager::~AudioManager() {
    
}

AudioManager* AudioManager::getInstance() {
    if( _instance == nullptr ) {
        _instance = new AudioManager();
    }
    return _instance;
}

void AudioManager::destroy() {
    if( _instance ) {
        delete _instance;
        _instance = nullptr;
    }
}

bool AudioManager::playMusic( const std::string& resource, bool loop ) {
    if( FileUtils::getInstance()->isFileExist( resource ) ) {
        return ( AudioEngine::play2d( resource, loop ) > 0 );
    }
    
    return false;
}

bool AudioManager::playEffect( const std::string& resource, bool loop ) {
    if( FileUtils::getInstance()->isFileExist( resource ) ) {
        return ( AudioEngine::play2d( resource, loop ) > 0 );
    }
    return false;
}

bool AudioManager::vibrate() {
    AudioEngine::vibrate();
    return true;
}