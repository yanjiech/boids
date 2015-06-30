//
//  TutorialLayer.cpp
//  Boids
//
//  Created by chenyanjie on 6/29/15.
//
//

#include "TutorialLayer.h"

using namespace cocos2d;

#define TUTORIAL_FILE "ui/page/ui_tutorial.csb"

TutorialLayer::TutorialLayer() :
_prompt( nullptr )
{
    
}

TutorialLayer::~TutorialLayer() {
    
}

TutorialLayer* TutorialLayer::create( eTutorialType type  ) {
    TutorialLayer* ret = new TutorialLayer();
    if( ret && ret->init( type ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TutorialLayer::init( eTutorialType type  ) {
    if( !Layer::init() ) {
        return false;
    }
    
    _type = type;
    _state = eTutorialState::TutorialStateUnstarted;
    _step = 0;
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( TutorialLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( TutorialLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( TutorialLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( TutorialLayer::onTouchEnded, this );
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    if( _type == eTutorialType::TutorialPrepare ) {
        this->schedule( CC_CALLBACK_1( TutorialLayer::updateTutorial, this ), "tutorial_updatge" );
    }
    
    _touchable_rect = Rect::ZERO;
    
    std::string root_csb_file = FileUtils::getInstance()->fullPathForFilename( TUTORIAL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( root_csb_file );
    this->addChild( _root_node );
    
    for( auto panel : _root_node->getChildren() ) {
        panel->setVisible( false );
    }
    
    return true;
}

bool TutorialLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    bool ret = false;
    if( _type == eTutorialType::TutorialPrepare ) {
        Point pos = this->convertTouchToNodeSpace( touch );
        bool contains_touch = _touchable_rect.containsPoint( pos );
        
        if( _state == TutorialStatePlaying ) {
            if( _touchable_rect.equals( Rect::ZERO ) ) {
                ret = true;
                this->gotoNextStep();
            }
            else if( contains_touch ) {
                ret = false;
                this->gotoNextStep();
            }
            else {
                ret = true;
            }
        }
        else if( _state == TutorialStateWaiting ) {
            ret = !contains_touch;
        }
        else {
            ret = true;
        }
    }
    return ret;
}

void TutorialLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void TutorialLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void TutorialLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void TutorialLayer::updateTutorial( float delta ) {
    switch( _state ) {
        case TutorialStateUnstarted:
            this->gotoNextStep();
            break;
        case TutorialStateWaiting:
            if( _step == 5 ) {
                Node* home_layer = this->getParent();
                Node* hero_2 = home_layer->getChildByName( "hero_layer" )->getChildByName( "root_node" )->getChildByName( "choose_panel" )->getChildByName( "choose_panel" )->getChildByName( "hero_2" );
                UIHeroDeploySlot* slot = dynamic_cast<UIHeroDeploySlot*>( hero_2->getChildren().at( 0 ) );
                if( slot->getHeroId() != "0" ) {
                    this->gotoNextStep();
                }
            }
            else if( _step == 7 ) {
                if( PlayerInfo::getInstance()->getOwnedUnitsInfo().at( "3" ).asValueMap().at( "equips" ).asValueMap().at( "weapon" ).asString() != "0" ) {
                    this->gotoNextStep();
                }
            }
            break;
        default:
            break;
    }
}

void TutorialLayer::onEnter() {
    Layer::onEnter();
    if( _type == eTutorialType::TutorialBattle ) {
        BattleLayer* battle_layer = dynamic_cast<BattleLayer*>( this->getParent() );
        battle_layer->getStoryLayer()->setStoryEndCallback( CC_CALLBACK_1( TutorialLayer::onStoryEnd, this ) );
        battle_layer->getUIBattleLayer()->hideSkillNodes();
    }
}

void TutorialLayer::onStoryEnd( UIStoryLayer* story ) {
    if( _type == eTutorialType::TutorialBattle ) {
        BattleLayer* battle_layer = dynamic_cast<BattleLayer*>( this->getParent() );
        battle_layer->endStory( story );
    }
}

void TutorialLayer::gotoNextStep() {
    ++_step;
    
    Node* current_panel = nullptr;
    for( auto panel : _root_node->getChildren() ) {
        if( panel->getName() == Utils::stringFormat( "step_%d", _step ) ) {
            panel->setVisible( true );
            current_panel = panel;
        }
        else {
            panel->setVisible( false );
        }
    }
    
    UIHomeLayer* home_layer = dynamic_cast<UIHomeLayer*>( this->getParent() );
    switch( _step ) {
        case 1:
        case 2:
            _state = TutorialStatePlaying;
            _touchable_rect = Rect::ZERO;
            break;
        case 3:
        {
            _state = TutorialStatePlaying;
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_touch_down" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "animation", true );
            Node* btn_hero = home_layer->getChildByName( "main_node" )->getChildByName( "btn_hero" );
            
            Size size = btn_hero->getContentSize();
            Point anchor_point = btn_hero->getAnchorPoint();
            Point local_pos = btn_hero->getPosition();
            _touchable_rect = Rect( local_pos.x - size.width * anchor_point.x, local_pos.y - size.height * anchor_point.y, size.width, size.height );
            skeleton->setPosition( Point( _touchable_rect.getMidX(), _touchable_rect.getMidY() ) );
        }
            break;
        case 4:
            _state = TutorialStatePlaying;
            _touchable_rect = Rect::ZERO;
            break;
        case 5:
        {
            _state = TutorialStateWaiting;
            Node* hero_layer = home_layer->getChildByName( "hero_layer" );
            ui::PageView* page_view = dynamic_cast<ui::PageView*>( hero_layer->getChildByName("root_node" )->getChildByName( "pv_hero_list" ) );
            Node* hero = page_view->getPage( 0 )->getChildByName( "3" );
            
            Point world_pos = hero->getParent()->convertToWorldSpace( hero->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            Point anchor_point = hero->getAnchorPoint();
            Size size = hero->getContentSize();
            _touchable_rect = Rect( local_pos.x - size.width * anchor_point.x, local_pos.y - size.height * anchor_point.y, size.width, size.height );
            
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_wheel" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "Animation-R", true );
            skeleton->setRotation( 15.0f );
            skeleton->setPosition( _touchable_rect.getMidX(), _touchable_rect.getMidY() );
            
            Node* hero_2 = hero_layer->getChildByName( "root_node" )->getChildByName( "choose_panel" )->getChildByName( "choose_panel" )->getChildByName( "hero_2" );
            UIHeroDeploySlot* slot = dynamic_cast<UIHeroDeploySlot*>( hero_2->getChildren().at( 0 ) );
            slot->setSelected( true );
        }
            break;
        case 6:
        {
            _state = TutorialStatePlaying;
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_touch_down" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "animation", true );
            
            Node* btn_equip = home_layer->getChildByName( "hero_layer" )->getChildByName("root_node")->getChildByName( "skill_equip_panel" )->getChildByName( "btn_equip" );
            Size size = btn_equip->getContentSize();
            Point anchor_point = btn_equip->getAnchorPoint();
            Point world_pos = btn_equip->getParent()->convertToWorldSpace( btn_equip->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            _touchable_rect = Rect( local_pos.x - size.width * anchor_point.x, local_pos.y - size.height * anchor_point.y, size.width, size.height );
            skeleton->setPosition( Point( _touchable_rect.getMidX(), _touchable_rect.getMidY() ) );
        }
            break;
        case 7:
        {
            _state = TutorialStateWaiting;
            
            Node* equip_layer = home_layer->getChildByName( "hero_layer" )->getChildByName( "detail_layer" );
            ui::PageView* pv_weapon = dynamic_cast<ui::PageView*>( equip_layer->getChildByName( "root_node" )->getChildByName( "equipPanel" )->getChildByName( "pv_weapon_list" ) );
            
            Node* weapon = pv_weapon->getPage( 0 )->getChildren().at( 0 );
            Point world_pos = weapon->getParent()->convertToWorldSpace( weapon->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            Point anchor_point = weapon->getAnchorPoint();
            Size size = weapon->getContentSize();
            _touchable_rect = Rect( local_pos.x - size.width * anchor_point.x, local_pos.y - size.height * anchor_point.y, size.width, size.height );
            
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_wheel" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "Animation-U", true );
            skeleton->setPosition( _touchable_rect.getMidX(), _touchable_rect.getMidY() );
        }
            break;
        case 8:
        {
            _state = TutorialStatePlaying;
            
            Node* equip_layer = home_layer->getChildByName( "hero_layer" )->getChildByName( "detail_layer" );
            Node* btn_back = equip_layer->getChildByName( "root_node" )->getChildByName( "btn_back" );
            Point world_pos = btn_back->getParent()->convertToWorldSpace( btn_back->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            Point anchor_point = btn_back->getAnchorPoint();
            Size size = btn_back->getContentSize();
            _touchable_rect = Rect( local_pos.x - size.width * anchor_point.x, local_pos.y - size.height * anchor_point.y, size.width, size.height );
            
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_touch_down" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "animation", true );
            skeleton->setPosition( Point( _touchable_rect.getMidX(), _touchable_rect.getMidY() ) );
        }
            break;
        case 9:
        {
            _state = TutorialStatePlaying;
            
            Node* hero_layer = home_layer->getChildByName( "hero_layer" );
            Node* btn_back = hero_layer->getChildByName( "root_node" )->getChildByName( "btn_finish" );
            Point world_pos = btn_back->getParent()->convertToWorldSpace( btn_back->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            Point anchor_point = btn_back->getAnchorPoint();
            Size size = btn_back->getContentSize();
            _touchable_rect = Rect( local_pos.x - size.width * anchor_point.x, local_pos.y - size.height * anchor_point.y, size.width, size.height );
            
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_touch_down" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "animation", true );
            skeleton->setPosition( Point( _touchable_rect.getMidX(), _touchable_rect.getMidY() ) );
        }
            break;
        case 10:
        {
            _state = TutorialStatePlaying;
            _touchable_rect = Rect::ZERO;
            
            Node* nd_level = home_layer->getChildByName( "main_node" )->getChildByName( "mapScrollView" )->getChildByName( "btn_level_1" );
            
            Point world_pos = nd_level->getParent()->convertToWorldSpace( nd_level->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_touch_down" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "animation", true );
            skeleton->setPosition( local_pos );
        }
            break;
        case 11:
        {
            _state = TutorialStatePlaying;
            _touchable_rect = Rect::ZERO;
            
            Node* nd_diff = home_layer->getChildByName( "main_node" )->getChildByName( "pn_wheel" );
            
            Point world_pos = nd_diff->getParent()->convertToWorldSpace( nd_diff->getPosition() );
            Point local_pos = this->convertToNodeSpace( world_pos );
            Size size = nd_diff->getContentSize();
            
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( "ui/effect_touch_down" );
            current_panel->addChild( skeleton );
            skeleton->setAnimation( 0, "animation", true );
            skeleton->setPosition( local_pos + Point( size.width / 2, 0 ) );

        }
            break;
        case 12:
            _state = TutorialStatePlaying;
            _touchable_rect = Rect::ZERO;
            break;
        default:
            //finished
            PlayerInfo::getInstance()->setNewUser( false );
            this->removeFromParent();
            break;
    }
}