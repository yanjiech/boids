//
//  UIBattleLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "UIBattleLayer.h"
#include "BattleLayer.h"

using namespace cocos2d;

#define DEFAULT_HINT_LENGTH 360.0f

#define MIN_SWIPE_DISTANCE 20.0
#define MAX_SWIPE_DISTANCE 200.0

#define DEFAULT_SKILL_ICON_PADDING 50.0

UISkillNode::UISkillNode() {
    
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
    
//    std::string resource = _unit_node->getUnitData()->name + "_avatar.png";
    std::string resource = "saber_avatar.png";
    Sprite* sp = Sprite::createWithSpriteFrameName( resource );
    this->addChild( sp );
    
    Sprite* mask = Sprite::createWithSpriteFrameName( "skill_mask.png" );
    _avatar = ProgressTimer::create( mask );
    _avatar->setType( ProgressTimer::Type::RADIAL );
    _avatar->setReverseDirection( true );
    this->addChild( _avatar );
    
    Point center_pos = Point( _avatar->getContentSize().width / 2, _avatar->getContentSize().height / 2 );
    sp->setPosition( center_pos );
    _avatar->setPosition( center_pos );
    
    this->ignoreAnchorPointForPosition( false );
    this->setContentSize( _avatar->getContentSize() );
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
            _hint_effect->setPosition( _unit_node->getPosition() + _hint_d_pos );
        }
    }
    else {
        Point hint_pos = _unit_node->getPosition();
        if( _hint_type == "arrow" ) {
            _hint_effect = Sprite::create( "ui/skillarrow.png" );
            _hint_effect->setRotation( angle );
            _hint_effect->setAnchorPoint( Point( 0, 0.5f ) );
            _battle_layer->addToEffectLayer( _hint_effect, hint_pos, 0 );
        }
        else if( _hint_type == "circle" ) {
            _hint_effect = Sprite::create( "ui/skillcircle.png" );
            _hint_effect->setScale( _range / 200.0f );
            _battle_layer->addToOnGroundLayer( _hint_effect, hint_pos, 0 );
        }
        else if( _hint_type == "dyn_circle" ) {
            _hint_effect = Sprite::create( "ui/skillcircle.png" );
            _hint_effect->setScale( _radius / 200.0f );
            _hint_d_pos = skill_dir * _min_range;
            hint_pos = hint_pos + _hint_d_pos;
            _battle_layer->addToOnGroundLayer( _hint_effect, hint_pos, 0 );
        }
    }
}

void UISkillNode::hideHint() {
    if( _hint_effect ) {
        _hint_effect->removeFromParent();
        _hint_effect = nullptr;
    }
}

void UISkillNode::updateFrame( float delta ) {
    float p = _unit_node->getSkillCDById( 0 );
    _avatar->setPercentage( ( 1.0f - p ) * 100.0f );
    if( _hint_effect ) {
        if( _hint_type == "dyn_circle" ) {
            _hint_effect->setPosition( _unit_node->getPosition() + _hint_d_pos );
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
    
    return true;
}

bool UIBattleLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began == false ) {
        _selected_skill = this->skillNodeForTouch( touch );
        if( _selected_skill != nullptr ) {
            _is_touch_began = true;
            _touch = touch;
            _selected_skill->showHint( Point::ZERO, 0 );
            _touch_down_pos = this->convertTouchToNodeSpace( touch );
            _touch_down_duration = 0;
            if( _selected_skill->shouldCastOnTouchDown() && _selected_skill->isSkillReady() ) {
                _selected_skill->getOwner()->setCharging( true, _selected_skill->getSkillAttribute( "charging_effect" ).asString() );
                _selected_skill->setChargeTime( 0 );
            }
            return true;
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
}

void UIBattleLayer::reset() {
    _is_touch_began = false;
    _selected_skill = nullptr;
    this->removeAllSkillNodes();
}

void UIBattleLayer::addSkillNode( UnitNode* unit ) {
    UISkillNode* skill_node = UISkillNode::create( _battle_layer, unit );
    _skill_nodes.push_back( skill_node );
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

UISkillNode* UIBattleLayer::skillNodeForTouch( cocos2d::Touch* touch ) {
    for( auto skill_node : _skill_nodes ) {
        Rect rect = Rect( 0, 0, skill_node->getContentSize().width, skill_node->getContentSize().height );
        if( rect.containsPoint( skill_node->convertTouchToNodeSpace( touch ) ) ) {
            return skill_node;
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