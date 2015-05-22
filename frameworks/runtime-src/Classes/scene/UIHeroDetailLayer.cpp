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

//equip cell
UIEquipmentCell::UIEquipmentCell() :
_equip_data( nullptr ),
_equip_sprite( nullptr )
{
    
}

UIEquipmentCell::~UIEquipmentCell() {
    
}

UIEquipmentCell* UIEquipmentCell::create( EquipmentData* equip_data ) {
    UIEquipmentCell* ret = new UIEquipmentCell();
    if( ret && ret->init( equip_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIEquipmentCell::init( EquipmentData* equip_data ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->setContentSize( Size( 158.0f, 144.0f ) );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    
    this->setEquipData( equip_data );
    
    _selected_sprite = Sprite::createWithSpriteFrameName( "ui_hero_selected.png" );
    _selected_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _selected_sprite, 3 );
    
    this->setSelected( false );
    
    return true;
}

UIEquipmentCell* UIEquipmentCell::create( const cocos2d::ValueMap& data ) {
    UIEquipmentCell* ret = new UIEquipmentCell();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIEquipmentCell::init( const cocos2d::ValueMap& data ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->setContentSize( Size( 158.0f, 144.0f ) );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    
    EquipmentData* eq_data = EquipmentData::create( data );
    this->setEquipData( eq_data );
    
    _selected_sprite = Sprite::createWithSpriteFrameName( "ui_hero_selected.png" );
    _selected_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _selected_sprite, 3 );
    
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
            this->addChild( _equip_sprite );
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
    
    Node* weapon_node = equip_panel->getChildByName( "nd_weapon" );
    _weapon_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeWeapon ) );
    weapon_node->addChild( _weapon_cell );
    
    Node* armor_node = equip_panel->getChildByName( "nd_armor" );
    _armor_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeArmor ) );
    armor_node->addChild( _armor_cell );
    
    Node* boot_node = equip_panel->getChildByName( "nd_boot" );
    _boot_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeBoot ) );
    boot_node->addChild( _boot_cell );
    
    Node* accessory_node = equip_panel->getChildByName( "nd_accessory" );
    _accessory_cell = UIEquipmentCell::create( unit_data->getEquipData( eEquipType::EquipTypeAccessory ) );
    accessory_node->addChild( _accessory_cell );
    
    _btn_prev = dynamic_cast<ui::Button*>( equip_panel->getChildByName( "leftButton" ) );
    _btn_next = dynamic_cast<ui::Button*>( equip_panel->getChildByName( "rightButton" ) );
    
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
                _current_equip_list = _pv_weapon_list;
                break;
            }
            case 2:
            {
                _btn_armor_tab->switchSpriteFrames();
                _pv_armor_list->setVisible( true );
                _pn_armor_tab->setPosition( Point( _pn_armor_tab->getPosition().x + 20, _pn_armor_tab->getPosition().y ) );
                _current_equip_list = _pv_armor_list;
                break;
            }
            case 3:
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
    return _is_touch_down;
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
                
            }
            else if( _selected_equiped_cell != nullptr ) {
                if( _target_equiped_cell == nullptr ) {
                    //take off equip
                    if( _selected_equiped_cell == _weapon_cell ) {
                        
                    }
                    else if( _selected_equiped_cell == _armor_cell ) {
                        
                    }
                    else if( _selected_equiped_cell == _boot_cell ) {
                        
                    }
                    else if( _selected_equiped_cell == _accessory_cell ) {
                        
                    }
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