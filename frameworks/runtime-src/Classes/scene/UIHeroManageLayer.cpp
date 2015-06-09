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
#include "../ArmatureManager.h"
#include "../manager/ResourceManager.h"
#include "../util/CocosUtils.h"

#define HERO_MANAGE_FILE "ui/page/ui_hero_manage.csb"

#define HERO_LIST_CELL_WIDTH 185
#define HERO_LIST_CELL_HEIGHT 150
#define HERO_DEPLOY_CELL_WIDTH 138
#define HERO_DEPLOY_CELL_HEIGHT 138

using namespace cocos2d;
using namespace cocostudio::timeline;

//hero slot
UIHeroManageHeroSlot::UIHeroManageHeroSlot() :
_hero_avatar( nullptr ),
_hero_data( nullptr ),
_weapon_data( nullptr ),
_armor_data( nullptr ),
_boot_data( nullptr ),
_accessory_data( nullptr ),
_hero_skeleton( nullptr ),
_selected_sprite( nullptr ),
_hero_rect_avatar( nullptr )
{
    
}

UIHeroManageHeroSlot::~UIHeroManageHeroSlot() {
    CC_SAFE_RELEASE( _hero_skeleton );
    CC_SAFE_RELEASE( _hero_data );
    CC_SAFE_RELEASE( _hero_rect_avatar );
    CC_SAFE_RELEASE( _weapon_data );
    CC_SAFE_RELEASE( _armor_data );
    CC_SAFE_RELEASE( _boot_data );
    CC_SAFE_RELEASE( _accessory_data );
}

UIHeroManageHeroSlot* UIHeroManageHeroSlot::create( const cocos2d::ValueMap& data, const std::string& hero_id, int flag ) {
    UIHeroManageHeroSlot* ret = new UIHeroManageHeroSlot();
    if( ret && ret->init( data, hero_id, flag ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroManageHeroSlot::init( const cocos2d::ValueMap& data, const std::string& hero_id, int flag ) {
    if( !Node::init() ) {
        return false;
    }
    
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    _hero_id = hero_id;
    this->setFlag( flag );
    
    this->setContentSize( Size( HERO_LIST_CELL_WIDTH, HERO_LIST_CELL_HEIGHT ) );
    _lock_sprite = Sprite::createWithSpriteFrameName( "ui_hero_lock.png" );
    _deployed_sprite = Sprite::createWithSpriteFrameName( "ui_hero_deployed.png" );
    _deployed_sprite->setPosition( Point( this->getContentSize().width - 50.0f, 20.0f ) );
    
    _lock_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _lock_sprite, 4 );

    this->addChild( _deployed_sprite, 5 );
    
    this->setDeployed( false );
    this->setSelected( false );
    
    this->loadHeroInfo( data );
    
    return true;
}

void UIHeroManageHeroSlot::loadHeroInfo( const cocos2d::ValueMap& data ) {
    if( data.empty() ) {
        this->setHeroSkeleton( nullptr );
        this->setUnitData( nullptr );
    }
    else {
        UnitData* unit_data = UnitData::create( data );
        this->setUnitData( unit_data );
        
        const std::string& hero_name = unit_data->name;

        Sprite* avatar = Sprite::createWithSpriteFrameName( "ui_hero_p_" + hero_name + ".png" );
        this->setHeroAvatar( avatar );
        
        avatar = Sprite::createWithSpriteFrameName( "ui_hero_f_" + hero_name + ".png" );
        this->setHeroRectAvatar( avatar );
        
        auto itr = data.find( "owned" );
        if( itr != data.end() ) {
            this->setOwned( itr->second.asBool() );
        }
        else {
            this->setOwned( false );
        }
        
        itr = data.find( "locked" );
        if( itr != data.end() ) {
            this->setLocked( itr->second.asBool() );
        }
        else {
            this->setLocked( false );
        }
        
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( ResourceManager::getInstance()->getPathForResource( hero_name, eResourceType::Character_Front ) );
        skeleton->setScale( 2.0f * unit_data->scale );
        this->setHeroSkeleton( skeleton );
    }
}

int UIHeroManageHeroSlot::getFlag() {
    return _flag;
}

void UIHeroManageHeroSlot::setFlag( int flag ) {
    _flag = flag;
}

bool UIHeroManageHeroSlot::isSelected() {
    return _is_selected;
}

void UIHeroManageHeroSlot::setSelected( bool b ) {
    _is_selected = b;
    if( _selected_sprite != nullptr ) {
        if( _is_selected ) {
            _selected_sprite->setVisible( true );
        }
        else {
            _selected_sprite->setVisible( false );
        }
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
        _hero_avatar->setGLProgramState( GLProgramState::getOrCreateWithGLProgramName( GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP ) );
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

bool UIHeroManageHeroSlot::isDeployed() {
    return _is_deployed;
}

void UIHeroManageHeroSlot::setDeployed( bool b ) {
    _is_deployed = b;
    if( _is_deployed ) {
        if( _deployed_sprite != nullptr ) {
            _deployed_sprite->setVisible( true );
        }
    }
    else {
        if( _deployed_sprite != nullptr ) {
            _deployed_sprite->setVisible( false );
        }
    }
}

spine::SkeletonAnimation* UIHeroManageHeroSlot::getHeroSkeleton() {
    return _hero_skeleton;
}

void UIHeroManageHeroSlot::setHeroSkeleton( spine::SkeletonAnimation* skeleton ) {
    CC_SAFE_RETAIN( skeleton );
    CC_SAFE_RELEASE( _hero_skeleton );
    _hero_skeleton = skeleton;
}

UnitData* UIHeroManageHeroSlot::getUnitData() {
    return _hero_data;
}

void UIHeroManageHeroSlot::setUnitData( UnitData* data ) {
    CC_SAFE_RETAIN( data );
    CC_SAFE_RELEASE( _hero_data );
    _hero_data = data;
}

cocos2d::Sprite* UIHeroManageHeroSlot::getHeroAvatar() {
    return _hero_avatar;
}

void UIHeroManageHeroSlot::setHeroAvatar( cocos2d::Sprite* avatar ) {
    if( _hero_avatar == nullptr ) {
        _hero_avatar = avatar;
        _hero_avatar->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
        this->addChild( _hero_avatar, 0 );
    }
    else {
        _hero_avatar->setSpriteFrame( avatar->getSpriteFrame() );
    }
}

cocos2d::Sprite* UIHeroManageHeroSlot::getHeroRectAvatar() {
    return _hero_rect_avatar;
}

void UIHeroManageHeroSlot::setHeroRectAvatar( cocos2d::Sprite* avatar ) {
    CC_SAFE_RETAIN( avatar );
    CC_SAFE_RELEASE( _hero_rect_avatar );
    _hero_rect_avatar = avatar;
}

//hero deploy slot
UIHeroDeploySlot::UIHeroDeploySlot() :
_avatar_sprite( nullptr )
{
    
}

UIHeroDeploySlot::~UIHeroDeploySlot() {
    
}

UIHeroDeploySlot* UIHeroDeploySlot::create( const std::string& slot_id ) {
    UIHeroDeploySlot* ret = new UIHeroDeploySlot();
    if( ret && ret->init( slot_id ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIHeroDeploySlot::init( const std::string& slot_id ) {
    if( !Node::init() ) {
        return false;
    }
    _slot_id = slot_id;
    
    this->setContentSize( Size( HERO_DEPLOY_CELL_WIDTH, HERO_DEPLOY_CELL_HEIGHT ) );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    
    _selected_sprite = Sprite::createWithSpriteFrameName( "ui_hero_selected.png" );
    _selected_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _selected_sprite, 1 );
    
    _lock_sprite = Sprite::createWithSpriteFrameName( "ui_hero_chain.png" );
    _lock_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
    this->addChild( _lock_sprite, 2 );
    
    this->setSelected( false );
    
    return true;
}

bool UIHeroDeploySlot::isLocked() {
    return _is_locked;
}

void UIHeroDeploySlot::setLocked( bool b ) {
    _is_locked = b;
    _lock_sprite->setVisible( _is_locked );
}

bool UIHeroDeploySlot::isSelected() {
    return _is_selected;
}

void UIHeroDeploySlot::setSelected( bool b ) {
    _is_selected = b;
    _selected_sprite->setVisible( _is_selected );
}

cocos2d::Sprite* UIHeroDeploySlot::getAvatar() {
    return _avatar_sprite;
}

void UIHeroDeploySlot::setAvatar( cocos2d::Sprite* avatar ) {
    if( _avatar_sprite == nullptr ) {
        if( avatar != nullptr ) {
            _avatar_sprite = Sprite::createWithSpriteFrame( avatar->getSpriteFrame() );
            _avatar_sprite->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height / 2 ) );
            this->addChild( _avatar_sprite );
        }
    }
    else {
        if( avatar == nullptr ) {
            _avatar_sprite->setVisible( false );
        }
        else {
            _avatar_sprite->setVisible( true );
            _avatar_sprite->setSpriteFrame( avatar->getSpriteFrame() );
        }
    }
}

//manage layer
UIHeroManageLayer::UIHeroManageLayer():
_selected_hero( nullptr ),
_selected_deploy_slot( nullptr ),
_selected_skill_tab( nullptr )
{
    
}

UIHeroManageLayer::~UIHeroManageLayer() {
    ActionTimelineCache::getInstance()->removeAction( HERO_MANAGE_FILE );
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
    
    _panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( root_csb_file );
    
    ui::Button* btn_confirm = dynamic_cast<ui::Button*>( _root_node->getChildByName( "btn_finish" ) );
    btn_confirm->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onConfirmTouched, this ) );
    
    Node* pn_skill_equip = _root_node->getChildByName( "skill_equip_panel" );
    
    ui::Button* btn_detail = dynamic_cast<ui::Button*>( pn_skill_equip->getChildByName( "btn_equip" ) );
    btn_detail->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onDetailTouched, this ) );
    
    ui::Button* btn_skill = dynamic_cast<ui::Button*>( pn_skill_equip->getChildByName( "btn_skill") );
    btn_skill->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onSkillTouched, this ) );
    
    Node* pn_buy = _root_node->getChildByName( "panel_buy" );
    ui::Button* btn_purchase = dynamic_cast<ui::Button*>( pn_buy->getChildByName( "btn_buy" ) );
    btn_purchase->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onPurchaseTouched, this ) );
    _lb_price = dynamic_cast<ui::Text*>( pn_buy->getChildByName( "lb_price" ) );
    
    _lb_upgrade_cost = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_upgrade_cost" ) );
    
    Node* pn_skill_info = _root_node->getChildByName( "skill_info_panel" );
    _btn_skill_1 = dynamic_cast<ui::Button*>( pn_skill_info->getChildByName( "btn_skill1" ) );
    _btn_skill_1->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onSkillTabTouched, this ) );
    this->setSkillTab( _btn_skill_1 );
    
    _lb_skill_desc = dynamic_cast<ui::Text*>( pn_skill_info->getChildByName( "lb_skill_desc" ) );
    
    ui::Text* lb_title_1 = dynamic_cast<ui::Text*>( _btn_skill_1->getChildByName( "lb_title" ) );
    Label* label_1 = lb_title_1->getLabelRenderer();
    label_1->setLineHeight( label_1->getLineHeight() - 6.0f );
    
    _btn_skill_2 = dynamic_cast<ui::Button*>( pn_skill_info->getChildByName( "btn_skill2" ) );
    _btn_skill_2->addTouchEventListener( CC_CALLBACK_2( UIHeroManageLayer::onSkillTabTouched, this ) );
    
    ui::Text* lb_title_2 = dynamic_cast<ui::Text*>( _btn_skill_2->getChildByName( "lb_title" ) );
    lb_title_2->setAdditionalKerning( -5.0f );
    Label* label_2 = lb_title_2->getLabelRenderer();
    label_2->setLineHeight( label_2->getLineHeight() - 6.0f );
    
    _pv_hero_list = dynamic_cast<ui::PageView*>( _root_node->getChildByName( "pv_hero_list" ) );
    
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
    
    Node* upgrade_effect_node = _root_node->getChildByName( "upgrade_effect_node" );
    _upgrade_effect = ArmatureManager::getInstance()->createArmature( "ui/effect_lvlup" );
    upgrade_effect_node->addChild( _upgrade_effect );
    
    _lb_hero_name = dynamic_cast<ui::Text*>( _root_node->getChildByName( "lb_hero_name" ) );
    
    _selected_hero = nullptr;
    
    PlayerInfo* player_info = PlayerInfo::getInstance();
    const ValueMap& all_units_info = player_info->getOwnedUnitsInfo();
    
    Node* pn_deployed = _root_node->getChildByName( "choose_panel" )->getChildByName( "choose_panel" );
    
    _deploy_slot_1 = pn_deployed->getChildByName( "hero_1" );
    _deploy_slot_2 = pn_deployed->getChildByName( "hero_2" );
    _deploy_slot_3 = pn_deployed->getChildByName( "hero_3" );
    _deploy_slot_4 = pn_deployed->getChildByName( "hero_4" );
    _deploy_slot_5 = pn_deployed->getChildByName( "hero_5" );
    
    const ValueMap& upgrade_cost_config = ResourceManager::getInstance()->getUnitLevelupCostConfig();
    for( auto pair : upgrade_cost_config ) {
        const ValueMap& config = pair.second.asValueMap();
        std::string hero_name = config.at( "name" ).asString();
        std::string hero_id = config.at( "hero_id" ).asString();
        UIHeroManageHeroSlot* slot = nullptr;
        bool is_owned = player_info->isUnitOwned( hero_name );
        bool is_locked = player_info->isUnitLocked( hero_name );
        if( is_owned ) {
            const ValueMap& hero_data = all_units_info.at( hero_id ).asValueMap();
            slot = UIHeroManageHeroSlot::create( hero_data, hero_id, 0 );
        }
        else {
            ValueMap hero_data;
            ValueVector skills;
            for( int i = 0; i < 2; i++ ) {
                ValueMap skill;
                skill["skill_id"] = Value( i );
                skill["level"] = Value( 1 );
                skills.push_back( Value( skill ) );
            }
            hero_data["skills"] = Value( skills );
            hero_data["name"] = Value( hero_name );
            hero_data["level"] = Value( 1 );
            slot = UIHeroManageHeroSlot::create( hero_data, hero_id, 0 );
        }
        slot->setOwned( is_owned );
        slot->setLocked( is_locked );
        _hero_slots.insert( hero_id, slot );
    }
    
    this->alignHeroSlots();

    //deployed units
    const ValueMap& deployed_unit_slot_ids = player_info->getPlayerDeployedUnitsSlotIds();
    int i_slot_id = 1;
    int total_slot_count = (int)deployed_unit_slot_ids.size();
    for( int i = 1; i <= total_slot_count; i++ ) {
        std::string slot_id = Utils::toStr( i );
        std::string hero_id = deployed_unit_slot_ids.at( slot_id ).asString();
        UIHeroDeploySlot* slot = UIHeroDeploySlot::create( slot_id );
        slot->setHeroId( hero_id );
        
        if( hero_id == "l" ) {
            slot->setLocked( true );
        }
        else {
            slot->setLocked( false );
            if( hero_id != "0" ) {
                UIHeroManageHeroSlot* hero = _hero_slots.at( hero_id );
                slot->setAvatar( hero->getHeroRectAvatar() );
            }
        }

        switch ( i_slot_id ) {
            case 1:
                _deploy_slot_1->addChild( slot );
                break;
            case 2:
                _deploy_slot_2->addChild( slot );
                break;
            case 3:
                _deploy_slot_3->addChild( slot );
                break;
            case 4:
                _deploy_slot_4->addChild( slot );
                break;
            case 5:
                _deploy_slot_5->addChild( slot );
                break;
            default:
                break;
        }
        i_slot_id++;
        _deploy_slots.pushBack( slot );
    }
    
    _current_page = 0;
    this->turnToPage( _current_page );
    
    this->setSelectedHero( _hero_slots.at( "1" ) );
    
    _is_dragging = false;
    _is_touch_down = false;
    _drag_avatar = nullptr;
    
    _is_selected_hero_owned = true;
    return true;
}

void UIHeroManageLayer::onConfirmTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        this->recordDeployedUnits();
        TouchableLayer* parent = dynamic_cast<TouchableLayer*>( this->getParent() );
        if( parent ) {
            parent->becomeTopLayer();
        }
        this->removeFromParentAndCleanup( true );
    }
}

void UIHeroManageLayer::onDetailTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        if( _selected_hero != nullptr && _selected_hero->isOwned() ) {
            _root_node->setVisible( false );
            UIHeroDetailLayer* hero_detail = UIHeroDetailLayer::create( _selected_hero );
            this->addChild( hero_detail, 2 );
        }
    }
}

void UIHeroManageLayer::onSkillTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        if( _selected_hero != nullptr && _selected_hero->isOwned() ) {
            _root_node->setVisible( false );
            UIHeroSkillLayer* hero_skill = UIHeroSkillLayer::create( _selected_hero );
            this->addChild( hero_skill, 2 );
        }
    }
}

void UIHeroManageLayer::onPrevTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        this->turnToPage( _current_page - 1 );
    }
}

void UIHeroManageLayer::onNextTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        this->turnToPage( _current_page + 1 );
    }
}

void UIHeroManageLayer::onUpgradeTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        if( _selected_hero && _selected_hero->isOwned() ) {
            PlayerInfo* player_info = PlayerInfo::getInstance();
            const ValueVector& upgrade_data = ResourceManager::getInstance()->getUnitLevelupCostConfig().at( _selected_hero->getUnitData()->name ).asValueMap().at( "costs" ).asValueVector();
            int total_gold = player_info->getGold();
            int current_level = _selected_hero->getUnitData()->level;
            int team_level = player_info->getTeamLevel();
            int max_level = upgrade_data.size() + 1;
            if( current_level < team_level && current_level < max_level ) {
                int cost = upgrade_data.at( current_level - 1 ).asInt();
                if( cost <= total_gold ) {
                    //cost gold
                    player_info->gainGold( -cost );
                    this->updatePlayerInfo();
                    
                    //update hero info
                    ValueMap new_data = player_info->upgradeHero( _selected_hero->getHeroId(), 1 );
                    _selected_hero->loadHeroInfo( new_data );
                    this->setSelectedHero( _selected_hero );
                    
                    _upgrade_effect->setAnimation( 0, "Idle", false );
                }
            }
        }
    }
}

void UIHeroManageLayer::onSkillTabTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        if( sender != _selected_skill_tab ) {
            ui::Button* btn = dynamic_cast<ui::Button*>( sender );
            this->setSkillTab( btn );
        }
    }
}

void UIHeroManageLayer::onPurchaseTouched( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
    if( type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
        CocosUtils::playTouchEffect();
        if( _selected_hero != nullptr && !_selected_hero->isOwned() && !_selected_hero->isLocked() ) {
            ValueMap hero_data = PlayerInfo::getInstance()->purchaseHero( _selected_hero->getHeroId(), _selected_hero->getUnitData()->name );
            if( !hero_data.empty() ) {
                hero_data["owned"] = Value( true );
                hero_data["locked"] = Value( false );
                _selected_hero->loadHeroInfo( hero_data );
                this->setSelectedHero( _selected_hero );
                this->alignHeroSlots();
            }
        }
    }
}

void UIHeroManageLayer::becomeTopLayer() {
    _root_node->setVisible( true );
}

void UIHeroManageLayer::setSelectedHero( UIHeroManageHeroSlot* hero ) {
    if( _selected_hero != nullptr ) {
        _selected_hero->setSelected( false );
    }
    _selected_hero = hero;
    //update panel info
    
    _hero_node->removeAllChildren();
    
    if( _selected_hero && _selected_hero->getHeroSkeleton() != nullptr ) {
        bool new_owned = _selected_hero->isOwned();
        if( new_owned != _is_selected_hero_owned ) {
            _is_selected_hero_owned = new_owned;
            this->stopAllActions();
            this->runAction( _panel_action );
            if( new_owned ) {
                _panel_action->play( "lock", false );
            }
            else {
                _panel_action->play( "unlock", false );
            }
        }
        
        if( _is_selected_hero_owned ) {
            //show upgrade cost
            int cost = ResourceManager::getInstance()->getUnitUpgradeCost( _selected_hero->getUnitData()->name, _selected_hero->getUnitData()->level + 1 );
            if( cost == -1 ) {
                _lb_upgrade_cost->setString( "max" );
            }
            else {
                _lb_upgrade_cost->setString( Utils::toStr( cost ) );
            }
        }
        else {
            //show hero skill
            this->setSkillTab( _btn_skill_1 );
            
            //show hero price
            int price = ResourceManager::getInstance()->getUnitPrice( _selected_hero->getUnitData()->name );
            _lb_price->setString( Utils::toStr( price ) );
            _lb_upgrade_cost->setString( "0" );
        }
        
        _selected_hero->setSelected( true );
        _hero_node->addChild( _selected_hero->getHeroSkeleton() );
        _selected_hero->getHeroSkeleton()->setAnimation( 0, "Idle", true );
        
        UnitData* data = _selected_hero->getUnitData();
        
        _lb_level->setVisible( true );
        _lb_hero_name->setVisible( true );
        _lb_hp->setVisible( true );
        _lb_mp->setVisible( true );
        _lb_def->setVisible( true );
        _lb_atk->setVisible( true );
        _lb_hit->setVisible( true );
        _lb_dodge->setVisible( true );
        _lb_cri->setVisible( true );
        _lb_ten->setVisible( true );
        _lb_other->setVisible( true );
        
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
    else {
        _lb_level->setVisible( false );
        _lb_hero_name->setVisible( false );
        _lb_hp->setVisible( false );
        _lb_mp->setVisible( false );
        _lb_def->setVisible( false );
        _lb_atk->setVisible( false );
        _lb_hit->setVisible( false );
        _lb_dodge->setVisible( false );
        _lb_cri->setVisible( false );
        _lb_ten->setVisible( false );
        _lb_other->setVisible( false );
        _star_1->setVisible( false );
        _star_2->setVisible( false );
        _star_3->setVisible( false );
    }
}

void UIHeroManageLayer::setSelectedDeploySlot( UIHeroDeploySlot* slot ) {
    if( _selected_deploy_slot != nullptr ) {
        _selected_deploy_slot->setSelected( false );
    }
    _selected_deploy_slot = slot;
    if( _selected_deploy_slot != nullptr ) {
        _selected_deploy_slot->setSelected( true );
    }
}

void UIHeroManageLayer::setTargetDeploySlot( UIHeroDeploySlot* slot ) {
    if( _target_deploy_slot != nullptr && _target_deploy_slot != _selected_deploy_slot ) {
        _target_deploy_slot->setSelected( false );
    }
    _target_deploy_slot = slot;
    if( _target_deploy_slot != nullptr && _target_deploy_slot != _selected_deploy_slot ) {
        _target_deploy_slot->setSelected( true );
    }
}

void UIHeroManageLayer::setSkillTab( cocos2d::ui::Button* tab ) {
    if( _selected_skill_tab != nullptr ) {
        _selected_skill_tab->switchSpriteFrames();
        _selected_skill_tab->setPosition( _selected_skill_tab->getPosition() - Point( 20.0, 0 ) );
    }
    _selected_skill_tab = tab;
    _selected_skill_tab->switchSpriteFrames();
    _selected_skill_tab->setPosition( _selected_skill_tab->getPosition() + Point( 20.0, 0 ) );
    
    if( _selected_hero != nullptr ) {
        int skill_id = 1;
        UnitData* unit_data = _selected_hero->getUnitData();
        if( tab == _btn_skill_1 ) {
            skill_id = 1;
        }
        else {
            skill_id = 2;
        }
        
        const ValueMap& skill_conf = unit_data->skills.at( skill_id - 1 ).asValueMap();
        std::string skill_name = skill_conf.at( "name" ).asString();
        int skill_level = skill_conf.at( "level" ).asInt();
        std::string desc = ResourceManager::getInstance()->getSkillDesc( skill_name, skill_level );
        _lb_skill_desc->setString( desc );
    }
}

void UIHeroManageLayer::turnToPage( int index ) {
    int total_pages = _pv_hero_list->getPages().size();
    if( index >= 0 && index < total_pages ) {
        _current_page = index;
        _pv_hero_list->scrollToPage( _current_page );
        _btn_prev->setVisible( _current_page != 0 );
        _btn_next->setVisible( _current_page != total_pages - 1 );
    }
}

void UIHeroManageLayer::updatePlayerInfo() {
    PlayerInfo* player_info = PlayerInfo::getInstance();
}

void UIHeroManageLayer::alignHeroSlots() {
    _pv_hero_list->removeAllPages();
    
    std::vector<std::string> all_keys;
    std::vector<std::string> owned_keys;
    std::vector<std::string> unlocked_keys;
    std::vector<std::string> locked_keys;
    
    for( auto pair : _hero_slots ) {
        UIHeroManageHeroSlot* slot = pair.second;
        if( slot->isOwned() ) {
            owned_keys.push_back( pair.first );
        }
        else if( slot->isLocked() ) {
            locked_keys.push_back( pair.first );
        }
        else {
            unlocked_keys.push_back( pair.first );
        }
    }
    
    for( auto str : owned_keys ) {
        all_keys.push_back( str );
    }
    
    for( auto str : unlocked_keys ) {
        all_keys.push_back( str );
    }
    
    for( auto str : locked_keys ) {
        all_keys.push_back( str );
    }
  
    ui::Layout* page = nullptr;
    
    int i = 0;
    int count = 6;
    
    const ValueMap& deployed_unit_slot_ids = PlayerInfo::getInstance()->getPlayerDeployedUnitsSlotIds();
    
    for( auto key : all_keys ) {
        if( i == 0 || ++i > count ) {
            i = 1;
            page = ui::Layout::create();
            page->setContentSize( _pv_hero_list->getContentSize() );
            _pv_hero_list->addPage( page );
        }
        UIHeroManageHeroSlot* slot = _hero_slots.at( key );
        for( auto pair : deployed_unit_slot_ids ) {
            if( pair.second.asString() == key ) {
                slot->setDeployed( true );
                break;
            }
        }
        slot->setPosition( Point( ( i - 0.5 ) * HERO_LIST_CELL_WIDTH, HERO_LIST_CELL_HEIGHT / 2.0f ) );
        page->addChild( slot );
    }
}

void UIHeroManageLayer::recordDeployedUnits() {
    ValueMap new_info;
    for( auto slot : _deploy_slots ) {
        if( !slot->isLocked() ) {
            new_info[slot->getSlotId()] = Value( slot->getHeroId() );
        }
    }
    PlayerInfo::getInstance()->setDeployUnits( new_info );
}

bool UIHeroManageLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( !_is_touch_down ) {
        _target_deploy_slot = nullptr;
        _touch_down_pos = this->convertTouchToNodeSpace( touch );
        UIHeroManageHeroSlot* hero = this->heroSlotForTouch( touch );
        if( hero != nullptr ) {
            _is_touch_down = true;
            this->setSelectedHero( hero );
            this->setSelectedDeploySlot( nullptr );
        }
        else {
            UIHeroDeploySlot* deploy_slot = this->deploySlotForTouch( touch );
            if( deploy_slot ) {
                _is_touch_down = true;
                this->setSelectedDeploySlot( deploy_slot );
                std::string hero_id = deploy_slot->getHeroId();
                auto itr = _hero_slots.find( hero_id );
                if( itr != _hero_slots.end() ) {
                    this->setSelectedHero( itr->second );
                }
                else {
                    this->setSelectedHero( nullptr );
                }
            }
            else {
                this->setSelectedDeploySlot( nullptr );
            }
        }
    }
    return true;
}

void UIHeroManageLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_down ) {
        Point new_pos = this->convertTouchToNodeSpace( touch );
        if( !_is_dragging ) {
            if( _selected_hero != nullptr && _selected_hero->isOwned() ) {
                if( new_pos.distance( _touch_down_pos ) > MIN_DRAG_BIAS ) {
                    if( _drag_avatar == nullptr ) {
                        _drag_avatar = Sprite::createWithSpriteFrame( _selected_hero->getHeroAvatar()->getSpriteFrame() );
                        _drag_avatar->setOpacity( 200 );
                        this->addChild( _drag_avatar );
                    }
                    else {
                        _drag_avatar->setSpriteFrame( _selected_hero->getHeroAvatar()->getSpriteFrame() );
                        _drag_avatar->setVisible( true );
                    }
                    _drag_avatar->setPosition( new_pos );
                    _is_dragging = true;
                }
            }
        }
        
        if( _is_dragging ) {
            _drag_avatar->setPosition( new_pos );
            UIHeroDeploySlot* slot = this->deploySlotForTouch( touch );
            this->setTargetDeploySlot( slot );
        }
    }
}

void UIHeroManageLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    _is_touch_down = false;
    _is_dragging = false;
    this->setTargetDeploySlot( nullptr );
    if( _drag_avatar != nullptr ) {
        _drag_avatar->setVisible( false );
    }
}

void UIHeroManageLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_down ) {
        if( _is_dragging ) {
            UIHeroDeploySlot* slot = this->deploySlotForTouch( touch );
            if( _selected_deploy_slot != nullptr && _selected_deploy_slot->getHeroId() != "0" ) {
                //undeploy hero
                if( slot == nullptr ) {
                    _selected_deploy_slot->setHeroId( "0" );
                    _selected_deploy_slot->setAvatar( nullptr );
                    _selected_hero->setDeployed( false );
                    this->setSelectedDeploySlot( nullptr );
                }
                //exchange hero
                else if( slot != _selected_deploy_slot ) {
                    std::string hero_id = slot->getHeroId();
                    Sprite* avatar = Sprite::createWithSpriteFrame( slot->getAvatar()->getSpriteFrame() );
                    slot->setAvatar( _selected_deploy_slot->getAvatar() );
                    slot->setHeroId( _selected_deploy_slot->getHeroId() );
                    _selected_deploy_slot->setHeroId( hero_id );
                    _selected_deploy_slot->setAvatar( avatar );
                    this->setSelectedDeploySlot( slot );
                }
            }
            else {
                //drag from list
                if( slot != nullptr ) {
                    //undeploy old hero
                    auto itr = _hero_slots.find( slot->getHeroId() );
                    if( itr != _hero_slots.end() ) {
                        itr->second->setDeployed( false );
                    }
                    //deploy new hero
                    _selected_hero->setDeployed( true );
                    //remove duplicated hero in deploy slots
                    for( auto ds : _deploy_slots ) {
                        if( ds != slot && ds->getHeroId() == _selected_hero->getHeroId() ) {
                            ds->setHeroId( "0" );
                            ds->setAvatar( nullptr );
                            break;
                        }
                    }
                    
                    slot->setHeroId( _selected_hero->getHeroId() );
                    slot->setAvatar( _selected_hero->getHeroRectAvatar() );
                }
            }
            
            this->setTargetDeploySlot( nullptr );
        }
        _is_dragging = false;
        _is_touch_down = false;
        if( _drag_avatar ) {
            _drag_avatar->setVisible( false );
        }
    }
}

UIHeroManageHeroSlot* UIHeroManageLayer::heroSlotForTouch( cocos2d::Touch* touch ) {
    ui::Layout* page = _pv_hero_list->getPages().at( _pv_hero_list->getCurPageIndex() );
    for( auto node : page->getChildren() ) {
        auto slot = dynamic_cast<UIHeroManageHeroSlot*>( node );
        if( !slot->isLocked() ) {
            Point pos = slot->convertTouchToNodeSpace( touch );
            Rect rect = Rect( 0, 0, slot->getContentSize().width, slot->getContentSize().height );
            if( rect.containsPoint( pos ) ) {
                return slot;
            }
        }
    }
    return nullptr;
}

UIHeroDeploySlot* UIHeroManageLayer::deploySlotForTouch( cocos2d::Touch* touch ) {
    for( auto slot : _deploy_slots ) {
        if( !slot->isLocked() ) {
            Point pos = slot->convertTouchToNodeSpace( touch );
            Rect rect = Rect( 0, 0, slot->getContentSize().width, slot->getContentSize().height );
            if( rect.containsPoint( pos ) ) {
                return slot;
            }
        }
    }
    return nullptr;
}

UIHeroDeploySlot* UIHeroManageLayer::deploySlotIntersectsRect( cocos2d::Touch* touch, const cocos2d::Size& size ) {
    for( auto slot : _deploy_slots ) {
        if( !slot->isLocked() ) {
            Point pos = slot->convertTouchToNodeSpace( touch );
            Rect slot_rect = Rect( 0, 0, slot->getContentSize().width, slot->getContentSize().height );
            Rect avatar_rect = Rect( pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height );
            if( slot_rect.intersectsRect( avatar_rect ) ) {
                return slot;
            }
        }
    }
    return nullptr;
}