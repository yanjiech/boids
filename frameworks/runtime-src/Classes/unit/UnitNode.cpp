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
#include "ui/CocosGUI.h"
#include "../BoidsMath.h"
#include "../data/PlayerInfo.h"
#include "../manager/AudioManager.h"
#include "../unit/skill/SkillCache.h"

#define DEFAULT_SHADOW_RADIUS 30.0
#define DEFAULT_HESITATE_FRAMES 5
#define DEFAULT_CATCH_UP_STOP_DISTANCE 250.0
#define DEFAULT_CATCH_UP_START_DISTANCE 700.0
#define DEFAULT_WANDER_RADIUS 200.0

using namespace cocos2d;

UnitNode::UnitNode() :
_state( eUnitState::Unknown_Unit_State ),
_next_state( eUnitState::Unknown_Unit_State ),
_guard_target( nullptr ),
_walk_path( nullptr ),
_tour_path( nullptr ),
_is_charging( false ),
_charging_effect( nullptr ),
_chasing_target( nullptr ),
_weapon( nullptr ),
_armor( nullptr ),
_boot( nullptr ),
_accessory( nullptr ),
_hint_node( nullptr )
{
}

UnitNode::~UnitNode() {
    CC_SAFE_RELEASE( _guard_target );
    CC_SAFE_RELEASE( _walk_path );
    CC_SAFE_RELEASE( _tour_path );
    CC_SAFE_RELEASE( _chasing_target );
    CC_SAFE_RELEASE( _weapon );
    CC_SAFE_RELEASE( _armor );
    CC_SAFE_RELEASE( _boot );
    CC_SAFE_RELEASE( _accessory );
}

eTargetCamp UnitNode::getCampByString( const std::string& camp_string ) {
    if( camp_string == UNIT_CAMP_ENEMY ) {
        return eTargetCamp::Enemy;
    }
    else if( camp_string == UNIT_CAMP_NEUTRAL ) {
        return eTargetCamp::NPC;
    }
    else if( camp_string == UNIT_CAMP_ALLY ) {
        return eTargetCamp::Ally;
    }
    else if( camp_string == UNIT_CAMP_PLAYER ) {
        return eTargetCamp::Player;
    }
    else if( camp_string == UNIT_CAMP_WILD ) {
        return eTargetCamp::Wild;
    }
    else if( camp_string == UNIT_CAMP_OPPONENT ) {
        return eTargetCamp::Unknown_Camp;
    }
    return eTargetCamp::Unknown_Camp;
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
    if( !TargetNode::init( battle_layer ) ) {
        return false;
    }
    
    ResourceManager* res_manager = ResourceManager::getInstance();
    
    UnitData* target_data = UnitData::create( unit_data );
    this->setTargetData( target_data );
    
    _direction = Point::ZERO;
    
    _face = eUnitFace::Front;
    
    _is_charging = false;
    
    this->setConcentrateOnWalk( false );
    
    auto itr = unit_data.find( "unit_camp" );
    if( itr != unit_data.end() ) {
        this->setTargetCamp( UnitNode::getCampByString( itr->second.asString() ) );
    }
    
    itr = unit_data.find( "tag_string" );
    if( itr != unit_data.end() ) {
        this->setUnitTags( itr->second.asString() );
    }
    
    itr = unit_data.find( "hold_position" );
    if( itr != unit_data.end() ) {
        //no move behavior
    }
    else {
        //with move behavior
    }
    
    if( target_data->is_double_face ) {
        std::string resource = res_manager->getPathForResource( target_data->name, eResourceType::Character_Double_Face );
        _front = ArmatureManager::getInstance()->createArmature( resource );
        _back = nullptr;
    }
    else {
        std::string front_res = res_manager->getPathForResource( target_data->name, eResourceType::Character_Front );
        _front = ArmatureManager::getInstance()->createArmature( front_res );
        std::string back_res = res_manager->getPathForResource( target_data->name, eResourceType::Character_Back );
        _back = ArmatureManager::getInstance()->createArmature( back_res );
    }
    if( _front ) {
        _front->setScale( target_data->scale );
        _desired_unit_scale = target_data->scale;
        _front->setStartListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationStart, this ) );
        _front->setCompleteListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationCompleted, this ) );
        _front->setEventListener( CC_CALLBACK_2( UnitNode::onSkeletonAnimationEvent, this ) );
        this->addChild( _front, eComponentLayer::Object );
        Rect bounding_box = _front->getBoundingBox();
        this->setContentSize( bounding_box.size );
    }
    
    if( _back ) {
        _back->setScale( target_data->scale );
        _back->setStartListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationStart, this ) );
        _back->setCompleteListener( CC_CALLBACK_1( UnitNode::onSkeletonAnimationCompleted, this ) );
        _back->setEventListener( CC_CALLBACK_2( UnitNode::onSkeletonAnimationEvent, this ) );
        this->addChild( _back, eComponentLayer::Object );
    }
    
    //shadow
    _shadow = Sprite::createWithSpriteFrameName( "unit_shadow.png" );
    _shadow->setScale( target_data->collide / DEFAULT_SHADOW_RADIUS );
    this->addChild( _shadow, eComponentLayer::MostBelow );
    
    _same_dir_frame_count = 0;
    this->setHesitateFrame( DEFAULT_HESITATE_FRAMES );
    this->setChasingTarget( nullptr );
    this->setShouldCatchUp( false );
    
    //debug
//    _custom_draw = DrawNode::create();
//    _custom_draw->drawLine( Point::ZERO, Point( 100.0, 0 ), Color4F::YELLOW );
//    this->addChild( _custom_draw, 10000 );
//    
//    _new_dir_draw = DrawNode::create();
//    _new_dir_draw->drawLine( Point::ZERO, Point( 100.0, 0 ), Color4F::BLUE );
//    this->addChild( _new_dir_draw, 10001 );
//    
//    _custom_draw->drawCircle( Point::ZERO, target_data->collide, 360, 100, false, Color4F::RED );
    //end debug
    
    _face = eUnitFace::Back;
    this->setCurrentSkeleton( _front );
    this->changeUnitDirection( Point( 1.0f, 0 ) );
    this->setNextUnitState( eUnitState::Unknown_Unit_State );
    this->changeUnitState( eUnitState::Idle );
    
    this->setWanderRadius( DEFAULT_WANDER_RADIUS );
    _relax_frames = 0;
    
    //skill
    for( auto v : target_data->skills ) {
        Skill* skill = Skill::create( this, v.asValueMap() );
        _skills.pushBack( skill );
    }
    
    Rect bounding_box = _current_skeleton->getBoundingBox();
    
    float hpbar_width = DEFAULT_HP_BAR_WIDTH;
    float hpbar_height = DEFAULT_HP_BAR_HEIGHT;
    
    _hp_bar = HpBar::create( Size( hpbar_width, hpbar_height ) );
    _hp_bar->setPosition( Point( 0, bounding_box.size.height * ( 1.0f - _current_skeleton->getAnchorPoint().y ) + 10.0f ) );
    this->addChild( _hp_bar, eComponentLayer::OverObject );
    
    itr = unit_data.find( "is_boss" );
    if( itr != unit_data.end() ) {
        this->setBoss( itr->second.asBool() );
        this->hideHP();
    }
    else {
        this->setBoss( false );
        itr = unit_data.find( "show_hp" );
        if( itr != unit_data.end() ) {
            if( itr->second.asBool() ) {
                this->showHP();
            }
            else {
                this->hideHP();
            }
        }
    }
    
    _has_hint_node = true;
    
    return true;
}

void UnitNode::updateFrame( float delta ) {
    if( _relax_frames > 0 && _state == eUnitState::Idle ) {
        --_relax_frames;
    }
    
    ++_same_dir_frame_count;
    this->updateBuffs( delta );
    TargetNode::updateFrame( delta );
    
    this->updateHintNode( delta );
    
    this->applyUnitState();
    
    this->updateSkills( delta );
    this->evaluateCatchUp();
    
    if( this->isAlive() && _is_charging && _charging_effect != nullptr ) {
        _charging_effect->setPosition( this->getLocalBonePos( "ChargingPoint" ) );
    }
}

void UnitNode::onSkeletonAnimationStart( int track_index ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    if( animation_name == "Attack" ) {
        
    }
    else if( animation_name == "Cast" || animation_name == "Cast2" ) {
        
    }
    else if( animation_name == "Die" ) {
        std::string audio_res = this->getUnitData()->name + "/die.mp3";
        AudioManager::getInstance()->playEffect( audio_res );
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
        this->changeUnitState( eUnitState::Disappear, true );
    }
}

void UnitNode::onSkeletonAnimationEvent( int track_index, spEvent* event ) {
    spTrackEntry* entry = this->getCurrentSkeleton()->getCurrent();
    std::string animation_name = std::string( entry->animation->name );
    std::string event_name = std::string( event->data->name );
    if( ( animation_name == "Cast" || animation_name == "Cast2" ) && event_name == "OnCasting" ) {
        this->onCasting();
        int sk_id = _using_skill_params["skill_id"].asInt();
        std::string audio_res = this->getUnitData()->name + Utils::stringFormat( "/cast%d.mp3", sk_id + 1 );
        AudioManager::getInstance()->playEffect( audio_res );
    }
    else if( animation_name == "Attack" && event_name == "OnAttacking" ) {
        std::string audio_res = this->getUnitData()->name + "/attack.mp3";
        AudioManager::getInstance()->playEffect( audio_res );
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

TargetNode* UnitNode::getChasingTarget() {
    if( _chasing_target && _chasing_target->isDying() ) {
        this->setChasingTarget( nullptr );
    }
    return _chasing_target;
}

void UnitNode::setChasingTarget( TargetNode* target ) {
    CC_SAFE_RETAIN( target );
    CC_SAFE_RELEASE( _chasing_target );
    _chasing_target = target;
    
}

void UnitNode::changeUnitState( eUnitState new_state, bool force ) {
    if( ( _state >= eUnitState::Dying || _next_state >= eUnitState::Dying ) && ( new_state < _state || new_state < _next_state ) ) {
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
        if( _state == Casting && _next_state != Casting && _next_state != Idle ) {
            SkillCache::getInstance()->removeSkillOfOwner( this );
            this->endSkill();
        }
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
                if( _is_charging ) {
                    _current_skeleton->setAnimation( 0, "Walk2", true );
                }
                else {
                    _current_skeleton->setAnimation( 0, "Walk", true );
                }
                break;
            case eUnitState::Idle:
                if( _is_charging ) {
                    _current_skeleton->setAnimation( 0, "Idle2", true );
                }
                else {
                    _current_skeleton->setAnimation( 0, "Idle", true );
                }
                break;
            case eUnitState::Attacking:
                _current_skeleton->setAnimation( 0, "Attack", false );
                break;
            case eUnitState::Dying:
                if( _current_skeleton->setAnimation( 0, "Die", false ) == nullptr ) {
                    this->changeUnitState( eUnitState::Disappear, true );
                }
                else {
                    this->onDying();
                }
                break;
            case eUnitState::Disappear:
                this->disappear();
                break;
            case eUnitState::UnderControl:
                _current_skeleton->setAnimation( 0, "Stun", true );
                break;
            case eUnitState::Dead:
                break;
            default:
                break;
        }
    }
}

void UnitNode::changeUnitDirection( const cocos2d::Point& new_dir ) {
    eUnitFace new_face = new_dir.y > 0 ? eUnitFace::Back : eUnitFace::Front;
    this->changeFace( new_face );
    
    UnitData* unit_data = dynamic_cast<UnitData*>( _target_data );
    
    int flipped_x = 0;
    if( new_dir.y > 0 ) {
        if( new_dir.x > 0 ) {
            flipped_x = ( unit_data->default_face_dir == 2 || unit_data->default_face_dir == 3 ) ? 1 : 0;
        }
        else {
            flipped_x = ( unit_data->default_face_dir == 0 || unit_data->default_face_dir == 1 ) ? 1 : 0;
        }
    }
    else {
        if( new_dir.x > 0 ) {
            flipped_x = ( unit_data->default_face_dir == 1 || unit_data->default_face_dir == 3 ) ? 1 : 0;
        }
        else {
            flipped_x = ( unit_data->default_face_dir == 0 || unit_data->default_face_dir == 2 ) ? 1 : 0;
        }
    }
    if( this->getCurrentSkeleton()->getSkeleton()->flipX != flipped_x ) {
        _front->getSkeleton()->flipX = flipped_x;
        if( _back ) {
            _back->getSkeleton()->flipX = flipped_x;
        }
        _same_dir_frame_count = 0;
    }
    Point normalized_dir = new_dir;
    normalized_dir.normalize();
    //debug
//    _custom_draw->setRotation( -new_dir.getAngle() * 180 / M_PI );
    //end debug
    this->setUnitDirection( normalized_dir );
}

void UnitNode::changeFace( eUnitFace face ) {
    UnitData* unit_data = dynamic_cast<UnitData*>( _target_data );
    if( !unit_data->is_double_face ) {
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

cocos2d::Point UnitNode::getBonePos( const std::string& bone_name ) {
    Point ret = this->getLocalBonePos( bone_name );
    ret = _current_skeleton->convertToWorldSpace( ret );
    ret = this->getParent()->convertToNodeSpace( ret );
    return ret;
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
    this->removeAllUnitComponents();
    if( _hint_node ) {
        _hint_node->removeFromParent();
        _hint_node = nullptr;
    }
    this->changeUnitState( eUnitState::Dead, true );
}

void UnitNode::onDying() {
    this->removeAllBuffs();
    Sprite* blood = Sprite::createWithSpriteFrameName( "unit_deadblood.png" );
    UnitNodeFadeoutComponent* component = UnitNodeFadeoutComponent::create( blood, "dead_blood", 3.0f, 0, true );
    component->setPosition( Point::ZERO );
    this->addUnitComponent( component, component->getName(), eComponentLayer::BelowObject );
}

void UnitNode::takeDamage( const cocos2d::ValueMap& result, TargetNode* atker ) {
    this->takeDamage( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), result.at( "miss" ).asBool(), atker );
}

void UnitNode::takeDamage( float amount, bool is_cri, bool is_miss, TargetNode* atker ) {
    UnitData* unit_data = dynamic_cast<UnitData*>( _target_data );
    if( this->isAttackable() && unit_data->current_hp > 0 ) {
        float damage = amount;
        for( auto itr = _buffs.begin(); itr != _buffs.end(); ++itr ) {
            damage = itr->second->filterDamage( damage, atker );
        }
        unit_data->current_hp -= damage;
        
        //jump damage number
        std::string jump_text_name = Utils::stringFormat( "damage_number_%d", BulletNode::getNextBulletId() );
        this->jumpNumber( damage, "damage", is_cri, jump_text_name );
        
        //update blood bar
        if( this->isBoss() ) {
            float percent = 100.0f * _target_data->current_hp / _target_data->hp;
            if( percent < 0 ) {
                percent = 0;
            }
            _battle_layer->getUIBattleLayer()->setBossHpPercent( percent );
        }
        else if( this->getTargetCamp() == eTargetCamp::Player ) {
            float percent = _target_data->current_hp / _target_data->hp * 100.0f;
            if( percent < 0 ) {
                percent = 0;
            }
            _hp_bar->setPercentage( percent );
        }
        
        //dying
        if( unit_data->current_hp <= 0 ) {
            this->changeUnitState( eUnitState::Dying, true );
            _battle_layer->clearChasingTarget( this );
            SkillCache::getInstance()->removeSkillOfOwner( this );
            this->endSkill();
        }
        else if( _chasing_target == nullptr ) {
            if( atker && atker->isAttackable() && atker->isAlive() ) {
                this->setChasingTarget( atker );
            }
        }
    }
}

void UnitNode::takeHeal( const cocos2d::ValueMap& result, int source_id ) {
    this->takeHeal( result.at( "damage" ).asFloat(), result.at( "cri" ).asBool(), source_id );
}

void UnitNode::takeHeal( float amount, bool is_cri, int source_id ) {
    _target_data->current_hp += amount;
    if( _target_data->current_hp > _target_data->hp ) {
        _target_data->current_hp = _target_data->hp;
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
    _hp_bar->setPercentage( _target_data->current_hp / _target_data->hp * 100.0f );
    
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
        std::string key = pair.at( 0 );
        std::string value = pair.at( 1 );
        if( key == "attackable" ) {
            this->setAttackable( value == "true" );
        }
        else {
            _target_data->setAttribute( pair.at( 0 ), pair.at( 1 ) );
        }
        _front->setScale( _target_data->scale );
        _desired_unit_scale = _target_data->scale;
        if( _back ) {
            _back->setScale( _target_data->scale );
        }
    }
}

void UnitNode::addBuff( const std::string& buff_id, Buff* buff, bool replace ) {
    if( replace ) {
        Buff* old_buff = this->getBuffOfType( buff->getBuffType() );
        if( old_buff ) {
            old_buff->end();
        }
    }
    _buffs.insert( buff_id, buff );
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

bool UnitNode::hasBuffOfType( const std::string& buff_type ) {
    return this->getBuffOfType( buff_type ) != nullptr;
}

Buff* UnitNode::getBuffOfType( const std::string& buff_type ) {
    for( auto itr = _buffs.begin(); itr != _buffs.end(); ++itr ) {
        if( itr->second->getBuffType() == buff_type ) {
            return itr->second;
        }
    }
    return nullptr;
}
    
void UnitNode::removeAllBuffs() {
    for( auto pair : _buffs ) {
        pair.second->end();
    }
    _buffs.clear();
}

void UnitNode::clearItems() {
    _items.clear();
}

void UnitNode::addItem( Item* item ) {
    _items.insert( item->getItemId(), item );
    Sprite* item_sprite = Sprite::createWithSpriteFrameName( item->getResource() );
    UnitNodeComponent* component = UnitNodeComponent::create( item_sprite, item->getName(), true );
    this->addUnitComponent( component, item->getName(), eComponentLayer::OverObject );
    Point icon_pos = Point( 0, this->getLocalHeadPos().y + item_sprite->getContentSize().height );
    component->setPosition( icon_pos );
    this->addUnitTag( item->getName() );
}

void UnitNode::removeItem( const std::string& item_name ) {
    for( auto itr = _items.begin(); itr != _items.end(); ++itr ) {
        if( itr->second->getName() == item_name ) {
            itr->second->removeFromUnit( this );
            _items.erase( itr );
            break;
        }
    }
}

bool UnitNode::hasItem( const std::string& item_name ) {
    for( auto itr = _items.begin(); itr != _items.end(); ++itr ) {
        if( itr->second->getName() == item_name ) {
            return true;
        }
    }
    return false;
}

void UnitNode::useSkill( int skill_id, const cocos2d::Point& dir, float range_per, float duration ) {
    if( !this->isDying() && !this->isCasting() ) {
        Point sk_dir = dir;
        if( sk_dir.x != 0 || sk_dir.y != 0 ) {
            this->changeUnitDirection( dir );
        }
        else {
            sk_dir = this->getUnitDirection();
        }
        _using_skill_params.clear();
        _using_skill_params["dir_x"] = Value( sk_dir.x );
        _using_skill_params["dir_y"] = Value( sk_dir.y );
        _using_skill_params["touch_down_duration"] = Value( duration );
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
        auto itr = _using_skill_params.find( "skill_id" );
        if( itr != _using_skill_params.end() ) {
            int sk_id = _using_skill_params.at( "skill_id" ).asInt();
            _skills.at( sk_id )->reload();
        }
        _using_skill_params.clear();
    }
}

void UnitNode::endCast() {
    if( this->isCasting() ) {
        _using_skill_params["state"] = Value( "end" );
        int sk_id = _using_skill_params.at( "skill_id" ).asInt();
        _skills.at( sk_id )->setSkillNode( nullptr );
        _unit_state_changed = true;
        this->setNextUnitState( eUnitState::Casting );
    }
}

bool UnitNode::getAdvisedNewDir( UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir ) {
    Vec2 unit_to_this = this->getPosition() - unit->getPosition();
    Vec2 this_to_unit = unit->getPosition() - this->getPosition();
    Vec2 this_dir = this->getUnitDirection();
    
    float this_dir_to_center = this_dir.cross( this_to_unit );
    float unit_dir_to_center = old_dir.cross( unit_to_this );
    
    if( this_dir_to_center * unit_dir_to_center <= 0 ) {
        new_dir = Geometry::clockwisePerpendicularVecToLine( unit_to_this );
    }
    else {
        if( unit_dir_to_center > 0 ) {
//            new_dir = Geometry::anticlockwisePerpendicularVecToLine( unit_to_this );
            new_dir = Geometry::clockwisePerpendicularVecToLine( unit_to_this );
        }
        else {
            new_dir = Geometry::anticlockwisePerpendicularVecToLine( unit_to_this );
//            new_dir = Geometry::clockwisePerpendicularVecToLine( unit_to_this );
        }
    }
    
    new_dir.normalize();
    
    //debug
//    _new_dir_draw->setRotation( -new_dir.getAngle() * 180 / M_PI );
//end debug
    return true;
}

void UnitNode::walkTo( const cocos2d::Point& new_pos ) {
    this->changeUnitState( eUnitState::Walking );
    Point origin_dir = new_pos - this->getPosition();
    origin_dir.normalize();
    float max_walk_length = new_pos.distance( this->getPosition() );
    Point new_dir = this->pushToward( origin_dir, max_walk_length );
    
    if( !this->isOscillate( new_dir ) ) {
        this->changeUnitDirection( new_dir );
    }
    
    _battle_layer->onUnitMoved( this );
}

void UnitNode::walkAlongPath( Path* path, float distance ) {
    if( path ) {
        Point last_pos = this->getPosition();
        Point new_pos = path->steps.back();
        Point to_pos = new_pos;
        if( new_pos.distance( last_pos ) > distance ) {
            Point dir = new_pos - last_pos;
            dir.normalize();
            new_pos = last_pos + dir * distance;
        }
        Point unit_pos = this->getPosition();
        if( unit_pos.distance( to_pos ) < 5.0 ) {
            path->steps.pop_back();
        }

        this->walkTo( new_pos );
    }
}

void UnitNode::walkAlongWalkPath( float distance ) {
    if( _walk_path ) {
        this->walkAlongPath( _walk_path, distance );
        
        if( _walk_path->steps.size() == 0 ) {
            this->setWalkPath( nullptr );
            this->changeUnitState( eUnitState::Idle );
            _relax_frames = DEFAULT_RELAX_FRAMES;
        }
    }
}

void UnitNode::walkAlongTourPath( float distance ) {
    if( _tour_path ) {
        this->walkAlongPath( _tour_path, distance );
        
        if( _tour_path->steps.size() == 0 ) {
            this->setTourPath( nullptr );
            this->changeUnitState( eUnitState::Idle );
            _relax_frames = DEFAULT_RELAX_FRAMES;
        }
    }
}

cocos2d::Point UnitNode::pushToward( const cocos2d::Point& dir, float distance ) {
    Point new_pos = dir * distance + this->getPosition();
    Point origin_dir = dir;
    std::vector<Collidable*> collidables;
    float max_walk_length = distance;
    Point new_dir = origin_dir;
    Point dest_pos = new_pos;
    Terrain::getInstance()->getMeshByUnitRadius( _target_data->collide )->getNearbyBorders( this->getPosition(), max_walk_length, collidables );
    
    for( auto id_u : _battle_layer->getAliveUnits() ) {
        if( id_u.second == this)
            continue;
        collidables.push_back(id_u.second);
    }
    
    for( auto pair : _battle_layer->getAllTowers() ) {
        collidables.push_back( pair.second );
    }
    
    std::set<Collidable*> steered_collidables;
    
    while( true ) {
        bool no_collide = true;
        
        for( auto c : collidables ) {
            if( c->willCollide( this, dest_pos ) ) {
                no_collide = false;
                int steered_count = (int)steered_collidables.count( c );
                if( steered_count == 0 ) {
                    steered_collidables.insert( c );
                    if ( !c->getAdvisedNewDir( this, cocos2d::Vec2( this->getPosition(), dest_pos ), new_dir ) ) {
                        return origin_dir;
                    }
                    dest_pos = this->getPosition() + new_dir.getNormalized() * max_walk_length;
                    break;
                }
                else {
                    steered_collidables.insert( c );
                    return origin_dir;
                }
            }
        }
        
        if( no_collide) {
            break;
        }
    }
    this->setPosition( dest_pos );
    return new_dir;
}

bool UnitNode::isUnderControl() {
    return _state == eUnitState::UnderControl;
}

bool UnitNode::willCast() {
    return _next_state == eUnitState::Casting;
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

bool UnitNode::isIdle() {
    return _state == eUnitState::Idle;
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
    if( UnitNode* leader = _battle_layer->getLeaderUnit() ) {
        const Point& leader_pos = leader->getPosition();
        float distance = pos.distance( leader_pos );
        if( !_should_catch_up && distance > DEFAULT_CATCH_UP_START_DISTANCE ) {
            this->setShouldCatchUp( true );
        }
        else if( _should_catch_up && distance < DEFAULT_CATCH_UP_STOP_DISTANCE ) {
            this->setShouldCatchUp( false );
        }
    }
}

void UnitNode::findPathToPosition( const cocos2d::Point& pos, int validate_frames ) {
    this->setWalkPath( Terrain::getInstance()->getMeshByUnitRadius( _target_data->collide )->findPath( this->getPosition(), pos, validate_frames ) );
}

bool UnitNode::isHarmless() {
    return _target_data->atk <= 0;
}

bool UnitNode::canAttack( TargetNode* target_node ) {
    if( this->getUnitData()->atk_range <= 0 ) {
        return false;
    }
    
    if( this->getPosition().distance( target_node->getPosition() ) > _target_data->atk_range + target_node->getTargetData()->collide ) {
        return false;
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
    TargetNode* target_node = _chasing_target;
    if( target_node != nullptr ) {
        UnitData* unit_data = dynamic_cast<UnitData*>( _target_data );
        if( unit_data->is_melee ) {
            DamageCalculate* damage_calculator = DamageCalculate::create( "normal", 0 );
            ValueMap result = damage_calculator->calculateDamage( _target_data, target_node->getTargetData() );
            target_node->takeDamage( result, this );
            
            if( !result.at( "miss" ).asBool() ) {
                std::string hit_resource = "effects/bullets/default_hit";
                spine::SkeletonAnimation* hit_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
                hit_effect->setScale( 0.7f );
                UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( hit_effect, Utils::stringFormat( "bullet_%d_hit", BulletNode::getNextBulletId() ), true );
                component->setPosition( target_node->getLocalHitPos() );
                target_node->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
                component->setAnimation( 0, "animation", false );
            }
        }
        else {
            DamageCalculate* damage_calculator = DamageCalculate::create( "normal", 0 );
            BulletNode* bullet = BulletNode::create( this, ResourceManager::getInstance()->getBulletData( _target_data->bullet_name ), damage_calculator, ValueMap() );
            bullet->shootAt( target_node );
        }
    }
}

void UnitNode::onCasting() {
    int sk_id = _using_skill_params["skill_id"].asInt();
    _skills.at( sk_id )->activate( _using_skill_params );
}

bool UnitNode::isUnitInDirectView( UnitNode* unit ) {
    UnitData* unit_data = dynamic_cast<UnitData*>( _target_data );
    if( Terrain::getInstance()->isBlocked( this->getPosition(), unit->getPosition() ) ) {
        return false;
    }
    else if( !Math::isPositionInRange( unit->getPosition(), this->getPosition(), unit_data->guard_radius + unit->getUnitData()->collide ) ) {
        return false;
    }
    return true;
}

bool UnitNode::needRelax() {
    return ( _relax_frames > 0 );
}

cocos2d::Point UnitNode::getNextWanderPos() {
    for( int i = 0; i < 3; i++ ) {
        float r = Utils::randomNumber( _wander_radius );
        float angle = Utils::randomFloat() * M_PI;
        Point new_pos = Point( _born_position.x + cosf( angle ) * r, _born_position.y + sinf( angle ) * r );
        if( _battle_layer->isPositionOK( new_pos, _target_data->collide ) ) {
            return new_pos;
        }
    }
    return Point::ZERO;
}

void UnitNode::setWalkPath( Path* path ) {
    CC_SAFE_RELEASE( _walk_path );
    _walk_path = path;
    CC_SAFE_RETAIN( _walk_path );
}

void UnitNode::setTourPath( Path* path ) {
    CC_SAFE_RELEASE( _tour_path );
    _tour_path = path;
    CC_SAFE_RETAIN( _tour_path );
}

UnitData* UnitNode::getUnitData() {
    return dynamic_cast<UnitData*>( _target_data );
}

void UnitNode::jumpNumber( float amount, const std::string& type, bool is_critical, const std::string& name ) {
    JumpText* jump_text = JumpText::create( Utils::toStr( (int)amount ), type, is_critical, _camp, name );
    this->addUnitComponent( jump_text, name, eComponentLayer::OverObject );
    jump_text->setPosition( this->getLocalHeadPos() );
    jump_text->start( is_critical );
}

Skill* UnitNode::getSkill( int sk_id ) {
    return _skills.at( sk_id );
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

bool UnitNode::shouldSkillCastOnTouchDown( int sk_id ) {
    return _skills.at( sk_id )->shouldCastOnTouchDown();
}

void UnitNode::makeSpeech( const std::string& content, float duration ) {
    Rect inset_rect = Rect( 100.0f, 40.0f, 60.0f, 60.0f );
    ui::Scale9Sprite* window = ui::Scale9Sprite::createWithSpriteFrameName( "chat_popup.png", inset_rect );
    window->setAnchorPoint( Point( 0.3f, 0 ) );
    Label* content_label = Label::createWithTTF( content, "simhei.ttf", 36.0 );
//    Label* content_label = Label::createWithSystemFont( content, "Helvetica", 40.0f );
    content_label->setTextColor( Color4B::BLACK );
    content_label->setLineBreakWithoutSpace( true );
    content_label->setDimensions( 300, 0 );
    content_label->setHorizontalAlignment( TextHAlignment::LEFT );
    content_label->setVerticalAlignment( TextVAlignment::CENTER );
    Size content_size = content_label->getContentSize();
    Size real_content_size = Size( content_size.width + 60.0f, content_size.height + 70.0f );
    window->setPreferredSize( real_content_size );
    content_label->setAnchorPoint( Point( 0.5f, 1.0f ) );
    content_label->setPosition( Point( real_content_size.width / 2, real_content_size.height - 20.0f ) );
    window->addChild( content_label );
    std::string name = Utils::stringFormat( "chat_popup_%d", BulletNode::getNextBulletId() );
    TimeLimitComponent* component = TimeLimitComponent::create( duration, window, name, true );
    component->setPosition( Point( this->getContentSize().width / 2, this->getContentSize().height ) );
    this->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
}

void UnitNode::setGuardTarget( UnitNode* guard_target ) {
    CC_SAFE_RELEASE( _guard_target );
    _guard_target = guard_target;
    CC_SAFE_RETAIN( _guard_target );
}

cocos2d::Point UnitNode::getGuardCenter() {
    if( _guard_target ) {
        return _guard_target->getPosition();
    }
    else {
        return this->getPosition();
    }
}

void UnitNode::setConcentrateOnWalk( bool b ) {
    _is_concentrate_on_walk = b;
    if( b ) {
        this->setChasingTarget( nullptr );
    }
}

void UnitNode::setCharging( bool b, std::string effect_resource ) {
    if( !this->isCasting() && !this->willCast() ) {
        _is_charging = b;
        this->changeUnitState( _state, true );
        if( b && !effect_resource.empty() ) {
            if( _charging_effect != nullptr ) {
                _charging_effect->setDuration( 0 );
            }
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( effect_resource );
            std::string name = Utils::stringFormat( "UnitCharging_%d", BulletNode::getNextBulletId() );
            _charging_effect = TimeLimitSpineComponent::create( INT_MAX, skeleton, name, true );
            _charging_effect->setPosition( this->getLocalBonePos( "ChargingPoint" ) );
            _charging_effect->setAnimation( 0, "animation", true );
            this->addUnitComponent( _charging_effect, name, eComponentLayer::OverObject );
        }
        else if( !b && _charging_effect != nullptr ) {
            _charging_effect->setDuration( 0 );
            _charging_effect = nullptr;
        }
    }
}

void UnitNode::setWeapon( Equipment* weapon ) {
    if( _weapon != nullptr ) {
        _target_data->sub( _weapon->getEquipData() );
    }
    CC_SAFE_RELEASE( _weapon );
    CC_SAFE_RETAIN( weapon );
    _weapon = weapon;
    if( _weapon ) {
        _target_data->add( _weapon->getEquipData() );
    }
}

void UnitNode::setArmor( Equipment* armor ) {
    if( _armor != nullptr ) {
        _target_data->sub( _armor->getEquipData() );
    }
    CC_SAFE_RELEASE( _armor );
    CC_SAFE_RETAIN( armor );
    _armor = armor;
    if( _armor ) {
        _target_data->add( _armor->getEquipData() );
    }
}

void UnitNode::setBoot( Equipment* boot ) {
    if( _boot != nullptr ) {
        _target_data->sub( _boot->getEquipData() );
    }
    CC_SAFE_RELEASE( _boot );
    CC_SAFE_RETAIN( boot );
    _boot = boot;
    if( _boot ) {
        _target_data->add( _boot->getEquipData() );
    }
}

void UnitNode::setAccessory( Equipment* accessory ) {
    if( _accessory != nullptr ) {
        _target_data->sub( _accessory->getEquipData() );
    }
    CC_SAFE_RELEASE( _accessory );
    CC_SAFE_RETAIN( accessory );
    _accessory = accessory;
    if( _accessory ) {
        _target_data->add( _accessory->getEquipData() );
    }
}

void UnitNode::setUnitScale( float scale ) {
    _desired_unit_scale = scale;
    ScaleTo* scale_to = ScaleTo::create( 0.5f, scale );
    _front->runAction( scale_to );
    if( _back != nullptr ) {
        ScaleTo* scale_to_back = ScaleTo::create( 0.5f, scale );
        _back->runAction( scale_to_back );
    }
}

float UnitNode::getUnitScale() {
    return _desired_unit_scale;
}

void UnitNode::setAttackable( bool b ) {
    TargetNode::setAttackable( b );
    if( !b ) {
        _battle_layer->clearChasingTarget( this );
    }
}

void UnitNode::updateHintNode( float delta ) {
    if( _has_hint_node ) {
        if( _hint_node != nullptr ) {
            //update hint node pos and rotation
            _hint_node->updateHintPos( _battle_layer, this->getPosition() );
        }
        else {
            if( this->hasUnitTag( BATTLE_HINT_BOSS ) ) {
                _hint_node = BattleHintNode::create( BATTLE_HINT_BOSS );
            }
            else if( this->hasUnitTag( BATTLE_HINT_HOSTAGE ) ) {
                _hint_node = BattleHintNode::create( BATTLE_HINT_HOSTAGE );
            }
            else if( this->hasUnitTag( BATTLE_HINT_VILLIGER ) ) {
                _hint_node = BattleHintNode::create( BATTLE_HINT_VILLIGER );
            }
            else if( this->hasUnitTag( BATTLE_HINT_VICE_BOSS ) ) {
                _hint_node = BattleHintNode::create( BATTLE_HINT_VICE_BOSS );
            }
            else if( this->hasUnitTag( BATTLE_HINT_MASTER ) ) {
                _hint_node = BattleHintNode::create( BATTLE_HINT_MASTER );
            }
            else {
                _has_hint_node = false;
            }
            if( _hint_node != nullptr ) {
                _battle_layer->addToLayer( _hint_node, eBattleSubLayer::FloatLayer, Point::ZERO, 0 );
            }
        }
    }
}

//private methods

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