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
    
public:
    UIHeroSkillLayer();
    virtual ~UIHeroSkillLayer();
    
    static UIHeroSkillLayer* create( UIHeroManageHeroSlot* hero );
    virtual bool init( UIHeroManageHeroSlot* hero );
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
};

#endif /* defined(__Boids__UIHeroSkillLayer__) */
