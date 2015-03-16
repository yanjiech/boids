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

using namespace cocos2d;

UILevelChooseLayer::UILevelChooseLayer() :
_map_data( nullptr ) {
    
}

UILevelChooseLayer::~UILevelChooseLayer() {
    if( _map_data ) {
        _map_data->release();
    }
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
#define MAIN_CSB_FILE "level_choose/main/main.csb"
#define LEVEL_CSB_FILE "level_choose/level/level.csb"
    
    std::string main_csb_file = FileUtils::getInstance()->fullPathForFilename( MAIN_CSB_FILE );
    _main_node = cocos2d::CSLoader::getInstance()->createNode( main_csb_file );
    this->addChild( _main_node );
    
    std::string level_csb_file = FileUtils::getInstance()->fullPathForFilename( LEVEL_CSB_FILE );
    _level_node = cocos2d::CSLoader::getInstance()->createNode( level_csb_file.c_str() );
    this->addChild( _level_node );
    _level_node->setVisible( false );
    
    _scrollview = dynamic_cast<ui::ScrollView*>( _main_node->getChildByName( "scrollView" ) );
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
    
    _level_info_label = dynamic_cast<ui::Text*>( _background_node->getChildByName( "missionText" ) );
    
    const rapidjson::Document& level_config = ResourceManager::getInstance()->getLevelConfig();
    
    int total_level_count = level_config["levels"].Size();
    for( int i = 1; i <= total_level_count; i++ ) {
        std::string button_name = Utils::stringFormat( "level%dButton", i );
        ui::Button* button = dynamic_cast<ui::Button*>( _scrollview->getChildByName( button_name ) );
        button->setTag( i );
        button->addTouchEventListener( CC_CALLBACK_2( UILevelChooseLayer::onLevelTouched, this ) );
    }
    
    return true;
}

void UILevelChooseLayer::onStartButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        SceneConfig scene_config;
        scene_config.ref_params.push_back( _map_data );
        scene_config.value_params["is_pvp"] = Value( _is_pvp );
        SceneManager::getInstance()->transitToScene( eSceneName::Battle, &scene_config );
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
        const rapidjson::Document& level_config = ResourceManager::getInstance()->getLevelConfig();
        const rapidjson::Value& level_obj = level_config["levels"][tag-1];
        MapData* new_map_data = new MapData( level_obj["path"].GetString() );
        this->setMapData( new_map_data );
        _is_pvp = level_obj.HasMember( "is_pvp" ) ? level_obj["is_pvp"].GetBool() : false;
        
        const rapidjson::Document& meta_json = _map_data->getMetaJson();
        if( meta_json.HasMember( "tasks" ) ) {
            const rapidjson::Value& task_array = meta_json["tasks"];
            int task_count = (int)task_array.Size();
            for( int i = 0; i < _mission_labels.size(); i++ ) {
                if( i < task_count ) {
                    _mission_labels[i]->setString( task_array[i]["desc"].GetString() );
                }
                else {
                    _mission_labels[i]->setString( "" );
                }
            }
        }
        if( meta_json.HasMember( "desc" ) ) {
            _level_info_label->setString( meta_json["desc"].GetString() );
        }
        else {
            _level_info_label->setString( "" );
        }
    }
}

void UILevelChooseLayer::setMapData( MapData* map_data ) {
    if( _map_data ) {
        _map_data->release();
    }
    _map_data = map_data;
}