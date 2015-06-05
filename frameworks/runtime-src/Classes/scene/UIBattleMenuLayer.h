//
//  UIBattleMenuLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/5/15.
//
//

#ifndef __Boids__UIBattleMenuLayer__
#define __Boids__UIBattleMenuLayer__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "spine/spine-cocos2dx.h"

class BattleLayer;

class UIBattleMenuLayer : public cocos2d::Layer {
private:
    BattleLayer* _battle_layer;
    
    cocos2d::Node* _pause_panel;
    cocostudio::timeline::ActionTimeline* _pause_panel_action;
    
    cocos2d::ui::Button* _btn_pause;
    
    //win panel
    cocos2d::Node* _win_panel;
    cocostudio::timeline::ActionTimeline* _win_panel_action;
    spine::SkeletonAnimation* _win_effect;
    
    cocos2d::Vector<cocos2d::Sprite*> _drop_items;
    cocos2d::Vector<cocos2d::ui::Text*> _drop_items_count;
    
    //lose panel
    cocos2d::Node* _lose_panel;
    
public:
    UIBattleMenuLayer();
    virtual ~UIBattleMenuLayer();
    
    static UIBattleMenuLayer* create( BattleLayer* battle_layer );
    virtual bool init( BattleLayer* battle_layer );
    
    void onPauseTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onHomeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onRestartTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onContinueTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void showResultPanel( bool win, const cocos2d::ValueMap& result );
    void showPausePanel();
    
    void hideMenu();
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
};

#endif /* defined(__Boids__UIBattleMenuLayer__) */
