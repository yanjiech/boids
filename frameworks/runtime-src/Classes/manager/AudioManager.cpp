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

void AudioManager::reset() {
    AudioEngine::stopAll();
    _audio_ids.clear();
}

bool AudioManager::playMusic( const std::string& resource, bool loop ) {
    if( FileUtils::getInstance()->isFileExist( resource ) ) {
        int ret  = AudioEngine::play2d( resource, loop );
        if( ret > 0 ) {
            _audio_ids[resource] = Value( ret );
        }
        return ( ret > 0 );
    }
    
    return false;
}

bool AudioManager::playEffect( const std::string& resource, bool loop ) {
    if( FileUtils::getInstance()->isFileExist( resource ) ) {
        int ret = AudioEngine::play2d( resource, loop );
        if( ret > 0 ) {
            _audio_ids[resource] = Value( ret );
        }
        return ret > 0;
    }
    return false;
}

void AudioManager::pauseMusic( const std::string& resource ) {
    auto itr = _audio_ids.find( resource );
    if( itr != _audio_ids.end() ) {
        AudioEngine::pause( itr->second.asInt() );
    }
}

void AudioManager::resumeMusic( const std::string& resource ) {
    auto itr = _audio_ids.find( resource );
    if( itr != _audio_ids.end() ) {
        AudioEngine::resume( itr->second.asInt() );
    }
}

void AudioManager::stopMusic( const std::string& resource ) {
    auto itr = _audio_ids.find( resource );
    if( itr != _audio_ids.end() ) {
        AudioEngine::stop( itr->second.asInt() );
        _audio_ids.erase( itr );
    }
}

bool AudioManager::vibrate() {
    AudioEngine::vibrate();
    return true;
}