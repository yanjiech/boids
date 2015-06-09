//
//  UIBattleMenuLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/5/15.
//
//

#include "UIBattleMenuLayer.h"
#include "BattleLayer.h"
#include "../manager/SceneManager.h"
#include "../AI/Terrain.h"
#include "../manager/ResourceManager.h"
#include "../data/PlayerInfo.h"
#include "../util/CocosUtils.h"

using namespace cocos2d;
using namespace cocostudio::timeline;

#define WIN_PANEL_FILE "ui/page/ui_win_page.csb"
#define LOSE_PANEL_FILE "ui/page/ui_lose_page.csb"
#define PAUSE_PANEL_FILE "ui/page/ui_pause_page.csb"

UIBattleMenuLayer::UIBattleMenuLayer() {
    
}

UIBattleMenuLayer::~UIBattleMenuLayer() {
    ActionTimelineCache::getInstance()->removeAction( PAUSE_PANEL_FILE );
    ActionTimelineCache::getInstance()->removeAction( WIN_PANEL_FILE );
    ActionTimelineCache::getInstance()->removeAction( LOSE_PANEL_FILE );
}

UIBattleMenuLayer* UIBattleMenuLayer::create( BattleLayer* battle_layer ) {
    UIBattleMenuLayer* ret = new UIBattleMenuLayer();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleMenuLayer::init( BattleLayer* battle_layer ) {
    if( !Layer::init() ) {
        return false;
    }
    
    this->setAnchorPoint( Point::ZERO );
    this->setPosition( Point::ZERO );
    this->ignoreAnchorPointForPosition( false );
    
    Size size = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    _battle_layer = battle_layer;
    
    //win panel
    std::string win_panel_file = FileUtils::getInstance()->fullPathForFilename( WIN_PANEL_FILE );
    _win_panel = cocos2d::CSLoader::getInstance()->createNode( win_panel_file );
    this->addChild( _win_panel );
    _win_panel->setVisible( false );
    
    ui::Button* btn_confirm = dynamic_cast<ui::Button*>( _win_panel->getChildByName( "btn_ok" ) );
    btn_confirm->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onConfirmTouched, this ) );
    
    Node* win_effect_node = _win_panel->getChildByName( "nd_light" );
    _win_effect = ArmatureManager::getInstance()->createArmature( "ui/effect_win" );
    win_effect_node->addChild( _win_effect );
    
    _win_panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( win_panel_file );
    
    for( int i = 1; i <= 5; i++ ) {
        Sprite* icon_frame = dynamic_cast<Sprite*>( _win_panel->getChildByName( Utils::stringFormat( "sp_item_frame_%d", i ) ) );
        _drop_items.pushBack( icon_frame );
        
        ui::Text* lb_count = dynamic_cast<ui::Text*>( _win_panel->getChildByName( Utils::stringFormat( "lb_item_count_%d", i ) ) );
        _drop_items_count.pushBack( lb_count );
    }
    
    _sp_star_1 = dynamic_cast<Sprite*>( _win_panel->getChildByName( "star_1" ) );
    _sp_star_2 = dynamic_cast<Sprite*>( _win_panel->getChildByName( "star_2" ) );
    _sp_star_3 = dynamic_cast<Sprite*>( _win_panel->getChildByName( "star_3" ) );
    
    //lose panel
    std::string lose_panel_file = FileUtils::getInstance()->fullPathForFilename( LOSE_PANEL_FILE );
    _lose_panel = cocos2d::CSLoader::getInstance()->createNode( lose_panel_file );
    this->addChild( _lose_panel );
    _lose_panel->setVisible( false );
    
    ui::Button* btn_lose_confirm = dynamic_cast<ui::Button*>( _lose_panel->getChildByName( "btn_ok" ) );
    btn_lose_confirm->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onConfirmTouched, this ) );
    
    _lose_panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( lose_panel_file );
    
    //pause panel
    std::string pause_panel_file = FileUtils::getInstance()->fullPathForFilename( PAUSE_PANEL_FILE );
    _pause_panel = cocos2d::CSLoader::getInstance()->createNode( pause_panel_file );
    this->addChild( _pause_panel );
    _pause_panel->setVisible( false );
    _pause_panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( pause_panel_file );
    
    ui::Layout* mission_panel = dynamic_cast<ui::Layout*>( _pause_panel->getChildByName( "mission_panel" ) );
    ui::Text* lb_mission_1 = dynamic_cast<ui::Text*>( mission_panel->getChildByName( "lb_mission_1" ) );
    _lb_mission_vec.pushBack( lb_mission_1 );
    ui::Text* lb_mission_2 = dynamic_cast<ui::Text*>( mission_panel->getChildByName( "lb_mission_2" ) );
    _lb_mission_vec.pushBack( lb_mission_2 );
    ui::Text* lb_mission_3 = dynamic_cast<ui::Text*>( mission_panel->getChildByName( "lb_mission_3" ) );
    _lb_mission_vec.pushBack( lb_mission_3 );
    
    Sprite* sp_mission_1 = dynamic_cast<Sprite*>( mission_panel->getChildByName( "sp_mission_1" ) );
    sp_mission_1->setVisible( false );
    _sp_mission_vec.pushBack( sp_mission_1 );
    Sprite* sp_mission_2 = dynamic_cast<Sprite*>( mission_panel->getChildByName( "sp_mission_2" ) );
    sp_mission_2->setVisible( false );
    _sp_mission_vec.pushBack( sp_mission_2 );
    Sprite* sp_mission_3 = dynamic_cast<Sprite*>( mission_panel->getChildByName( "sp_mission_3" ) );
    sp_mission_3->setVisible( false );
    _sp_mission_vec.pushBack( sp_mission_3 );
    
    ui::Button* btn_home = dynamic_cast<ui::Button*>( _pause_panel->getChildByName( "btn_home" ) );
    btn_home->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onHomeTouched, this ) );
    
    ui::Button* btn_restart = dynamic_cast<ui::Button*>( _pause_panel->getChildByName( "btn_restart" ) );
    btn_restart->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onRestartTouched, this ) );
    
    ui::Button* btn_continue = dynamic_cast<ui::Button*>( _pause_panel->getChildByName( "btn_continue" ) );
    btn_continue->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onContinueTouched, this ) );

    _btn_pause = ui::Button::create( "ui_menu_pause_normal.png", "ui_menu_pause_touched.png", "ui_menu_pause_touched.png", cocos2d::ui::Widget::TextureResType::PLIST );
    _btn_pause->setPosition( Point( origin.x + 50.0f, origin.y + size.height - 50.0f ) );
    _btn_pause->setAnchorPoint( Point( 0, 1.0f ) );
    _btn_pause->addTouchEventListener( CC_CALLBACK_2( UIBattleMenuLayer::onPauseTouched, this ) );
    this->addChild( _btn_pause );
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( UIBattleMenuLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( UIBattleMenuLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( UIBattleMenuLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( UIBattleMenuLayer::onTouchEnded, this );
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    return true;
}

void UIBattleMenuLayer::loadTasks() {
    cocos2d::Vector<GameTask*> tasks = _battle_layer->getMapLogic()->getGameTasks();
    
    int task_count = tasks.size();
    int lb_count = _lb_mission_vec.size();
    int count = MAX( task_count, lb_count );
    
    for( int i = 0; i < count; i++ ) {
        if( i < lb_count ) {
            ui::Text* lb_mission = _lb_mission_vec.at( i );
            Sprite* sp_mission = _sp_mission_vec.at( i );
            
            if( i < task_count ) {
                GameTask* task = tasks.at( i );
                std::string state = task->getTaskState();
                
                if( state == GAME_TASK_STATE_FINISHED ) {
                    sp_mission->setVisible( true );
                    sp_mission->setSpriteFrame( "ui_pause_finish" );
                }
                else if( state == GAME_TASK_STATE_FAILED ) {
                    sp_mission->setVisible( true );
                    sp_mission->setSpriteFrame( "ui_pause_failed" );
                }
                else {
                    sp_mission->setVisible( false );
                }
            }
            else {
                lb_mission->setVisible( false );
                sp_mission->setVisible( false );
            }
        }
    }
}

void UIBattleMenuLayer::onPauseTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        this->showPausePanel();
    }
}

void UIBattleMenuLayer::onHomeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        SceneManager::getInstance()->transitToScene( eSceneName::SceneLevelChoose );
    }
}

void UIBattleMenuLayer::onRestartTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
    }
}

void UIBattleMenuLayer::onContinueTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        _pause_panel->stopAction( _pause_panel_action );
        _pause_panel->runAction( _pause_panel_action );
        _pause_panel_action->play( "disappear", false );
        _pause_panel_action->setLastFrameCallFunc( CC_CALLBACK_0( UIBattleMenuLayer::hideMenu, this ) );
    }
}

void UIBattleMenuLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        if( _did_win ) {
            int level_id = Utils::toInt( _battle_layer->getLevelId() );
            PlayerInfo::getInstance()->updateMissionRecord( level_id, _completed_mission );
            SceneManager::getInstance()->transitToScene( eSceneName::SceneLevelChoose );
        }
    }
}

void UIBattleMenuLayer::showResultPanel( bool win, const cocos2d::ValueMap& result ) {
    _btn_pause->setVisible( false );
    _pause_panel->setVisible( false );
    _did_win = win;
    if( win ) {
        _win_panel->setVisible( true );
        _lose_panel->setVisible( false );
        _win_effect->setAnimation( 0, "Idle", true );
        _win_panel->runAction( _win_panel_action );
        _win_panel_action->play( "appear", false );
        
        MapLogic* map_logic = _battle_layer->getMapLogic();
        const cocos2d::Vector<GameTask*>& tasks = map_logic->getGameTasks();
        _completed_mission = 0;
        for( auto task : tasks ) {
            if( task->getTaskState() == GAME_TASK_STATE_FINISHED ) {
                _completed_mission++;
            }
        }
        
        if( _completed_mission >= 1 ) {
            _sp_star_1->setSpriteFrame( "ui_winpage_star.png" );
        }
        else {
            _sp_star_1->setSpriteFrame( "ui_winpage_star_huise.png" );
        }
        if( _completed_mission >= 2 ) {
            _sp_star_1->setSpriteFrame( "ui_winpage_star.png" );
        }
        else {
            _sp_star_1->setSpriteFrame( "ui_winpage_star_huise.png" );
        }
        if( _completed_mission>= 3 ) {
            _sp_star_1->setSpriteFrame( "ui_winpage_star.png" );
        }
        else {
            _sp_star_1->setSpriteFrame( "ui_winpage_star_huise.png" );
        }
        
        //drop items
        int i = 0;
        for( auto pair : result ) {
            std::string key = pair.first;
            int count = pair.second.asInt();
            std::string resource;
            if( key == "gold" ) {
                resource = "ui_winpage_gold.png";
                PlayerInfo::getInstance()->gainGold( count );
            }
            else {
                const ValueMap& equip_config = ResourceManager::getInstance()->getEquipData( key );
                resource = equip_config.at( "name" ).asString() + ".png";
                PlayerInfo::getInstance()->gainEquip( key, count );
            }
            Sprite* icon = Sprite::createWithSpriteFrameName( resource );
            Sprite* frame = _drop_items.at( i );
            icon->setPosition( Point( frame->getContentSize().width / 2, frame->getContentSize().height / 2 ) );
            frame->addChild( icon );
            
            _drop_items_count.at( i )->setVisible( true );
            _drop_items_count.at( i )->setString( Utils::stringFormat( "x%d", count ) );
            i++;
        }
        for( int j = i; j < 5; j++ ) {
            _drop_items_count.at( j )->setVisible( false );
        }
    }
    else {
        _win_panel->setVisible( false );
        _lose_panel->setVisible( true );
        _lose_panel->runAction( _lose_panel_action );
        _lose_panel_action->play( "appear", false );
    }
}

void UIBattleMenuLayer::showPausePanel() {
    _btn_pause->setVisible( false );
    _win_panel->setVisible( false );
    _pause_panel->setVisible( true );
    _pause_panel_action->clearLastFrameCallFunc();
    _pause_panel->stopAction( _pause_panel_action );
    _pause_panel->runAction( _pause_panel_action );
    _pause_panel_action->play( "appear", false );
}

void UIBattleMenuLayer::hideMenu() {
    _btn_pause->setVisible( true );
    _win_panel->setVisible( false );
    _pause_panel->setVisible( false );
}

bool UIBattleMenuLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _btn_pause->isVisible() ) {
        return false;
    }
    return true;
}

void UIBattleMenuLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleMenuLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleMenuLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}