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

class Skill : public cocos2d::Ref {
private:
    std::string _name;
    float _cd;
    
    
public:
    Skill();
    virtual ~Skill();
    
    static Skill* create( const std::string& name );
    virtual bool init( const std::string& name );
    
    const std::string& getName() { return _name; }
    float getCD() { return _cd; }
};

#endif /* defined(__Boids__Skill__) */
