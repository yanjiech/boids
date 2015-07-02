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
    
    cocos2d::Vector<cocos2d::ui::Text*> _lb_mission_vec;
    
    cocos2d::Vector<cocos2d::Sprite*> _sp_mission_vec;
    
    cocostudio::timeline::ActionTimeline* _pause_panel_action;
    
    cocos2d::ui::Button* _btn_pause;
    
    //win panel
    cocos2d::Node* _win_panel;
    cocostudio::timeline::ActionTimeline* _win_panel_action;
    spine::SkeletonAnimation* _win_effect;
    
    cocos2d::ui::Text* _lb_team_level;
    cocos2d::ui::LoadingBar* _pb_team_exp;
    
    cocos2d::Vector<cocos2d::Sprite*> _drop_items;
    cocos2d::Vector<cocos2d::ui::Text*> _drop_items_count;
    
    spine::SkeletonAnimation* _level_up_effect;
    
    int _current_level;
    int _target_level;
    float _target_percent;
    
    //lose panel
    cocos2d::Node* _lose_panel;
    cocostudio::timeline::ActionTimeline* _lose_panel_action;
    
    int _completed_mission;
    bool _did_win;
    
    cocos2d::Sprite* _sp_star_1;
    cocos2d::Sprite* _sp_star_2;
    cocos2d::Sprite* _sp_star_3;
    
public:
    UIBattleMenuLayer();
    virtual ~UIBattleMenuLayer();
    
    static UIBattleMenuLayer* create( BattleLayer* battle_layer );
    virtual bool init( BattleLayer* battle_layer );
    
    void loadTasks();
    
    void onPauseTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onHomeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onRestartTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onContinueTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void showResultPanel( bool win, const cocos2d::ValueMap& result );
    void showPausePanel();
    
    void updateExpBar( float delta );
    
    void hideMenu();
    void hidePauseButton();
    void removePauseButton();
    
    void onTaskChanged( int i, bool succ );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
};

#endif /* defined(__Boids__UIBattleMenuLayer__) */
