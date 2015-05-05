//
//  ArmatureManager.cpp
//  Boids
//
//  Created by Xin Xu on 10/29/14.
//
//

#include "ArmatureManager.h"
#include "Utils.h"

using namespace cocos2d;

ArmatureManager* ArmatureManager::_instance = nullptr;

ArmatureManager *ArmatureManager::getInstance() {
    if( _instance == nullptr ) {
        _instance = new ArmatureManager();
    }
    return _instance;
}

ArmatureManager::ArmatureManager() {
    
}

void ArmatureManager::loadArmatureData(const std::string& resourceName) {
    auto it = _collection.find(resourceName);
    if (it != _collection.end()) {
        return;
    }
    
    std::string atlasFile, jsonFile;
    atlasFile = Utils::stringFormat("%s/skeleton.atlas", resourceName.c_str());
    jsonFile = Utils::stringFormat("%s/skeleton.json", resourceName.c_str());
//    binaryFile = Utils::stringFormat("%s/skeleton.skel", resourceName.c_str());
    ArmatureData data;
	data._atlasData = spAtlas_createFromFile(atlasFile.c_str(), 0);
    spSkeletonJson* json = spSkeletonJson_create(data._atlasData);
    json->scale = 1;
    data._skeletonData = spSkeletonJson_readSkeletonDataFile(json, jsonFile.c_str());
    CCASSERT(data._skeletonData, json->error ? json->error : "Error reading skeleton data file.");
    spSkeletonJson_dispose(json);
    _collection[resourceName] = data;
}

void ArmatureManager::unloadArmatureData(const std::string& resourceName) {
    auto it = _collection.find(resourceName);
    if (it != _collection.end()) {
        spSkeletonData_dispose(it->second._skeletonData);
        spAtlas_dispose(it->second._atlasData);
        _collection.erase(it);
    }
}

void ArmatureManager::clearArmatureData() {
    for( auto pair : _collection ) {
        spSkeletonData_dispose( pair.second._skeletonData);
        spAtlas_dispose( pair.second._atlasData );
    }
    _collection.clear();
}

spine::SkeletonAnimation *ArmatureManager::createArmature(const std::string& resourceName) {
    auto it = _collection.find(resourceName);
    if (it == _collection.end()) {
        this->loadArmatureData(resourceName);
        it = _collection.find(resourceName);
    }
    auto data = it->second;
    return spine::SkeletonAnimation::createWithData(data._skeletonData);
}

Point ArmatureManager::getBonePosition(spine::SkeletonAnimation *animation, const std::string& boneName) {
    auto bone = animation->findBone(boneName);
    if (bone) {
        return Vec2(bone->worldX, bone->worldY);
    }
    return Vec2::ZERO;
}

Point ArmatureManager::getSlotPosition(spine::SkeletonAnimation *animation, const std::string& slotName) {
    auto slot = animation->findSlot(slotName);
    if (slot) {
        return Vec2(slot->bone->worldX, slot->bone->worldY);
    }
    return Vec2::ZERO;
}