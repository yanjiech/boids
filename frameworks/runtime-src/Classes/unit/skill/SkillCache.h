//
//  SkillCache.h
//  Boids
//
//  Created by chenyanjie on 3/31/15.
//
//

#ifndef __Boids__SkillCache__
#define __Boids__SkillCache__

#include "SkillNode.h"

class SkillCache {
private:
    SkillCache();
    static SkillCache* _instance;
    
    cocos2d::Vector<SkillNode*> _skill_cache;
    
public:
    ~SkillCache();
    static SkillCache* getInstance();
    static void destroy();
    
    void updateFrame( float delta );
    
    void reset();
    
    void addSkill( SkillNode* skill );
    void removeSkill( SkillNode* skill );
    void removeSkillOfOwner( UnitNode* owner );
};

#endif /* defined(__Boids__SkillCache__) */
