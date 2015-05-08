//
//  HpBar.h
//  Boids
//
//  Created by Yanjie Chen on 3/16/15.
//
//

#ifndef __Boids__HpBar__
#define __Boids__HpBar__

#include "cocos2d.h"

class HpBar : public cocos2d::Node {
private:
    cocos2d::ProgressTimer* _background;
    cocos2d::ProgressTimer* _red_bar;
    cocos2d::ProgressTimer* _yellow_bar;
    cocos2d::ProgressTimer* _green_bar;
    
    float _percentage;
    
private:
    cocos2d::ProgressTimer* addBar( const cocos2d::Color4F& color, const cocos2d::Size& size, const std::string& texture_name );
    
    void updateVisible();
    
public:
    HpBar();
    virtual ~HpBar();
    
    static HpBar* create( const cocos2d::Size& size );
    virtual bool init( const cocos2d::Size& size );
    
    void setPercentage( float percentage );
    float getPercentage();
};

#endif /* defined(__Boids__HpBar__) */
