//
//  SkillCache.cpp
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#include "SkillCache.h"

using namespace cocos2d;

SkillCache* SkillCache::_instance = nullptr;

SkillCache::SkillCache() {
    
}

SkillCache::~SkillCache() {
    
}

SkillCache* SkillCache::getInstance() {
    if( _instance == nullptr ) {
        _instance = new SkillCache();
    }
    return _instance;
}

void SkillCache::destroy() {
    if( _instance ) {
        delete _instance;
        _instance = nullptr;
    }
}

void SkillCache::updateFrame( float delta ) {
    auto itr = _skill_cache.begin();
    while( itr != _skill_cache.end() ) {
        SkillNode* skill = *itr;
        if( skill->shouldRecycle() ) {
            itr = _skill_cache.erase( itr );
        }
        else {
            skill->updateFrame( delta );
            ++itr;
        }
    }
}

void SkillCache::reset() {
    for( auto skill : _skill_cache ) {
        skill->end();
    }
    _skill_cache.clear();
}

void SkillCache::addSkill( SkillNode* skill ) {
    _skill_cache.pushBack( skill );
}

void SkillCache::removeSkill( SkillNode* skill ) {
    _skill_cache.eraseObject( skill );
}

void SkillCache::removeSkillOfOwner( UnitNode* owner ) {
    for( auto itr = _skill_cache.begin(); itr != _skill_cache.end(); ++itr ) {
        if( (*itr)->getOwner() == owner ) {
            (*itr)->end();
            break;
        }
    }
}