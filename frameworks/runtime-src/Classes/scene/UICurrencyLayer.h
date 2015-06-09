//
//  UICurrencyLayer.h
//  Boids
//
//  Created by chenyanjie on 6/9/15.
//
//

#ifndef __Boids__UICurrencyLayer__
#define __Boids__UICurrencyLayer__

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../data/PlayerInfo.h"

class UICurrencyLayer : public cocos2d::Layer, public PlayerInfoDelegate {
private:
    cocos2d::ui::Text* _lb_gold;
    cocos2d::ui::Text* _lb_diamond;
    cocos2d::ui::Text* _lb_stone;
    
public:
    UICurrencyLayer();
    virtual ~UICurrencyLayer();
    
    static UICurrencyLayer* create();
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    
    virtual void updatePlayerInfo( PlayerInfo* player_info );
};

#endif /* defined(__Boids__UICurrencyLayer__) */
