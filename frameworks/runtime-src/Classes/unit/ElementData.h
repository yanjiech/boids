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
    
    int view_range;
    
    std::string bullet_name;
    
public:
    ElementData();
    virtual ~ElementData();
    
    static ElementData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const std::string& value );
    
    void add( ElementData* other );
    void sub( ElementData* other );
};

class UnitData : public ElementData {
public:
    UnitData();
    virtual ~UnitData();
    
    static UnitData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const std::string& value );
    
    float guard_radius;
    
    bool is_melee;
    bool is_double_face;
    int default_face_dir;
    
    int star;
    int price_type;
    int price;
    int place;
    
    std::vector<std::string> skill_names;
};

#endif /* defined(__Boids__ElementData__) */
