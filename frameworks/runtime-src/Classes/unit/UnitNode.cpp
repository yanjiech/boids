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
#define DEFAULT_CATCH_UP_STOP_DISTANCE 150.0
#define DEFAULT_CATCH_UP_START_DISTANCE 250.0
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
    
    const ValueMap& unit_config = ResourceManager::getInstance()->getUnitData( name );
    
    this->unit_id = unit_config.at( "id" ).asInt();
    this->display_name = unit_config.at( "displayname" ).asString();
    this->hp = unit_config.at( "hp" ).asFloat() + this->level * (float)unit_config.at( "hpgr" ).asFloat();
    this->current_hp = this->hp;
    this->mp = unit_config.at( "mp" ).asFloat() + this->level * (float)unit_config.at( "mpgr" ).asFloat();
    this->current_mp = this->mp;
    this->atk = unit_config.at( "atk" ).asFloat() + this->level * unit_config.at( "atkgr" ).asFloat();
    this->def = unit_config.at( "def" ).asFloat() + this->level * unit_config.at( "defgr" ).asFloat();
    this->move_speed = unit_config.at( "movespeed" ).asFloat();
    this->atk_speed = unit_config.at( "attackspeed" ).asFloat();
    this->collide = unit_config.at( "collide" ).asFloat();
    this->critical = unit_config.at( "cri" ).asFloat() + this->level * unit_config.at( "crigr" ).asFloat();
    this->tenacity = unit_config.at( "ten" ).asFloat() + this->level * unit_config.at( "tengr" ).asFloat();
    this->hit = unit_config.at( "hit" ).asFloat() + this->level * unit_config.at( "hitgr" ).asFloat();
    this->dodge = unit_config.at( "dodge" ).asFloat() + this->level * unit_config.at( "dodge" ).asFloat();
    
    this->guard_radius = unit_config.at( "guard_radius" ).asFloat();
    this->atk_range = unit_config.at( "range" ).asFloat();
    
    this->recover = unit_config.at( "rec" ).asFloat() + this->level * unit_config.at( "recgr" ).asFloat();
    this->scale = unit_config.at( "scale" ).asFloat();
    
    this->role = unit_config.at( "position" ).asString();
                                
    this->is_melee = unit_config.at( "is_melee" ).asBool();
    this->is_double_face = unit_config.at( "double_face" ).asBool();
    this->default_face_dir = unit_config.at( "faceleft" ).asInt();
    
    auto itr = unit_config.find( "bullet_name" );
    if( itr != unit_config.end() ) {
        this->bullet_name = itr->second.asString();
    }
    else {
        this->bullet_name = "";
    }
    
    const ValueVector& skill_vector = unit_config.at( "skills" ).asValueVector();
    for( auto itr = skill_vector.begin(); itr != skill_vector.end(); ++itr ) {
        std::string skl_name = itr->asString();
        skill_names.push_back( skl_name );
    }
    
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
    else if( key == "mp" ) {
        this->mp = (float)Utils::toDouble( value );
        this->current_mp = this->mp;
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

UnitNode::UnitNode() :
_state( eUnitState::Unknown_Unit_State ) {
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
    
    itr = unit_data.find( "skills" );
    if( itr != unit_data.end() ) {
        const cocos2d::ValueVector& skill_vector = itr->second.asValueVector();
        for( auto sk_itr = skill_vector.begin(); sk_itr != skill_vector.end(); ++sk_itr ) {
            const cocos2d::ValueMap& sk_data = sk_itr->asValueMap();
            Skill* skill = Skill::create( this, sk_data );
            _skills.pushBack( skill );
        }
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
        _front->setCompleteListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationCompleted, this ) );
        _front->setEventListener( CC_CALLBACK_2( UnitNode::onSkeletonAnimationEvent, this ) );
        this->addChild( _front, eComponentLayer::Object );
    }
    
    if( _back ) {
        _back->setScale( _unit_data->scale );
        _back->setStartListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationStart, this ) );
        _back->setCompleteListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationCompleted, this ) );
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
    
    _face = eUnitFace::Back;
    this->setCurrentSkeleton( _front );
    this->changeUnitDirection( Point( 1.0f, 0 ) );
    this->setNextUnitState( eUnitState::Unknown_Unit_State );
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
        
        if( !this->isDying() ) {
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
    this->updateBuffs( delta );
    this->updateSkills( delta );
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
}

void UnitNode::onSkeletonAnimationCompleted( int track_index ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    if( animation_name == "Attack" ) {
        this->changeUnitState( eUnitState::Idle );
    }
    else if( animation_name == "Cast_2" || animation_name == "Cast2_2" ) {
        this->endSkill();
        this->changeUnitState( eUnitState::Idle );
    }
    else if( animation_name == "Cast" || animation_name == "Cast2" ) {
        if( _using_skill_params["multi_action"].asBool() ) {
            if( _using_skill_params["state"].asString() == "start" ) {
                _unit_state_changed = true;
                this->setNextUnitState( eUnitState::Casting );
                _using_skill_params["state"] = Value( "continue" );
            }
        }
        else {
            this->endSkill();
            this->changeUnitState( eUnitState::Idle );
        }
    }
//    else if( animation_name == "Cast_1" || animation_name == "Cast2_1" ) {
//        if( _using_skill_params["state"].asString() == "end" ) {
//            _unit_state_changed = true;
//            this->setNextUnitState( eUnitState::Casting );
//        }
//    }
    else if( animation_name == "Die" ) {
        this->changeUnitState( eUnitState::Disappear );
    }
}

void UnitNode::onSkeletonAnimationEvent( int track_index, spEvent* event ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    std::string event_name = std::string( event->data->name );
    if( ( animation_name == "Cast" || animation_name == "Cast2" ) && event_name == "OnCasting" ) {
        this->onCasting();
    }
    else if( animation_name == "Attack" && event_name == "OnAttacking" ) {
        this->onAttacking();
    }
    else if( animation_name == "Attack" && event_name == "OnAttackBegan" ) {
        this->onAttackBegan();
    }
    else if( animation_name == "Cast" && event_name == "OnJuneng" ) {
        this->onCharging();
    }
    else if( animation_name == "Cast2" && event_name == "OnJuneng" ) {
        this->onCharging();
    }
}

void UnitNode::changeUnitState( eUnitState new_state, bool force ) {
    if( _state >= eUnitState::Dying && new_state < _state ) {
        return;
    }
    if( force || ( ( _next_state == eUnitState::Unknown_Unit_State && new_state != _state ) || new_state >= eUnitState::Dying ) ) {
        _next_state = new_state;
        _unit_state_changed = true;
    }
}

void UnitNode::applyUnitState() {
    if( _unit_state_changed ) {
        _unit_state_changed = false;
        _state = _next_state;
        _next_state = eUnitState::Unknown_Unit_State;
        switch( _state ) {
            case eUnitState::Casting:
            {
                std::string state = _using_skill_params["state"].asString();
                int sk_id = _using_skill_params.at( "skill_id" ).asInt();
                if( state == "start" ) {
                    if( sk_id == 0 ) {
                        _current_skeleton->setAnimation( 0, "Cast", false );
                    }
                    else if( sk_id == 1 ) {
                        if( _current_skeleton->setAnimation( 0, "Cast2", false ) == nullptr ) {
                            _current_skeleton->setAnimation( 0, "Cast", false );
                        }
                    }
                }
                else if( state == "end" ) {
                    if( sk_id == 0 ) {
                        _current_skeleton->setAnimation( 0, "Cast_2", false );
                    }
                    else if( sk_id == 1 ) {
                        _current_skeleton->setAnimation( 0, "Cast2_2", false );
                    }
                }
                else if( _using_skill_params["multi_action"].asBool() ) {
                    if( sk_id == 0 ) {
                        _current_skeleton->setAnimation( 0, "Cast_1", true );
                    }
                    else if( sk_id == 1 ) {
                        _current_skeleton->setAnimation( 0, "Cast2_1", true );
                    }
                }
            }
                break;
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
            case eUnitState::UnderControl:
                _current_skeleton->setAnimation( 0, "Stun", true );
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
            rotation = ( _unit_data->default_face_dir == 2 || _unit_data->default_face_dir == 3 ) ? 180.0f : 0;
        }
        else {
            rotation = ( _unit_data->default_face_dir == 0 || _unit_data->default_face_dir == 1 ) ? 180.0f : 0;
        }
    }
    else {
        if( new_dir.x > 0 ) {
            rotation = ( _unit_data->default_face_dir == 1 || _unit_data->default_face_dir == 3 ) ? 180.0f : 0;
        }
        else {
            rotation = ( _unit_data->default_face_dir == 0 || _unit_data->default_face_dir == 2 ) ? 180.0f : 0;
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
        if( _face != face ) {
            if( face == eUnitFace::Front ) {
                _front->setVisible( true );
                _back->setVisible( false );
                _back->clearTrack();
                this->setCurrentSkeleton( _front );
            }
            else {
                _front->setVisible( false );
                _back->setVisible( true );
                _front->clearTrack();
                this->setCurrentSkeleton( _back );
            }
            _face = face;
            this->changeUnitState( _state, true );
            _same_dir_frame_count = 0;
        }
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

cocos2d::Point UnitNode::getLocalEmitPos() {
    return ArmatureManager::getInstance()->getBonePosition( _current_skeleton, "EmitPoint" );
}

cocos2d::Point UnitNode::getLocalHeadPos() {
    return ArmatureManager::getInstance()->getBonePosition( _current_skeleton, "tou" );
}

cocos2d::Point UnitNode::getLocalBonePos( const std::string& bone_name ) {
    return ArmatureManager::getInstance()->getBonePosition( _current_skeleton, bone_name );
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

void UnitNode::takeDamage( const cocos2d::ValueMap& result, int source_id ) {
    this->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), source_id );
}

void UnitNode::takeDamage( float amount, bool is_cri, bool is_miss, int source_id ) {
    if( _unit_data->current_hp > 0 ) {
        float damage = amount;
        for( auto itr = _buffs.begin(); itr != _buffs.end(); ++itr ) {
            ShieldBuff* buff = dynamic_cast<ShieldBuff*>( itr->second );
            if( buff ) {
                damage = buff->absorbDamage( amount );
                break;
            }
        }
        _unit_data->current_hp -= damage;
        
        if( _unit_data->current_hp <= 0 ) {
            _unit_data->current_hp = 0;
        }
        
        //jump damage number
        std::string jump_text_name = Utils::stringFormat( "damage_number_%d", BulletNode::getNextBulletId() );
        this->jumpNumber( damage, "damage", is_cri, jump_text_name );
        
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

void UnitNode::takeHeal( const cocos2d::ValueMap& result, int source_id ) {
    this->takeHeal( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), source_id );
}

void UnitNode::takeHeal( float amount, bool is_cri, int source_id ) {
    _unit_data->current_hp += amount;
    if( _unit_data->current_hp > _unit_data->hp ) {
        _unit_data->current_hp = _unit_data->hp;
    }
    //add heal effect
    std::string resource = "effects/heal";
    std::string component_name = Utils::stringFormat( "heal_effect_%d", BulletNode::getNextBulletId() );
    spine::SkeletonAnimation* effect = ArmatureManager::getInstance()->createArmature( resource );
    effect->setScale( 0.7f );
    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( effect, component_name, true );
    component->setPosition( Point::ZERO );
    this->addUnitComponent( component, component_name, eComponentLayer::OverObject );
    component->setAnimation( 0, "animation", false );
    
    //update blood bar
    _hp_bar->setPercentage( _unit_data->current_hp / _unit_data->hp * 100.0f );
    
    //jump number
    std::string jump_text_name = Utils::stringFormat( "heal_number_%d", BulletNode::getNextBulletId() );
    this->jumpNumber( amount, "heal", is_cri, jump_text_name );
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

void UnitNode::addBuff( const std::string& buff_id, Buff* buff ) {
    if( !this->hasBuff( buff_id ) ) {
        _buffs.insert( buff_id, buff );
    }
}

void UnitNode::removeBuff( const std::string& buff_id ) {
    auto itr = _buffs.find( buff_id );
    if( itr != _buffs.end() ) {
        _buffs.erase( itr );
    }
}

bool UnitNode::hasBuff( const std::string& buff_id ) {
    auto itr = _buffs.find( buff_id );
    return itr != _buffs.end();
}
    
void UnitNode::removeAllBuffs() {
    _buffs.clear();
}

void UnitNode::useSkill( int skill_id, const cocos2d::Point& dir, float range_per ) {
    if( !this->isDying() && !this->isCasting() ) {
        Point sk_dir = dir;
        if( sk_dir.x != 0 || sk_dir.y != 0 ) {
            this->changeUnitDirection( dir );
        }
        else {
            sk_dir = this->getUnitDirection();
        }
        _using_skill_params["dir_x"] = Value( sk_dir.x );
        _using_skill_params["dir_y"] = Value( sk_dir.y );
        _using_skill_params["multi_action"] = Value( _skills.at( skill_id )->shouldContinue() );
        _using_skill_params["skill_id"] = Value( skill_id );
        _using_skill_params["range_per"] = Value( range_per );
        _using_skill_params["state"] = Value( "start" );
        _using_skill_params["charging_effect"] = Value( _skills.at( skill_id )->getChargingEffect() );
        _using_skill_params["charging_effect_pos"] = Value( _skills.at( skill_id )->getChargingEffectPos() );
        this->changeUnitState( eUnitState::Casting, true );
    }
}

void UnitNode::endSkill() {
    if( this->isCasting() ) {
        int sk_id = _using_skill_params.at( "skill_id" ).asInt();
        _skills.at( sk_id )->reload();
        _using_skill_params.clear();
    }
}

void UnitNode::endCast() {
    if( this->isCasting() ) {
        _using_skill_params["state"] = Value( "end" );
        _unit_state_changed = true;
        this->setNextUnitState( eUnitState::Casting );
    }
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
    _battle_layer->onUnitMoved( this );
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
    return _state >= eUnitState::Dying;
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
    cocos2d::Vector<UnitNode*> candidates = _battle_layer->getAliveOpponents( _camp );
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

void UnitNode::onCharging() {
    std::string resource = _using_skill_params["charging_effect"].asString();
    std::string effect_pos = _using_skill_params["charging_effect_pos"].asString();
    if( resource != "" ) {
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        std::string name = Utils::stringFormat( "charging_%d", BulletNode::getNextBulletId() );
        UnitNodeFollowSpineComponent* charging_component = UnitNodeFollowSpineComponent::create( this, effect_pos, skeleton, name, true );
        if( effect_pos != "" ) {
            charging_component->setPosition( this->getLocalBonePos( effect_pos ) );
        }
        this->addUnitComponent( charging_component, charging_component->getName(), eComponentLayer::OverObject );
        charging_component->setAnimation( 0, "animation", false );
    }
}

void UnitNode::onAttackBegan() {
    
}

void UnitNode::onAttacking() {
    if( _chasing_target ) {
        if( _unit_data->is_melee ) {
            UnitNode* target_unit = dynamic_cast<UnitNode*>( _chasing_target );
            if( target_unit ) {
                DamageCalculate* damage_calculator = DamageCalculate::create( "normal", 0 );
                ValueMap result = damage_calculator->calculateDamage( _unit_data, target_unit->getUnitData() );
                target_unit->takeDamage( result, _deploy_id );
                
                if( !result.at( "miss" ).asBool() ) {
                    std::string hit_resource = "effects/bullets/default_hit";
                    spine::SkeletonAnimation* hit_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
                    hit_effect->setScale( 0.7f );
                    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( hit_effect, Utils::stringFormat( "bullet_%d_hit", BulletNode::getNextBulletId() ), true );
                    component->setPosition( target_unit->getLocalHitPos() );
                    target_unit->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
                    component->setAnimation( 0, "animation", false );
                }
            }
        }
        else {
            DamageCalculate* damage_calculator = DamageCalculate::create( "normal", 0 );
            BulletNode* bullet = BulletNode::create( this, ResourceManager::getInstance()->getBulletData( _unit_data->bullet_name ), damage_calculator, nullptr );
            bullet->shootAt( this, _chasing_target );
        }
    }
}

void UnitNode::onCasting() {
    int sk_id = _using_skill_params["skill_id"].asInt();
    _skills.at( sk_id )->activate( _using_skill_params );
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
    _unit_tags.clear();
    std::vector<std::string> tags;
    Utils::split( tag_string, tags, ',' );
    for( auto str : tags ) {
        _unit_tags.push_back( Value( str ) );
    }
}

bool UnitNode::hasUnitTag( const std::string& tag_name ) {
    for( auto itr = _unit_tags.begin(); itr != _unit_tags.end(); ++itr ) {
        if( itr->asString() == tag_name ) {
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

std::string UnitNode::getSkillHintTypeById( int sk_id ) {
    return _skills.at( sk_id )->getSkillHintType();
}

float UnitNode::getSkillRadiusById( int sk_id ) {
    return _skills.at( sk_id )->getSkillRadius();
}

float UnitNode::getSkillRangeById( int sk_id ) {
    return _skills.at( sk_id )->getSkillRange();
}

float UnitNode::getSkillMinRangeById( int sk_id ) {
    return _skills.at( sk_id )->getSkillMinRange();
}

float UnitNode::getSkillMaxRangeById( int sk_id ) {
    return _skills.at( sk_id )->getSkillMaxRange();
}

float UnitNode::getSkillCDById( int sk_id ) {
    return _skills.at( sk_id )->getSkillCD();
}

bool UnitNode::isSkillReadyById( int sk_id ) {
    return _skills.at( sk_id )->isSkillReady();
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

void UnitNode::updateBuffs( float delta ) {
    cocos2d::Map<std::string, Buff*> buffs = _buffs;
    for( auto pair : buffs ) {
        Buff* buff = pair.second;
        if( buff->shouldRecycle() ) {
            this->removeBuff( buff->getBuffId() );
        }
        else {
            buff->updateFrame( delta );
        }
    }
}

void UnitNode::updateSkills( float delta ) {
    for( auto skill : _skills ) {
        skill->updateFrame( delta );
    }
}

void UnitNode::riseup( float duration, float delta_height ) {
    MoveBy* rise = MoveBy::create( duration, Point( 0, delta_height ) );
    rise->setTag( 10000 );
    _current_skeleton->runAction( rise );
}

void UnitNode::falldown( float duration, float delta_height ) {
    _current_skeleton->stopActionByTag( 10000 );
    _current_skeleton->setPosition( Point::ZERO );
}

bool UnitNode::isAlive() {
    return _state < eUnitState::Dying;
}