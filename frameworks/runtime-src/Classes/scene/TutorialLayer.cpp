//
//  TutorialLayer.cpp
//  Boids
//
//  Created by chenyanjie on 6/29/15.
//
//

#include "TutorialLayer.h"

using namespace cocos2d;

TutorialLayer::TutorialLayer() :
_prompt( nullptr )
{
    
}

TutorialLayer::~TutorialLayer() {
    
}

TutorialLayer* TutorialLayer::create( eTutorialType type  ) {
    TutorialLayer* ret = new TutorialLayer();
    if( ret && ret->init( type ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TutorialLayer::init( eTutorialType type  ) {
    if( !Layer::init() ) {
        return false;
    }
    
    _type = type;
    _state = eTutorialState::TutorialStateUnstarted;
    _step = 0;
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( TutorialLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( TutorialLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( TutorialLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( TutorialLayer::onTouchEnded, this );
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    if( _type == eTutorialType::TutorialPrepare ) {
        this->schedule( CC_CALLBACK_1( TutorialLayer::updateTutorial, this ), "tutorial_updatge" );
    }
    
    _touchable_rect = Rect::ZERO;
    return true;
}

bool TutorialLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    bool ret = false;
    if( _type == eTutorialType::TutorialPrepare ) {
        Point pos = this->convertTouchToNodeSpace( touch );
        bool contains_touch = _touchable_rect.containsPoint( pos );
        
        if( _state == TutorialStatePlaying ) {
            if( _touchable_rect.equals( Rect::ZERO ) ) {
                ret = true;
                this->gotoNextStep();
            }
            else if( contains_touch ) {
                ret = false;
                this->gotoNextStep();
            }
            else {
                ret = true;
            }
        }
        else if( _state == TutorialStateWaiting ) {
            ret = !contains_touch;
        }
        else {
            ret = true;
        }
    }
    return ret;
}

void TutorialLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void TutorialLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void TutorialLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void TutorialLayer::updateTutorial( float delta ) {
    switch( _state ) {
        case TutorialStateUnstarted:
            this->gotoNextStep();
            break;
        case TutorialStateWaiting:
            break;
        default:
            break;
    }
}

void TutorialLayer::onEnter() {
    Layer::onEnter();
    if( _type == eTutorialType::TutorialBattle ) {
        BattleLayer* battle_layer = dynamic_cast<BattleLayer*>( this->getParent() );
        battle_layer->getStoryLayer()->setStoryEndCallback( CC_CALLBACK_1( TutorialLayer::onStoryEnd, this ) );
        battle_layer->getUIBattleLayer()->hideSkillNodes();
    }
}

void TutorialLayer::onStoryEnd( UIStoryLayer* story ) {
    if( _type == eTutorialType::TutorialBattle ) {
        BattleLayer* battle_layer = dynamic_cast<BattleLayer*>( this->getParent() );
        battle_layer->endStory( story );
    }
}

void TutorialLayer::gotoNextStep() {
    UIHomeLayer* home_layer = dynamic_cast<UIHomeLayer*>( this->getParent() );
    if( _prompt ) {
        _prompt->removeFromParent();
        _prompt = nullptr;
    }
    ++_step;
    switch( _step ) {
        case 1:
            //touch screen to continue
            _state = TutorialStatePlaying;
            _touchable_rect = Rect::ZERO;
            break;
        case 2:
        {
            //touch hero
            _state = TutorialStateWaiting;
        }
            break;
        case 3:
            //deploy hero
            _state = TutorialStateWaiting;
            break;
        case 4:
            //touch equip
            _state = TutorialStateWaiting;
            break;
        case 5:
            //manage equip
            _state = TutorialStateWaiting;
            break;
        case 6:
            //touch back
            _state = TutorialStateWaiting;
            break;
        case 7:
            //touch skill
            break;
        case 8:
            //touch back
            break;
        case 9:
            //touch confirm
            break;
        case 10:
            //touch level
            break;
        case 11:
            //touch start
            break;
        default:
            //finished
            this->removeFromParent();
            break;
    }
}