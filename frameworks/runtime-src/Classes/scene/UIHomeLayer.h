//
//  UIHomeLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__UIHomeLayer__
#define __Boids__UIHomeLayer__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MapData.h"
#include "UICurrencyLayer.h"
#include "TouchableLayer.h"
#include "UIHeroManageLayer.h"

typedef enum {
    LevelDiffEasy = 1,
    LevelDiffMedium = 2,
    LevelDiffHard = 3
}eLevelDifficulty;

class UIHomeLayer : public TouchableLayer, public PlayerInfoDelegate {
private:
    //home panel
    cocos2d::Node* _main_node;
    cocos2d::ui::ScrollView* _scrollview;
    cocos2d::Sprite* _background_node;
    
    MapData* _map_data;
    bool _is_pvp;
    
    eLevelDifficulty _difficulty;
    
    cocostudio::timeline::ActionTimeline* _panel_action;
    
    cocos2d::ui::Text* _lb_player_name;
    cocos2d::ui::Text* _lb_team_level;
    
    //level detail
    cocos2d::Node* _level_node;
    
    cocos2d::Vector<cocos2d::ui::Text*> _mission_labels;
    cocos2d::ui::Text* _lb_title;
    cocos2d::ui::Text* _lb_level_desc;
    cocos2d::Node* _nd_diff;
    cocos2d::Sprite* _sp_boss_frame;
    cocos2d::Vector<cocos2d::Sprite*> _stars;
    
    //hero layer
    UIHeroManageLayer* _hero_layer;
    
    //other
    UICurrencyLayer* _currency_layer;
    
    std::string _level_id;
    
public:
    UIHomeLayer();
    virtual ~UIHomeLayer();
    
    static UIHomeLayer* create();
    
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    
    virtual void updatePlayerInfo( PlayerInfo* player_info );
    
    UICurrencyLayer* getCurrencyLayer() { return _currency_layer; }
    
    void onStartButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onBackButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onLevelTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onStoreTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onHeroTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onTeamSkillTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    eLevelDifficulty getDifficulty();
    void setDifficulty( eLevelDifficulty diff );
    void onDifficultyTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void setMapData( MapData* map_data );
    
    void loadDeployedHeros();
    
    virtual void becomeTopLayer();
    
    void loadMapOfDifficulty( int diff );
};

#endif /* defined(__Boids__UILevelChooseLayer__) */
