//
//  UIHeroDetailLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#include "UIHeroDetailLayer.h"
#include "../Utils.h"
#include "../data/PlayerInfo.h"
#include "../manager/ResourceManager.h"
#include "../manager/AudioManager.h"
#include "../util/CocosUtils.h"

using namespace cocos2d;

#define HERO_DETAIL_FILE "ui/page/ui_hero_detail.csb"
#define SELL_PANEL_FILE "ui/page/ui_sell.csb"

#define REPO_CELL_WIDTH 
#define REPO_CELL_HEIGHT

#define EQUIP_CELL_WIDTH 158
#define EQUIP_CELL_HEIGHT 144

//equip cell
UIEquipmentCell::UIEquipmentCell() :
_equip_data( nullptr ),
_equip_sprite( nullptr ),
_placeholder( nullptr )
{
    
}

UIEquipmentCell::~UIEquipmentCell() {
    
}

UIEquipmentCell* UIEquipmentCell::create( EquipmentData* equip_data, cocos2d::Sprite* placeholder, const std::string& bg_sprite_name ) {
    UIEquipmentCell* ret = new UIEquipmentCell();
    if( ret && ret->init( equip_data, placeholder, bg_sprite_name ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIEquipmentCell::init( EquipmentData* equip_data, cocos2d::Sprite* placeholder, const std::string& bg_sprite_name ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->setPlaceHolder( placeholder );
    this->setContentSize( Size( EQUIP_CELL_WIDTH, EQUIP_CELL_HEIGHT ) );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    
    this->setEquipData( equip_data );
    
    _selected_sprite = Sprite::createWithSpriteFrameName( "ui_detail_icon02.png" );
    _selected_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _selected_sprite, 1 );
    
    if( bg_sprite_name != "" ) {
        Sprite* bg_sprite = Sprite::createWithSpriteFrameName( bg_sprite_name );
        bg_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
        this->addChild( bg_sprite, 0 );
    }
    
    this->setSelected( false );
    
    return true;
}

UIEquipmentCell* UIEquipmentCell::create( const cocos2d::ValueMap& data, const std::string& bg_sprite_name ) {
    UIEquipmentCell* ret = new UIEquipmentCell();
    if( ret && ret->init( data, bg_sprite_name ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIEquipmentCell::init( const cocos2d::ValueMap& data, const std::string& bg_sprite_name ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->setContentSize( Size( 158.0f, 144.0f ) );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    
    EquipmentData* eq_data = EquipmentData::create( data );
    this->setEquipData( eq_data );
    
    _selected_sprite = Sprite::createWithSpriteFrameName( "ui_detail_icon02.png" );
    _selected_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _selected_sprite, 1 );
    
    if( bg_sprite_name != "" ) {
        Sprite* bg_sprite = Sprite::createWithSpriteFrameName( bg_sprite_name );
        bg_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
        this->addChild( bg_sprite, 0 );
    }
    
    this->setSelected( false );
    
    return true;
}

EquipmentData* UIEquipmentCell::getEquipData() {
    return _equip_data;
}

void UIEquipmentCell::setEquipData( EquipmentData* data ) {
    CC_SAFE_RETAIN( data );
    CC_SAFE_RELEASE( _equip_data );
    _equip_data = data;
    if( _equip_data ) {
        Sprite* eq_sprite = Sprite::createWithSpriteFrameName( _equip_data->name + ".png" );
        this->setEquipSprite( eq_sprite );
    }
    else {
        this->setEquipSprite( nullptr );
    }
}

cocos2d::Sprite* UIEquipmentCell::getEquipSprite() {
    return _equip_sprite;
}

void UIEquipmentCell::setEquipSprite( cocos2d::Sprite* icon ) {
    if( _equip_sprite == nullptr ) {
        if( icon ) {
            _equip_sprite = Sprite::createWithSpriteFrame( icon->getSpriteFrame() );
            _equip_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
            this->addChild( _equip_sprite, 2 );
        }
    }
    else {
        if( icon ) {
            _equip_sprite->setVisible( true );
            _equip_sprite->setSpriteFrame( icon->getSpriteFrame() );
        }
        else {
            _equip_sprite->setVisible( false );
        }
    }
    if( _placeholder ) {
        if( _equip_sprite ) {
            _placeholder->setVisible( !_equip_sprite->isVisible() );
        }
        else {
            _placeholder->setVisible( true );
        }
    }
}

bool UIEquipmentCell::isSelected() {
    return _is_selected;
}

void UIEquipmentCell::setSelected( bool b ) {
    _is_selected = b;
    if( _selected_sprite != nullptr ) {
        _selected_sprite->setVisible( _is_selected );
    }
}

//detail layer
UIHeroDetailLayer::UIHeroDetailLayer()
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
    this->setName( "detail_layer" );
    _is_touch_down = false;
    _is_dragging = false;
    _drag_equip = nullptr;
    _target_equiped_cell = nullptr;
    _current_equip_list = nullptr;
    _selected_equiped_cell = nullptr;
    _selected_repo_cell = nullptr;
    
    _hero = hero;
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_DETAIL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    _root_node->setName( "root_node" );
    
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_back" ) );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onBackTouched, this ) );
    
    Node* status_panel = _root_node->getChildByName( "statusPanel" );
    
    Node* avatar_container = status_panel->getChildByName( "nd_hero_avatar" );
    std::string avatar_resource = "ui_hero_p_" + hero->getUnitData()->name + ".png";
    Sprite* hero_avatar = Sprite::createWithSpriteFrameName( avatar_resource );
    hero_avatar->setScale( 1.5f );
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
    
    _lb_hp_extra->setVisible( false );
    _lb_mp_extra->setVisible( false );
    _lb_def_extra->setVisible( false );
    _lb_atk_extra->setVisible( false );
    
    _lb_hit_extra->setVisible( false );
    _lb_dod_extra->setVisible( false );
    _lb_cri_extra->setVisible( false );
    _lb_ten_extra->setVisible( false );
    _lb_other_extra->setVisible( false );

    Node* equip_panel = _root_node->getChildByName( "equipPanel" );
    
    UnitData* unit_data = _hero->getUnitData();
    
    Sprite* sp_weapon_placeholder = dynamic_cast<Sprite*>( equip_panel->getChildByName( "sp_weapon" ) );
    
    Node* weapon_node = equip_panel->getChildByName( "nd_weapon" );
    _weapon_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeWeapon ), sp_weapon_placeholder );
    weapon_node->addChild( _weapon_cell );
    
    Sprite* sp_armor_placeholder = dynamic_cast<Sprite*>( equip_panel->getChildByName( "sp_armor" ) );
    
    Node* armor_node = equip_panel->getChildByName( "nd_armor" );
    _armor_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeArmor ), sp_armor_placeholder );
    armor_node->addChild( _armor_cell );
    
    Sprite* sp_boot_placeholder = dynamic_cast<Sprite*>( equip_panel->getChildByName( "sp_boot" ) );
    
    Node* boot_node = equip_panel->getChildByName( "nd_boot" );
    _boot_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeBoot ), sp_boot_placeholder );
    boot_node->addChild( _boot_cell );
    
    Sprite* sp_accessory_placeholder = dynamic_cast<Sprite*>( equip_panel->getChildByName( "sp_accessory" ) );
    
    Node* accessory_node = equip_panel->getChildByName( "nd_accessory" );
    _accessory_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeAccessory ), sp_accessory_placeholder );
    accessory_node->addChild( _accessory_cell );
    
    _btn_prev = dynamic_cast<ui::Button*>( equip_panel->getChildByName( "leftButton" ) );
    _btn_prev->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onPrevTouched, this ) );
    _btn_next = dynamic_cast<ui::Button*>( equip_panel->getChildByName( "rightButton" ) );
    _btn_next->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onNextTouched, this ) );
    
    _pv_weapon_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_weapon_list" ) );
    ui::Layout* page = ui::Layout::create();
    _pv_weapon_list->addPage( page );
    _pv_weapon_list->scrollToPage( 0 );
    
    _pv_armor_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_armor_list" ) );
    page = ui::Layout::create();
    _pv_armor_list->addPage( page );
    _pv_armor_list->scrollToPage( 0 );
    
    _pv_boot_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_boot_list" ) );
    page = ui::Layout::create();
    _pv_boot_list->addPage( page );
    _pv_boot_list->scrollToPage( 0 );
    
    _pv_accessory_list = dynamic_cast<ui::PageView*>( equip_panel->getChildByName( "pv_accessory_list" ) );
    page = ui::Layout::create();
    _pv_accessory_list->addPage( page );
    _pv_accessory_list->scrollToPage( 0 );
    
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
    
    _pn_equip_area = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_equip_area" ) );
    
    ui::Button* btn_sell = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_sell" ) );
    btn_sell->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onSellTouched, this ) );
    
    this->reloadHeroData();
    
    _current_tab = 0;
    this->switchToTab( 3 );
    
    std::string sell_csb_file = FileUtils::getInstance()->fullPathForFilename( SELL_PANEL_FILE );
    _pn_sell = cocos2d::CSLoader::getInstance()->createNode( sell_csb_file );
    this->addChild( _pn_sell, 2 );
    _pn_sell->setVisible( false );
    
    _lb_gold = dynamic_cast<ui::Text*>( _pn_sell->getChildByName( "lb_gold" ) );
    _sp_icon_frame = dynamic_cast<Sprite*>( _pn_sell->getChildByName( "sp_icon_frame" ) );
    
    ui::Button* btn_sell_cancel = dynamic_cast<ui::Button*>( _pn_sell->getChildByName( "btn_cancel" ) );
    btn_sell_cancel->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onSellCancelTouched, this ) );
    
    ui::Button* btn_sell_confirm = dynamic_cast<ui::Button*>( _pn_sell->getChildByName( "btn_sell" ) );
    btn_sell_confirm->addTouchEventListener( CC_CALLBACK_2( UIHeroDetailLayer::onSellConfirmTouched, this ) );
    
    return true;
}

void UIHeroDetailLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}

void UIHeroDetailLayer::onSellTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        if( _selected_repo_cell ) {
            CocosUtils::playTouchEffect();
            _pn_sell->setVisible( true );
            
            std::string price = Utils::toStr( _selected_repo_cell->getEquipData()->price );
            _lb_gold->setString( price );
            
            Sprite* icon = Sprite::createWithSpriteFrame( _selected_repo_cell->getEquipSprite()->getSpriteFrame() );
            _sp_icon_frame->removeAllChildren();
            icon->setPosition( _sp_icon_frame->getContentSize().width / 2, _sp_icon_frame->getContentSize().height / 2 );
            _sp_icon_frame->addChild( icon );
        }
    }
}

void UIHeroDetailLayer::onSellConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        PlayerInfo* player_info = PlayerInfo::getInstance();
        if( player_info->sellEquip( _selected_repo_cell->getEquipData()->obj_id ) ) {
            this->loadRepoEquips( (eEquipType)_current_tab, _current_equip_list->getCurPageIndex() );
            std::string audio_res = "common/sell.mp3";
            AudioManager::getInstance()->playEffect( audio_res );
        }
        _pn_sell->setVisible( false );
    }
}

void UIHeroDetailLayer::onSellCancelTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        _pn_sell->setVisible( false );
    }
}

void UIHeroDetailLayer::onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        int cur_page = _current_equip_list->getCurPageIndex();
        if( this->loadRepoEquips( (eEquipType)_current_tab, cur_page - 1 ) ) {
            _current_equip_list->scrollToPage( cur_page - 1 );
        }
    }
}

void UIHeroDetailLayer::onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        int cur_page = _current_equip_list->getCurPageIndex();
        if( this->loadRepoEquips( (eEquipType)_current_tab, cur_page + 1 ) ) {
            _current_equip_list->scrollToPage( cur_page + 1 );
        }
    }
}

void UIHeroDetailLayer::onTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        ui::Button* tab = dynamic_cast<ui::Button*>( sender );
        if( tab ) {
            this->switchToTab( tab->getTag() );
        }
    }
}

void UIHeroDetailLayer::switchToTab( int i ) {
    if( i != _current_tab ) {
        switch( _current_tab ) {
            case 3:
            {
                _btn_weapon_tab->switchSpriteFrames();
                _pv_weapon_list->setVisible( false );
                _pn_weapon_tab->setPosition( Point( _pn_weapon_tab->getPosition().x - 20, _pn_weapon_tab->getPosition().y ) );
                break;
            }
            case 1:
            {
                _btn_armor_tab->switchSpriteFrames();
                _pv_armor_list->setVisible( false );
                _pn_armor_tab->setPosition( Point( _pn_armor_tab->getPosition().x - 20, _pn_armor_tab->getPosition().y ) );
                break;
            }
            case 2:
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
            case 3:
            {
                _btn_weapon_tab->switchSpriteFrames();
                _pv_weapon_list->setVisible( true );
                _pn_weapon_tab->setPosition( Point( _pn_weapon_tab->getPosition().x + 20, _pn_weapon_tab->getPosition().y ) );
                _current_equip_list = _pv_weapon_list;
                break;
            }
            case 1:
            {
                _btn_armor_tab->switchSpriteFrames();
                _pv_armor_list->setVisible( true );
                _pn_armor_tab->setPosition( Point( _pn_armor_tab->getPosition().x + 20, _pn_armor_tab->getPosition().y ) );
                _current_equip_list = _pv_armor_list;
                break;
            }
            case 2:
            {
                _btn_boot_tab->switchSpriteFrames();
                _pv_boot_list->setVisible( true );
                _pn_boot_tab->setPosition( Point( _pn_boot_tab->getPosition().x + 20, _pn_boot_tab->getPosition().y ) );
                _current_equip_list = _pv_boot_list;
                break;
            }
            case 4:
            {
                _btn_accessory_tab->switchSpriteFrames();
                _pv_accessory_list->setVisible( true );
                _pn_accessory_tab->setPosition( Point( _pn_accessory_tab->getPosition().x + 20, _pn_accessory_tab->getPosition().y ) );
                _current_equip_list = _pv_accessory_list;
                break;
            }
            default:
                _current_equip_list = nullptr;
                break;
        }
        _current_tab = i;
        this->loadRepoEquips( (eEquipType)_current_tab, _current_equip_list->getCurPageIndex() );
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

bool UIHeroDetailLayer::loadRepoEquips( eEquipType type, int page_index ) {
    this->setSelectedRepoCell( nullptr );
    if( page_index >= 0 ) {
        int start_index = 6 * page_index;
        ValueVector data = PlayerInfo::getInstance()->getEquipsByRange( (int)type, start_index, 7, 2 );
        
        if( _current_equip_list->getPages().size() <= page_index ) {
            ui::Layout* new_page = ui::Layout::create();
            _current_equip_list->addPage( new_page );
        }
        ui::Layout* page = _current_equip_list->getPages().at( page_index );
        page->removeAllChildren();
        
        if( data.size() > 0 ) {
            int i = 0;
            for( auto itr = data.begin(); itr != data.end(); ++itr ) {
                UIEquipmentCell* cell = UIEquipmentCell::create( itr->asValueMap(), "ui_detail_icon01.png" );
                Point cell_pos = Point::ZERO;
                switch( i ) {
                    case 0:
                        cell_pos = Point( 120, 250 );
                        break;
                    case 1:
                        cell_pos = Point( 315, 250 );
                        break;
                    case 2:
                        cell_pos = Point( 510, 250 );
                        break;
                    case 3:
                        cell_pos = Point( 120, 90 );
                        break;
                    case 4:
                        cell_pos = Point( 315, 90 );
                        break;
                    case 5:
                        cell_pos = Point( 510, 90 );
                        break;
                    default:
                        break;
                }
                cell->setPosition( cell_pos );
                page->addChild( cell );
                if( ++i >= 6 ) {
                    break;
                }
            }
        }
        
        _btn_prev->setVisible( start_index > 0 );
        _btn_next->setVisible( data.size() == 7 );
        
        return true;
    }

    return false;
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

void UIHeroDetailLayer::setSelectedEquipedCell( UIEquipmentCell* cell ) {
    if( _selected_equiped_cell != nullptr ) {
        _selected_equiped_cell->setSelected( false );
    }
    _selected_equiped_cell = cell;
    if( _selected_equiped_cell != nullptr ) {
        _selected_equiped_cell->setSelected( true );
    }
}

void UIHeroDetailLayer::setSelectedRepoCell( UIEquipmentCell* cell ) {
    if( _selected_repo_cell != nullptr ) {
        _selected_repo_cell->setSelected( false );
    }
    _selected_repo_cell = cell;
    if( _selected_repo_cell != nullptr ) {
        _selected_repo_cell->setSelected( true );
    }
    this->updateCompareInfo();
}

void UIHeroDetailLayer::setTargetEquipedCell( UIEquipmentCell* cell ) {
    if( _target_equiped_cell != nullptr && _target_equiped_cell != _selected_equiped_cell ) {
        _target_equiped_cell->setSelected( false );
    }
    _target_equiped_cell = cell;
    if( _target_equiped_cell != nullptr && _target_equiped_cell != _selected_equiped_cell ) {
        _target_equiped_cell->setSelected( true );
    }
}

void UIHeroDetailLayer::updateCompareInfo() {
    if( _selected_repo_cell != nullptr ) {
        EquipmentData* old_equip_data = nullptr;
        EquipmentData* new_equip_data = _selected_repo_cell->getEquipData();
        eEquipType type = new_equip_data->equip_type;
        switch( type ) {
            case EquipTypeWeapon:
                old_equip_data = _weapon_cell->getEquipData();
                break;
            case EquipTypeArmor:
                old_equip_data = _armor_cell->getEquipData();
                break;
            case EquipTypeBoot:
                old_equip_data = _boot_cell->getEquipData();
                break;
            case EquipTypeAccessory:
                old_equip_data = _accessory_cell->getEquipData();
                break;
            default:
                break;
        }
        if( old_equip_data == nullptr ) {
            old_equip_data = EquipmentData::create( ValueMap() );
        }
        
        this->updateExtraLabel( _lb_hp_extra, new_equip_data->hp - old_equip_data->hp );
        this->updateExtraLabel( _lb_mp_extra, new_equip_data->mp - old_equip_data->mp );
        this->updateExtraLabel( _lb_atk_extra, new_equip_data->atk - old_equip_data->atk );
        this->updateExtraLabel( _lb_def_extra, new_equip_data->def - old_equip_data->def );
        this->updateExtraLabel( _lb_hit_extra, new_equip_data->hit - old_equip_data->hit );
        this->updateExtraLabel( _lb_dod_extra, new_equip_data->dod - old_equip_data->dod );
        this->updateExtraLabel( _lb_cri_extra, new_equip_data->cri - old_equip_data->cri );
        this->updateExtraLabel( _lb_ten_extra, new_equip_data->ten - old_equip_data->ten );
        
    }
    else {
        _lb_hp_extra->setVisible( false );
        _lb_mp_extra->setVisible( false );
        _lb_atk_extra->setVisible( false );
        _lb_def_extra->setVisible( false );
        _lb_hit_extra->setVisible( false );
        _lb_dod_extra->setVisible( false );
        _lb_cri_extra->setVisible( false );
        _lb_ten_extra->setVisible( false );
    }
}

void UIHeroDetailLayer::updateExtraLabel( cocos2d::ui::Text* label, int value ) {
    if( value != 0 ) {
        label->setVisible( true );
        if( value > 0 ) {
            label->setColor( Color3B::GREEN );
            label->setString( Utils::stringFormat( "+%d", value ) );
        }
        else {
            label->setColor( Color3B::RED );
            label->setString( Utils::stringFormat( "%d", value ) );
        }
    }
    else {
        label->setVisible( false );
    }
}

bool UIHeroDetailLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( !_is_touch_down ) {
        _touch_down_pos = this->convertTouchToNodeSpace( touch );
        UIEquipmentCell* selected_cell = this->equipedCellForTouch( touch );
        this->setSelectedEquipedCell( selected_cell );
        
        selected_cell = this->repoCellForTouch( touch );
        this->setSelectedRepoCell( selected_cell );
        
        if( _selected_equiped_cell != nullptr || _selected_repo_cell != nullptr ) {
            _is_touch_down = true;
        }
    }
    return true;
}

void UIHeroDetailLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_down ) {
        Point pos = this->convertTouchToNodeSpace( touch );
        if( !_is_dragging ) {
            Sprite* new_equip_icon = nullptr;
            if( _selected_equiped_cell != nullptr &&_selected_equiped_cell->getEquipData() != nullptr ) {
                if( pos.distance( _touch_down_pos ) > MIN_DRAG_BIAS ) {
                    _is_dragging = true;
                    new_equip_icon = _selected_equiped_cell->getEquipSprite();
                }
            }
            else if( _selected_repo_cell != nullptr && _selected_repo_cell->getEquipData() != nullptr ) {
                if( pos.distance( _touch_down_pos ) > MIN_DRAG_BIAS ) {
                    _is_dragging = true;
                    new_equip_icon = _selected_repo_cell->getEquipSprite();
                }
            }
            if( new_equip_icon != nullptr ) {
                if( _drag_equip == nullptr ) {
                    _drag_equip = Sprite::createWithSpriteFrame( new_equip_icon->getSpriteFrame() );
                    this->addChild( _drag_equip, 10 );
                }
                _drag_equip->setVisible( true );
                _drag_equip->setSpriteFrame( new_equip_icon->getSpriteFrame() );
                _drag_equip->setPosition( pos );
            }
        }
        else {
            _drag_equip->setPosition( pos );
            this->checkTargetCell();
        }
    }
}

void UIHeroDetailLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_down ) {
        _is_touch_down = false;
        _is_dragging = false;
        if( _drag_equip != nullptr ) {
            _drag_equip->setVisible( false );
        }
        this->setTargetEquipedCell( nullptr );
        this->setSelectedEquipedCell( nullptr );
    }
}

void UIHeroDetailLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_down ) {
        if( _is_dragging ) {
            Point pos = this->convertTouchToNodeSpace( touch );
            _drag_equip->setPosition( pos );
            this->checkTargetCell();
            
            if( _selected_repo_cell != nullptr ) {
                if( _target_equiped_cell != nullptr ) {
                    //change equip
                    PlayerInfo* player_info = PlayerInfo::getInstance();
                    std::string hero_id = _hero->getHeroId();
                    eEquipType type = _selected_repo_cell->getEquipData()->equip_type;
                    if( _target_equiped_cell->getEquipData() != nullptr ) {
                        //take off first
                        player_info->updateEquip( _target_equiped_cell->getEquipData()->obj_id, false );
                    }
                    
                    //put on equip
                    std::string obj_id = _selected_repo_cell->getEquipData()->obj_id;
                    player_info->updateEquip( obj_id, true );
                    ValueMap new_hero_data = player_info->updateUnitEquip( hero_id, EquipmentData::stringFromType( type ), obj_id );
                    _hero->loadHeroInfo( new_hero_data );
                    this->reloadHeroData();
                    _target_equiped_cell->setEquipData( _selected_repo_cell->getEquipData() );
                    
                    int cur_page = _current_equip_list->getCurPageIndex();
                    if( !this->loadRepoEquips( (eEquipType)_current_tab, cur_page ) && cur_page > 0 ) {
                        _current_equip_list->scrollToPage( cur_page - 1 );
                    }
                }
            }
            else if( _selected_equiped_cell != nullptr ) {
                if( _target_equiped_cell == nullptr ) {
                    //take off equip
                    std::string obj_id = _selected_equiped_cell->getEquipData()->obj_id;
                    eEquipType type = _selected_equiped_cell->getEquipData()->equip_type;
                    const std::string& hero_id = _hero->getHeroId();
                    
                    PlayerInfo* player_info = PlayerInfo::getInstance();
                    player_info->updateEquip( obj_id, false );
                    ValueMap new_hero_data = player_info->updateUnitEquip( hero_id, EquipmentData::stringFromType( type ), "0" );
                    _hero->loadHeroInfo( new_hero_data );
                    this->reloadHeroData();
                    
                    _selected_equiped_cell->setEquipData( nullptr );
                    this->loadRepoEquips( (eEquipType)_current_tab, _current_equip_list->getCurPageIndex() );
                }
            }
        }
        
        this->setSelectedEquipedCell( nullptr );
        this->setTargetEquipedCell( nullptr );
        _is_touch_down = false;
        _is_dragging = false;
        if( _drag_equip != nullptr ) {
            _drag_equip->setVisible( false );
        }
    }
}

//private methods
UIEquipmentCell* UIHeroDetailLayer::equipedCellForTouch( cocos2d::Touch* touch ) {
    Point pos = _weapon_cell->convertTouchToNodeSpace( touch );
    Rect rect = Rect( 0, 0, _weapon_cell->getContentSize().width, _weapon_cell->getContentSize().height );
    if( rect.containsPoint( pos ) ) {
        return _weapon_cell;
    }
    
    pos = _armor_cell->convertTouchToNodeSpace( touch );
    if( rect.containsPoint( pos ) ) {
        return _armor_cell;
    }
    
    pos = _boot_cell->convertTouchToNodeSpace( touch );
    if( rect.containsPoint( pos ) ) {
        return _boot_cell;
    }
    
    pos = _accessory_cell->convertTouchToNodeSpace( touch );
    if( rect.containsPoint( pos ) ) {
        return _accessory_cell;
    }
    
    return nullptr;
}

UIEquipmentCell* UIHeroDetailLayer::repoCellForTouch( cocos2d::Touch* touch ) {
    ui::Layout* layout = _current_equip_list->getPage( _current_equip_list->getCurPageIndex() );
    for( auto nd : layout->getChildren() ) {
        UIEquipmentCell* cell = dynamic_cast<UIEquipmentCell*>( nd );
        if( cell ) {
            Point pos = cell->convertTouchToNodeSpace( touch );
            Rect rect = Rect( 0, 0, cell->getContentSize().width, cell->getContentSize().height );
            if( rect.containsPoint( pos ) ) {
                return cell;
            }
        }
    }
    
    return nullptr;
}

void UIHeroDetailLayer::checkTargetCell() {
    UIEquipmentCell* cell = _selected_equiped_cell;
    if( cell == nullptr ) {
        cell = _selected_repo_cell;
    }
    Point pos = _drag_equip->getPosition();
    Rect rect = _pn_equip_area->getBoundingBox();
    if( rect.containsPoint( pos ) ) {
        int equip_id = cell->getEquipData()->equip_id;
        int type = equip_id / 1e7;
        switch( type ) {
            case 3:
                this->setTargetEquipedCell( _weapon_cell );
                break;
            case 1:
                this->setTargetEquipedCell( _armor_cell );
                break;
            case 2:
                this->setTargetEquipedCell( _boot_cell );
                break;
            case 4:
                this->setTargetEquipedCell( _accessory_cell );
                break;
            default:
                this->setTargetEquipedCell( nullptr );
                break;
        }
    }
    else {
        this->setTargetEquipedCell( nullptr );
    }
}