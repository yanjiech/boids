//
//  ElementData.cpp
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#include "ElementData.h"
#include "../Utils.h"

using namespace cocos2d;

ElementData::ElementData() {
    
}

ElementData::~ElementData() {
    
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
}