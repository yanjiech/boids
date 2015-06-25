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

#define MIN_DRAG_BIAS 20.0f

class UIHeroManageHeroSlot : public cocos2d::Node {
private:
    int _flag;
    
    cocos2d::Sprite* _hero_avatar;
    cocos2d::Sprite* _hero_rect_avatar;
    cocos2d::Sprite* _lock_sprite;
    cocos2d::Sprite* _selected_sprite;
    cocos2d::Sprite* _deployed_sprite;
    
    bool _is_selected;
    bool _is_locked;
    bool _is_owned;
    
    bool _is_deployed;
    
    spine::SkeletonAnimation* _hero_skeleton;
    UnitData* _hero_data;
    EquipmentData* _weapon_data;
    EquipmentData* _armor_data;
    EquipmentData* _boot_data;
    EquipmentData* _accessory_data;
    std::string _hero_id;
    
    cocos2d::ui::Scale9Sprite* _sp_hint_bg;
    
    cocos2d::Node* _root;
    
public:
    enum eHeroSlotType {
        HeroSlotTypeList = 0,
        HeroSlotTypeDeployed = 1
    };
    
    UIHeroManageHeroSlot();
    virtual ~UIHeroManageHeroSlot();
    
    static UIHeroManageHeroSlot* create( const cocos2d::ValueMap& data, const std::string& hero_id, int flag, cocos2d::Node* root );
    virtual bool init( const cocos2d::ValueMap& data, const std::string& hero_id, int flag, cocos2d::Node* root );
    
    void loadHeroInfo( const cocos2d::ValueMap& data );
    
    int getFlag();
    void setFlag( int flag );
    
    bool isSelected();
    void setSelected( bool b );
    
    bool isLocked();
    void setLocked( bool b );
    
    bool isOwned();
    void setOwned( bool b );
    
    bool isDeployed();
    void setDeployed( bool b );
    
    spine::SkeletonAnimation* getHeroSkeleton();
    void setHeroSkeleton( spine::SkeletonAnimation* skeleton );
    
    UnitData* getUnitData();
    void setUnitData( UnitData* data );
    
    cocos2d::Sprite* getHeroAvatar();
    void setHeroAvatar( cocos2d::Sprite* avatar );
    
    cocos2d::Sprite* getHeroRectAvatar();
    void setHeroRectAvatar( cocos2d::Sprite* avatar );
    
    const std::string& getHeroId() { return _hero_id; }
    void setHeroId( const std::string& hero_id ) { _hero_id = hero_id; }
    
    void showHint();
    void hideHint();
    
    void updateFrame( float delta );
    void adjustHintPos();
};

class UIHeroDeploySlot : public cocos2d::Node {
private:
    std::string _slot_id;
    std::string _hero_id;
    
    cocos2d::Sprite* _lock_sprite;
    cocos2d::Sprite* _selected_sprite;
    cocos2d::Sprite* _avatar_sprite;
    
    bool _is_locked;
    bool _is_selected;
    
public:
    UIHeroDeploySlot();
    virtual ~UIHeroDeploySlot();
    
    static UIHeroDeploySlot* create( const std::string& slot_id );
    virtual bool init( const std::string& slot_id );
    
    const std::string& getSlotId() { return _slot_id; }
    void setSlotId( const std::string& slot_id ) { _slot_id = slot_id; }
    
    const std::string& getHeroId() { return _hero_id; }
    void setHeroId( const std::string& hero_id ) { _hero_id = hero_id; }
    
    bool isLocked();
    void setLocked( bool b );
    
    bool isSelected();
    void setSelected( bool b );
    
    cocos2d::Sprite* getAvatar();
    void setAvatar( cocos2d::Sprite* avatar );
};

class UIHeroManageLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    cocostudio::timeline::ActionTimeline* _panel_action;
    
    cocos2d::ui::PageView* _pv_hero_list;
    
    cocos2d::ui::Text* _lb_price;
    cocos2d::ui::Text* _lb_upgrade_cost;
    
    cocos2d::ui::Button* _btn_skill_1;
    cocos2d::ui::Button* _btn_skill_2;
    cocos2d::ui::Button* _selected_skill_tab;
    
    cocos2d::ui::Text* _lb_skill_desc;
    
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
    bool _is_selected_hero_owned;
    UIHeroDeploySlot* _selected_deploy_slot;
    
    UIHeroDeploySlot* _target_deploy_slot;
    
    int _current_page;
    
    cocos2d::Map<std::string,UIHeroManageHeroSlot*> _hero_slots;
    cocos2d::Vector<UIHeroDeploySlot*> _deploy_slots;
    
    UIHeroManageHeroSlot* heroSlotForTouch( cocos2d::Touch* touch );
    UIHeroDeploySlot* deploySlotForTouch( cocos2d::Touch* touch );
    UIHeroDeploySlot* deploySlotIntersectsRect( cocos2d::Touch* touch, const cocos2d::Size& size );
    
    spine::SkeletonAnimation* _upgrade_effect;
    
    cocos2d::Point _touch_down_pos;
    cocos2d::Sprite* _drag_avatar;
    bool _is_touch_down;
    bool _is_dragging;
    
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
    
    void onSkillTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onPurchaseTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    virtual void becomeTopLayer();
    
    void setSelectedHero( UIHeroManageHeroSlot* hero, bool anim = true );
    void setSelectedDeploySlot( UIHeroDeploySlot* slot );
    void setTargetDeploySlot( UIHeroDeploySlot* slot );
    
    void setSkillTab( cocos2d::ui::Button* tab );
    
    void turnToPage( int index );
    
    void updatePlayerInfo();
    
    void alignHeroSlots();
    
    bool recordDeployedUnits();
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
};

#endif /* defined(__Boids__UIHeroManageLayer__) */
