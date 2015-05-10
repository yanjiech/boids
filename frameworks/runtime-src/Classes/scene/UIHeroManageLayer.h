//
//  UIHeroManageLayer.h
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#ifndef __Boids__UIHeroManageLayer__
#define __Boids__UIHeroManageLayer__

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TouchableLayer.h"

class UIHeroManageLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    
public:
    UIHeroManageLayer();
    virtual ~UIHeroManageLayer();
    
    static UIHeroManageLayer* create();
    virtual bool init();
    
    void onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onDetailTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
};

#endif /* defined(__Boids__UIHeroManageLayer__) */
