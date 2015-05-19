//
//  UIHeroManageLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 5/9/15.
//
//

#include "UIHeroManageLayer.h"
#include "UIHeroDetailLayer.h"
#include "UIHeroSkillLayer.h"
#include "../data/PlayerInfo.h"
#include "../Utils.h"

#define HERO_MANAGE_FILE "ui/page/ui_hero_manage.csb"

using namespace cocos2d;

//hero slot
UIHeroManageHeroSlot::UIHeroManageHeroSlot() :
_hero_avatar( nullptr ),
_hero_data( nullptr ),
_hero_skeleton( nullptr )
{
    
}

UIHeroManageHeroSlot::~UIHeroManageHeroSlot() {
    
}

UIHeroManageHeroSlot* UIHeroManageHeroSlot::create( const cocos2d::ValueMap& data, const std::string& slot_id, int flag ) {
    UIHeroManageHeroSlot* ret = new UIHeroManageHeroSlot();
    if( ret && ret->init( data, slot_id, flag ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroManageHeroSlot::init( const cocos2d::ValueMap& data, const std::string& slot_id, int flag ) {
    if( !Node::init() ) {
        return false;
    }
    
    UnitData* unit_data = UnitData::create( data );
    this->setUnitData( unit_data );
    
    _slot_id = slot_id;
    
    const std::string& hero_name = unit_data->name;
    
    _selected_sprite = Sprite::createWithSpriteFrameName( "ui_hero_selected.png" );
    this->addChild( _selected_sprite );
    
    if( flag == 0 ) {
        _hero_avatar = Sprite::createWithSpriteFrameName( "ui_hero_p_" + hero_name + ".png" );
        _lock_sprite = Sprite::createWithSpriteFrameName( "ui_hero_lock.png" );
    }
    else if( flag == 1 ) {
        _hero_avatar = Sprite::createWithSpriteFrameName( "ui_hero_f_" + hero_name + ".png" );
        _lock_sprite = Sprite::createWithSpriteFrameName( "ui_hero_chain.png" );
    }
    this->addChild( _lock_sprite );
    this->addChild( _hero_avatar );
    
    this->setSelected( false );
    
    auto itr = data.find( "locked" );
    if( itr != data.end() ) {
        this->setOwned( itr->second.asBool() );
    }
    else {
        this->setLocked( false );
    }
    
    itr = data.find( "owned" );
    if( itr != data.end() ) {
        this->setOwned( itr->second.asBool() );
    }
    else {
        this->setOwned( false );
    }
    
    return true;
}

bool UIHeroManageHeroSlot::isSelected() {
    return _is_selected;
}

void UIHeroManageHeroSlot::setSelected( bool b ) {
    _is_selected = b;
    if( _is_selected ) {
        _selected_sprite->setVisible( true );
    }
    else {
        _selected_sprite->setVisible( false );
    }
}

bool UIHeroManageHeroSlot::isLocked() {
    return _is_locked;
}

void UIHeroManageHeroSlot::setLocked( bool b ) {
    _is_locked = b;
    if( _is_locked ) {
        _lock_sprite->setVisible( true );
        _hero_avatar->setColor( Color3B::BLACK );
    }
    else {
        _lock_sprite->setVisible( false );
        _hero_avatar->setColor( Color3B::WHITE );
    }
}

bool UIHeroManageHeroSlot::isOwned() {
    return _is_owned;
}

void UIHeroManageHeroSlot::setOwned( bool b ) {
    _is_owned = b;
    if( _is_owned ) {
        _hero_avatar->setGLProgramState( GLProgramState::getOrCreateWithGLProgramName( GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP ) );
    }
    else {
        _hero_avatar->setGLProgramState( GLProgramState::getOrCreateWithGLProgramName( GLProgram::SHADER_NAME_GREY_NO_MVP ) );
    }
}

spine::SkeletonAnimation* UIHeroManageHeroSlot::getHeroSkeleton() {
    return _hero_skeleton;
}

void UIHeroManageHeroSlot::setHeroSkeleton( spine::SkeletonAnimation* skeleton ) {
    CC_SAFE_RELEASE( _hero_skeleton );
    _hero_skeleton = skeleton;
    CC_SAFE_RETAIN( _hero_skeleton );
}

UnitData* UIHeroManageHeroSlot::getUnitData() {
    return _hero_data;
}

void UIHeroManageHeroSlot::setUnitData( UnitData* data ) {
    CC_SAFE_RELEASE( _hero_data );
    _hero_data = data;
    CC_SAFE_RETAIN( _hero_data );
}

cocos2d::Sprite* UIHeroManageHeroSlot::getHeroAvatar() {
    return _hero_avatar;
}

void UIHeroManageHeroSlot::setHeroAvatar( cocos2d::Sprite* avatar ) {
    _hero_avatar->setSpriteFrame( avatar->getSpriteFrame() );
}

//manage layer
UIHeroManageLayer::UIHeroManageLayer():
_selected_hero( nullptr )
{
    
}

UIHeroManageLayer::~UIHeroManageLayer() {
    
}

UIHeroManageLayer* UIHeroManageLayer::create() {
    UIHeroManageLayer* ret = new UIHeroManageLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroManageLayer::init() {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_MANAGE_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_confirm = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_finish" ) );
    btn_confirm->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onConfirmTouched, this ) );
    
    ui::Button* btn_detail = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_equip" ) );
    btn_detail->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onDetailTouched, this ) );
    
    ui::Button* btn_skill = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_skill") );
    btn_skill->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onSkillTouched, this ) );
    
    _pv_hero_list = dynamic_cast<ui::PageView*>( _root_node->getChildByName( "pv_hero_list" ) );
    
    _lb_diamond = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_diamond" ) );
    _lb_gold = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_gold" ) );
    
    _star_1 = dynamic_cast<Sprite*>( _root_node->getChildByName( "star_1" ) );
    _star_2 = dynamic_cast<Sprite*>( _root_node->getChildByName( "star_2" ) );
    _star_3 = dynamic_cast<Sprite*>( _root_node->getChildByName( "star_3" ) );
    
    _lb_level = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_level" ) );
    _lb_hp = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_hp" ) );
    _lb_mp = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_mp" ) );
    _lb_def = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_def" ) );
    _lb_atk = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_atk" ) );
    
    _lb_hit = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_hit" ) );
    _lb_dodge = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_dod" ) );
    _lb_cri = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_cri" ) );
    _lb_ten = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_ten" ) );
    _lb_other = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_range" ) );
    
    _btn_upgrade = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_upgrade" ) );
    _btn_upgrade->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onUpgradeTouched, this ) );
    _btn_prev = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_prev" ) );
    _btn_prev->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onPrevTouched, this ) );
    _btn_next = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_next" ) );
    _btn_next->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onNextTouched, this ) );
    
    _hero_node = _root_node->getChildByName( "hero_node" );
    
    _lb_hero_name = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_hero_name" ) );
    
    _selected_hero = nullptr;
    
    PlayerInfo* player_info = PlayerInfo::getInstance();
    _lb_diamond->setString( Utils::toStr( player_info->getDiamond() ) );
    _lb_gold->setString( Utils::toStr( player_info->getGold() ) );
    
    _deploy_slot_1 = _root_node->getChildByName( "hero_1" );
    _deploy_slot_2 = _root_node->getChildByName( "hero_2" );
    _deploy_slot_3 = _root_node->getChildByName( "hero_3" );
    _deploy_slot_4 = _root_node->getChildByName( "hero_4" );
    _deploy_slot_5 = _root_node->getChildByName( "hero_5" );
    
    const ValueMap& all_units_info = player_info->getAllUnitsInfo();
    
    ValueVector deployed_unit_slot_ids = player_info->getPlayerDeployedUnitsSlotIds();
    //deployed units
    int i_slot_id = 1;
    for( auto itr = deployed_unit_slot_ids.begin(); itr != deployed_unit_slot_ids.end(); ++itr ) {
        std::string slot_id = itr->asString();
        const ValueMap& data = all_units_info.at( slot_id ).asValueMap();
        UIHeroManageHeroSlot* hero = UIHeroManageHeroSlot::create( data, slot_id, 1 );
        switch ( i_slot_id ) {
            case 1:
                _deploy_slot_1->addChild( hero );
                break;
            case 2:
                _deploy_slot_2->addChild( hero );
                break;
            case 3:
                _deploy_slot_3->addChild( hero );
                break;
            case 4:
                _deploy_slot_4->addChild( hero );
                break;
            case 5:
                _deploy_slot_5->addChild( hero );
                break;
            default:
                break;
        }
    }
    
    //all units
    for( auto itr = all_units_info.begin(); itr != all_units_info.end(); ++itr ) {
        
    }
    
    return true;
}

void UIHeroManageLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        this->removeFromParentAndCleanup( true );
    }
}

void UIHeroManageLayer::onDetailTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        _root_node->setVisible( false );
        UIHeroDetailLayer* hero_detail = UIHeroDetailLayer::create();
        this->addChild( hero_detail, 2 );
    }
}

void UIHeroManageLayer::onSkillTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        this->setVisible( false );
        UIHeroSkillLayer* hero_skill = UIHeroSkillLayer::create();
        this->addChild( hero_skill, 2 );
    }
}

void UIHeroManageLayer::onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
    }
}

void UIHeroManageLayer::onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
    }
}

void UIHeroManageLayer::onUpgradeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
    }
}

void UIHeroManageLayer::becomeTopLayer() {
    _root_node->setVisible( true );
}

void UIHeroManageLayer::setSelectedHero( UIHeroManageHeroSlot* hero ) {
    if( _selected_hero != hero ) {
        _selected_hero = hero;
        //update panel info
        
        _hero_node->removeAllChildren();
        _hero_node->addChild( _selected_hero->getHeroSkeleton() );
        _selected_hero->getHeroSkeleton()->setAnimation( 0, "Idle", true );
        
        UnitData* data = _selected_hero->getUnitData();
        
        _lb_level->setString( Utils::toStr( data->level ) );
        _lb_hero_name->setString( data->display_name );
        _lb_hp->setString( Utils::toStr( data->hp ) );
        _lb_mp->setString( Utils::toStr( data->mp ) );
        _lb_def->setString( Utils::toStr( data->def ) );
        _lb_atk->setString( Utils::toStr( data->atk ) );
        _lb_hit->setString( Utils::toStr( data->hit ) );
        _lb_dodge->setString( Utils::toStr( data->dodge ) );
        _lb_cri->setString( Utils::toStr( data->critical ) );
        _lb_ten->setString( Utils::toStr( data->tenacity ) );
        _lb_other->setString( Utils::toStr( data->atk_range ) );
        
        int star = data->star;
        if( star >= 1 ) {
            _star_1->setVisible( true );
        }
        else {
            _star_1->setVisible( false );
        }
        if( star >= 2 ) {
            _star_2->setVisible( true );
        }
        else {
            _star_2->setVisible( false );
        }
        if( star >= 3 ) {
            _star_3->setVisible( true );
        }
        else {
            _star_3->setVisible( false );
        }
    }
}

bool UIHeroManageLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    return true;
}

void UIHeroManageLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIHeroManageLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIHeroManageLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}