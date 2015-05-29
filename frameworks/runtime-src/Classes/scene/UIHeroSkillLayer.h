//
//  UIHeroSkillLayer.h
//  Boids
//
//  Created by chenyanjie on 5/19/15.
//
//

#ifndef __Boids__UIHeroSkillLayer__
#define __Boids__UIHeroSkillLayer__

#include "UIHeroManageLayer.h"

class UIHeroSkillLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    UIHeroManageHeroSlot* _hero;
    
    cocos2d::ui::Text* _lb_level_1;
    cocos2d::ui::Text* _lb_level_2;
    
    cocos2d::ui::Text* _lb_upgrade_cost_1;
    cocos2d::ui::Text* _lb_upgrade_cost_2;
    
    cocos2d::ui::Text* _lb_skill_desc_1;
    cocos2d::ui::Text* _lb_skill_desc_2;
    
    cocos2d::ui::Button* _btn_upgrade_1;
    cocos2d::ui::Button* _btn_upgrade_2;
    
    cocos2d::ui::Layout* _pn_hint;
    
public:
    UIHeroSkillLayer();
    virtual ~UIHeroSkillLayer();
    
    static UIHeroSkillLayer* create( UIHeroManageHeroSlot* hero );
    virtual bool init( UIHeroManageHeroSlot* hero );
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onUpgradeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onHintTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void loadSkillData();
};

#endif /* defined(__Boids__UIHeroSkillLayer__) */
