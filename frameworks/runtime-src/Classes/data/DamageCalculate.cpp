//
//  DamageCalculate.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/13/15.
//
//

#include "DamageCalculate.h"
#include "../unit/UnitNode.h"
#include "../Utils.h"
#include "../constant/BoidsConstant.h"

using namespace cocos2d;

#define CRITICAL_FACTOR 1.5f

DamageCalculate::DamageCalculate() {
    
}

DamageCalculate::~DamageCalculate() {
    
}

DamageCalculate* DamageCalculate::create( const std::string& calculator_name, float base_damage ) {
    DamageCalculate* ret = new DamageCalculate();
    if( ret && ret->init( calculator_name, base_damage ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DamageCalculate::init( const std::string& calculator_name, float base_damage ) {
    _calculator_name = calculator_name;
    _calculator = nullptr;
    _base_damage = base_damage;
    return true;
}

cocos2d::ValueMap DamageCalculate::calculateDamage( class ElementData* atker_data, class ElementData* defer_data ) {
    cocos2d::ValueMap ret;
    
    do {
        if( !this->doesHit( atker_data->hit, defer_data->dodge, atker_data->level, defer_data->level ) ) {
            ret["damage"] = Value( 0 );
            ret["miss"] = Value( true );
            ret["cri"] = Value( false );
            break;
        }
        
        ret["miss"] = Value( false );
        
        float damage = 0;
        if( _calculator ) {
            damage = _calculator( _base_damage, atker_data, defer_data );
        }
        else {
            damage = DamageCalculate::calculateDamage( _calculator_name, _base_damage, atker_data, defer_data );
        }
        if( this->doesCritical( atker_data->critical, defer_data->tenacity, atker_data->level, defer_data->level ) ) {
            damage *= CRITICAL_FACTOR;
            ret["cri"] = Value( true );
        }
        else {
            ret["cri"] = Value( false );
        }
        ret["damage"] = Value( damage );
    }while( false );
    return ret;
}

cocos2d::ValueMap DamageCalculate::calculateDamageWithoutMiss( class ElementData* atker_data, class ElementData* defer_data ) {
    cocos2d::ValueMap ret;
    
    do {
        ret["miss"] = Value( false );
        
        float damage = 0;
        if( _calculator ) {
            damage = _calculator( _base_damage, atker_data, defer_data );
        }
        else {
            damage = DamageCalculate::calculateDamage( _calculator_name, _base_damage, atker_data, defer_data );
        }
        if( this->doesCritical( atker_data->critical, defer_data->tenacity, atker_data->level, defer_data->level ) ) {
            damage *= CRITICAL_FACTOR;
            ret["cri"] = Value( true );
        }
        else {
            ret["cri"] = Value( false );
        }
        ret["damage"] = Value( damage );
    }while( false );
    return ret;
}

float DamageCalculate::calculateDamage( const std::string calculator_name, float base_damage, class ElementData* atker_data, class ElementData* defer_data ) {
    if( calculator_name == "normal" ) {
        return ( base_damage + atker_data->atk ) * DamageCalculate::calculateResistance( defer_data->def, 0, 0 );
    }
    else if( calculator_name == SKILL_NAME_WRATH_OF_THUNDER ) {
        return ( base_damage ) * DamageCalculate::calculateResistance( defer_data->def, 0, 0 );
    }
    else if( calculator_name == SKILL_NAME_LIGHTNING_CHAIN ) {
        return ( base_damage ) * DamageCalculate::calculateResistance( defer_data->def, 0, 0 );
    }
    else if( calculator_name == SKILL_NAME_BARRAGE_OF_ARROW ) {
        return ( base_damage ) * DamageCalculate::calculateResistance( defer_data->def, 0, 0 );
    }
    else if ( calculator_name == SKILL_NAME_CRAZYSHOOT ) {
        return ( base_damage ) * DamageCalculate::calculateResistance( defer_data->def, 0, 0 );
    }
    else {
        return ( base_damage ) * DamageCalculate::calculateResistance( defer_data->def, 0, 0 );
    }
}

float DamageCalculate::calculateResistance( float def, float negl, float neglp ) {
    return 1.0f - powf( ( def - negl ) * ( 1.0f - neglp ), 0.4f ) / 100.0f;
}

bool DamageCalculate::doesHit( float hit, float dodge, float atker_level, float defer_level ) {
    float hit_chance = hit / ( hit + dodge ) * 2.0f * atker_level / ( atker_level + defer_level );
    float rand = Utils::randomFloat();
    return rand <= hit_chance;
}

bool DamageCalculate::doesCritical( float cri, float ten, float atker_level, float defer_level ) {
    float chance = 0;
    if( cri > ten ) {
        chance = ( cri - ten ) / ( ten * 2.0f + 1.0f ) * 2.0 * atker_level / ( defer_level + atker_level );
    }
    else {
        chance = 1.0 / ( ten * 2.0f * ( 80.0f - defer_level ) );
    }
    float rand = Utils::randomFloat();
    return rand <= chance;
}