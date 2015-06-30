//
//  UIHeroSkillLayer.cpp
//  Boids
//
//  Created by chenyanjie on 5/19/15.
//
//

#include "UIHeroSkillLayer.h"
#include "../manager/resourceManager.h"
#include "../Utils.h"
#include "../data/PlayerInfo.h"
#include "../util/CocosUtils.h"

#define HERO_SKILL_FILE "ui/page/ui_hero_skill.csb"

using namespace cocos2d;

UIHeroSkillLayer::UIHeroSkillLayer() {
    
}

UIHeroSkillLayer::~UIHeroSkillLayer() {
    
}

UIHeroSkillLayer* UIHeroSkillLayer::create( UIHeroManageHeroSlot* hero ) {
    UIHeroSkillLayer* ret = new UIHeroSkillLayer();
    if( ret && ret->init( hero ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroSkillLayer::init( UIHeroManageHeroSlot* hero ) {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    this->setName( "skill_layer" );
    _hero = hero;
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( HERO_SKILL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_back" ) );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onBackTouched, this ) );
    
    _lb_level_1 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_skill_level_1" ) );
    _lb_level_2 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_skill_level_2" ) );
    
    _lb_upgrade_cost_1 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_upgrade_cost_1" ) );
    _lb_upgrade_cost_2 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_upgrade_cost_2" ) );
    
    _btn_upgrade_1 = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_upgrade_skill_1" ) );
    _btn_upgrade_1->setTag( 1 );
    _btn_upgrade_1->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onUpgradeTouched, this ) );
    
    _btn_upgrade_2 = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_upgrade_skill_2" ) );
    _btn_upgrade_2->setTag( 2 );
    _btn_upgrade_2->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onUpgradeTouched, this ) );
    
    _pn_hint = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_skill_hint_1" ) );
    _pn_hint->setVisible( false );
    
    ui::Layout* pn_skill_1 = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_skill_1" ) );
    _lb_skill_desc_1 = dynamic_cast<ui::Text*>( pn_skill_1->getChildByName( "lb_skill_desc_1" ) );
    
    ui::Layout* pn_skill_2 = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "pn_skill_2" ) );
    _lb_skill_desc_2 = dynamic_cast<ui::Text*>( pn_skill_2->getChildByName( "lb_skill_desc_2" ) );
    
    ui::Button* btn_hint = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_skill_hint_1" ) );
    btn_hint->addTouchEventListener( CC_CALLBACK_2( UIHeroSkillLayer::onHintTouched, this ) );
    
    //skill_icon
    Node* pn_skill_icon_1 = _root_node->getChildByName( "pn_skill_icon_1" );
    _sp_skill_icon_1 = dynamic_cast<Sprite*>( pn_skill_icon_1->getChildByName( "skill_icon" ) );
    
    Node* pn_skill_icon_2 = _root_node->getChildByName( "pn_skill_icon_2" );
    _sp_skill_icon_2 = dynamic_cast<Sprite*>( pn_skill_icon_2->getChildByName( "skill_icon" ) );
    
    _lb_skill_name_1 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_skill_name_1" ) );
    _lb_skill_name_2 = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_skill_name_2" ) );
    
    this->loadSkillData( 1 );
    this->loadSkillData( 2 );
    
    return true;
}

void UIHeroSkillLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}

void UIHeroSkillLayer::onUpgradeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        Node* node = dynamic_cast<Node*>( sender );
        int tag = node->getTag();
        this->upgradeSkill( tag );
    }
}

void UIHeroSkillLayer::onHintTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        _pn_hint->setVisible( !_pn_hint->isVisible() );
    }
}

void UIHeroSkillLayer::loadSkillData( int i ) {
    UnitData* unit_data = _hero->getUnitData();
    ResourceManager* resource_manager = ResourceManager::getInstance();
 
    const ValueMap& skill_data = unit_data->skills.at( i - 1 ).asValueMap();
    std::string skill_name = skill_data.at( "name" ).asString();
    int skill_level = skill_data.at( "level" ).asInt();
    
    const ValueMap& skill_config = resource_manager->getSkillData( skill_name );
    
    std::string icon = skill_config.at( "icon" ).asString();
    std::string display_name = skill_config.at( "name" ).asString();
    std::string desc = skill_config.at( "desc" ).asString();
    
    desc = resource_manager->getSkillDesc( skill_name, skill_level );
    
    int upgrade_cost = resource_manager->getSkillUpgradeCost( skill_name, skill_level + 1 );
    
    if( i == 1 ) {
        _sp_skill_icon_1->setSpriteFrame( icon );
        _lb_skill_desc_1->setString( desc );
        _lb_level_1->setString( Utils::toStr( skill_level ) );
        _lb_skill_name_1->setString( display_name );
        _lb_skill_name_1->setAdditionalKerning( -4.0f );
        
        std::string op_hint = skill_config.at( "op_hint" ).asString();
        ui::Text* lb_hint = dynamic_cast<ui::Text*>( _pn_hint->getChildByName( "lb_skill_hint_1" ) );
        lb_hint->setString( op_hint );
        
        if( upgrade_cost < 0 ) {
            _lb_upgrade_cost_1->setString( "满级" );
        }
        else {
            _lb_upgrade_cost_1->setString( Utils::toStr( upgrade_cost ) );
        }
    }
    else if( i == 2 ) {
        _sp_skill_icon_2->setSpriteFrame( icon );
        _lb_skill_desc_2->setString( desc );
        _lb_level_2->setString( Utils::toStr( skill_level ) );
        _lb_skill_name_2->setString( display_name );
        _lb_skill_name_2->setAdditionalKerning( -4.0f );
        if( upgrade_cost < 0 ) {
            _lb_upgrade_cost_2->setString( "满级" );
        }
        else {
            _lb_upgrade_cost_2->setString( Utils::toStr( upgrade_cost ) );
        }
    }
}

void UIHeroSkillLayer::upgradeSkill( int i ) {
    UnitData* unit_data = _hero->getUnitData();
    const ValueMap& skill_data = unit_data->skills.at( i - 1 ).asValueMap();
    std::string skill_name = skill_data.at( "name" ).asString();

    ValueMap result = PlayerInfo::getInstance()->upgradeSkill( _hero->getHeroId(), skill_name, 1 );
    if( !result.empty() ) {
        _hero->loadHeroInfo( result );
        this->loadSkillData( i );
    }
}

//private methods