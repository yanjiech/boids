//
//  UnitNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "UnitNode.h"
#include "../manager/ResourceManager.h"
#include "../scene/BattleLayer.h"
#include "../manager/AudioManager.h"
#include "../ArmatureManager.h"
#include "../BoidsMath.h"
#include "../AI/Terrain.h"
#include "../AI/Path.h"
#include "../constant/BoidsConstant.h"
#include "../Utils.h"
#include "BulletNode.h"
#include "JumpText.h"

#define DEFAULT_SHADOW_RADIUS 30.0
#define DEFAULT_HESITATE_FRAMES 5
#define DEFAULT_CATCH_UP_STOP_DISTANCE 180.0
#define DEFAULT_CATCH_UP_START_DISTANCE 300.0
#define DEFAULT_WANDER_RADIUS 200.0

#define DEFAULT_HP_BAR_WIDTH 100.0
#define DEFAULT_HP_BAR_HEIGHT 10.0

using namespace cocos2d;

UnitData::UnitData() {
    
}

UnitData::~UnitData() {
    
}

UnitData* UnitData::create( const cocos2d::ValueMap& data ) {
    UnitData* ret = new UnitData();
    if( ret && ret->init( data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitData::init( const cocos2d::ValueMap& data ) {
    this->name = data.at( "name" ).asString();
    this->level = data.at( "level" ).asInt();
    
    const rapidjson::Value& unit_config = ResourceManager::getInstance()->getUnitData( name );
    
    this->unit_id = unit_config["id"].GetInt();
    this->display_name = std::string( unit_config["displayname"].GetString(), unit_config["displayname"].GetStringLength() );
    this->hp = (float)unit_config["hp"].GetDouble() + this->level * (float)unit_config["hpgr"].GetDouble();
    this->current_hp = this->hp;
    this->atk = (float)unit_config["atk"].GetDouble() + this->level * (float)unit_config["atkgr"].GetDouble();
    this->def = (float)unit_config["def"].GetDouble() + this->level * (float)unit_config["defgr"].GetDouble();
    this->move_speed = (float)unit_config["movespeed"].GetDouble();
    this->atk_speed = (float)unit_config["attackspeed"].GetDouble();
    this->collide = (float)unit_config["collide"].GetDouble();
    this->critical = (float)unit_config["cri"].GetDouble() + this->level * (float)unit_config["crigr"].GetDouble();
    this->tenacity = (float)unit_config["ten"].GetDouble() + this->level * (float)unit_config["tengr"].GetDouble();
    this->hit = (float)unit_config["hit"].GetDouble() + this->level * (float)unit_config["hitgr"].GetDouble();
    this->dodge = (float)unit_config["dodge"].GetDouble() + this->level * (float)unit_config["dodge"].GetDouble();
    
    this->guard_radius = (float)unit_config["guard_radius"].GetDouble();
    this->atk_range = (float)unit_config["range"].GetDouble();
    
    this->recover = (float)unit_config["rec"].GetDouble() + this->level * (float)unit_config["recgr"].GetDouble();
    this->scale = (float)unit_config["scale"].GetDouble();
    
    this->role = std::string( unit_config["position"].GetString(), unit_config["position"].GetStringLength() );
    
    this->is_melee = unit_config["is_melee"].GetBool();
    this->is_double_face = unit_config["double_face"].GetBool();
    this->default_face_dir = unit_config["faceleft"].GetInt();
    
    if( unit_config.HasMember( "bullet_name" ) ) {
        this->bullet_name = std::string( unit_config["bullet_name"].GetString(), unit_config["bullet_name"].GetStringLength() );
    }
    else {
        this->bullet_name = "";
    }
    
    for( rapidjson::Value::ConstValueIterator itr = unit_config["skills"].onBegin(); itr != unit_config["skills"].onEnd(); ++itr ) {
        std::string skl_name = std::string( itr->GetString(), itr->GetStringLength() );
        skill_names.push_back( skl_name );
    }
    
    //todo
//    cocos2d::Vector<Skill*> skills;
    return true;
}

void UnitData::setAttribute( const std::string& key, const std::string& value ) {
    if( key == "atk" ) {
        this->atk = (float)Utils::toDouble( value );
    }
    else if( key == "attackspeed" ) {
        this->atk_speed = (float)Utils::toDouble( value );
    }
    else if( key == "collide" ) {
        this->collide = (float)Utils::toDouble( value );
    }
    else if( key == "cri" ) {
        this->critical = (float)Utils::toDouble( value );
    }
    else if( key == "def" ) {
        this->def = (float)Utils::toDouble( value );
    }
    else if( key == "displayname" ) {
        this->display_name = value;
    }
    else if( key == "dodge" ) {
        this->dodge = (float)Utils::toDouble( value );
    }
    else if( key == "guard_radius" ) {
        this->guard_radius = (float)Utils::toDouble( value );
    }
    else if( key == "hit" ) {
        this->hit = (float)Utils::toDouble( value );
    }
    else if( key == "hp" ) {
        this->hp = (float)Utils::toDouble( value );
        this->current_hp = this->hp;
    }
    else if( key == "id" ) {
        this->unit_id = Utils::toInt( value );
    }
    else if( key == "movespeed" ) {
        this->move_speed = (float)Utils::toDouble( value );
    }
    else if( key == "name" ) {
        this->name = value;
    }
    else if( key == "position" ) {
        this->role = value;
    }
    else if( key == "range" ) {
        this->atk_range = (float)Utils::toDouble( value );
    }
    else if( key == "rec" ) {
        this->recover = (float)Utils::toDouble( value );
    }
    else if( key == "ten") {
        this->tenacity = (float)Utils::toDouble( value );
    }
}

UnitNode::UnitNode() : _state( eUnitState::Unknown_Unit_State ) {
}

UnitNode::~UnitNode() {
    if( _unit_data ) {
        _unit_data->release();
    }
}

eUnitCamp UnitNode::getCampByString( const std::string& camp_string ) {
    if( camp_string == UNIT_CAMP_ENEMY ) {
        return eUnitCamp::Enemy;
    }
    else if( camp_string == UNIT_CAMP_NEUTRAL ) {
        return eUnitCamp::NPC;
    }
    else if( camp_string == UNIT_CAMP_ALLY ) {
        return eUnitCamp::Ally;
    }
    else if( camp_string == UNIT_CAMP_PLAYER ) {
        return eUnitCamp::Player;
    }
    else if( camp_string == UNIT_CAMP_WILD ) {
        return eUnitCamp::Wild;
    }
    else if( camp_string == UNIT_CAMP_OPPONENT ) {
        return eUnitCamp::Unknown_Camp;
    }
    return eUnitCamp::Unknown_Camp;
}

UnitNode* UnitNode::create( BattleLayer* battle_layer, const cocos2d::ValueMap& unit_data ) {
    UnitNode* ret = new UnitNode();
    if( ret && ret->init( battle_layer, unit_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UnitNode::init( BattleLayer* battle_layer, const cocos2d::ValueMap& unit_data ) {
    if( !TargetNode::init() ) {
        return false;
    }
    
    ResourceManager* res_manager = ResourceManager::getInstance();
    
    _battle_layer = battle_layer;
    _unit_data = UnitData::create( unit_data );
    _unit_data->retain();
    _direction = Point::ZERO;
    
    _face = eUnitFace::Front;
    
    auto itr = unit_data.find( "unit_camp" );
    if( itr != unit_data.end() ) {
        this->setUnitCamp( UnitNode::getCampByString( itr->second.asString() ) );
    }
    
    itr = unit_data.find( "is_boss" );
    if( itr != unit_data.end() ) {
        this->setBoss( itr->second.asBool() );
    }
    
    itr = unit_data.find( "tag_string" );
    if( itr != unit_data.end() ) {
        this->setUnitTags( itr->second.asString() );
    }
    
    itr = unit_data.find( "hold_position" );
    if( itr != unit_data.end() ) {
        this->setMovable( !itr->second.asBool() );
    }
    else {
        this->setMovable( true );
    }
    
    if( _unit_data->is_double_face ) {
        std::string resource = res_manager->getPathForResource( _unit_data->name, eResourceType::Character_Double_Face );
        _front = ArmatureManager::getInstance()->createArmature( resource );
        _back = nullptr;
    }
    else {
        std::string front_res = res_manager->getPathForResource( _unit_data->name, eResourceType::Character_Front );
        _front = ArmatureManager::getInstance()->createArmature( front_res );
        std::string back_res = res_manager->getPathForResource( _unit_data->name, eResourceType::Character_Back );
        _back = ArmatureManager::getInstance()->createArmature( back_res );
        
    }
    if( _front ) {
        _front->setScale( _unit_data->scale );
        _front->setStartListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationStart, this ) );
        _front->setEndListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationEnded, this ) );
        _front->setEventListener( CC_CALLBACK_2( UnitNode::onSkeletonAnimationEvent, this ) );
        this->addChild( _front, eComponentLayer::Object );
    }
    
    if( _back ) {
        _back->setScale( _unit_data->scale );
        _back->setStartListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationStart, this ) );
        _back->setEndListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationEnded, this ) );
        _back->setEventListener( CC_CALLBACK_2( UnitNode::onSkeletonAnimationEvent, this ) );
        this->addChild( _back, eComponentLayer::Object );
    }
    
    //shadow
    _shadow = Sprite::createWithSpriteFrameName( "unit_shadow.png" );
    _shadow->setScale( _unit_data->collide / DEFAULT_SHADOW_RADIUS );
    this->addChild( _shadow, eComponentLayer::MostBelow );
    
    _deploy_id = 0;
    _same_dir_frame_count = 0;
    this->setHesitateFrame( DEFAULT_HESITATE_FRAMES );
    this->setChasingTarget( nullptr );
    this->setShouldCatchUp( false );
    this->setConcentrateOnWalk( false );
    
    this->setCurrentSkeleton( _front );
    this->changeFace( eUnitFace::Front );
    this->changeUnitState( eUnitState::Idle );
    
    this->setWanderRadius( DEFAULT_WANDER_RADIUS );
    _relax_frames = 0;
    
    Rect bounding_box = _current_skeleton->getBoundingBox();
    
    float hpbar_width = DEFAULT_HP_BAR_WIDTH;
    float hpbar_height = DEFAULT_HP_BAR_HEIGHT;
    
    _hp_bar = HpBar::create( Size( hpbar_width, hpbar_height ) );
    _hp_bar->setPosition( Point( bounding_box.size.width / 2, bounding_box.size.height * ( 1.0f - _current_skeleton->getAnchorPoint().y ) + 10.0f ) );
    this->addChild( _hp_bar, eComponentLayer::OverObject );
    
    itr = unit_data.find( "show_hp" );
    if( itr != unit_data.end() ) {
        if( itr->second.asBool() ) {
            this->showHP();
        }
        else {
            this->hideHP();
        }
    }
    
    return true;
}

void UnitNode::updateFrame( float delta ) {
    do {
        if( _relax_frames > 0 && _state == eUnitState::Idle ) {
            --_relax_frames;
        }
        ++_same_dir_frame_count;
        this->applyUnitState();
        
        if( _state != eUnitState::Dying && _state != eUnitState::Dead && _state != eUnitState::Disappear ) {
            //skill
            auto itr = _behaviors.find( BEHAVIOR_NAME_SKILL );
            if( itr != _behaviors.end() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            //attack
            itr = _behaviors.find( BEHAVIOR_NAME_ATTACK );
            if( itr != _behaviors.end() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            itr = _behaviors.find( BEHAVIOR_NAME_MOVE );
            if( itr != _behaviors.end() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            itr = _behaviors.find( BEHAVIOR_NAME_IDLE );
            if( itr != _behaviors.end() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            
            this->evaluateCatchUp();
        }
    }while( false );
    
    this->updateComponents( delta );
}

void UnitNode::onSkeletonAnimationStart( int track_index ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    if( animation_name == "Attack" ) {
        
    }
    else if( animation_name == "Cast" || animation_name == "Cast2" ) {
        
    }
    else if( animation_name == "Die" ) {
        
    }
}

void UnitNode::onSkeletonAnimationEnded( int track_index ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    if( animation_name == "Attack" || animation_name == "Cast_2" || animation_name == "Cast2_2" ) {
        this->changeUnitState( eUnitState::Idle );
    }
    else if( animation_name == "Cast" || animation_name == "Cast2" ) {
        this->changeUnitState( eUnitState::Idle );
    }
    else if( animation_name == "Die" ) {
        this->changeUnitState( eUnitState::Disappear );
    }
}

void UnitNode::onSkeletonAnimationEvent( int track_index, spEvent* event ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    std::string event_name = std::string( event->data->name );
    if( ( animation_name == "Cast" || animation_name == "Cast2") && event_name == "OnCasting" ) {
        
    }
    else if( animation_name == "Attack" && event_name == "OnAttacking" ) {
        this->onAttacking();
    }
    else if( animation_name == "Attack" && event_name == "OnAttackBegan" ) {
        
    }
    else if( animation_name == "Cast" && event_name == "OnJuneng" ) {
        
    }
    else if( animation_name == "Cast2" && event_name == "OnJuneng" ) {
        
    }
}

void UnitNode::changeUnitState( eUnitState new_state ) {
    if( _state != new_state ) {
        if( _state >= eUnitState::Dying && new_state < _state ) {
            return;
        }
        _state = new_state;
        _unit_state_changed = true;
    }
}

void UnitNode::applyUnitState() {
    if( _unit_state_changed ) {
        _unit_state_changed = false;
        switch( _state ) {
            case eUnitState::Walking:
                _current_skeleton->setAnimation( 0, "Walk", true );
                break;
            case eUnitState::Idle:
                _current_skeleton->setAnimation( 0, "Idle", true );
                break;
            case eUnitState::Attacking:
                _current_skeleton->setAnimation( 0, "Attack", false );
                break;
            case eUnitState::Dying:
                _current_skeleton->setAnimation( 0, "Die", false );
                this->onDying();
                break;
            case eUnitState::Disappear:
                this->disappear();
                break;
            default:
                break;
        }
    }
}

void UnitNode::changeUnitDirection( const cocos2d::Point& new_dir ) {
    eUnitFace new_face = new_dir.y > 0 ? eUnitFace::Back : eUnitFace::Front;
    this->changeFace( new_face );
    
    float rotation = 0;
    if( new_dir.y > 0 ) {
        if( new_dir.x > 0 ) {
            rotation = ( _unit_data->default_face_dir == 2 or _unit_data->default_face_dir == 3 ) ? 180.0f : 0;
        }
        else {
            rotation = ( _unit_data->default_face_dir == 0 or _unit_data->default_face_dir == 1 ) ? 180.0f : 0;
        }
    }
    else {
        if( new_dir.x > 0 ) {
            rotation = ( _unit_data->default_face_dir == 1 or _unit_data->default_face_dir == 3 ) ? 180.0f : 0;
        }
        else {
            rotation = ( _unit_data->default_face_dir == 0 or _unit_data->default_face_dir == 2 ) ? 180.0f : 0;
        }
    }
    if( this->getCurrentSkeleton()->getRotationSkewY() != rotation ) {
        this->getCurrentSkeleton()->setRotationSkewY( rotation );
        _same_dir_frame_count = 0;
    }
    Point normalized_dir = new_dir;
    normalized_dir.normalize();
    this->setUnitDirection( normalized_dir );
}

void UnitNode::changeFace( eUnitFace face ) {
    if( !_unit_data->is_double_face ) {
        if( face == eUnitFace::Front ) {
            _front->setVisible( true );
            _back->setVisible( false );
            this->setCurrentSkeleton( _front );
        }
        else {
            _front->setVisible( false );
            _back->setVisible( true );
            this->setCurrentSkeleton( _back );
        }
    }
    if( _face != face ) {
        _face = face;
        _unit_state_changed = true;
        _same_dir_frame_count = 0;
        this->applyUnitState();
    }
}

cocos2d::Point UnitNode::getLocalHitPos() {
    return ArmatureManager::getInstance()->getBonePosition( _current_skeleton, "shen" );
}

cocos2d::Point UnitNode::getHitPos() {
    Point ret = this->getLocalHitPos();
    ret = _current_skeleton->convertToWorldSpace( ret );
    ret = this->getParent()->convertToNodeSpace( ret );
    return ret;
}

cocos2d::Point UnitNode::getEmitPos() {
    Point ret = ArmatureManager::getInstance()->getBonePosition( _current_skeleton, "EmitPoint" );
    ret = _current_skeleton->convertToWorldSpace( ret );
    ret = this->getParent()->convertToNodeSpace( ret );
    return ret;
}

cocos2d::Point UnitNode::getLocalHeadPos() {
    return ArmatureManager::getInstance()->getBonePosition( _current_skeleton, "tou" );
}

void UnitNode::appear() {
    std::string resource = "effects/unit_appear";
    spine::SkeletonAnimation* appear_effect = ArmatureManager::getInstance()->createArmature( resource );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( appear_effect, "unit_appear_effect", true );
    component->setPosition( this->getLocalHitPos() );
    this->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
    component->setAnimation( 0, "animation", false );
}

void UnitNode::disappear() {
    FadeTo* fadeout = FadeTo::create( 1.2f, 0 );
    CallFunc* callback = CallFunc::create( CC_CALLBACK_0( UnitNode::onDisappearEnd, this ) );
    Sequence* seq = Sequence::create( fadeout, callback, nullptr );
    _current_skeleton->runAction( seq );
}

void UnitNode::onDisappearEnd() {
    this->changeUnitState( eUnitState::Dead );
}

void UnitNode::onDying() {
    Sprite* blood = Sprite::createWithSpriteFrameName( "unit_deadblood.png" );
    UnitNodeFadeoutComponent* component = UnitNodeFadeoutComponent::create( blood, "dead_blood", 3.0f, 0, true );
    component->setPosition( Point::ZERO );
    this->addUnitComponent( component, component->getName(), eComponentLayer::BelowObject );
}

void UnitNode::takeDamage( float amount, bool is_cri, bool is_miss, int source_id ) {
    if( _unit_data->current_hp > 0 ) {
        _unit_data->current_hp -= amount;
        
        if( _unit_data->current_hp <= 0 ) {
            _unit_data->current_hp = 0;
        }
        
        //jump damage number
        std::string jump_text_name = Utils::stringFormat( "damage_from_%d_%f_jump_text", source_id, _battle_layer->getGameTime() );
        this->jumpNumber( amount, "damage", is_cri, jump_text_name );
        
        //update blood bar
        _hp_bar->setPercentage( _unit_data->current_hp / _unit_data->hp * 100.0f );
        
        //dying
        if( _unit_data->current_hp == 0 ) {
            this->changeUnitState( eUnitState::Dying );
        }
        else if( _chasing_target == nullptr ) {
            UnitNode* atker = _battle_layer->getAliveUnitByDeployId( source_id );
            if( atker ) {
                this->setChasingTarget( atker );
            }
        }
    }
}

void UnitNode::takeHeal( float amount, bool is_cri, int source_id ) {
    _unit_data->current_hp += amount;
    if( _unit_data->current_hp > _unit_data->hp ) {
        _unit_data->current_hp = _unit_data->hp;
    }
    //add heal effect
    std::string resource = "effects/heal";
    std::string component_name = Utils::stringFormat( "heal_from_%d", source_id );
    spine::SkeletonAnimation* effect = ArmatureManager::getInstance()->createArmature( resource );
    effect->setScale( 0.7f );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( effect, component_name, true );
    component->setPosition( this->getLocalHitPos() );
    this->addUnitComponent( component, component_name, eComponentLayer::OverObject );
    component->setAnimation( 0, "animation", false );
    
    //update blood bar
    _hp_bar->setPercentage( _unit_data->current_hp / _unit_data->hp * 100.0f );
    
    //jump number
    std::string jump_text_name = component_name + "_jump_text";
    this->jumpNumber( amount, "heal", is_cri, component_name );
}

void UnitNode::riseup( float duration, float height ) {
    
}

void UnitNode::falldown() {
    
}

void UnitNode::setGLProgrameState( const std::string& name ) {
    GLProgramState* gl_program_state = GLProgramState::getOrCreateWithGLProgramName( name );
    if( gl_program_state ) {
        _front->setGLProgramState( gl_program_state );
        if( _back ) {
            _back->setGLProgramState( gl_program_state );
        }
    }
}

bool UnitNode::addUnitComponent( UnitNodeComponent* component, const std::string& key, eComponentLayer layer_type ) {
    if( _components.find( key ) == _components.end() ) {
        _components.insert( key, component );
        this->addChild( component, layer_type );
        return true;
    }
    return false;
}

void UnitNode::removeUnitComponent( const std::string& key ) {
    auto itr = _components.find( key );
    if( itr != _components.end() ) {
        UnitNodeComponent* component = itr->second;
        _components.erase( itr );
        component->removeFromParent();
    }
}

void UnitNode::adjustAllUnitComponents() {
    
}

void UnitNode::removeAllUnitComponents() {
    for( auto pair : _components ) {
        pair.second->removeFromParent();
    }
    _components.clear();
}

void UnitNode::showHP() {
    _show_hp = true;
    if( _hp_bar ) {
        _hp_bar->setVisible( true );
    }
}

void UnitNode::hideHP() {
    _show_hp = false;
    if( _hp_bar ) {
        _hp_bar->setVisible( false );
    }
}

void UnitNode::applyCustomChange( const std::string& content_string ) {
    std::vector<std::string> change_pairs;
    Utils::split( content_string, change_pairs, ',' );
    for( auto str : change_pairs ) {
        std::vector<std::string> pair;
        Utils::split( str, pair, ':' );
        _unit_data->setAttribute( pair.at( 0 ), pair.at( 1 ) );
    }
}

void UnitNode::addBuff( const std::string& name, Buff* buff ) {
    if( !this->hasBuff( name ) ) {
        _buffs.insert( name, buff );
    }
}

void UnitNode::removeBuff( const std::string& name ) {
    auto itr = _buffs.find( name );
    if( itr != _buffs.end() ) {
        _buffs.erase( itr );
    }
}

bool UnitNode::hasBuff( const std::string& name ) {
    auto itr = _buffs.find( name );
    return itr != _buffs.end();
}
    
void UnitNode::removeAllBuffs() {
    
}

void UnitNode::useSkill( int skill_id, cocos2d::Point& dir, float range ) {
    
}

bool UnitNode::willCollide( cocos2d::Point pos, float radius ) {
    cocos2d::Vec2 d = this->getPosition() - pos;
    return ( Fuzzy::_less(d.lengthSquared(), powf( _unit_data->collide + radius, 2.0f ) ) ); //单位和单位允许相切，因为push单位的结果是刚好相切的
}

bool UnitNode::willCollide( UnitNode* unit) {
    return this->willCollide( unit->getPosition(), unit->getUnitData()->collide );
}

bool UnitNode::willCollide( UnitNode* unit, cocos2d::Point unit_new_pos ) {
    cocos2d::Vec2 d = this->getPosition() - unit_new_pos;
    return ( Fuzzy::_less(d.lengthSquared(), powf( _unit_data->collide + unit->getUnitData()->collide, 2.0f ) ) );
}

bool UnitNode::getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir ) {
    cocos2d::Vec2 circle_center_dir( unit->getPosition(), this->getPosition() );
    if ( Fuzzy::_greater( circle_center_dir.cross( old_dir ), 0.0f ) ) {
        new_dir = Geometry::anticlockwisePerpendicularVecToLine( circle_center_dir );
    }
    else {
        new_dir = Geometry::clockwisePerpendicularVecToLine( circle_center_dir );
    }
    return true;
}

void UnitNode::addBehavior( const std::string& key, BehaviorBase* behavior ) {
    _behaviors.insert( key, behavior );
}

void UnitNode::removeBehavior( const std::string& key ) {
    _behaviors.erase( key );
}

void UnitNode::walkTo( const cocos2d::Point& new_pos ) {
    this->changeUnitState( eUnitState::Walking );
    Point origin_dir = new_pos - this->getPosition();
    std::vector<Collidable*> collidables;
    float max_walk_length = new_pos.distance( this->getPosition() );
    Point new_dir = origin_dir;
    Point dest_pos = new_pos;
    Terrain::getInstance()->getMeshByUnitRadius( _unit_data->collide )->getNearbyBorders( this->getPosition(), max_walk_length, collidables );
    
    for( auto id_u : _battle_layer->getAliveUnits() ) { //单位们拷一遍到collidables中。以后如果单位特别多，也可以针对单位使用rtree。
        if( id_u.second == this)
            continue; //不和自己碰撞
        collidables.push_back(id_u.second);
    }
    
    std::set<Collidable*> steered_collidables;
    
    for (;;) {//其实很少转两次，一般最多转一次。但转两次的情况也是存在的，即在一个钝角处，先被边A转向，再被边B转向，结果还是小于指定的角度区间。先被单位A转向，再被单位B转向也是一样的
        bool no_steer = true;
        
        //看看目前的方案是否有碰撞，如果有任意一个碰撞，那么尝试绕过它。
        for( auto c : collidables ) {
            if( c->willCollide( this, dest_pos ) ) {
                //如果碰撞了一个曾经绕过的东西，那就不用尝试再绕了，走不通
                if( steered_collidables.count( c ) ) {
                    return;
                }
                
                steered_collidables.insert( c );
                
                no_steer = false;
                
                if ( !c->getAdvisedNewDir( this, cocos2d::Vec2( this->getPosition(), dest_pos ), new_dir ) ) { //如果已经给不出建议方向了，那么说明没角度(正撞在墙上了)，说明走不通。如果是绕到这个方向再撞墙的，那也可以说明原方向走不通。
                    return;
                }
                
                if( Geometry::deviateLessThan90( origin_dir, new_dir ) ) {
                    dest_pos = this->getPosition() + new_dir.getNormalized() * max_walk_length;
                    break; //break是为了进到外面那个for循环，再从头判
                }
                else {//如果超过了的话，也说明走不通
                    return;
                }
            }
        }
        
        if( no_steer) {
            break;
        }
    }
    
    if( !this->isOscillate( new_dir ) ) {
        this->changeUnitDirection( new_dir );
    }
    this->setPosition( dest_pos );
}

void UnitNode::walkAlongPath( float distance ) {
    if( _walk_path ) {
        Point last_pos = this->getPosition();
        Point new_pos = _walk_path->steps.at( 0 );
        if( new_pos.distance( last_pos ) > distance ) {
            Point dir = _walk_path->steps.at( 0 ) - last_pos;
            dir.normalize();
            new_pos = last_pos + dir * distance;
        }
        
        this->walkTo( new_pos );
        
        //whether reach destination
        Point unit_pos = this->getPosition();
        if( unit_pos.distance( _walk_path->steps.at( 0 ) ) < 5.0 ) {
            _walk_path->steps.erase( _walk_path->steps.begin() );
            if( _walk_path->steps.size() == 0 ) {
                _walk_path.reset();
                this->changeUnitState( eUnitState::Idle );
                _relax_frames = DEFAULT_RELAX_FRAMES;
            }
        }
    }
}

bool UnitNode::isUnderControl() {
    return _state == eUnitState::UnderControl;
}

bool UnitNode::isCasting() {
    return _state == eUnitState::Casting;
}

bool UnitNode::isAttacking() {
    return _state == eUnitState::Attacking;
}

bool UnitNode::isWalking() {
    return _state == eUnitState::Walking;
}

bool UnitNode::isDying() {
    return _state == eUnitState::Dying;
}

bool UnitNode::isOscillate( const cocos2d::Point& new_dir ) {
    const cocos2d::Point& last_dir = this->getUnitDirection();
    if( ( new_dir.x > 0 && last_dir.x <= 0 )
       || ( new_dir.x <= 0 && last_dir.x > 0 )
       || ( new_dir.y > 0 && last_dir.y <= 0 )
       || ( new_dir.y <= 0 && last_dir.y > 0 ) ) {
        if( _same_dir_frame_count < _hesitate_frame ) {
            return true;
        }
    }
    return false;
}

void UnitNode::evaluateCatchUp() {
    const Point& pos = this->getPosition();
    const Point& leader_pos = _battle_layer->getLeaderUnit()->getPosition();
    float distance = pos.distance( leader_pos );
    if( !_should_catch_up && distance > DEFAULT_CATCH_UP_START_DISTANCE ) {
        this->setShouldCatchUp( true );
    }
    else if( _should_catch_up && distance < DEFAULT_CATCH_UP_STOP_DISTANCE ) {
        this->setShouldCatchUp( false );
    }
}

void UnitNode::findPathToPosition( const cocos2d::Point& pos, int validate_frames ) {
    _walk_path = Terrain::getInstance()->getMeshByUnitRadius( _unit_data->collide )->findPath( this->getPosition(), pos, validate_frames );
}

bool UnitNode::isHarmless() {
    return _unit_data->atk <= 0;
}

TargetNode* UnitNode::getAttackTarget() {
    TargetNode* ret = _chasing_target;
    float min_distance = ( _chasing_target == nullptr ) ? 10000.0f : _chasing_target->getPosition().distance( this->getPosition() );
    std::list<UnitNode*> candidates = _battle_layer->getAliveOpponents( _camp );
    for( auto unit : candidates ) {
        if( unit->isAttackable() && this->isUnitInDirectView( unit ) ) {
            float distance = this->getPosition().distance( unit->getPosition() );
            if( distance < min_distance ) {
                ret = unit;
                min_distance = distance;
            }
        }
        
    }
    return ret;
}

bool UnitNode::canAttack( TargetNode* target_node ) {
    if( UnitNode* unit = dynamic_cast<UnitNode*>( target_node ) ) {
        if( this->getPosition().distance( unit->getPosition() ) > _unit_data->atk_range + unit->getUnitData()->collide ) {
            return false;
        }
    }
    
    if( Terrain::getInstance()->isBlocked( this->getPosition(), target_node->getPosition() ) ) {
        return false;
    }
    
    return true;
}

void UnitNode::attack( TargetNode* unit ) {
    this->setChasingTarget( unit );
    Point dir = unit->getPosition() - this->getPosition();
    dir.normalize();
    this->changeUnitDirection( dir );
    this->changeUnitState( eUnitState::Attacking );
}

void UnitNode::onCharging( int i ) {
    
}

void UnitNode::onAttackBegan() {
    
}

void UnitNode::onAttacking() {
    if( _chasing_target ) {
        if( _unit_data->is_melee ) {
            UnitNode* target_unit = dynamic_cast<UnitNode*>( _chasing_target );
            if( target_unit ) {
                DamageCalculate* damage_calculator = DamageCalculate::create( "normal" );
                ValueMap result = damage_calculator->calculateDamage( 0, _unit_data, target_unit->getUnitData() );
                target_unit->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), _deploy_id );
            }
        }
        else {
            DamageCalculate* damage_calculator = DamageCalculate::create( "normal" );
            BulletNode* bullet = BulletNode::create( this, ResourceManager::getInstance()->getBulletData( _unit_data->bullet_name ), damage_calculator, nullptr );
            bullet->shootAt( this, _chasing_target );
        }
    }
}

void UnitNode::onCasting() {
    
}

bool UnitNode::isUnitInDirectView( UnitNode* unit ) {
    if( Terrain::getInstance()->isBlocked( this->getPosition(), unit->getPosition() ) ) {
        return false;
    }
    else if( this->getPosition().distance( unit->getPosition() ) > _unit_data->guard_radius + unit->getUnitData()->collide ) {
        return false;
    }
    return true;
}

void UnitNode::setUnitTags( const std::string& tag_string ) {
    Utils::split( tag_string, _unit_tags, ',' );
}

bool UnitNode::hasUnitTag( const std::string& tag_name ) {
    for( auto itr = _unit_tags.begin(); itr != _unit_tags.end(); ++itr ) {
        if( itr->compare( tag_name ) == 0 ) {
            return true;
        }
    }
    return false;
}

bool UnitNode::isFoeOfCamp( eUnitCamp opponent_camp ) {
    eUnitCamp camp = this->getUnitCamp();
    if( camp == eUnitCamp::Player || camp == eUnitCamp::Ally ) {
        if( opponent_camp == eUnitCamp::Enemy || opponent_camp == eUnitCamp::Wild ) {
            return true;
        }
    }
    else if( camp == eUnitCamp::Enemy ) {
        if( opponent_camp == eUnitCamp::Player || opponent_camp == eUnitCamp::Ally || opponent_camp == eUnitCamp::Wild ) {
            return true;
        }
    }
    else if( camp == eUnitCamp::Wild ) {
        if( opponent_camp == eUnitCamp::Player || opponent_camp == eUnitCamp::Ally || opponent_camp == eUnitCamp::Enemy ) {
            return true;
        }
    }
    return false;
}

bool UnitNode::needRelax() {
    return ( _relax_frames > 0 );
}

cocos2d::Point UnitNode::getNextWanderPos() {
    for( int i = 0; i < 3; i++ ) {
        float r = Utils::randomNumber( _wander_radius );
        float angle = Utils::randomFloat() * M_PI;
        Point new_pos = Point( _born_position.x + cosf( angle ) * r, _born_position.y + sinf( angle ) * r );
        if( _battle_layer->isPositionOK( new_pos, _unit_data->collide ) ) {
            return new_pos;
        }
    }
    return Point::ZERO;
}

void UnitNode::setWalkPath( const Path& path ) {
    _walk_path = std::unique_ptr<Path>( new Path( INT_MAX ) );
    _walk_path->steps = path.steps;
}

void UnitNode::jumpNumber( float amount, const std::string& type, bool is_critical, const std::string& name ) {
    JumpText* jump_text = JumpText::create( Utils::toStr( (int)amount ), type, is_critical, _camp, name );
    this->addUnitComponent( jump_text, name, eComponentLayer::OverObject );
    jump_text->setPosition( this->getLocalHeadPos() );
    jump_text->start( is_critical );
}

//private methods
void UnitNode::updateComponents( float delta ) {
    cocos2d::Map<std::string, UnitNodeComponent*> components = _components;
    for( auto pair : components ) {
        UnitNodeComponent* comp = pair.second;
        if( comp->shouldRecycle() ) {
            this->removeUnitComponent( comp->getName() );
        }
        else {
            comp->updateFrame( delta );
        }
    }
}