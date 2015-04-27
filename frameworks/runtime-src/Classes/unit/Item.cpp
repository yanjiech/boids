//
//  Item.cpp
//  Boids
//
//  Created by chenyanjie on 4/27/15.
//
//

#include "Item.h"
#include "BulletNode.h"

using namespace cocos2d;

Item::Item() {
    
}

Item::~Item() {
    
}

Item* Item::create( const cocos2d::ValueMap& item_data ) {
    Item* ret = new Item();
    if( ret && ret->init( item_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Item::init( const cocos2d::ValueMap& item_data ) {
    _item_data = item_data;
    _item_id = BulletNode::getNextBulletId();
    return true;
}

std::string Item::getName() {
    return _item_data["item_name"].asString();
}

std::string Item::getResource() {
    return _item_data["item_resource"].asString();
}