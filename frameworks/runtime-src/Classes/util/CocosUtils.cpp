//
//  CocosUtils.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "CocosUtils.h"

#define MAX_BOIDS_EVENT_ACTION_ID 0x7fffffff

using namespace cocos2d;

int CocosUtils::_global_boids_event_action_id = 0;

const rapidjson::Value& CocosUtils::NULL_RAPIDJSON_VALUE = rapidjson::Value();

std::string CocosUtils::getNextGlobalBoidsEventActionId() {
    ++_global_boids_event_action_id;
    return Utils::stringFormat( "boids_event_action_%d", _global_boids_event_action_id );
}

cocos2d::ValueMap CocosUtils::jsonObjectToValueMap( const rapidjson::Value& value ) {
    CCASSERT( value.IsObject(), "json value should be object" );
    ValueMap ret;
    for( rapidjson::Value::ConstMemberIterator itr = value.MemberonBegin(); itr != value.MemberonEnd(); ++itr ) {
        std::string key = std::string( itr->name.GetString(), itr->name.GetStringLength() );
        if( itr->value.IsObject() ) {
            ret.insert( std::make_pair( key, Value( CocosUtils::jsonObjectToValueMap( itr->value ) ) ) );
        }
        else if( itr->value.IsArray() ) {
            ret.insert( std::make_pair( key, Value( CocosUtils::jsonArrayToValueVector( itr->value ) ) ) );
        }
        else {
            ret.insert( std::make_pair( key, CocosUtils::jsonValueToValue( itr->value ) ) );
        }
    }
    return ret;
}

cocos2d::ValueVector CocosUtils::jsonArrayToValueVector( const rapidjson::Value& value ) {
    CCASSERT( value.IsArray(), "json value should be array" );
    ValueVector ret;
    
    for( rapidjson::Value::ConstValueIterator itr = value.onBegin(); itr != value.onEnd(); ++itr ) {
        if( itr->IsObject() ) {
            ret.push_back( Value( CocosUtils::jsonObjectToValueMap( *itr ) ) );
        }
        else if( itr->IsArray() ) {
            ret.push_back( Value( CocosUtils::jsonArrayToValueVector( *itr ) ) );
        }
        else {
            ret.push_back( CocosUtils::jsonValueToValue( *itr ) );
        }
    }
    
    return ret;
}

cocos2d::Value CocosUtils::jsonValueToValue( const rapidjson::Value& value ) {
    if( value.IsBool() ) {
        return Value( value.GetBool() );
    }
    else if( value.IsDouble() ) {
        return Value( value.GetDouble() );
    }
    else if( value.IsInt() ) {
        return Value( value.GetInt() );
    }
    else if( value.IsString() ) {
        return Value( value.GetString() );
    }
    else if( value.IsUint() ) {
        return Value( (int)value.GetUint() );
    }
    else if( value.IsInt64() ) {
        return Value( (int)value.GetInt64() );
    }
    else if( value.IsUint64() ) {
        return Value( (int)value.GetUint64() );
    }
    else if( value.IsNumber() ) {
        return Value( (int)value.GetInt() );
    }
    else {
        log( "unknown json value type detected!" );
        return Value();
    }
}