//
//  UIHeroDetailLayer.h
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#ifndef __Boids__UIHeroDetailLayer__
#define __Boids__UIHeroDetailLayer__

#include "UIHeroManageLayer.h"

class UIEquipmentCell : public cocos2d::Node {
private:
    EquipmentData* _equip_data;
    
    bool _is_selected;
    
    cocos2d::Sprite* _selected_sprite;
    cocos2d::Sprite* _equip_sprite;
    
public:
    UIEquipmentCell();
    virtual ~UIEquipmentCell();
    
    static UIEquipmentCell* create( EquipmentData* equip_data, const std::string& bg_sprite_name = "" );
    virtual bool init( EquipmentData* equip_data, const std::string& bg_sprite_name = "" );
    
    static UIEquipmentCell* create( const cocos2d::ValueMap& data, const std::string& bg_sprite_name = "" );
    virtual bool init( const cocos2d::ValueMap& data, const std::string& bg_sprite_name = "" );
    
    EquipmentData* getEquipData();
    void setEquipData( EquipmentData* data );
    
    cocos2d::Sprite* getEquipSprite();
    void setEquipSprite( cocos2d::Sprite* icon );
    
    bool isSelected();
    void setSelected( bool b );
};

class UIHeroDetailLayer : public TouchableLayer {
private:
    cocos2d::Node* _root_node;
    UIHeroManageHeroSlot* _hero;
    
    cocos2d::ui::Text* _lb_hero_name;
    cocos2d::ui::Text* _lb_hero_level;
    
    cocos2d::ui::Text* _lb_hp;
    cocos2d::ui::Text* _lb_mp;
    cocos2d::ui::Text* _lb_def;
    cocos2d::ui::Text* _lb_atk;
    
    cocos2d::ui::Text* _lb_hit;
    cocos2d::ui::Text* _lb_dod;
    cocos2d::ui::Text* _lb_cri;
    cocos2d::ui::Text* _lb_ten;
    cocos2d::ui::Text* _lb_other;
    
    cocos2d::ui::Text* _lb_hp_extra;
    cocos2d::ui::Text* _lb_mp_extra;
    cocos2d::ui::Text* _lb_def_extra;
    cocos2d::ui::Text* _lb_atk_extra;
    
    cocos2d::ui::Text* _lb_hit_extra;
    cocos2d::ui::Text* _lb_dod_extra;
    cocos2d::ui::Text* _lb_cri_extra;
    cocos2d::ui::Text* _lb_ten_extra;
    cocos2d::ui::Text* _lb_other_extra;
    
    cocos2d::Vector<UIEquipmentCell*> _weapon_list;
    cocos2d::Vector<UIEquipmentCell*> _armor_list;
    cocos2d::Vector<UIEquipmentCell*> _boot_list;
    cocos2d::Vector<UIEquipmentCell*> _accessory_list;
    
    UIEquipmentCell* _weapon_cell;
    UIEquipmentCell* _armor_cell;
    UIEquipmentCell* _boot_cell;
    UIEquipmentCell* _accessory_cell;
    
    UIEquipmentCell* _selected_equiped_cell;
    UIEquipmentCell* _selected_repo_cell;
    
    UIEquipmentCell* _target_equiped_cell;
    
    bool _is_touch_down;
    bool _is_dragging;
    cocos2d::Point _touch_down_pos;
    
    cocos2d::Sprite* _drag_equip;
    
    cocos2d::ui::Button* _btn_prev;
    cocos2d::ui::Button* _btn_next;
    
    cocos2d::ui::PageView* _pv_weapon_list;
    cocos2d::ui::PageView* _pv_armor_list;
    cocos2d::ui::PageView* _pv_boot_list;
    cocos2d::ui::PageView* _pv_accessory_list;
    
    cocos2d::ui::PageView* _current_equip_list;
    
    cocos2d::ui::Button* _btn_weapon_tab;
    cocos2d::ui::Button* _btn_armor_tab;
    cocos2d::ui::Button* _btn_boot_tab;
    cocos2d::ui::Button* _btn_accessory_tab;
    
    cocos2d::ui::Layout* _pn_weapon_tab;
    cocos2d::ui::Layout* _pn_armor_tab;
    cocos2d::ui::Layout* _pn_boot_tab;
    cocos2d::ui::Layout* _pn_accessory_tab;
    
    cocos2d::ui::Layout* _pn_equip_area;
    
    int _current_tab;
    
    UIEquipmentCell* equipedCellForTouch( cocos2d::Touch* touch );
    UIEquipmentCell* repoCellForTouch( cocos2d::Touch* touch );
    
    void checkTargetCell();
    
    //sell panel
    cocos2d::Node* _pn_sell;
    cocos2d::ui::Text* _lb_gold;
    cocos2d::Sprite* _sp_icon_frame;
    
    
public:
    UIHeroDetailLayer();
    virtual ~UIHeroDetailLayer();
    
    static UIHeroDetailLayer* create( UIHeroManageHeroSlot* hero );
    virtual bool init( UIHeroManageHeroSlot* hero );
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onSellTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onSellConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onSellCancelTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void switchToTab( int i );
    
    void reloadHeroData();
    
    bool loadRepoEquips( eEquipType type, int page_index );
    
    void setCurrentEquiptList( cocos2d::ui::PageView* page_view );
    
    void setSelectedEquipedCell( UIEquipmentCell* cell );
    void setSelectedRepoCell( UIEquipmentCell* cell );
    void setTargetEquipedCell( UIEquipmentCell* cell );
    
    void updateCompareInfo();
    void updateExtraLabel( cocos2d::ui::Text* label, int value );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
};


#endif /* defined(__Boids__UIHeroDetailLayer__) */
