//
//  UIHeroSkillLayer.h
//  Boids
//
//  Created by chenyanjie on 5/19/15.
//
//

#ifndef __Boids__UIHeroSkillLayer__
#define __Boids__UIHeroSkillLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "TouchableLayer.h"

class UIHeroSkillLayer : public TouchableLayer {
public:
    UIHeroSkillLayer();
    virtual ~UIHeroSkillLayer();
    
    static UIHeroSkillLayer* create();
    virtual bool init();
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
};

#endif /* defined(__Boids__UIHeroSkillLayer__) */
