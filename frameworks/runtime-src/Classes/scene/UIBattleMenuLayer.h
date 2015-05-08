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

class BattleLayer;

class UIBattleMenuLayer : public cocos2d::Layer {
private:
    BattleLayer* _battle_layer;
    
    cocos2d::Node* _win_panel;
    cocos2d::Node* _pause_panel;
    
    cocos2d::ui::Button* _btn_pause;
    
public:
    UIBattleMenuLayer();
    virtual ~UIBattleMenuLayer();
    
    static UIBattleMenuLayer* create( BattleLayer* battle_layer, const std::string& result );
    virtual bool init( BattleLayer* battle_layer, const std::string& result );
    
    void onHomeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onRestartTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onContinueTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void showResultPanel();
    void showPausePanel();
    
    void hideMenu();
};

#endif /* defined(__Boids__UIBattleMenuLayer__) */
