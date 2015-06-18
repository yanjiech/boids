//
//  UIHomeLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "UIHomeLayer.h"
#include "../Utils.h"
#include "../manager/SceneManager.h"
#include "../manager/ResourceManager.h"
#include "../data/PlayerInfo.h"
#include "UIHeroManageLayer.h"
#include "UITeamTalentLayer.h"
#include "../util/CocosUtils.h"

#define MAIN_CSB_FILE "ui/page/ui_home_page.csb"
#define LEVEL_CSB_FILE "ui/page/ui_home_detail.csb"

using namespace cocos2d;
using namespace cocostudio::timeline;

UIHomeLayer::UIHomeLayer() :
_map_data( nullptr ) {
    
}

UIHomeLayer::~UIHomeLayer() {
    CC_SAFE_RELEASE( _map_data );
    ActionTimelineCache::getInstance()->removeAction( MAIN_CSB_FILE );
}

UIHomeLayer* UIHomeLayer::create() {
    UIHomeLayer* ret = new UIHomeLayer();
    if( ret && ret->init() ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHomeLayer::init() {
    //detail
    std::string level_csb_file = FileUtils::getInstance()->fullPathForFilename( LEVEL_CSB_FILE );
    _level_node = cocos2d::CSLoader::getInstance()->createNode( level_csb_file );
    this->addChild( _level_node );
    
    _background_node = dynamic_cast<Sprite*>( _level_node->getChildByName( "background" ) );
    ui::Button* btn_back = dynamic_cast<ui::Button*>( _level_node->getChildByName( "backButton" ) );
    btn_back->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onBackButtonTouched, this ) );
    
    ui::Button* btn_start = dynamic_cast<ui::Button*>( _level_node->getChildByName( "battleButton" ) );
    btn_start->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onStartButtonTouched, this ) );
    
    ui::Text* mission_label_1 = dynamic_cast<ui::Text*>( _level_node->getChildByName( "lb_mission_1" ) );
    ui::Text* mission_label_2 = dynamic_cast<ui::Text*>( _level_node->getChildByName( "lb_mission_2" ) );
    ui::Text* mission_label_3 = dynamic_cast<ui::Text*>( _level_node->getChildByName( "lb_mission_3" ) );
    _mission_labels.pushBack( mission_label_1 );
    _mission_labels.pushBack( mission_label_2 );
    _mission_labels.pushBack( mission_label_3 );
    
    Sprite* sp_star_1 = dynamic_cast<Sprite*>( _level_node->getChildByName( "sp_star_1" ) );
    Sprite* sp_star_2 = dynamic_cast<Sprite*>( _level_node->getChildByName( "sp_star_2" ) );
    Sprite* sp_star_3 = dynamic_cast<Sprite*>( _level_node->getChildByName( "sp_star_3" ) );
    _stars.pushBack( sp_star_1 );
    _stars.pushBack( sp_star_2 );
    _stars.pushBack( sp_star_3 );
    
    _level_node->setVisible( false );
    
    _lb_title = dynamic_cast<ui::Text*>( _level_node->getChildByName( "lb_title" ) );
    _lb_title->setAdditionalKerning( -4.0f );
    _lb_level_desc = dynamic_cast<ui::Text*>( _level_node->getChildByName( "lb_level_desc" ) );
    _sp_boss_frame = dynamic_cast<Sprite*>( _level_node->getChildByName( "sp_boss_frame" ) );
    
    _nd_diff = _level_node->getChildByName( "nd_diff" );
    
    //main
    std::string main_csb_file = FileUtils::getInstance()->fullPathForFilename( MAIN_CSB_FILE );
    _main_node = cocos2d::CSLoader::getInstance()->createNode( main_csb_file );
    this->addChild( _main_node );
    
    _panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( main_csb_file );
    
    _lb_player_name = dynamic_cast<ui::Text*>( _main_node->getChildByName( "lb_player_name" ) );
    _lb_player_name->setAdditionalKerning( -5.0f );
    
    _lb_team_level = dynamic_cast<ui::Text*>( _main_node->getChildByName( "lb_team_lv" ) );
    
    ui::Layout* pn_wheel = dynamic_cast<ui::Layout*>( _main_node->getChildByName( "pn_wheel" ) );
    pn_wheel->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onDifficultyTouched, this ) );
    
    _scrollview = dynamic_cast<ui::ScrollView*>( _main_node->getChildByName( "mapScrollView" ) );
    
    ui::Button* btn_store = dynamic_cast<ui::Button*>( _main_node->getChildByName( "btn_store" ) );
    btn_store->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onStoreTouched, this ) );
    
    ui::Button* btn_team_skill = dynamic_cast<ui::Button*>( _main_node->getChildByName( "btn_team_skill" ) );
    btn_team_skill->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onTeamSkillTouched, this ) );
    
    ui::Button* btn_hero = dynamic_cast<ui::Button*>( _main_node->getChildByName( "btn_hero" ) );
    btn_hero->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onHeroTouched, this ) );
    
    this->loadMapOfDifficulty( 1 );
    
    this->loadDeployedHeros();
    
    for( auto node : _scrollview->getChildren() ) {
        ui::Button* btn = dynamic_cast<ui::Button*>( node );
        if( btn ) {
            btn->addTouchEventListener( CC_CALLBACK_2( UIHomeLayer::onLevelTouched, this ) );
        }
    }
    
    _currency_layer = UICurrencyLayer::create();
    this->addChild( _currency_layer, 1000 );
    
    _difficulty = eLevelDifficulty::LevelDiffEasy;
    
    this->updatePlayerInfo( PlayerInfo::getInstance() );
    
    return true;
}

void UIHomeLayer::onEnterTransitionDidFinish() {
    Layer::onEnterTransitionDidFinish();
    PlayerInfo::getInstance()->registerListener( PLAYER_INFO_BASE_INFO, this );
    this->updatePlayerInfo( PlayerInfo::getInstance() );
}

void UIHomeLayer::onExitTransitionDidStart() {
    Layer::onExitTransitionDidStart();
    PlayerInfo::getInstance()->unregisterListener( PLAYER_INFO_BASE_INFO, this );
}

void UIHomeLayer::updatePlayerInfo( PlayerInfo* player_info ) {
    _lb_player_name->setString( player_info->getPlayerName() );
    _lb_team_level->setString( Utils::toStr( player_info->getTeamLevel() ) );
}

void UIHomeLayer::onStartButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        cocos2d::Vector<cocos2d::Ref*> a_ref_params;
        a_ref_params.pushBack( _map_data );
        
        cocos2d::ValueMap a_value_params;
        a_value_params["is_pvp"] = Value( _is_pvp );
        a_value_params["level_id"] = Value( _level_id );
        
        SceneConfig* scene_config = SceneConfig::create( a_ref_params, a_value_params );
        SceneManager::getInstance()->transitToScene( eSceneName::SceneBattle, scene_config );
    }
}

void UIHomeLayer::onBackButtonTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        _level_node->setVisible( false );
        _main_node->setVisible( true );
        _currency_layer->setVisible( true );
    }
}

void UIHomeLayer::onLevelTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        _main_node->setVisible( false );
        _level_node->setVisible( true );
        _currency_layer->setVisible( false );
        auto node = dynamic_cast<Node*>( sender );
        int tag = node->getTag();
        _level_id = Utils::stringFormat( "%d%03d", (int)_difficulty, tag );
        const ValueMap& all_level_config = ResourceManager::getInstance()->getLevelConfig();
        const ValueMap& level_config = all_level_config.at( _level_id ).asValueMap();
        MapData* new_map_data = MapData::create( level_config.at( "map_res" ).asString(), level_config.at( "map_meta" ).asString() );
        this->setMapData( new_map_data );
        _is_pvp = false;
        auto itr = level_config.find( "is_pvp" );
        if( itr != level_config.end() ) {
            _is_pvp = itr->second.asBool();
        }
        
        const ValueMap& meta_json = _map_data->getMetaJson();
        itr = meta_json.find( "tasks" );
        if( itr != meta_json.end() ) {
            const ValueVector& task_array = itr->second.asValueVector();
            int task_count = (int)task_array.size();
            for( int i = 0; i < _mission_labels.size(); i++ ) {
                if( i < task_count ) {
                    _mission_labels.at( i )->setString( task_array.at( i ).asValueMap().at( "desc" ).asString() );
                }
                else {
                    _mission_labels.at( i )->setString( "" );
                }
            }
        }
        
        int star = PlayerInfo::getInstance()->getLevelStar( Utils::toInt( _level_id ) );
        for( int i = 0; i < star; i++ ) {
            _stars.at( i )->setVisible( true );
        }
        if( star < 3 ) {
            for( int i = star; i < 3; i++ ) {
                _stars.at( i )->setVisible( false );
            }
        }
        
        const ValueMap& level_conf = ResourceManager::getInstance()->getLevelConfig().at( _level_id ).asValueMap();
        _lb_title->setString( level_conf.at( "name" ).asString() );
        _lb_level_desc->setString( level_conf.at( "desc" ).asString() );
        
        _nd_diff->removeAllChildren();
        if( _difficulty == eLevelDifficulty::LevelDiffEasy ) {
            Sprite* sp_diff = Sprite::createWithSpriteFrameName( "ui_home_detail_putong.png" );
            _nd_diff->addChild( sp_diff );
        }
        else if( _difficulty == eLevelDifficulty::LevelDiffMedium ) {
            Sprite* sp_diff = Sprite::createWithSpriteFrameName( "ui_home_detail_jingying.png" );
            _nd_diff->addChild( sp_diff );
        }
    }
}

void UIHomeLayer::onStoreTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
    }
}

void UIHomeLayer::onHeroTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        UIHeroManageLayer* hero_layer = UIHeroManageLayer::create();
        this->addChild( hero_layer, 2 );
    }
}

void UIHomeLayer::onTeamSkillTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ){
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        UITeamTalentLayer* talent_layer = UITeamTalentLayer::create();
        this->addChild( talent_layer, 2 );
    }
}

eLevelDifficulty UIHomeLayer::getDifficulty() {
    return _difficulty;
}

void UIHomeLayer::setDifficulty( eLevelDifficulty diff ) {
    if( _difficulty != diff ) {
        _difficulty = diff;
        this->stopAction( _panel_action );
        this->runAction( _panel_action );
        switch( _difficulty ) {
            case eLevelDifficulty::LevelDiffEasy:
                _panel_action->play( "to_easy", false );
                break;
            case eLevelDifficulty::LevelDiffMedium:
                _panel_action->play( "to_medium", false );
                break;
            case eLevelDifficulty::LevelDiffHard:
                _panel_action->play( "to_hard", false );
                break;
            default:
                break;
        }
    }
}

void UIHomeLayer::onDifficultyTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        int old_diff = (int)_difficulty;
        int new_diff = old_diff % 2 + 1;
        this->setDifficulty( (eLevelDifficulty)( new_diff ) );
        this->loadMapOfDifficulty( new_diff );
    }
}

void UIHomeLayer::setMapData( MapData* map_data ) {
    CC_SAFE_RETAIN( map_data );
    CC_SAFE_RELEASE( _map_data );
    _map_data = map_data;
}

void UIHomeLayer::loadDeployedHeros() {
    ValueVector deployed_units = PlayerInfo::getInstance()->getPlayerDeployedUnitsInfo();
    for( int i = 0; i < 5; i++ ) {
        Sprite* shadow = dynamic_cast<Sprite*>( _main_node->getChildByName( Utils::stringFormat( "hero_%d", i + 1 ) ) );
        if( shadow ) {
            shadow->removeAllChildren();
            if( i < deployed_units.size() ) {
                shadow->setVisible( true );
                 const ValueMap& info = deployed_units.at( i ).asValueMap();
                std::string name = info.at( "name" ).asString();
                std::string resource = ResourceManager::getInstance()->getPathForResource( name, eResourceType::Character_Front );
                spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
                skeleton->setAnimation( 0, "Idle", true );
                skeleton->setPosition( Point( shadow->getContentSize().width / 2, shadow->getContentSize().height / 2 ) );
                shadow->addChild( skeleton );
                
                const ValueMap& unit_config = ResourceManager::getInstance()->getUnitData( name );
                skeleton->setScale( unit_config.at( "scale" ).asFloat() * 1.2f );
            }
            else {
                shadow->setVisible( false );
            }
        }
    }
}

void UIHomeLayer::becomeTopLayer() {
    this->loadDeployedHeros();
}

void UIHomeLayer::loadMapOfDifficulty( int diff ) {
    PlayerInfo* player_info = PlayerInfo::getInstance();
    const ValueMap& all_level_config = ResourceManager::getInstance()->getLevelConfig();
    int count = (int)all_level_config.size() / 2;
    for( int i = 1; i <= count; i++ ) {
        int level_id = diff * 1000 + i;
        std::string btn_name = Utils::stringFormat( "btn_level_%d", i );
        ui::Button* btn = dynamic_cast<ui::Button*>( _scrollview->getChildByName( btn_name ) );
        ui::Layout* pn_star = dynamic_cast<ui::Layout*>( _scrollview->getChildByName( Utils::stringFormat( "pn_level_%d", i ) ) );
        switch( diff ) {
            case 1:
            {
                //easy difficulty
                if( player_info->isLevelCompleted( level_id ) ) {
                    pn_star->setVisible( true );
                    int level_star = player_info->getLevelStar( level_id );
                    int j;
                    for( j = 1; j <= level_star; j++ ) {
                        Sprite* sp_star = dynamic_cast<Sprite*>( pn_star->getChildByName( Utils::stringFormat( "star_%d", j ) ) );
                        sp_star->setVisible( true );
                    }
                    for( int k = j; k <= 3; k++ ) {
                        Sprite* sp_star = dynamic_cast<Sprite*>( pn_star->getChildByName( Utils::stringFormat( "star_%d", k ) ) );
                        sp_star->setVisible( false );
                    }
                    
                    btn->setVisible( true );
                    btn->loadTextureNormal( "ui_home_cp_completed.png", ui::Widget::TextureResType::PLIST );
                    btn->loadTexturePressed( "ui_home_cp_completed.png", ui::Widget::TextureResType::PLIST );
                }
                else {
                    pn_star->setVisible( false );
                    if( ( i == 1 ) || player_info->isLevelCompleted( level_id - 1 ) ) {
                        btn->setVisible( true );
                        btn->loadTextureNormal( "ui_home_cp_opened.png", ui::Widget::TextureResType::PLIST );
                        btn->loadTexturePressed( "ui_home_cp_opened.png", ui::Widget::TextureResType::PLIST );
                    }
                    else {
                        btn->setVisible( false );
                    }
                }
            }
                break;
            case 2:
            {
                //medium difficulty
                if( player_info->isLevelCompleted( level_id ) ) {
                    pn_star->setVisible( true );
                    int level_star = player_info->getLevelStar( level_id );
                    int j;
                    for( j = 1; j <= level_star; j++ ) {
                        Sprite* sp_star = dynamic_cast<Sprite*>( pn_star->getChildByName( Utils::stringFormat( "star_%d", j ) ) );
                        sp_star->setVisible( true );
                    }
                    for( int k = j; k <= 3; k++ ) {
                        Sprite* sp_star = dynamic_cast<Sprite*>( pn_star->getChildByName( Utils::stringFormat( "star_%d", k ) ) );
                        sp_star->setVisible( false );
                    }
                    
                    btn->setVisible( true );
                    btn->loadTextureNormal( "ui_home_cp_completed.png", ui::Widget::TextureResType::PLIST );
                    btn->loadTexturePressed( "ui_home_cp_completed.png", ui::Widget::TextureResType::PLIST );
                }
                else {
                    pn_star->setVisible( false );
                    if( player_info->getLevelStar( level_id - 1000 ) == 3 ) {
                        btn->setVisible( true );
                        btn->loadTextureNormal( "ui_home_cp_opened.png", ui::Widget::TextureResType::PLIST );
                        btn->loadTexturePressed( "ui_home_cp_opened.png", ui::Widget::TextureResType::PLIST );
                    }
                    else {
                        btn->setVisible( false );
                    }
                }
            }
                
                break;
            case 3:
                btn->setVisible( false );
                break;
            default:
                break;
        }
    }
}