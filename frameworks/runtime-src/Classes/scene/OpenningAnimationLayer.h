//
//  OpenningAnimationLayer.h
//  Boids
//
//  Created by chenyanjie on 6/10/15.
//
//

#ifndef __Boids__OpenningAnimationLayer__
#define __Boids__OpenningAnimationLayer__

#include "TouchableLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class OpenningAnimationLayer : public TouchableLayer {
    cocos2d::ui::Button* _btn_next;
    cocos2d::Sprite* _sp_next;
    cocos2d::ui::Button* _btn_enter;
    cocos2d::ui::Button* _btn_skip;
    cocos2d::Node* _root_node;
    cocostudio::timeline::ActionTimeline* _panel_action;
    
    int _current_shot;
    int _total_shot;
    
public:
    OpenningAnimationLayer();
    virtual ~OpenningAnimationLayer();
    
    static OpenningAnimationLayer* create();
    virtual bool init();
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
    
    void onSkipTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onEnterTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void start( float delta );
    void end();
    
    virtual void onEnterTransitionDidFinish();
    
    void onAnimationComplete();
};

#endif /* defined(__Boids__OpenningAnimationLayer__) */
