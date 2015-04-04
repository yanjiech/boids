//
//  Skill.h
//  Boids
//
//  Created by Yanjie Chen on 3/9/15.
//
//

#ifndef __Boids__Skill__
#define __Boids__Skill__

#include "cocos2d.h"

enum eSkillState {
    SkillStateLoading = 1,
    SkillStateReady = 2,
    SkillStateCasting = 3
};

class UnitNode;

class Skill : public cocos2d::Ref {
private:
    std::string _skill_name;
    int _level;
    
    float _full_cd;
    float _elapse;
    
    eSkillState _state;
    
    cocos2d::ValueMap _skill_data;
    
    UnitNode* _owner;
public:
    Skill();
    virtual ~Skill();
    
    static Skill* create( UnitNode* owner, const cocos2d::ValueMap& data );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data );
    
    virtual void updateFrame( float delta );
    
    void activate( const cocos2d::ValueMap& params );
    void stopload();
    void reload();
    
    const std::string& getSkillName() { return _skill_name; }
    void setSkillName( const std::string& skill_name ) { _skill_name = skill_name; }
    
    int getSkillLevel() { return _level; }
    void setSkillLevel( float level ) { _level = level; }
    
    const cocos2d::ValueMap& getSkillData() { return _skill_data; }
    
    float getFullCD() { return _full_cd; }
    void setFullCD( float cd ) { _full_cd = cd; }
    
    eSkillState getSkillState() { return _state; }
    void setSkillState( eSkillState new_state ) { _state = new_state; }
    
    bool isSkillReady();
    
    float getSkillCD();
    
    float getSkillRadius();
    float getSkillRange();
    float getSkillMaxRange();
    float getSkillMinRange();
    std::string getSkillHintType();
    
    bool shouldContinue();
};

#endif /* defined(__Boids__Skill__) */
