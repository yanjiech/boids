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
    this->stopAll();
}

bool AudioManager::playMusic( const std::string& resource, bool loop ) {
    if( FileUtils::getInstance()->isFileExist( resource ) ) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic( resource.c_str(), loop );
        return true;
    }
    
    return false;
}

bool AudioManager::playEffect( const std::string& resource, bool loop ) {
    if( FileUtils::getInstance()->isFileExist( resource ) ) {
        int aid = SimpleAudioEngine::getInstance()->playEffect( resource.c_str() );
        _audio_ids[resource] = Value( aid );
        return true;
    }
    return false;
}

void AudioManager::pauseMusic( const std::string& resource ) {
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioManager::resumeMusic( const std::string& resource ) {
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AudioManager::stopMusic( const std::string& resource ) {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void AudioManager::stopEffect( const std::string& resource ) {
    auto itr = _audio_ids.find( resource );
    if( itr != _audio_ids.end() ) {
        SimpleAudioEngine::getInstance()->stopEffect( itr->second.asInt() );
        _audio_ids.erase( itr );
    }
}

bool AudioManager::vibrate() {
    AudioEngine::vibrate();
    return true;
}

void AudioManager::stopAll() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    AudioEngine::stopAll();
    _audio_ids.clear();
}