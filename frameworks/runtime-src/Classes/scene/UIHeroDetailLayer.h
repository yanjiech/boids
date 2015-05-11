//
//  UIHeroDetailLayer.h
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#ifndef __Boids__UIHeroDetailLayer__
#define __Boids__UIHeroDetailLayer__

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TouchableLayer.h"

class UIHeroDetailLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    
public:
    UIHeroDetailLayer();
    virtual ~UIHeroDetailLayer();
    
    static UIHeroDetailLayer* create();
    virtual bool init();
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
};


#endif /* defined(__Boids__UIHeroDetailLayer__) */
