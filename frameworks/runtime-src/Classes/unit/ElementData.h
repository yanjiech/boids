//
//  ElementData.h
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#ifndef __Boids__ElementData__
#define __Boids__ElementData__

#include "cocos2d.h"

class ElementData : public cocos2d::Ref {
public:
    int unit_id;
    std::string name;
    std::string display_name;
    int level;
    float hp;
    float current_hp;
    float mp;
    float current_mp;
    float atk;
    float def;
    float move_speed;
    float atk_speed;
    float collide;
    float critical;
    float tenacity;
    float hit;
    float dodge;
    
    float atk_range;
    
    float recover;
    float scale;
    
    std::string bullet_name;
    
public:
    ElementData();
    virtual ~ElementData();
    
    static ElementData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const std::string& value );
};

#endif /* defined(__Boids__ElementData__) */
