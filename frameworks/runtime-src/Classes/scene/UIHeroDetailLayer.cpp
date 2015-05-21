//
//  UIHeroDetailLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#include "UIHeroDetailLayer.h"
#include "../Utils.h"

using namespace cocos2d;

#define HERO_DETAIL_FILE "ui/page/ui_hero_detail.csb"

UIHeroDetailLayer::UIHeroDetailLayer() :
_current_equip_list( nullptr )
{
    
}

UIHeroDetailLayer::~UIHeroDetailLayer() {
}

UIHeroDetailLayer* UIHeroDetailLayer::create( UIHeroManageHeroSlot* hero ) {
    UIHeroDetailLayer* ret = new UIHeroDetailLayer();
    if( ret && ret->init( hero ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroDetailLayer::init( UIHeroManageHeroSlot* hero ) {
    if( !TouchableLayer::init() ) {
        return false;
    }
    _hero = hero;
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_DETAIL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_back" ) );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onBackTouched, this ) );
    
    Node* status_panel = _root_node->getChildByName( "statusPanel" );
    
    Node* avatar_container = status_panel->getChildByName( "nd_hero_avatar" );
    std::string avatar_resource = "ui_hero_p_" + hero->getUnitData()->name + ".png";
    Sprite* hero_avatar = Sprite::createWithSpriteFrameName( avatar_resource );
    avatar_container->addChild( hero_avatar );
    
    _lb_hero_name = dynamic_cast<ui::Text*>( status_panel->getChildByName( "heroNameText" ) );
    _lb_hero_level = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_level" ) );
    
    _lb_hp = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_hp" ) );
    _lb_mp = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_mp" ) );
    _lb_def = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_def" ) );
    _lb_atk = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_atk" ) );
    
    _lb_hit = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_hit" ) );
    _lb_dod = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_dod" ) );
    _lb_cri = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_cri" ) );
    _lb_ten = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_ten" ) );
    _lb_other = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_other" ) );
    
    _lb_hp_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_hp" ) );
    _lb_mp_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_mp" ) );
    _lb_def_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_def" ) );
    _lb_atk_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_atk" ) );
    
    _lb_hit_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_hit" ) );
    _lb_dod_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_dod" ) );
    _lb_cri_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_cri" ) );
    _lb_ten_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_ten" ) );
    _lb_other_extra = dynamic_cast<ui::Text*>( status_panel->getChildByName( "lb_extra_other" ) );

    Node* equip_panel = _root_node->getChildByName( "equipPanel" );
    
    Node* weapon_node = equip_panel->getChildByName( "nd_weapon" );
    
    Node* armor_node = equip_panel->getChildByName( "nd_armor" );
    
    Node* boot_node = equip_panel->getChildByName( "nd_boot" );
    
    Node* accessory_node = equip_panel->getChildByName( "nd_accessory" );
    
    _btn_prev = dynamic_cast<ui::Button*>( equip_panel->getChildByName( "leftButton" ) );
    _btn_next = dynamic_cast<ui::Button*>( equip_panel->getChildByName( "rightButton" ) );
    
    _pv_weapon_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_weapon_list" ) );
    _pv_armor_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_armor_list" ) );
    _pv_boot_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_boot_list" ) );
    _pv_accessory_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_accessory_list" ) );
    
    _pn_weapon_tab = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "weaponPanel" ) );
    _pn_armor_tab = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "armourPanel" ) );
    _pn_boot_tab = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "shoesPanel" ) );
    _pn_accessory_tab = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "accessoryPanel" ) );
    
    _btn_weapon_tab = dynamic_cast<ui::Button*>( _pn_weapon_tab->getChildByName( "btn_weapon" ) );
    _btn_weapon_tab->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onTabTouched, this ) );
    
    _btn_armor_tab = dynamic_cast<ui::Button*>( _pn_armor_tab->getChildByName( "btn_armor" ) );
    _btn_armor_tab->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onTabTouched, this ) );
    
    _btn_boot_tab = dynamic_cast<ui::Button*>( _pn_boot_tab->getChildByName( "btn_boot" ) );
    _btn_boot_tab->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onTabTouched, this ) );
    
    _btn_accessory_tab = dynamic_cast<ui::Button*>( _pn_accessory_tab->getChildByName( "btn_accessory" ) );
    _btn_accessory_tab->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onTabTouched, this ) );
    
    this->reloadHeroData();
    
    _current_tab = 0;
    this->switchToTab( 1 );
    
    return true;
}

void UIHeroDetailLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}

void UIHeroDetailLayer::onSellTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
    }
}

void UIHeroDetailLayer::onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
    }
}

void UIHeroDetailLayer::onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {}
}

void UIHeroDetailLayer::onTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        ui::Button* tab = dynamic_cast<ui::Button*>( sender );
        if( tab ) {
            this->switchToTab( tab->getTag() );
        }
    }
}

void UIHeroDetailLayer::switchToTab( int i ) {
    if( i != _current_tab ) {
        switch( _current_tab ) {
            case 1:
            {
                _btn_weapon_tab->switchSpriteFrames();
                _pv_weapon_list->setVisible( false );
                _pn_weapon_tab->setPosition( Point( _pn_weapon_tab->getPosition().x - 20, _pn_weapon_tab->getPosition().y ) );
                break;
            }
            case 2:
            {
                _btn_armor_tab->switchSpriteFrames();
                _pv_armor_list->setVisible( false );
                _pn_armor_tab->setPosition( Point( _pn_armor_tab->getPosition().x - 20, _pn_armor_tab->getPosition().y ) );
                break;
            }
            case 3:
            {
                _btn_boot_tab->switchSpriteFrames();
                _pv_boot_list->setVisible( false );
                _pn_boot_tab->setPosition( Point( _pn_boot_tab->getPosition().x - 20, _pn_boot_tab->getPosition().y ) );
                break;
            }
            case 4:
            {
                _btn_accessory_tab->switchSpriteFrames();
                _pv_accessory_list->setVisible( false );
                _pn_accessory_tab->setPosition( Point( _pn_accessory_tab->getPosition().x - 20, _pn_accessory_tab->getPosition().y ) );
                break;
            }
            default:
                break;
        }
        this->stopAllActions();
        switch( i ) {
            case 1:
            {
                _btn_weapon_tab->switchSpriteFrames();
                _pv_weapon_list->setVisible( true );
                _pn_weapon_tab->setPosition( Point( _pn_weapon_tab->getPosition().x + 20, _pn_weapon_tab->getPosition().y ) );
                break;
            }
            case 2:
            {
                _btn_armor_tab->switchSpriteFrames();
                _pv_armor_list->setVisible( true );
                _pn_armor_tab->setPosition( Point( _pn_armor_tab->getPosition().x + 20, _pn_armor_tab->getPosition().y ) );
                break;
            }
            case 3:
            {
                _btn_boot_tab->switchSpriteFrames();
                _pv_boot_list->setVisible( true );
                _pn_boot_tab->setPosition( Point( _pn_boot_tab->getPosition().x + 20, _pn_boot_tab->getPosition().y ) );
                break;
            }
            case 4:
            {
                _btn_accessory_tab->switchSpriteFrames();
                _pv_accessory_list->setVisible( true );
                _pn_accessory_tab->setPosition( Point( _pn_accessory_tab->getPosition().x + 20, _pn_accessory_tab->getPosition().y ) );
                break;
            }
            default:
                break;
        }
        _current_tab = i;
    }
}

void UIHeroDetailLayer::reloadHeroData() {
    UnitData* data = _hero->getUnitData();
    _lb_hero_level->setString( Utils::toStr( data->level ) );
    _lb_hero_name->setString( data->display_name );
    
    _lb_hp->setString( Utils::toStr( (int)data->hp ) );
    _lb_mp->setString( Utils::toStr( (int)data->mp ) );
    _lb_def->setString( Utils::toStr( (int)data->def ) );
    _lb_atk->setString( Utils::toStr( (int)data->atk ) );
    
    _lb_hit->setString( Utils::toStr( (int)data->hit ) );
    _lb_dod->setString( Utils::toStr( (int)data->dodge ) );
    _lb_cri->setString( Utils::toStr( (int)data->critical ) );
    _lb_ten->setString( Utils::toStr( (int)data->tenacity ) );
    _lb_other->setString( Utils::toStr( (int)data->atk_range ) );
}

void UIHeroDetailLayer::setCurrentEquiptList( cocos2d::ui::PageView* page_view ) {
    if( _current_equip_list != nullptr ) {
        _current_equip_list->setVisible( false );
    }
    _current_equip_list = page_view;
    if( _current_equip_list ) {
        _current_equip_list->setVisible( true );
        int total_pages = _current_equip_list->getPages().size();
        int current_page = _current_equip_list->getCurPageIndex();
        _btn_prev->setVisible( current_page > 0 );
        _btn_next->setVisible( current_page < total_pages - 1 );
    }
    else {
        _btn_prev->setVisible( false );
        _btn_next->setVisible( false );
    }
}