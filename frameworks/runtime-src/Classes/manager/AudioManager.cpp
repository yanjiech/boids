//
//  AudioManager.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#include "AudioManager.h"

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