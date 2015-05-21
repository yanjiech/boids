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
#include "UIHeroManageLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class UIEquipmentCell : public cocos2d::Node {
private:
    bool _is_selected;
    bool _is_locked;
    bool _is_in_use;
    
    cocos2d::Sprite* _equip_icon;
    
public:
    UIEquipmentCell();
    virtual ~UIEquipmentCell();
    
    static UIEquipmentCell* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
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
    
    int _current_tab;
    
public:
    UIHeroDetailLayer();
    virtual ~UIHeroDetailLayer();
    
    static UIHeroDetailLayer* create( UIHeroManageHeroSlot* hero );
    virtual bool init( UIHeroManageHeroSlot* hero );
    
    void onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onSellTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    void onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void onTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type );
    
    void switchToTab( int i );
    
    void reloadHeroData();
    
    void setCurrentEquiptList( cocos2d::ui::PageView* page_view );
};


#endif /* defined(__Boids__UIHeroDetailLayer__) */
