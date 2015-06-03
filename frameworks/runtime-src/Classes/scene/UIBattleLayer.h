//
//  UIBattleLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__UIBattleLayer__
#define __Boids__UIBattleLayer__

#include "cocos2d.h"
#include <list>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


class UnitNode;
class BattleLayer;

class UISkillNode : public cocos2d::Node {
private:
    BattleLayer* _battle_layer;
    UnitNode* _unit_node;

    cocos2d::Sprite* _sp_ready;
    cocos2d::ProgressTimer* _pb_loading;
    cocos2d::ProgressTimer* _pb_mp;
    std::string _hint_type;
    float _range;
    float _radius;
    float _min_range;
    float _max_range;
    float _max_charge_time;
    cocos2d::Point _hint_d_pos;
    
    cocos2d::Node* _hint_effect;
    
    bool _is_skill_ready;
    
public:
    UISkillNode();
    virtual ~UISkillNode();
    
    static UISkillNode* create( BattleLayer* battle_layer, UnitNode* unit_node );
    virtual bool init( BattleLayer* battle_layer, UnitNode* unit_node );
    
    UnitNode* getOwner() { return _unit_node; }
    
    void activate( const cocos2d::Point& dir, float range_per, float duration = 0 );
    
    void showHint( const cocos2d::Point& dir, float range_per );
    void hideHint();
    
    virtual void updateFrame( float delta );
    
    bool shouldCastOnTouchDown();
    
    bool isSkillReady();
    
    cocos2d::Value getSkillAttribute( const std::string& key );
    
    void setChargeTime( float time );
    
    class Skill* getSkill();
};

class UIBattleLayer : public cocos2d::Layer {
private:
    BattleLayer* _battle_layer;
    bool _is_touch_began;
    UISkillNode* _selected_skill;
    
    std::list<UISkillNode*> _skill_nodes;
    
    cocos2d::Point _touch_down_pos;
    cocos2d::Touch* _touch;
    
    float _touch_down_duration;
    
    cocos2d::Node* _root_node;
    cocostudio::timeline::ActionTimeline* _battle_panel_action;
    
    //statistics panel
    cocos2d::ui::Text* _lb_time;
    cocos2d::ui::Text* _lb_killed;
    cocos2d::ui::LoadingBar* _pb_killed;
    
    cocos2d::ui::Layout* _pn_boss_and_wave;
    //boss panel
    cocos2d::ui::Layout* _pn_boss;
    cocos2d::Node* _nd_boss_avatar;
    cocos2d::ui::Text* _lb_boss_name;
    cocos2d::ui::LoadingBar* _pb_boss_hp;
    
    //wave panel
    cocos2d::ui::Layout* _pn_wave;
    cocos2d::Node* _nd_monster_avatar;
    cocos2d::Sprite* _sp_flag;
    cocos2d::ui::LoadingBar* _pb_wave;
    
    int _need_kill;
    int _already_killed;
    
public:
    UIBattleLayer();
    virtual ~UIBattleLayer();
    
    static UIBattleLayer* create( BattleLayer* battle_layer );
    virtual bool init( BattleLayer* battle_layer );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
    
    virtual void updateFrame( float delta );
    
    void reset();
    
    void addSkillNode( UnitNode* unit );
    void removeSkillNode( UnitNode* unit );
    
    void removeAllSkillNodes();
    
    void showBossPanel();
    void hideBossPanel();
    void showWavePanel();
    void hideWavePanel();
    
    void setBossInfo( class UnitData* unit_data );
    void setBossHpPercent( float percent );
    void setWavePercent( float percent );
    
    void setNeedKill( int count );
    void setKilled( int count );
    
    void updateKillUI();
    
private:
    UISkillNode* skillNodeForTouch( cocos2d::Touch* touch );
    void alignSkillNodesWithPadding( float padding );
};

#endif /* defined(__Boids__UIBattleLayer__) */
