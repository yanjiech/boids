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
#include "spine/spine-cocos2dx.h"
#include "../unit/ElementData.h"

class UIHeroManageHeroSlot : public cocos2d::Node {
private:
    cocos2d::Sprite* _hero_avatar;
    cocos2d::Sprite* _lock_sprite;
    cocos2d::Sprite* _selected_sprite;
    
    bool _is_selected;
    bool _is_locked;
    bool _is_owned;
    
    spine::SkeletonAnimation* _hero_skeleton;
    UnitData* _hero_data;
    std::string _slot_id;
    
public:
    UIHeroManageHeroSlot();
    virtual ~UIHeroManageHeroSlot();
    
    static UIHeroManageHeroSlot* create( const cocos2d::ValueMap& data, const std::string& slot_id, int flag );
    virtual bool init( const cocos2d::ValueMap& data, const std::string& slot_id, int flag );
    
    bool isSelected();
    void setSelected( bool b );
    
    bool isLocked();
    void setLocked( bool b );
    
    bool isOwned();
    void setOwned( bool b );
    
    spine::SkeletonAnimation* getHeroSkeleton();
    void setHeroSkeleton( spine::SkeletonAnimation* skeleton );
    
    UnitData* getUnitData();
    void setUnitData( UnitData* data );
    
    cocos2d::Sprite* getHeroAvatar();
    void setHeroAvatar( cocos2d::Sprite* avatar );
    
    const std::string& getSlotId() { return _slot_id; }
    void setSlotId( const std::string& slot_id ) { _slot_id = slot_id; }
};

class UIHeroManageLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    
    cocos2d::ui::PageView* _pv_hero_list;
    
    cocos2d::ui::Text* _lb_diamond;
    cocos2d::ui::Text* _lb_gold;
    
    cocos2d::Sprite* _star_1;
    cocos2d::Sprite* _star_2;
    cocos2d::Sprite* _star_3;
    
    cocos2d::ui::Text* _lb_level;
    cocos2d::ui::Text* _lb_hp;
    cocos2d::ui::Text* _lb_mp;
    cocos2d::ui::Text* _lb_def;
    cocos2d::ui::Text* _lb_atk;
    
    cocos2d::ui::Text* _lb_hit;
    cocos2d::ui::Text* _lb_dodge;
    cocos2d::ui::Text* _lb_cri;
    cocos2d::ui::Text* _lb_ten;
    cocos2d::ui::Text* _lb_other;
    
    cocos2d::ui::Button* _btn_upgrade;
    cocos2d::ui::Button* _btn_prev;
    cocos2d::ui::Button* _btn_next;
    
    cocos2d::Node* _hero_node;
    
    cocos2d::Node* _deploy_slot_1;
    cocos2d::Node* _deploy_slot_2;
    cocos2d::Node* _deploy_slot_3;
    cocos2d::Node* _deploy_slot_4;
    cocos2d::Node* _deploy_slot_5;
    
    cocos2d::ui::Text* _lb_hero_name;
    
    UIHeroManageHeroSlot* _selected_hero;
    
public:
    UIHeroManageLayer();
    virtual ~UIHeroManageLayer();
    
    static UIHeroManageLayer* create();
    virtual bool init();
    
    void onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onDetailTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onSkillTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onUpgradeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    virtual void becomeTopLayer();
    
    void setSelectedHero( UIHeroManageHeroSlot* hero );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
};

#endif /* defined(__Boids__UIHeroManageLayer__) */
