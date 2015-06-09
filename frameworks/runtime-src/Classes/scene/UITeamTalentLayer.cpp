//
//  UITeamTalentLayer.cpp
//  Boids
//
//  Created by chenyanjie on 5/26/15.
//
//

#include "UITeamTalentLayer.h"
#include "../Utils.h"
#include "../data/PlayerInfo.h"
#include "../manager/ResourceManager.h"
#include "../util/CocosUtils.h"

using namespace cocos2d;

#define TEAM_TALENT_FILE "ui/page/ui_team_talent.csb"

UITeamTalentLayer::UITeamTalentLayer()
{
    
}

UITeamTalentLayer::~UITeamTalentLayer() {
    
}

UITeamTalentLayer* UITeamTalentLayer::create() {
    UITeamTalentLayer* ret = new UITeamTalentLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UITeamTalentLayer::init() {
    if( !TouchableLayer::init() ) {
        return false;
    }
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( TEAM_TALENT_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_back" ) );
    btn_back->setVisible( false );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onBackTouched, this ) );
    
    ui::Button* btn_confirm = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_confirm" ) );
    btn_confirm->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onConfirmTouched, this ) );
    
    ui::Button* btn_reset = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_reset" ) );
    btn_reset->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onResetTouched, this ) );
    
    ui::Button* btn_atk = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_atk" ) );
    btn_atk->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onTabTouched, this ) );
    
    ui::Button* btn_def = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_def" ) );
    btn_def->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onTabTouched, this ) );
    
    ui::Button* btn_spt = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_spt" ) );
    btn_spt->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onTabTouched, this ) );
    
    for( int i = 0; i < 12; i++ ) {
        _talent_states[i].is_selected = false;
        _talent_states[i].is_enabled = false;
        _talent_states[i].cost = 0;
        
        ui::Layout* talent = dynamic_cast<ui::Layout*>( _root_node->getChildByTag( i + 1 ) );
        talent->addTouchEventListener( CC_CALLBACK_2( UITeamTalentLayer::onTalentNodeTouched, this ) );
    }
    
    ui::Text* lb_atk_points = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_atk_points" ) );
    _lb_used_points.pushBack( lb_atk_points );
    
    ui::Text* lb_def_points = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_def_points" ) );
    _lb_used_points.pushBack( lb_def_points );
    
    ui::Text* lb_spt_points = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_spt_points" ) );
    _lb_used_points.pushBack( lb_spt_points );
    
    _lb_total_points = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_total_points" ) );
    
    _pn_hint = dynamic_cast<ui::ImageView*>( _root_node->getChildByName( "pn_hint" ) );
    
    this->hideHint();
    
    for( int p = 1; p <= 3; p++ ) {
        this->reloadTabContent( p );
    }
    
    _selected_tab = 0;
    this->switchToTab( 1 );
    
    return true;
}

void UITeamTalentLayer::onBackTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}

void UITeamTalentLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        this->recordTalentPoints();
        this->reloadTabContent( _selected_tab );
        this->updateDisplayedContent();
        
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        parent->becomeTopLayer();
        parent->removeChild( this );
    }
}

void UITeamTalentLayer::onResetTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        this->resetAllInvest();
        this->updateDisplayedContent();
    }
}

void UITeamTalentLayer::onTalentNodeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    ui::Layout* talent = dynamic_cast<ui::Layout*>( sender );
    if( type == cocos2d::ui::Widget::TouchEventType::BEGAN ) {
        CocosUtils::playTouchEffect();
        this->showHint( talent->getTag() );
    }
    else if( type == cocos2d::ui::Widget::TouchEventType::CANCELED ) {
        this->hideHint();
    }
    else if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        this->hideHint();
        this->investTalentPoint( talent );
    }
}

void UITeamTalentLayer::onTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        ui::Button* tab = dynamic_cast<ui::Button*>( sender );
        this->switchToTab( tab->getTag() % 10 );
    }
}

void UITeamTalentLayer::switchToTab( int i ) {
    if( i != _selected_tab ) {
        if( _selected_tab != 0 ) {
            this->resetCurrentInvest();
            ui::Button* last_tab = dynamic_cast<ui::Button*>( _root_node->getChildByTag( _selected_tab * 100 + _selected_tab * 10 + _selected_tab ) );
            if( last_tab ) {
                last_tab->switchSpriteFrames();
            }
        }
        _selected_tab = i;
        if( _selected_tab != 0 ) {
            ui::Button* new_tab = dynamic_cast<ui::Button*>( _root_node->getChildByTag( _selected_tab * 100 + _selected_tab * 10 + _selected_tab ) );
            if( new_tab ) {
                new_tab->switchSpriteFrames();
            }
        }
        
        this->reloadTabContent( i );
        this->updateDisplayedContent();
    }
}

void UITeamTalentLayer::reloadTabContent( int i ) {
    PlayerInfo* player_info = PlayerInfo::getInstance();
    _total_points = player_info->getTotalTalentPoints();
    const ValueVector& activated_talents = player_info->getTeamTalent( Utils::toStr( i ) );
    const ValueMap& talent_config = ResourceManager::getInstance()->getTalentConfig().at( Utils::toStr( i ) ).asValueMap();
    
    //reset
    for( int p = 1; p <= 12; p++ ) {
        _talent_states[p-1].is_selected = false;
        _talent_states[p-1].is_enabled = false;
        _talent_states[p-1].cost = talent_config.at( Utils::toStr( p ) ).asValueMap().at( "cost" ).asInt();
    }
    
    for( auto v : activated_talents ) {
        int p = v.asInt();
        _talent_states[p-1].is_enabled = true;
    }
    
    int total_used_points = 0;
    for( int p = 1; p <= 12; p++ ) {
        bool is_enabled = _talent_states[p-1].is_enabled;
        if( is_enabled ) {
            total_used_points += _talent_states[p-1].cost;
        }
    }
    _total_used_points[i-1] = total_used_points;
}

void UITeamTalentLayer::updateDisplayedContent() {
    for( int p = 1; p <= 12; p++ ) {
        ui::Layout* talent = dynamic_cast<ui::Layout*>( _root_node->getChildByTag( p ) );
        bool is_enabled = _talent_states[p-1].is_enabled;
        this->setTalentEnabled( talent, is_enabled );
        bool is_selectd = _talent_states[p-1].is_selected;
        this->setTalentSelected( talent, is_selectd );
        ui::Text* lb_cost = dynamic_cast<ui::Text*>( talent->getChildByName( "star_number" ) );
        lb_cost->setString( Utils::toStr( _talent_states[p-1].cost ) );
    }
    int total_remained_points = _total_points;
    for( int p = 0; p < 3; p++ ) {
        total_remained_points -= _total_used_points[p];
        _lb_used_points.at( p )->setString( Utils::toStr( _total_used_points[p] ) );
    }
    _lb_total_points->setString( Utils::toStr( total_remained_points ) );
}

void UITeamTalentLayer::showHint( int i ) {
    _pn_hint->setVisible( true );
    ui::Layout* talent = dynamic_cast<ui::Layout*>( _root_node->getChildByTag( i ) );
    float factor = ( ( i - 1 ) % 4 ) < 2 ? 1 : -1;
    float x = talent->getPosition().x + factor * talent->getContentSize().width / 2 + factor * _pn_hint->getContentSize().width / 2;
    float y = talent->getPosition().y + _pn_hint->getContentSize().height / 2;
    _pn_hint->setPosition( Point( x, y ) );
    ui::Text* hint = dynamic_cast<ui::Text*>( _pn_hint->getChildByName( "lb_hint" ) );
    
    const ValueMap& talent_config = ResourceManager::getInstance()->getTalentConfig().at( Utils::toStr( _selected_tab ) ).asValueMap().at( Utils::toStr( i ) ).asValueMap();
    hint->setString( talent_config.at( "desc" ).asString() );
}

void UITeamTalentLayer::hideHint() {
    _pn_hint->setVisible( false );
}

void UITeamTalentLayer::investTalentPoint( cocos2d::ui::Layout* talent ) {
    int tag = talent->getTag();
    if( !_talent_states[tag-1].is_enabled && !_talent_states[tag-1].is_selected ) {
        int cost = _talent_states[tag-1].cost;
        int total_remained_points = _total_points;
        for( int i = 0; i < 3; i++ ) {
            total_remained_points -= _total_used_points[i];
        }
        if( total_remained_points >= cost ) {
            _total_used_points[_selected_tab-1] += cost;
            _talent_states[tag-1].is_selected = true;
            _talent_states[tag-1].is_enabled = true;
            this->updateDisplayedContent();
        }
    }
    else if( _talent_states[tag-1].is_selected ) {
        int cost = _talent_states[tag-1].cost;
        _total_used_points[_selected_tab-1] -= cost;

        _talent_states[tag-1].is_selected = false;
        _talent_states[tag-1].is_enabled = false;
        this->updateDisplayedContent();
    }
}

void UITeamTalentLayer::resetCurrentInvest() {
    for( int i = 0; i < 12; i++ ) {
        if( _talent_states[i].is_selected ) {
            _total_used_points[_selected_tab-1] -= _talent_states[i].cost;
            _talent_states[i].is_selected = false;
        }
    }
}

void UITeamTalentLayer::resetAllInvest() {
    for( int i = 0; i < 12; i++ ) {
        if( _talent_states[i].is_enabled || _talent_states[i].is_selected ) {
            _total_used_points[_selected_tab-1] -= _talent_states[i].cost;
            _talent_states[i].is_selected = false;
            _talent_states[i].is_enabled = false;
        }
    }
    PlayerInfo::getInstance()->resetTeamTalent( Utils::toStr( _selected_tab ) );
}

void UITeamTalentLayer::recordTalentPoints() {
    ValueVector activated_talents;
    for( int i = 0; i < 12; i++ ) {
        if( _talent_states[i].is_enabled ) {
            activated_talents.push_back( Value( i + 1 ) );
        }
    }
    PlayerInfo::getInstance()->setTeamTalent( Utils::toStr( _selected_tab ), activated_talents );
}

//private methods
bool UITeamTalentLayer::isTalentSelected( cocos2d::ui::Layout* talent ) {
    int tag = talent->getTag();
    return _talent_states[tag-1].is_selected;
}

void UITeamTalentLayer::setTalentSelected( cocos2d::ui::Layout* talent, bool b ) {
    int tag = talent->getTag();
    _talent_states[tag-1].is_selected = b;
    
    Node* selected_node = talent->getChildByName( "skill_focus" );
    if( selected_node ) {
        selected_node->setVisible( b );
    }
}

bool UITeamTalentLayer::isTalentEnabled( cocos2d::ui::Layout* talent ) {
    int tag = talent->getTag();
    return _talent_states[tag-1].is_enabled;
}

void UITeamTalentLayer::setTalentEnabled( cocos2d::ui::Layout* talent, bool b ) {
    int tag = talent->getTag();
    _talent_states[tag-1].is_enabled = b;
    Sprite* icon = dynamic_cast<Sprite*>( talent->getChildByName( "skill_icon" ) );
    if( b ) {
        icon->setGLProgramState( GLProgramState::getOrCreateWithGLProgramName( GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP ) );
    }
    else {
        icon->setGLProgramState( GLProgramState::getOrCreateWithGLProgramName( GLProgram::SHADER_NAME_GREY_NO_MVP ) );
    }
}