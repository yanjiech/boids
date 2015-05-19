//
//  UILevelChooseLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "UILevelChooseLayer.h"
#include "../Utils.h"
#include "../manager/SceneManager.h"
#include "../manager/ResourceManager.h"
#include "../data/PlayerInfo.h"
#include "UIHeroManageLayer.h"

#define MAIN_CSB_FILE "ui/page/ui_home_page.csb"
#define LEVEL_CSB_FILE "level_choose/level/level.csb"

using namespace cocos2d;

UILevelChooseLayer::UILevelChooseLayer() :
_map_data( nullptr ) {
    
}

UILevelChooseLayer::~UILevelChooseLayer() {
    CC_SAFE_RELEASE( _map_data );
}

UILevelChooseLayer* UILevelChooseLayer::create() {
    UILevelChooseLayer* ret = new UILevelChooseLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UILevelChooseLayer::init() {
    std::string main_csb_file = FileUtils::getInstance()->fullPathForFilename( MAIN_CSB_FILE );
    _main_node = cocos2d::CSLoader::getInstance()->createNode( main_csb_file );
    this->addChild( _main_node );
    
    std::string level_csb_file = FileUtils::getInstance()->fullPathForFilename( LEVEL_CSB_FILE );
    _level_node = cocos2d::CSLoader::getInstance()->createNode( level_csb_file.c_str() );
    this->addChild( _level_node );
    _level_node->setVisible( false );
    
    _scrollview = dynamic_cast<ui::ScrollView*>( _main_node->getChildByName( "mapScrollView" ) );
    _background_node = dynamic_cast<Sprite*>( _level_node->getChildByName( "background" ) );
    _back_button = dynamic_cast<ui::Button*>( _background_node->getChildByName( "backButton" ) );
    _back_button->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onBackButtonTouched, this ) );
    
    _start_button = dynamic_cast<ui::Button*>( _background_node->getChildByName( "battleButton" ) );
    _start_button->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onStartButtonTouched, this ) );
    
    ui::Text* mission_label_1 = dynamic_cast<ui::Text*>( _background_node->getChildByName( "mission1" ) );
    ui::Text* mission_label_2 = dynamic_cast<ui::Text*>( _background_node->getChildByName( "mission2" ) );
    ui::Text* mission_label_3 = dynamic_cast<ui::Text*>( _background_node->getChildByName( "mission3" ) );
    _mission_labels.push_back( mission_label_1 );
    _mission_labels.push_back( mission_label_2 );
    _mission_labels.push_back( mission_label_3 );
    
    ui::Button* btn_store = dynamic_cast<ui::Button*>( _main_node->getChildByName( "btn_store" ) );
    btn_store->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onStoreTouched, this ) );
    
    ui::Button* btn_team_skill = dynamic_cast<ui::Button*>( _main_node->getChildByName( "btn_team_skill" ) );
    btn_team_skill->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onTeamSkillTouched, this ) );
    
    ui::Button* btn_hero = dynamic_cast<ui::Button*>( _main_node->getChildByName( "btn_hero" ) );
    btn_hero->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onHeroTouched, this ) );
    
    _level_info_label = dynamic_cast<ui::Text*>( _background_node->getChildByName( "missionText" ) );
    
    const ValueMap& level_config = ResourceManager::getInstance()->getLevelConfig();
    
    int total_level_count = level_config.at( "levels" ).asValueVector().size();
    for( int i = 1; i <= total_level_count; i++ ) {
        std::string button_name = Utils::stringFormat( "level%dButton", i );
        ui::Button* button = dynamic_cast<ui::Button*>( _scrollview->getChildByName( button_name ) );
        button->setTag( i );
        button->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onLevelTouched, this ) );
    }
    
    ValueVector deployed_units = PlayerInfo::getInstance()->getPlayerDeployedUnitsInfo();
    for( int i = 0; i < deployed_units.size(); i++ ) {
        const ValueMap& info = deployed_units.at( i ).asValueMap();
        Sprite* shadow = dynamic_cast<Sprite*>( _main_node->getChildByName( Utils::stringFormat( "hero_%d", i + 1 ) ) );
        if( shadow ) {
            shadow->setVisible( true );
            std::string name = info.at( "name" ).asString();
            std::string resource = ResourceManager::getInstance()->getPathForResource( name, eResourceType::Character_Front );
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
            skeleton->setAnimation( 0, "Idle", true );
            skeleton->setPosition( Point( shadow->getContentSize().width / 2, shadow->getContentSize().height / 2 ) );
            shadow->addChild( skeleton );
        }
    }
    
    return true;
}

void UILevelChooseLayer::onStartButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        cocos2d::Vector<cocos2d::Ref*> a_ref_params;
        a_ref_params.pushBack( _map_data );
        
        cocos2d::ValueMap a_value_params;
        a_value_params["is_pvp"] = Value( _is_pvp );
        
        SceneConfig* scene_config = SceneConfig::create( a_ref_params, a_value_params );
        SceneManager::getInstance()->transitToScene( eSceneName::SceneBattle, scene_config );
    }
}

void UILevelChooseLayer::onBackButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        _level_node->setVisible( false );
        _main_node->setVisible( true );
    }
}

void UILevelChooseLayer::onLevelTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        _main_node->setVisible( false );
        _level_node->setVisible( true );
        auto node = dynamic_cast<Node*>( sender );
        int tag = node->getTag();
        const ValueMap& level_config = ResourceManager::getInstance()->getLevelConfig();
        const ValueMap& level_obj = level_config.at( "levels" ).asValueVector().at( tag - 1 ).asValueMap();
        MapData* new_map_data = MapData::create( level_obj.at( "path" ).asString() );
        this->setMapData( new_map_data );
        _is_pvp = false;
        auto itr = level_obj.find( "is_pvp" );
        if( itr != level_obj.end() ) {
            _is_pvp = itr->second.asBool();
        }
        
        const ValueMap& meta_json = _map_data->getMetaJson();
        itr = meta_json.find( "tasks" );
        if( itr != meta_json.end() ) {
            const ValueVector& task_array = itr->second.asValueVector();
            int task_count = (int)task_array.size();
            for( int i = 0; i < _mission_labels.size(); i++ ) {
                if( i < task_count ) {
                    _mission_labels[i]->setString( task_array.at( i ).asValueMap().at( "desc" ).asString() );
                }
                else {
                    _mission_labels[i]->setString( "" );
                }
            }
        }
        itr = meta_json.find( "desc" );
        if( itr != meta_json.end() ) {
            _level_info_label->setString( itr->second.asString() );
        }
        else {
            _level_info_label->setString( "" );
        }
    }
}

void UILevelChooseLayer::onStoreTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
    }
}

void UILevelChooseLayer::onHeroTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        UIHeroManageLayer* hero_layer = UIHeroManageLayer::create();
        this->addChild( hero_layer, 2 );
    }
}

void UILevelChooseLayer::onTeamSkillTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ){
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {

    }
}

void UILevelChooseLayer::setMapData( MapData* map_data ) {
    CC_SAFE_RELEASE( _map_data );
    _map_data = map_data;
    CC_SAFE_RETAIN( _map_data );
}