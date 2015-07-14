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

enum eEquipType {
    EquipTypeWeapon = 3,
    EquipTypeArmor = 1,
    EquipTypeBoot = 2,
    EquipTypeAccessory = 4
};

class EquipmentData : public cocos2d::Ref {
public:
    std::string obj_id;
    int equip_id;
    eEquipType equip_type;
    std::string name;
    std::string display_name;
    float hp;
    float mp;
    float atk;
    float def;
    float hit;
    float dod;
    float cri;
    float ten;
    
    int price;
    
public:
    static std::string stringFromType( eEquipType type );
    
    EquipmentData();
    virtual ~EquipmentData();
    
    static EquipmentData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const cocos2d::Value& value );
};

class ElementData : public cocos2d::Ref {
public:
    int unit_id;
    std::string name;
    std::string display_name;
    int level;
    float hp;
    float mp;
    float atk;
    float def;
    float move_speed;
    float atk_speed;
    float critical;
    float tenacity;
    float hit;
    float dodge;
    float atk_range;
    float recover;
    float guard_radius;
    float collide;

    float final_hp;
    float final_mp;
    float final_atk;
    float final_def;
    float final_move_speed;
    float final_atk_speed;
    float final_critical;
    float final_tenacity;
    float final_hit;
    float final_dodge;
    float final_atk_range;
    float final_recover;
    float final_guard_radius;
    float final_collide;
    
    int view_range;

    float scale;
    std::string bullet_name;
    
private:
    EquipmentData* _weapon_data;
    EquipmentData* _armor_data;
    EquipmentData* _boot_data;
    EquipmentData* _accessory_data;
    
public:
    ElementData();
    virtual ~ElementData();
    
    static ElementData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const std::string& value );
    
    void add( ElementData* other );
    void sub( ElementData* other );
    
    void add( EquipmentData* data );
    void sub( EquipmentData* data );
    
    EquipmentData* getEquipData( eEquipType type );
    void setEquipData( eEquipType type, EquipmentData* data );
};

class UnitData : public ElementData {
public:
    bool is_melee;
    bool is_double_face;
    int default_face_dir;
    
    int star;
    int price_type;
    int price;
    int place;
    
    cocos2d::ValueVector skills;
    
public:
    UnitData();
    virtual ~UnitData();
    
    static UnitData* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
    
    virtual void setAttribute( const std::string& key, const std::string& value );
};

#endif /* defined(__Boids__ElementData__) */
