//
//  UITeamTalentLayer.h
//  Boids
//
//  Created by chenyanjie on 5/26/15.
//
//

#ifndef __Boids__UITeamTalentLayer__
#define __Boids__UITeamTalentLayer__

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TouchableLayer.h"

struct TalentState {
    bool is_selected;
    bool is_enabled;
    int cost;
};

class UITeamTalentLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    
    cocos2d::ui::Text* _lb_total_points;
    
    cocos2d::ui::ImageView* _pn_hint;
    
    int _selected_tab;
    
    TalentState _talent_states[12];
    int _total_points;
    int _total_used_points[3];
    
    cocos2d::Vector<cocos2d::ui::Text*> _lb_used_points;
    
public:
    UITeamTalentLayer();
    virtual ~UITeamTalentLayer();
    
    static UITeamTalentLayer* create();
    virtual bool init();
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onResetTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onTalentNodeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void switchToTab( int i );
    
    void reloadTabContent( int i );
    void updateDisplayedContent();
    
    void showHint( int i );
    void hideHint();
    
    void investTalentPoint( cocos2d::ui::Layout* talent );
    void resetCurrentInvest();
    void resetAllInvest();
    void recordTalentPoints();
    
private:
    bool isTalentSelected( cocos2d::ui::Layout* talent );
    void setTalentSelected( cocos2d::ui::Layout* talent, bool b );
    
    bool isTalentEnabled( cocos2d::ui::Layout* talent );
    void setTalentEnabled( cocos2d::ui::Layout* talent, bool b );
};

#endif /* defined(__Boids__UITeamTalentLayer__) */
