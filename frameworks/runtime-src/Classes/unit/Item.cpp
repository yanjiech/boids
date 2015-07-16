//
//  Item.cpp
//  Boids
//
//  Created by chenyanjie on 4/27/15.
//
//

#include "Item.h"
#include "UnitNode.h"
#include "BulletNode.h"
#include "../Utils.h"
#include "../scene/BattleLayer.h"
#include "../manager/ResourceManager.h"

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

void Item::removeFromUnit( class UnitNode* owner ) {
    owner->removeUnitComponent( this->getName() );
    owner->removeUnitTag( this->getName() );
}

DropItem::DropItem() {
    
}

DropItem::~DropItem() {
    
}

DropItem* DropItem::create( const cocos2d::ValueMap& item_data ) {
    DropItem* ret = new DropItem();
    if( ret && ret->init( item_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DropItem::init( const cocos2d::ValueMap& item_data ) {
    _item_id = item_data.at( "item_id" ).asString();
    _count = item_data.at( "count" ).asInt();
    _state = eItemState::ItemStateFly;
    
    std::string resource = "";
    float scale = 1.0f;
    if( _item_id == "gold" ) {
        resource = "ui_drop_jinbi.png";
    }
    else {
        const ValueMap& item_config = ResourceManager::getInstance()->getEquipConfig().at( _item_id ).asValueMap();
        std::string name = item_config.at( "name" ).asString();
        resource = name + ".png";
        scale = 0.5f;
        
//        int type = int( item_id / 1e7 );
//        int subtype = int( item_id / 1e5 ) % 10;
//        int quality = int( item_id / 1e6 ) % 10;
//        
//        if( type == 1 ) {
//            resource = "ui_drop_kuijia";
//        }
//        else if( type == 2 ) {
//            resource = "ui_drop_xie";
//        }
//        else if( type == 3 ) {
//            if( subtype == 3 ) {
//                resource = "ui_drop_jian";
//            }
//            else {
//                resource = "ui_drop_gong";
//            }
//        }
//        else if( type == 4 ) {
//            resource = "ui_drop_xianglian";
//        }
//        resource += Utils::stringFormat( "_%d.png", quality );
    }
    Sprite* icon = Sprite::createWithSpriteFrameName( resource );
    icon->setAnchorPoint( Point( 0.5f, 0.2f ) );
    icon->setScale( scale );
    this->setContentSize( icon->getContentSize() * scale * 1.2f );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    icon->setPosition( Point( this->getContentSize().width / 2, 0 ) );
    this->addChild( icon, 2 );
    
    std::string effect_resource = "effects/common/drop_item";
    _effect = ArmatureManager::getInstance()->createArmature( effect_resource );
    _effect->setPosition( Point( this->getContentSize().width / 2, 0 ) );
    this->addChild( _effect, 3 );
    _effect->setAnimation( 0, "animation", true );
    
    _should_recycle = false;
    
    return true;
}

void DropItem::dropTo( const cocos2d::Point& pos ) {
    Point old_pos = this->getPosition();
    float y = MAX( old_pos.y, pos.y );

    ccBezierConfig config;
    config.endPosition = pos;
    config.controlPoint_1 = Point( old_pos.x + ( pos.x - old_pos.x ) / 3, y + 80.0f );
    config.controlPoint_2 = Point( old_pos.x + ( pos.x - old_pos.x ) * 2 / 3, y + 60.0f );
    BezierTo* move_to = BezierTo::create( 0.5f, config );
//    MoveTo* move_to = MoveTo::create( 0.5, pos );
    CallFunc* callback = CallFunc::create( CC_CALLBACK_0( DropItem::onDropCompleted, this ) );
    Sequence* seq = Sequence::create( move_to, callback, nullptr );
    this->runAction( seq );
}

void DropItem::onDropCompleted() {
    this->setItemState( eItemState::ItemStateReady );
    BattleLayer* battle_layer = dynamic_cast<BattleLayer*>( this->getParent()->getParent()->getParent() );
    this->setLocalZOrder( battle_layer->zorderForPositionOnObjectLayer( this->getPosition() ) );
}