//
//  UIBattleLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "UIBattleLayer.h"
#include "BattleLayer.h"

#define BATTLE_PANEL_FILE "ui/page/ui_battle.csb"

using namespace cocos2d;
using namespace cocostudio::timeline;

#define DEFAULT_HINT_LENGTH 360.0f

#define MIN_SWIPE_DISTANCE 10.0
#define MAX_SWIPE_DISTANCE 200.0

#define DEFAULT_SKILL_ICON_PADDING 50.0

UIItemSpot::UIItemSpot() {
    
}

UIItemSpot::~UIItemSpot() {
    
}

UIItemSpot* UIItemSpot::create( const cocos2d::Color3B& color ) {
    UIItemSpot* ret = new UIItemSpot();
    if( ret && ret->init( color ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIItemSpot::init( const cocos2d::Color3B& color ) {
    if( !Node::init() ) {
        return false;
    }
    
    _color = color;
    
    _icon = Sprite::create( "effects/tuowei.png" );
    _icon->setColor( color );
    this->addChild( _icon, 2 );
//    _icon->setScale( 0.5f );
    
    this->setScale( 0.5f );
    this->setPosition( Point::ZERO );
    this->setAnchorPoint( Point::ZERO );
    this->ignoreAnchorPointForPosition( false );
    
    return true;
}

void UIItemSpot::flyAlong( const cocos2d::Point& start, const cocos2d::Point& end ) {
    float scale = this->getScale();
    _icon->setPosition( start / scale );
    _streak = MotionStreak::create( 0.2f, 3, _icon->getContentSize().width, _color, "effects/tuowei.png" );
    _streak->setPosition( start / scale );
    this->addChild( _streak, 1 );
//    _streak->setScale( 0.5f );

    Point origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    
    ccBezierConfig config;
    config.controlPoint_1 = Point( origin.x + size.width * 0.25f, origin.y + size.height * 0.5f ) / scale;
    config.controlPoint_2 = Point( origin.x + size.width * 0.75f, origin.y + size.height * 0.75f ) / scale;
    config.endPosition = end / scale;
    
    BezierTo* fly = BezierTo::create( 0.5f, config );
    CallFunc* callback = CallFunc::create( CC_CALLBACK_0( UIItemSpot::onReachDestination, this ) );
    Sequence* seq = Sequence::create( fly, callback, nullptr );
    _icon->runAction( seq );
    
    this->schedule( CC_CALLBACK_1( UIItemSpot::updateFrame, this ), "item_spot_update" );
}

void UIItemSpot::updateFrame( float delta ) {
    _streak->setPosition( _icon->getPosition() );
}

void UIItemSpot::onReachDestination() {
    _streak->removeFromParent();
    this->removeFromParent();
}

UISkillNode::UISkillNode() :
_hint_effect( nullptr ),
_hint_effect_dyn( nullptr )
{
    
}

UISkillNode::~UISkillNode() {
}

UISkillNode* UISkillNode::create( BattleLayer* battle_layer, UnitNode* unit_node ) {
    UISkillNode* ret = new UISkillNode();
    if( ret && ret->init( battle_layer, unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UISkillNode::init( BattleLayer* battle_layer, UnitNode* unit_node ) {
    if( !Node::init() ) {
        return false;
    }
    _battle_layer = battle_layer;
    _unit_node = unit_node;
    _hint_effect = nullptr;
    
    std::string resource = _unit_node->getUnitData()->name + "_avatar.png";
//    std::string resource = "saber_avatar.png";
    Sprite* sp = Sprite::createWithSpriteFrameName( resource );
    this->addChild( sp, 4 );
    
    _sp_ready = Sprite::createWithSpriteFrameName( "ui_battle_diban01.png" );
    _sp_ready->setVisible( false );
    this->addChild( _sp_ready, 1 );
    
    Sprite* diban = Sprite::createWithSpriteFrameName( "ui_battle_mp.png" );
    _pb_mp = ProgressTimer::create( diban );
    _pb_mp->setType( ProgressTimer::Type::RADIAL );
    _pb_mp->setReverseDirection( true );
    _pb_mp->setPercentage( 100.0f );
    this->addChild( _pb_mp, 10 );
    
    Sprite* mask = Sprite::createWithSpriteFrameName( "ui_battle_icon04.png" );
    _pb_loading = ProgressTimer::create( mask );
    _pb_loading->setType( ProgressTimer::Type::RADIAL );
    _pb_loading->setReverseDirection( true );
    this->addChild( _pb_loading, 5 );
    
    Point center_pos = Point( _pb_loading->getContentSize().width / 2, _pb_loading->getContentSize().height / 2 );
    sp->setPosition( center_pos );
    _pb_loading->setPosition( center_pos );
    _pb_mp->setPosition( center_pos );
    _sp_ready->setPosition( center_pos );
    
    this->ignoreAnchorPointForPosition( false );
    this->setContentSize( _pb_loading->getContentSize() );
    this->setAnchorPoint( Point( 0.5f, 0.5f ) );
    
    _hint_type = _unit_node->getSkillHintTypeById( 0 );
    _range = _unit_node->getSkillRangeById( 0 );
    _hint_d_pos = Point::ZERO;
    
    Value v_min_range = unit_node->getSkill( 0 )->getAttribute( "min_range" );
    if( !v_min_range.isNull() ) {
        _min_range = v_min_range.asFloat();
    }
    else {
        _min_range = 0;
    }
    
    Value v_max_range = unit_node->getSkill( 0 )->getAttribute( "max_range" );
    if( !v_max_range.isNull() ) {
        _max_range = v_max_range.asFloat();
    }
    else {
        _max_range = 0;
    }
    
    Value v_max_charge_time = unit_node->getSkill( 0 )->getAttribute( "max_time" );
    if( !v_max_charge_time.isNull() ) {
        _max_charge_time = v_max_charge_time.asFloat();
    }
    else {
        _max_charge_time = 0;
    }
    if( _hint_type == "dyn_circle" ) {
        _radius = unit_node->getSkillRadiusById( 0 );
    }
    
    _is_skill_ready = false;
    
    return true;
}

void UISkillNode::activate( const cocos2d::Point& dir, float range_per, float duration ) {
    if( _unit_node->isSkillReadyById( 0 ) ) {
        _unit_node->useSkill( 0, dir, range_per, duration );
    }
}

void UISkillNode::showHint( const cocos2d::Point& dir, float range_per ) {
    Point skill_dir = dir;
    if( skill_dir.x == 0 && skill_dir.y == 0 ) {
        skill_dir = _unit_node->getUnitDirection();
    }
    float angle = -skill_dir.getAngle() * 180.0f / M_PI;
    if( _hint_effect != nullptr ) {
        if( _hint_type == "arrow" ) {
            _hint_effect->setRotation( angle );
        }
        else if( _hint_type == "dyn_circle" ) {
            _range = _min_range + ( _max_range - _min_range ) * range_per;
            _hint_d_pos = skill_dir * _range;
            _hint_d_pos = Point( _hint_d_pos.x, _hint_d_pos.y / 1.74f );
            _hint_effect_dyn->setPosition( _unit_node->getPosition() + _hint_d_pos );
        }
    }
    else {
        Point hint_pos = _unit_node->getPosition();
        if( _hint_type == "arrow" ) {
            _hint_effect = Sprite::createWithSpriteFrameName( "skillarrow.png" );
            _hint_effect->setRotation( angle );
            _hint_effect->setAnchorPoint( Point( 0, 0.5f ) );
            _battle_layer->addToEffectLayer( _hint_effect, hint_pos, 0 );
        }
        else if( _hint_type == "circle" ) {
            _hint_effect = Sprite::createWithSpriteFrameName( "skillcircle.png" );
            _hint_effect->setScale( _range / 215.0f );
            _battle_layer->addToOnGroundLayer( _hint_effect, hint_pos, 0 );
        }
        else if( _hint_type == "dyn_circle" ) {
            _hint_effect = Sprite::createWithSpriteFrameName( "skillcircle.png" );
            _hint_effect->setScale( _max_range / 215.0f );
            _battle_layer->addToOnGroundLayer( _hint_effect, hint_pos, 0 );
            
            _hint_effect_dyn = Sprite::createWithSpriteFrameName( "skillcircle2.png" );
            _hint_effect_dyn->setScale( _radius / 215.0f );
            _hint_d_pos = skill_dir * _min_range;
            _hint_d_pos = Point( _hint_d_pos.x, _hint_d_pos.y / 1.74f );
            hint_pos = hint_pos + _hint_d_pos;
            _battle_layer->addToOnGroundLayer( _hint_effect_dyn, hint_pos, 1 );
        }
    }
}

void UISkillNode::hideHint() {
    if( _hint_effect ) {
        _hint_effect->removeFromParent();
        _hint_effect = nullptr;
    }
    if( _hint_effect_dyn ) {
        _hint_effect_dyn->removeFromParent();
        _hint_effect_dyn = nullptr;
    }
}

void UISkillNode::updateFrame( float delta ) {
    _pb_mp->setPercentage( 100.0f * _unit_node->getTargetData()->current_mp / _unit_node->getTargetData()->mp );
    bool is_ready = _unit_node->isSkillReadyById( 0 );
    if( is_ready != _is_skill_ready ) {
        _is_skill_ready = is_ready;
        if( is_ready ) {
            _pb_loading->setPercentage( 0 );
            _sp_ready->setVisible( true );
        }
        else {
            _sp_ready->setVisible( false );
        }
    }
    if( !_is_skill_ready ) {
        float p = _unit_node->getSkillCDById( 0 );
        _pb_loading->setPercentage( ( 1.0f - p ) * 100.0f );
    }
    if( _hint_effect && _hint_effect->isVisible() ) {
        if( _hint_type == "dyn_circle" ) {
            _hint_effect_dyn->setPosition( _unit_node->getPosition() + _hint_d_pos );
            _hint_effect->setPosition( _unit_node->getPosition() );
        }
        else {
            _hint_effect->setPosition( _unit_node->getPosition() );
        }
    }
}

bool UISkillNode::shouldCastOnTouchDown() {
    return _unit_node->shouldSkillCastOnTouchDown( 0 );
}

bool UISkillNode::isSkillReady() {
    return _unit_node->isSkillReadyById( 0 );
}

Skill* UISkillNode::getSkill() {
    return _unit_node->getSkill( 0 );
}

cocos2d::Value UISkillNode::getSkillAttribute( const std::string& key ) {
    return _unit_node->getSkill( 0 )->getAttribute( key );
}

void UISkillNode::setChargeTime( float time ) {
    if( _hint_effect != nullptr && _max_charge_time != 0 ) {
        float percent = time / _max_charge_time;
        if( percent > 1.0f ) {
            percent = 1.0f;
        }
        float length = _min_range + ( _max_range - _min_range ) * percent;
        _hint_effect->setScaleX( length / DEFAULT_HINT_LENGTH );
    }
}

UIBattleLayer::UIBattleLayer() {
    
}

UIBattleLayer::~UIBattleLayer() {
    ActionTimelineCache::getInstance()->removeAction( BATTLE_PANEL_FILE );
}

UIBattleLayer* UIBattleLayer::create( BattleLayer* battle_layer ) {
    UIBattleLayer* ret = new UIBattleLayer();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleLayer::init( BattleLayer* battle_layer ) {
    if( !Layer::init() ) {
        return false;
    }
    _battle_layer = battle_layer;
    _is_touch_began = false;
    _selected_skill = nullptr;
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches( true );
    touch_listener->onTouchBegan = CC_CALLBACK_2( UIBattleLayer::onTouchBegan, this );
    touch_listener->onTouchMoved = CC_CALLBACK_2( UIBattleLayer::onTouchMoved, this );
    touch_listener->onTouchCancelled = CC_CALLBACK_2( UIBattleLayer::onTouchCancelled, this );
    touch_listener->onTouchEnded = CC_CALLBACK_2( UIBattleLayer::onTouchEnded, this );
    _eventDispatcher->addEventListenerWithSceneGraphPriority( touch_listener, this );
    
    std::string battle_panel_file = FileUtils::getInstance()->fullPathForFilename( BATTLE_PANEL_FILE );
    _root_node = cocos2d::CSLoader::getInstance()->createNode( battle_panel_file );
    this->addChild( _root_node );
    
    _battle_panel_action = ActionTimelineCache::getInstance()->loadAnimationActionWithFlatBuffersFile( battle_panel_file );
    
    Node* pn_time = _root_node->getChildByName( "time_panel" );
    _lb_time = dynamic_cast<ui::Text*>( pn_time->getChildByName( "lb_time" ) );
    
    Node* pn_killed = _root_node->getChildByName( "number_panel" );
    _lb_killed = dynamic_cast<ui::Text*>( pn_killed->getChildByName( "lb_killed" ) );
    _pb_killed = dynamic_cast<ui::LoadingBar*>( pn_killed->getChildByName( "pn_killed" ) );
    
    _pn_boss_and_wave = dynamic_cast<ui::Layout*>( _root_node->getChildByName( "boss_panel" ) );
    
    _pn_boss = dynamic_cast<ui::Layout*>( _pn_boss_and_wave->getChildByName( "boss_hp_panel" ) );
    _nd_boss_avatar = _pn_boss->getChildByName( "nd_boss_avatar" );
    _lb_boss_name = dynamic_cast<ui::Text*>( _pn_boss->getChildByName( "lb_boss_name" ) );
    _lb_boss_name->setAdditionalKerning( -5.0f );
    _pb_boss_hp = dynamic_cast<ui::LoadingBar*>( _pn_boss->getChildByName( "pb_boss_hp" ) );
    
    _pn_wave = dynamic_cast<ui::Layout*>( _pn_boss_and_wave->getChildByName( "wave_bar_panel" ) );
    _nd_monster_avatar = _pn_wave->getChildByName( "nd_monster_avatar" );
    _sp_flag = dynamic_cast<Sprite*>( _pn_wave->getChildByName( "sp_flag" ) );
    _pb_wave = dynamic_cast<ui::LoadingBar*>( _pn_wave->getChildByName( "pb_wave" ) );
    
    _need_kill = 0;
    _already_killed = 0;
    
    this->hideBossPanel();
    this->hideWavePanel();
    this->setKilled( 0 );
    
    return true;
}

bool UIBattleLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began == false ) {
        _selected_skill = this->skillNodeForTouch( touch );
        if( _selected_skill != nullptr ) {
            Skill* skill = _selected_skill->getSkill();
            if( skill->isSkillReady() && !_selected_skill->getOwner()->isCasting() && !_selected_skill->getOwner()->willCast() ) {
                _is_touch_began = true;
                _touch = touch;
                _touch_down_pos = this->convertTouchToNodeSpace( touch );
                _touch_down_duration = 0;
                _selected_skill->showHint( Point::ZERO, 0 );
                if( _selected_skill->shouldCastOnTouchDown() ) {
                    _selected_skill->getOwner()->setCharging( true, _selected_skill->getSkillAttribute( "charging_effect" ).asString() );
                    _selected_skill->setChargeTime( 0 );
                }
                return true;
            }
            else if( skill->hasExtraDamage() ){
                UnitNode* owner = _selected_skill->getOwner();
                _selected_skill = nullptr;
                SkillNode* skill_node = skill->getSkillNode();
                if( skill_node != nullptr && skill->isCasting() ) {
                    skill_node->refreshDamage();
                    return true;
                }
                else if( owner->isCasting() ) {
                    return true;
                }
            }
        }
    }
    return false;
}

void UIBattleLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began && _touch == touch ) {
        Point touch_pos = this->convertTouchToNodeSpace( touch );
        Point dir = Point::ZERO;
        float range_per = 0;
        float distance = _touch_down_pos.distance( touch_pos );
        if( distance >= MIN_SWIPE_DISTANCE ) {
            dir = touch_pos - _touch_down_pos;
            dir.normalize();
            if( distance > MAX_SWIPE_DISTANCE ) {
                distance = MAX_SWIPE_DISTANCE;
            }
            range_per = distance / MAX_SWIPE_DISTANCE;
        }
        _selected_skill->showHint( dir, range_per );
    }
}

void UIBattleLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began ) {
        _selected_skill->hideHint();
        _selected_skill = nullptr;
        _is_touch_began = false;
        _touch = nullptr;
    }
}

void UIBattleLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began && _touch == touch ) {
        Point touch_pos = this->convertTouchToNodeSpace( touch );
        Point dir = Point::ZERO;
        float range_per = 0;
        float distance = _touch_down_pos.distance( touch_pos );
        if( distance >= MIN_SWIPE_DISTANCE ) {
            dir = touch_pos - _touch_down_pos;
            dir.normalize();
            if( distance > MAX_SWIPE_DISTANCE ) {
                distance = MAX_SWIPE_DISTANCE;
            }
            range_per = ( distance - MIN_SWIPE_DISTANCE ) / ( MAX_SWIPE_DISTANCE - MIN_SWIPE_DISTANCE );
        }
        _selected_skill->hideHint();
        if( _selected_skill->shouldCastOnTouchDown() ) {
            if( _selected_skill->getOwner()->isCharging() && _selected_skill->isSkillReady() ) {
                _selected_skill->getOwner()->setCharging( false );
                _selected_skill->activate( dir, range_per, _touch_down_duration );
            }
        }
        else {
            _selected_skill->activate( dir, range_per );
        }
        
        _is_touch_began = false;
        _touch = nullptr;
        _selected_skill = nullptr;
    }
}

void UIBattleLayer::updateFrame( float delta ) {
    for( UISkillNode* skill_node : _skill_nodes ) {
        skill_node->updateFrame( delta );
    }
    if( _is_touch_began ) {
        _touch_down_duration += delta;
        _selected_skill->setChargeTime( _touch_down_duration );
    }
    
    //update time
    int game_time = (int)_battle_layer->getGameTime();
    int minutes = game_time / 60;
    int seconds = game_time % 60;
    
    _lb_time->setString( Utils::stringFormat( "%02d:%02d", minutes, seconds ) );
}

void UIBattleLayer::reset() {
    _is_touch_began = false;
    _selected_skill = nullptr;
    this->removeAllSkillNodes();
}

void UIBattleLayer::addSkillNode( UnitNode* unit ) {
    UISkillNode* skill_node = UISkillNode::create( _battle_layer, unit );
    _skill_nodes.pushBack( skill_node );
    this->addChild( skill_node );
    this->alignSkillNodesWithPadding( DEFAULT_SKILL_ICON_PADDING );
}

void UIBattleLayer::removeSkillNode( UnitNode* unit ) {
    for( auto itr = _skill_nodes.begin(); itr != _skill_nodes.end(); ++itr ) {
        UISkillNode* skill_node = *itr;
        if( skill_node->getOwner() == unit ) {
            if( _selected_skill == skill_node ) {
                _selected_skill->hideHint();
                _selected_skill = nullptr;
                _is_touch_began = false;
                _touch = nullptr;
            }
            skill_node->removeFromParent();
            _skill_nodes.erase( itr );
            this->alignSkillNodesWithPadding( DEFAULT_SKILL_ICON_PADDING );
            break;
        }
    }
}

void UIBattleLayer::removeAllSkillNodes() {
    auto itr = _skill_nodes.begin();
    while( itr != _skill_nodes.end() ) {
        UISkillNode* skill_node = *itr;
        if( _selected_skill == skill_node ) {
            _selected_skill->hideHint();
            _selected_skill = nullptr;
            _is_touch_began = false;
            _touch = nullptr;
        }
        skill_node->removeFromParent();
        itr = _skill_nodes.erase( itr );
    }
    _skill_nodes.clear();
}

void UIBattleLayer::showSkillNodes() {
    for( auto skill_node : _skill_nodes ) {
        skill_node->setVisible( true );
    }
}

void UIBattleLayer::hideSkillNodes() {
    for( auto skill_node : _skill_nodes ) {
        skill_node->setVisible( false );
    }
}

cocos2d::Point UIBattleLayer::getSkillPos( int i ) {
    if( _skill_nodes.size() > i ) {
        return _skill_nodes.at( i )->getPosition();
    }
    return Point::ZERO;
}

UISkillNode* UIBattleLayer::skillNodeForTouch( cocos2d::Touch* touch ) {
    for( auto skill_node : _skill_nodes ) {
        if( skill_node->isVisible() ) {
            Rect rect = Rect( 0, 0, skill_node->getContentSize().width, skill_node->getContentSize().height );
            if( rect.containsPoint( skill_node->convertTouchToNodeSpace( touch ) ) ) {
                return skill_node;
            }
        }
    }
    return nullptr;
}

void UIBattleLayer::alignSkillNodesWithPadding( float padding ) {
    int count = (int)_skill_nodes.size();
    if( count > 0 ) {
        Point visible_origin = Director::getInstance()->getVisibleOrigin();
        Size visible_size = Director::getInstance()->getVisibleSize();
        Size skill_icon_size = _skill_nodes.front()->getContentSize();
        float x = visible_origin.x + visible_size.width - ( padding + skill_icon_size.width ) * ( count + 0.2f );
        for( auto skill_icon : _skill_nodes ) {
            skill_icon->setPosition( Point( x, skill_icon_size.height ) );
            x += padding + skill_icon_size.width;
        }
    }
}

void UIBattleLayer::showBossPanel() {
    _pn_boss->setVisible( true );
}

void UIBattleLayer::hideBossPanel() {
    _pn_boss->setVisible( false );
}

void UIBattleLayer::showWavePanel() {
    _pn_wave->setVisible( true );
}

void UIBattleLayer::hideWavePanel() {
    _pn_wave->setVisible( false );
}

void UIBattleLayer::setBossHpPercent( float percent ) {
    _pb_boss_hp->setPercent( percent );
}

void UIBattleLayer::setBossInfo( class UnitData* unit_data ) {
    _lb_boss_name->setString( unit_data->display_name );
    _nd_boss_avatar->removeAllChildren();
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( Utils::stringFormat( "ui_boss_%s.png", unit_data->name.c_str() ) );
    if( frame ) {
        Sprite* sp_boss_avatar = Sprite::createWithSpriteFrame( frame );
        _nd_boss_avatar->addChild( sp_boss_avatar );
    }
    this->setBossHpPercent( 100 );
}

void UIBattleLayer::setWavePercent( float percent ) {
    _pb_wave->setPercent( percent );
}

void UIBattleLayer::setNeedKill( int count ) {
    _need_kill = count;
    this->updateKillUI();
}

void UIBattleLayer::setKilled( int count ) {
    _already_killed = count;
    this->updateKillUI();
}

void UIBattleLayer::updateKillUI() {
    if( _need_kill != 0 ) {
        _lb_killed->setString( Utils::stringFormat( "%d/%d", _already_killed, _need_kill ) );
        _pb_killed->setPercent( (float)_already_killed / (float)_need_kill );
    }
    else {
        _lb_killed->setString( Utils::stringFormat( "%d", _already_killed ) );
    }
}

void UIBattleLayer::showItemSpot( const cocos2d::Point& start, int quality ) {
    Color3B color = Color3B::YELLOW;
    switch( quality ) {
        case 1:
            color = Color3B::WHITE;
            break;
        case 2:
            color = Color3B( 51, 200, 3 );
            break;
        case 3:
            color = Color3B( 0, 160, 255 );
            break;
        case 4:
            color = Color3B( 208, 76, 253 );
            break;
        case 5:
            color = Color3B::RED;
            break;
        default:
            break;
    }
    
    UIItemSpot* item_spot = UIItemSpot::create( color );
    item_spot->setPosition( Point::ZERO );
    this->addChild( item_spot, 10000 );
    Point end = item_spot->convertToNodeSpace( _lb_killed->getParent()->convertToWorldSpace( _lb_killed->getPosition() ) );
    item_spot->flyAlong( start, end );
}