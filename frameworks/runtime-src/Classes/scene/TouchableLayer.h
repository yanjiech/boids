//
//  TouchableLayer.h
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#ifndef __Boids__TouchableLayer__
#define __Boids__TouchableLayer__

#include "cocos2d.h"

class TouchableLayer : public cocos2d::Layer {
protected:
    cocos2d::EventListenerTouchOneByOne* _touch_listener;
    
public:
    TouchableLayer();
    virtual ~TouchableLayer();
    
    virtual bool init();
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
    
    virtual void becomeTopLayer() {}
    
    virtual void setVisible( bool b );
};

#endif /* defined(__Boids__TouchableLayer__) */
