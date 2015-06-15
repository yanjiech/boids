//
//  UICurrencyLayer.cpp
//  Boids
//
//  Created by chenyanjie on 6/9/15.
//
//

#include "UICurrencyLayer.h"
#include "../Utils.h"

using namespace cocos2d;

#define CURRENCY_LAYER_FILE "ui/page/ui_cost.csb"

UICurrencyLayer::UICurrencyLayer() {
    
}

UICurrencyLayer::~UICurrencyLayer() {
}

UICurrencyLayer* UICurrencyLayer::create() {
    UICurrencyLayer* ret = new UICurrencyLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UICurrencyLayer::init() {
    if( !Layer::init() ) {
        return false;
    }
    
    std::string file_path = FileUtils::getInstance()->fullPathForFilename( CURRENCY_LAYER_FILE );
    Node* nd_currency = CSLoader::getInstance()->createNode( file_path );
    this->addChild( nd_currency );
    
    _lb_diamond = dynamic_cast<ui::Text*>( nd_currency->getChildByName( "lb_diamond" ) );
    _lb_diamond->setAdditionalKerning( -3.0f );
    
    _lb_gold = dynamic_cast<ui::Text*>( nd_currency->getChildByName( "lb_gold" ) );
    _lb_gold->setAdditionalKerning( -3.0f );
    
    _lb_stone = dynamic_cast<ui::Text*>( nd_currency->getChildByName( "lb_stone" ) );
    _lb_stone->setAdditionalKerning( -3.0f );
    
    return true;
}

void UICurrencyLayer::onEnterTransitionDidFinish() {
    Layer::onEnterTransitionDidFinish();
    PlayerInfo::getInstance()->registerListener( PLAYER_INFO_CURRENCY, this );
    this->updatePlayerInfo( PlayerInfo::getInstance() );
}

void UICurrencyLayer::onExitTransitionDidStart() {
    Layer::onExitTransitionDidStart();
    PlayerInfo::getInstance()->unregisterListener( PLAYER_INFO_CURRENCY, this );
}

void UICurrencyLayer::updatePlayerInfo( PlayerInfo* player_info ) {
    _lb_diamond->setString( Utils::toStr( player_info->getDiamond() ) );
    _lb_gold->setString( Utils::toStr( player_info->getGold() ) );
    _lb_stone->setString( Utils::toStr( player_info->getStone() ) );
}