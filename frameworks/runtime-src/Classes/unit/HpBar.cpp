//
//  HpBar.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/16/15.
//
//

#include "HpBar.h"

using namespace cocos2d;

#define BACKGROUND_TEXTURE_NAME "HpBar_backgroundTextureName"
#define GREEN_TEXTUER_NAME "HpBar_greenTextureName"
#define YELLOW_TEXTURE_NAME "HpBar_yellowTextureName"
#define RED_TEXTURE_NAME "HpBar_redTextureName"

#define RED_PERCENTAGE 20.0
#define YELLOW_PERCENTAGE 50.0

#define H_MARGIN 1.0f
#define V_MARGIN 1.0f

HpBar::HpBar() {
    
}

HpBar::~HpBar() {
    
}

HpBar* HpBar::create( const cocos2d::Size& size ) {
    HpBar* ret = new HpBar();
    if( ret && ret->init( size ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool HpBar::init( const cocos2d::Size& size ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->ignoreAnchorPointForPosition( false );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    this->setContentSize( size );
    
    Size inner_size = Size( size.width - 2 * H_MARGIN, size.height - 2 * V_MARGIN );
    
    _background = this->addBar( Color4F( 0, 0, 0, 1.0f ), size, BACKGROUND_TEXTURE_NAME );
    _red_bar = this->addBar( Color4F( 1.0f, 0, 0, 1.0f ), inner_size, RED_TEXTURE_NAME );
    _yellow_bar = this->addBar( Color4F( 1.0f, 1.0f, 0, 1.0f ), inner_size, YELLOW_TEXTURE_NAME );
    _green_bar = this->addBar( Color4F( 0, 1.0f, 0.1843f, 1.0f ), inner_size, GREEN_TEXTUER_NAME );
    
    _percentage = 100.0f;
    this->updateVisible();
    
    return true;
}

void HpBar::setPercentage( float percentage ) {
    if( percentage != _percentage ) {
        _percentage = percentage;
        _yellow_bar->setPercentage( percentage );
        _green_bar->setPercentage( percentage );
        _red_bar->setPercentage( percentage );
        this->updateVisible();
    }
}

float HpBar::getPercentage() {
    return _percentage;
}

void HpBar::updateVisible() {
    if( _percentage <= RED_PERCENTAGE ) {
        _red_bar->setVisible( true );
        _yellow_bar->setVisible( false );
        _green_bar->setVisible( false );
    }
    else if( _percentage <= YELLOW_PERCENTAGE ) {
        _red_bar->setVisible( false );
        _yellow_bar->setVisible( true );
        _green_bar->setVisible( false );
    }
    else {
        _red_bar->setVisible( false );
        _yellow_bar->setVisible( false );
        _green_bar->setVisible( true );
    }
}

//private method
ProgressTimer* HpBar::addBar( const cocos2d::Color4F& color, const cocos2d::Size& size, const std::string& texture_name ) {
    cocos2d::ProgressTimer* bar = nullptr;
    RenderTexture* texture = RenderTexture::create( 1.0f, 1.0f );
    texture->beginWithClear( color.r, color.g, color.b, color.a );
    texture->end();
    Sprite* sp = Sprite::createWithTexture( texture->getSprite()->getTexture() );
    bar = ProgressTimer::create( sp );
    bar->setType( cocos2d::ProgressTimer::Type::BAR );
    bar->setMidpoint( Point( 0, 0.5f ) );
    bar->setBarChangeRate( Point( 1.0f, 0 ) );
    bar->setPercentage( 100.0f );
    bar->setPosition( Point::ZERO );
    bar->setScale( size.width, size.height );
    this->addChild( bar );
    return bar;
}