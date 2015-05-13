//
//  UIStoryLayer.cpp
//  Boids
//
//  Created by chenyanjie on 4/13/15.
//
//

#include "UIStoryLayer.h"

#define DEFAULT_LETTERS_PER_ROW 60
#define DEFAULT_INTERVAL 0.1f
#define DEFAULT_WAIT_INTERVAL 5.0f

using namespace cocos2d;

UIStoryLayer::UIStoryLayer() :
_sp_speaker( nullptr ),
_is_enabled( false ),
_current_line_index( 0 ),
_current_letter_pos( 0 ),
_interval( 0 ),
_callback( nullptr )
{
    
}

UIStoryLayer::~UIStoryLayer() {
    
}

UIStoryLayer* UIStoryLayer::create( const StoryEndCallback& callback ) {
    UIStoryLayer* ret = new UIStoryLayer();
    if( ret && ret->init( callback ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIStoryLayer::init( const StoryEndCallback& callback ) {
    if( !Layer::init() ) {
        return false;
    }

    this->reset();
    this->setStoryEndCallback( callback );
    
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    
    Sprite* forest_bg = Sprite::create( "ui/dialog/duihuakuang_forest.png" );
    forest_bg->setPosition( Point( origin.x + size.width / 2, origin.y + forest_bg->getContentSize().height / 2 ) );
    this->addChild( forest_bg, 1 );
    
    Sprite* dialog_frame = Sprite::create( "ui/dialog/duihuakuang_01.png" );
    dialog_frame->setPosition( Point( origin.x + size.width / 2, origin.y + dialog_frame->getContentSize().height / 2 + 50.0 ) );
    this->addChild( dialog_frame, 2 );
    
    _lb_line = Label::createWithSystemFont( "", "Arial", 32.0f );
    _lb_line->setColor( Color3B::BLACK );
    _lb_line->setAnchorPoint( Point( 0.5f, 0 ) );
    _lb_line->setVerticalAlignment( cocos2d::TextVAlignment::TOP );
    _lb_line->setHorizontalAlignment( cocos2d::TextHAlignment::LEFT );
    _lb_line->setDimensions( dialog_frame->getContentSize().width - 400.0f, dialog_frame->getContentSize().height - 100.0f );
    _lb_line->setString( "" );
    _lb_line->setPosition( Point( origin.x + size.width / 2, origin.y + 100.0f ) );
    this->addChild( _lb_line, 3 );
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( UIStoryLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( UIStoryLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( UIStoryLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( UIStoryLayer::onTouchEnded, this );
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    return true;
}

bool UIStoryLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* unused_event ) {
    if( _is_enabled ) {
        return true;
    }
    return false;
}

void UIStoryLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* unused_event ) {
    
}

void UIStoryLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* unused_event ) {
    
}

void UIStoryLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* unused_event ) {
    if( _is_enabled ) {
        if( _interval > DEFAULT_INTERVAL ) {
            _elapse = 0;
            _interval = DEFAULT_INTERVAL;
            this->gotoNextLine();
        }
        else {
            this->showLetters( INT_MAX );
        }
    }
}

void UIStoryLayer::updateFrame( float delta ) {
    if( _is_enabled ) {
        _elapse += delta;
        if( _elapse > _interval ) {
            _elapse = 0;
            this->showLetters( 1 );
        }
    }
}

void UIStoryLayer::reset() {
    _current_letter_pos = 0;
    _current_line_index = 0;
    _elapse = 0;
    _interval = DEFAULT_INTERVAL;
    _max_letters_per_row = DEFAULT_LETTERS_PER_ROW;
}

void UIStoryLayer::showLetters( int count ) {
    if( _interval > DEFAULT_INTERVAL ) {
        _interval = DEFAULT_INTERVAL;
        if( !this->gotoNextLine() ) {
            return;
        }
    }
    std::string displayed_line = _lb_line->getString();
    int append_letter_count =  MIN( (int)_current_line.size() - _current_letter_pos, count );
    
    while( append_letter_count > 0 ) {
        int step_append_count = _max_letters_per_row - ( _current_letter_pos + 1 ) % _max_letters_per_row;
        step_append_count = MIN( append_letter_count, step_append_count );
        std::u16string append_u16_str = _current_line.substr( _current_letter_pos, step_append_count );
        std::string append_u8_str;
        StringUtils::UTF16ToUTF8( append_u16_str, append_u8_str );
        displayed_line.append( append_u8_str );
        
        _current_letter_pos += step_append_count;
        append_letter_count -= step_append_count;
        
        if( ( _current_letter_pos + 1 ) % _max_letters_per_row == 0 ) {
            displayed_line.append( "\r\n" );
        }
    }
    _lb_line->setString( displayed_line );
    
    if( _current_letter_pos >= _current_line.size() ) {
        this->setInterval( DEFAULT_WAIT_INTERVAL );
        ++_current_line_index;
        _current_letter_pos = 0;
    }
}

bool UIStoryLayer::isEnabled() {
    return _is_enabled;
}

void UIStoryLayer::setEnabled( bool b ) {
    _is_enabled = b;
}

float UIStoryLayer::getInterval() {
    return _interval;
}

void UIStoryLayer::setInterval( float interval ) {
    _interval = interval;
}

int UIStoryLayer::getMaxLettersPerRow() {
    return _max_letters_per_row;
}

void UIStoryLayer::setMaxLettersPerRow( int count ) {
    _max_letters_per_row = count;
}

void UIStoryLayer::setStoryData( const cocos2d::ValueMap& data ) {
    _story_name = data.at( "name" ).asString();
    _story_data = data.at( "story_data" ).asValueVector();
    this->reset();
    this->gotoNextLine();
}

void UIStoryLayer::setStoryEndCallback( const StoryEndCallback& callback ) {
    _callback = callback;
}

void UIStoryLayer::setSpeaker( const std::string& name, bool left_or_right ) {
    std::string speaker_resource = "ui/dialog/" + name + ".png";
    if( _sp_speaker ) {
        _sp_speaker->removeFromParentAndCleanup( true );
    }
    _sp_speaker = Sprite::create( speaker_resource );
    this->addChild( _sp_speaker, 0 );
    
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    if( left_or_right ) {
        _sp_speaker->setAnchorPoint( Point( 0, 0 ) );
        _sp_speaker->setPosition( Point( origin.x + 50.0f, 0 ) );
        _sp_speaker->setFlippedX( true );
    }
    else {
        _sp_speaker->setAnchorPoint( Point( 1.0f, 0 ) );
        _sp_speaker->setPosition( Point( origin.x + size.width - 50.0f, 0 ) );
        _sp_speaker->setFlippedX( false );
    }
}

//private methods
bool UIStoryLayer::gotoNextLine() {
    _lb_line->setString( "" );
    if( _current_line_index >= _story_data.size() ) {
        this->setEnabled( false );
        if( _callback ) {
            _callback( this );
        }
        return false;
    }
    else {
        const cocos2d::ValueMap& data = _story_data.at( _current_line_index ).asValueMap();
        std::string next_line = data.at( "line" ).asString();
        StringUtils::UTF8ToUTF16( next_line, _current_line );
        
        std::string speaker = data.at( "speaker" ).asString();
        bool left_or_right = data.at( "left_or_right" ).asBool();
        this->setSpeaker( speaker, left_or_right );
        
        return true;
    }
}