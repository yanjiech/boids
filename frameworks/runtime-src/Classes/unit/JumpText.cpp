//
//  JumpText.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/16/15.
//
//

#include "JumpText.h"
#include "UnitNode.h"

using namespace cocos2d;

JumpText::JumpText() {
    
}

JumpText::~JumpText() {
    
}

JumpText* JumpText::create( const std::string& text, const std::string& type, bool is_critical, int unit_camp, const std::string& name ) {
    JumpText* ret = new JumpText();
    if( ret && ret->init( text, type, is_critical, unit_camp, name ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool JumpText::init( const std::string& text, const std::string& type, bool is_critical, int unit_camp, const std::string& name ) {
    cocos2d::Label* text_label = nullptr;
    if( type == "heal" ) {
        text_label = Label::createWithCharMap( "ui/number_heal.png", 33, 40, 43 );
    }
    else if( type == "damage" ) {
        if( unit_camp == eTargetCamp::Player ) {
            text_label = Label::createWithCharMap( "ui/number_player_damage.png", 33, 40, 43 );
        }
        else {
            text_label = Label::createWithCharMap( "ui/number_enemy_damage.png", 33, 40, 43 );
        }
    }
    if( !is_critical ) {
        text_label->setScale( 0.6f );
    }
    text_label->setString( text );
    if( !UnitNodeComponent::init( text_label, name, true ) ) {
        return false;
    }
    return true;
}

void JumpText::start( bool is_critical ) {
    if( is_critical ) {
        ScaleBy* scale = ScaleBy::create( 0.1f, 1.4f );
        ScaleTo* scale2 = ScaleTo::create( 0.02f, 1.0f );
        DelayTime* delay = DelayTime::create( 0.1f );
        FadeTo* fade = FadeTo::create( 0.5f, 0 );
        MoveBy* move_up = MoveBy::create( 0.5f, Point( 0, 100.0f ) );
        Spawn* spawn = Spawn::create( move_up, fade, nullptr );
        CallFunc* end_func = CallFunc::create( CC_CALLBACK_0( JumpText::stop, this ) );
        Sequence* seq = Sequence::create( scale, scale2, delay, spawn, end_func, nullptr );
        _node->runAction( seq );
    }
    else {
        ScaleBy* scale = ScaleBy::create( 0.15f, 1.2f );
        DelayTime* delay = DelayTime::create( 0.1f );
        FadeTo* fade = FadeTo::create( 0.5f, 0 );
        MoveBy* move_up = MoveBy::create( 0.5f, Point( 0, 100.0f ) );
        Spawn* spawn = Spawn::create( move_up, fade, nullptr );
        CallFunc* end_func = CallFunc::create( CC_CALLBACK_0( JumpText::stop, this ) );
        Sequence* seq = Sequence::create( scale, delay, spawn, end_func, nullptr );
        _node->runAction( seq );
    }
}

void JumpText::stop() {
    this->setShouldRecycle( true );
}