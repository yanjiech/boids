//
//  ArmatureManager.h
//  Boids
//
//  Created by Xin Xu on 10/29/14.
//
//

#ifndef __Boids__ArmatureManager__
#define __Boids__ArmatureManager__

#include <spine/spine-cocos2dx.h>

class ArmatureData {
public:
    spSkeletonData *_skeletonData;
    spAtlas *_atlasData;
};

class ArmatureManager {
public:
    virtual ~ArmatureManager() {}
    static ArmatureManager *getInstance();
    void loadArmatureData(const std::string& resourceName);
    void unloadArmatureData(const std::string& resourceName);
    void clearArmatureData();
    spine::SkeletonAnimation *createArmature(const std::string& resourceName);
    
    // helper functions for spine animation
    cocos2d::Point getBonePosition(spine::SkeletonAnimation *animation, const std::string& boneName);
    cocos2d::Point getSlotPosition(spine::SkeletonAnimation *animation, const std::string& slotName);
    
private:
    static ArmatureManager *_instance;
    ArmatureManager();
    
    std::unordered_map<std::string, ArmatureData> _collection;
};



#endif /* defined(__Boids__ArmatureManager__) */
