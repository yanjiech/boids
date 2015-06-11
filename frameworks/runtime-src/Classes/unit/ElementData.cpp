//
//  ElementData.cpp
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#include "ElementData.h"
#include "../Utils.h"
#include "../manager/ResourceManager.h"
#include "../data/PlayerInfo.h"

using namespace cocos2d;

ElementData::ElementData() :
unit_id( 0 ),
level( 0 ),
name( "" ),
display_name( "" ),
hp( 0 ),
current_hp( 0 ),
mp( 0 ),
current_mp( 0 ),
atk( 0 ),
def( 0 ),
move_speed( 0 ),
atk_speed( 0 ),
collide( 0 ),
critical( 0 ),
tenacity( 0 ),
hit( 0 ),
dodge( 0 ),
atk_range( 0 ),
bullet_name( "" ),
view_range( 0 ),
_weapon_data( nullptr ),
_armor_data( nullptr ),
_boot_data( nullptr ),
_accessory_data( nullptr )
{
    
}

ElementData::~ElementData() {
    CC_SAFE_RELEASE( _weapon_data );
    CC_SAFE_RELEASE( _armor_data );
    CC_SAFE_RELEASE( _boot_data );
    CC_SAFE_RELEASE( _accessory_data );
}

ElementData* ElementData::create( const cocos2d::ValueMap& data ) {
    ElementData* ret = new ElementData();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool ElementData::init( const cocos2d::ValueMap& data ) {
    if( !data.empty() ) {
        this->unit_id = data.at( "id" ).asInt();
        this->level = data.at( "level" ).asInt();
        this->name = data.at( "name" ).asString();
        this->display_name = data.at( "displayname" ).asString();
        this->hp = data.at( "hp" ).asFloat() + this->level * (float)data.at( "hpgr" ).asFloat();
        this->current_hp = this->hp;
        this->mp = data.at( "mp" ).asFloat() + this->level * (float)data.at( "mpgr" ).asFloat();
        this->current_mp = this->mp;
        this->atk = data.at( "atk" ).asFloat() + this->level * data.at( "atkgr" ).asFloat();
        this->def = data.at( "def" ).asFloat() + this->level * data.at( "defgr" ).asFloat();
        this->move_speed = data.at( "movespeed" ).asFloat();
        this->atk_speed = data.at( "attackspeed" ).asFloat();
        this->collide = data.at( "collide" ).asFloat();
        this->critical = data.at( "cri" ).asFloat() + this->level * data.at( "crigr" ).asFloat();
        this->tenacity = data.at( "ten" ).asFloat() + this->level * data.at( "tengr" ).asFloat();
        this->hit = data.at( "hit" ).asFloat() + this->level * data.at( "hitgr" ).asFloat();
        this->dodge = data.at( "dodge" ).asFloat() + this->level * data.at( "dodge" ).asFloat();
        this->atk_range = data.at( "range" ).asFloat();
        
        auto itr = data.find( "bullet_name" );
        if( itr != data.end() ) {
            this->bullet_name = itr->second.asString();
        }
        else {
            this->bullet_name = "";
        }
        
        itr = data.find( "view_range" );
        if( itr != data.end() ) {
            this->view_range = itr->second.asInt();
        }
    }
    
    return true;
}

void ElementData::setAttribute( const std::string& key, const std::string& value ) {
    if( key == "atk" ) {
        this->atk = (float)Utils::toDouble( value );
    }
    else if( key == "attackspeed" ) {
        this->atk_speed = (float)Utils::toDouble( value );
    }
    else if( key == "collide" ) {
        this->collide = (float)Utils::toDouble( value );
    }
    else if( key == "cri" ) {
        this->critical = (float)Utils::toDouble( value );
    }
    else if( key == "def" ) {
        this->def = (float)Utils::toDouble( value );
    }
    else if( key == "displayname" ) {
        this->display_name = value;
    }
    else if( key == "dodge" ) {
        this->dodge = (float)Utils::toDouble( value );
    }
    else if( key == "hit" ) {
        this->hit = (float)Utils::toDouble( value );
    }
    else if( key == "hp" ) {
        this->hp = (float)Utils::toDouble( value );
        this->current_hp = this->hp;
    }
    else if( key == "mp" ) {
        this->mp = (float)Utils::toDouble( value );
        this->current_mp = this->mp;
    }
    else if( key == "id" ) {
        this->unit_id = Utils::toInt( value );
    }
    else if( key == "movespeed" ) {
        this->move_speed = (float)Utils::toDouble( value );
    }
    else if( key == "name" ) {
        this->name = value;
    }
    else if( key == "range" ) {
        this->atk_range = (float)Utils::toDouble( value );
    }
    else if( key == "rec" ) {
        this->recover = (float)Utils::toDouble( value );
    }
    else if( key == "ten") {
        this->tenacity = (float)Utils::toDouble( value );
    }
    else if( key == "view_range" ) {
        this->view_range = Utils::toInt( value );
    }
}

void ElementData::add( ElementData* other ) {
    this->level += other->level;
    this->hp += other->hp;
    this->current_hp += other->current_hp;
    this->mp += other->mp;
    this->current_mp += other->current_mp;
    this->atk += other->atk;
    this->def += other->def;
    this->move_speed += other->move_speed;
    this->atk_speed += other->atk_speed;
    this->collide += other->collide;
    this->critical += other->critical;
    this->tenacity += other->tenacity;
    this->hit += other->hit;
    this->dodge += other->dodge;
    this->atk_range += other->atk_range;
    this->view_range += other->view_range;
}

void ElementData::sub( ElementData* other ) {
    this->level -= other->level;
    this->hp -= other->hp;
    if( this->current_hp > this->hp ) {
        this->current_hp = this->hp;
    }
    this->mp -= other->mp;
    if( this->current_mp > this->mp ) {
        this->current_mp = this->mp;
    }
    this->atk -= other->atk;
    this->def -= other->def;
    this->move_speed -= other->move_speed;
    this->atk_speed -= other->atk_speed;
    this->collide -= other->collide;
    this->critical -= other->critical;
    this->tenacity -= other->tenacity;
    this->hit -= other->hit;
    this->dodge -= other->dodge;
    this->atk_range -= other->atk_range;
    this->view_range -= other->view_range;
}

void ElementData::add( class EquipmentData* data ) {
    this->hp += data->hp;
    this->current_hp = this->hp;
    this->mp += data->mp;
    this->current_mp = this->mp;
    this->atk += data->atk;
    this->def += data->def;
    this->hit += data->hit;
    this->dodge += data->dod;
    this->critical += data->cri;
    this->tenacity += data->ten;
}

void ElementData::sub( class EquipmentData* data ) {
    this->hp -= data->hp;
    this->mp -= data->mp;
    this->atk -= data->atk;
    this->def -= data->def;
    this->hit -= data->hit;
    this->dodge -= data->dod;
    this->critical -= data->cri;
    this->tenacity -= data->ten;
}

EquipmentData* ElementData::getEquipData( eEquipType type ) {
    switch( type )
    {
        case eEquipType::EquipTypeWeapon:
            return _weapon_data;
        case eEquipType::EquipTypeArmor:
            return _armor_data;
        case eEquipType::EquipTypeBoot:
            return _boot_data;
        case eEquipType::EquipTypeAccessory:
            return _accessory_data;
        default:
            break;
    }
    return nullptr;
}

void ElementData::setEquipData( eEquipType type, EquipmentData* data ) {
    CC_SAFE_RETAIN( data );
    switch( type )
    {
        case eEquipType::EquipTypeWeapon:
        {
            if( _weapon_data != nullptr ) {
                this->sub( _weapon_data );
                _weapon_data->release();
            }
            _weapon_data = data;
            if( _weapon_data != nullptr ) {
                this->add( _weapon_data );
            }
            break;
        }
            
        case eEquipType::EquipTypeArmor:
        {
            if( _armor_data != nullptr ) {
                this->sub( _armor_data );
                _armor_data->release();
            }
            _armor_data = data;
            if( _armor_data != nullptr ) {
                this->add( _armor_data );
            }
            break;
        }
        case eEquipType::EquipTypeBoot:
        {
            if( _boot_data != nullptr ) {
                this->sub( _boot_data );
                _boot_data->release();
            }
            _boot_data = data;
            if( _boot_data != nullptr ) {
                this->add( _boot_data );
            }
            break;
        }
        case eEquipType::EquipTypeAccessory:
        {
            if( _accessory_data != nullptr ) {
                this->sub( _accessory_data );
                _accessory_data->release();
            }
            _accessory_data = data;
            if( _accessory_data != nullptr ) {
                this->add( _accessory_data );
            }
            break;
        }
        default:
            CC_SAFE_RELEASE( data );
            break;
    }
}

//unit data
UnitData::UnitData() {
    
}

UnitData::~UnitData() {
}

UnitData* UnitData::create( const cocos2d::ValueMap& data ) {
    UnitData* ret = new UnitData();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitData::init( const cocos2d::ValueMap& data ) {
    std::string name = data.at( "name" ).asString();
    int level = data.at( "level" ).asInt();
    
    ValueMap unit_config = ResourceManager::getInstance()->getUnitData( name );
    unit_config["name"] = Value( name );
    unit_config["level"] = Value( level );
    
    if( !ElementData::init( unit_config ) ) {
        return false;
    }
    
    this->guard_radius = unit_config.at( "guard_radius" ).asFloat();
    
    this->recover = unit_config.at( "rec" ).asFloat() + this->level * unit_config.at( "recgr" ).asFloat();
    this->scale = unit_config.at( "scale" ).asFloat();
    
    this->is_melee = unit_config.at( "is_melee" ).asBool();
    this->is_double_face = unit_config.at( "double_face" ).asBool();
    this->default_face_dir = unit_config.at( "faceleft" ).asInt();
    
    this->star = unit_config.at( "star" ).asInt();
    this->price_type = unit_config.at( "price_type" ).asInt();
    this->price = unit_config.at( "price" ).asInt();
    this->place = unit_config.at( "place" ).asInt();
    
    //equips
    auto itr = data.find( "equips" );
    if( itr != data.end() ) {
        const ValueMap& equips = data.at( "equips" ).asValueMap();
        const ValueMap& all_equips = PlayerInfo::getInstance()->getAllEquipsInfo();
        itr = all_equips.find( equips.at( "weapon" ).asString() );
        if( itr != all_equips.end() ) {
            EquipmentData* ed = EquipmentData::create( itr->second.asValueMap() );
            this->setEquipData( eEquipType::EquipTypeWeapon, ed );
        }
        
        itr = all_equips.find( equips.at( "armor" ).asString() );
        if( itr != all_equips.end() ) {
            EquipmentData* ed = EquipmentData::create( itr->second.asValueMap() );
            this->setEquipData( eEquipType::EquipTypeArmor, ed );
        }
        
        itr = all_equips.find( equips.at( "boot" ).asString() );
        if( itr != all_equips.end() ) {
            EquipmentData* ed = EquipmentData::create( itr->second.asValueMap() );
            this->setEquipData( eEquipType::EquipTypeBoot, ed );
        }
        
        itr = all_equips.find( equips.at( "accessory" ).asString() );
        if( itr != all_equips.end() ) {
            EquipmentData* ed = EquipmentData::create( itr->second.asValueMap() );
            this->setEquipData( eEquipType::EquipTypeAccessory, ed );
        }
    }
    
    //skills
    const ValueVector& skill_config = unit_config.at( "skills" ).asValueVector();
    itr = data.find( "skills" );
    ValueVector skill_vector = itr->second.asValueVector();
    for( auto sk : skill_vector ) {
        ValueMap& vm = sk.asValueMap();
        auto sitr = vm.find( "skill_id" );
        if( sitr != vm.end() ) {
            int skill_id = sitr->second.asInt();
            std::string skill_name = skill_config.at( skill_id ).asString();
            vm["name"] = Value( skill_name );
        }
        skills.push_back( Value( vm ) );
    }
    
    return true;
}

void UnitData::setAttribute( const std::string& key, const std::string& value ) {
    ElementData::setAttribute( key, value );
    if( key == "guard_radius" ) {
        this->guard_radius = (float)Utils::toDouble( value );
    }
    else if( key == "star" ) {
        this->star = Utils::toInt( value );
    }
    else if( key == "price_type" ) {
        this->price_type = Utils::toInt( value );
    }
    else if( key == "price" ) {
        this->price = Utils::toInt( value );
    }
    else if( key == "place" ) {
        this->place = Utils::toInt( value );
    }
    else if( key == "scale" ) {
        this->scale = (float)Utils::toDouble( value );
    }
}

//equipment data
std::string EquipmentData::stringFromType( eEquipType type ) {
    switch( type ) {
        case eEquipType::EquipTypeWeapon:
            return "weapon";
        case eEquipType::EquipTypeArmor:
            return "armor";
        case eEquipType::EquipTypeBoot:
            return "boot";
        case eEquipType::EquipTypeAccessory:
            return "accessory";
        default:
            return "";
    }
}

EquipmentData::EquipmentData() :
obj_id( "0" ),
equip_id( 0 ),
hp( 0 ),
mp( 0 ),
atk( 0 ),
def( 0 ),
hit( 0 ),
dod( 0 ),
cri( 0 ),
ten( 0 )
{
    
}

EquipmentData::~EquipmentData() {
    
}

EquipmentData* EquipmentData::create( const cocos2d::ValueMap& data ) {
    EquipmentData* ret = new EquipmentData();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool EquipmentData::init( const cocos2d::ValueMap& data ) {
    auto itr = data.find( "obj_id" );
    if( itr != data.end() ) {
        this->obj_id = itr->second.asString();
        std::string equip_id = data.at( "id" ).asString();
        const cocos2d::ValueMap& equip_config = ResourceManager::getInstance()->getEquipConfig().at( equip_id ).asValueMap();
        for( auto pair : equip_config ) {
            this->setAttribute( pair.first, pair.second );
        }
    }
    
    return true;
}

void EquipmentData::setAttribute( const std::string& key, const cocos2d::Value& value ) {
    if( key == "display_name" ) {
        this->display_name = value.asString();
    }
    else if( key == "equip_id" ) {
        this->equip_id = value.asInt();
        this->equip_type = (eEquipType)( this->equip_id / 1e7 );
    }
    else if( key == "name" ) {
        this->name = value.asString();
    }
    else if( key == "hp" ) {
        this->hp = value.asFloat();
    }
    else if( key == "mp" ) {
        this->mp = value.asFloat();
    }
    else if( key == "atk" ) {
        this->atk = value.asFloat();
    }
    else if( key == "def" ) {
        this->def = value.asFloat();
    }
    else if( key == "hit" ) {
        this->hit = value.asFloat();
    }
    else if( key == "dod" ) {
        this->dod = value.asFloat();
    }
    else if( key == "cri" ) {
        this->cri = value.asFloat();
    }
    else if( key == "ten" ) {
        this->ten = value.asFloat();
    }
    else if( key == "price" ) {
        this->price = value.asInt();
    }
}
