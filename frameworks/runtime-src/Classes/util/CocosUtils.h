//
//  CocosUtils.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__CocosUtils__
#define __Boids__CocosUtils__

#include "cocos2d.h"
#include "../Utils.h"
#include "external/json/Document.h"

class CocosUtils {
private:
    static int _global_boids_event_action_id;
    
public:
    static const rapidjson::Value& NULL_RAPIDJSON_VALUE;
    
    static std::string getNextGlobalBoidsEventActionId();
    
    static cocos2d::ValueMap jsonObjectToValueMap( const rapidjson::Value& value );
    static cocos2d::ValueVector jsonArrayToValueVector( const rapidjson::Value& value );
    static cocos2d::Value jsonValueToValue( const rapidjson::Value& value );
};

#endif /* defined(__Boids__CocosUtils__) */
