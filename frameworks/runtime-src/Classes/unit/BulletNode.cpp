//
//  BulletNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/13/15.
//
//

#include "BulletNode.h"
#include "../util/CocosUtils.h"
#include "UnitNode.h"
#include "Buff.h"
#include "../scene/BattleLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../manager/ResourceManager.h"
#include "../manager/AudioManager.h"
#include "../BoidsMath.h"
#include "../AI/Terrain.h"

#define HIT_DISTANCE 20.0

using namespace cocos2d;

int BulletNode::_global_bullet_id = 0;

BulletNode::BulletNode() :
_source_unit( nullptr ),
_damage_calculator( nullptr ),
_target_id( -1 ),
_streak( nullptr ),
_duration( 0 ) {
    
}

BulletNode::~BulletNode() {
    CC_SAFE_RELEASE( _source_unit );
    CC_SAFE_RELEASE( _damage_calculator );
}


int BulletNode::getNextBulletId() {
    if( ++_global_bullet_id == 0x00ffffff ) {
        _global_bullet_id = 0;
    }
    return _global_bullet_id;
}

BulletNode* BulletNode::create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    BulletNode* ret = new BulletNode();
    if( ret && ret->init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BulletNode::init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    if( !Node::init() ) {
        return false;
    }
    
    _bullet_data = bullet_data;
    _buff_data = buff_data;
    
    int bullet_id = BulletNode::getNextBulletId();
    this->setBulletId( bullet_id );
    _battle_layer = unit_node->getBattleLayer();
    this->setTargetId( -1 );
    
    if( _bullet_data.find( "speed" ) != _bullet_data.end() ) {
        _speed = _bullet_data.at( "speed" ).asFloat();
    }
    else {
        _speed = DEFAULT_BULLET_SPEED;
    }
    
    this->setSourceUnit( unit_node );
    this->setDamageCalculator( damage_calculator );
    
    _target_pos = Point::ZERO;
    
    _accumulator = 0;
    _duration = 1.0f;
    
    auto itr = _bullet_data.find( "track_target" );
    if( itr != _bullet_data.end() ) {
        _track_target = itr->second.asBool();
    }
    else {
        _track_target = true;
    }
    
    itr = _bullet_data.find( "will_miss" );
    if( itr != _bullet_data.end() ) {
        _will_miss = itr->second.asBool();
    }
    else {
        _will_miss = true;
    }
    
    itr = _bullet_data.find( "need_rotate" );
    if( itr != _bullet_data.end() ) {
        _need_rotate = itr->second.asBool();
    }
    else {
        _need_rotate = false;
    }
    
    itr = _bullet_data.find( "damage_radius" );
    if( itr != _bullet_data.end() ) {
        _damage_radius = itr->second.asFloat();
    }
    else {
        _damage_radius = 0;
    }
    
    itr = _bullet_data.find( "duration" );
    if( itr != _bullet_data.end() ) {
        _duration = itr->second.asFloat();
    }
    else {
        _duration = 0;
    }
    
    _should_recycle = false;
    
    _hit_type = "";
    _hit_name = "";
    
    itr = _bullet_data.find( "hit_type" );
    if( itr != _bullet_data.end() ) {
        _hit_type = itr->second.asString();
        _hit_name = _bullet_data.at( "hit_name" ).asString();
    }
    
    itr = _bullet_data.find( "hit_scale" );
    if( itr != _bullet_data.end() ) {
        _hit_scale = itr->second.asFloat();
    }
    else {
        _hit_scale = 1.0f;
    }
    
    if( _hit_type != "" ) {
        _show_hit_effect = true;
    }
    else {
        _show_hit_effect = false;
    }
    
    itr = _bullet_data.find( "bomb_type" );
    if( itr != _bullet_data.end() ) {
        _bomb_type = itr->second.asString();
        _bomb_name = _bullet_data.at( "bomb_name" ).asString();
    }
    
    if( _bomb_type != "" ) {
        _show_bomb_effect = true;
    }
    else {
        _show_bomb_effect = false;
    }
    
    return true;
}

void BulletNode::shootAt( class TargetNode* target, int emit_pos_type ) {
    do {
        if( target ) {
            _target_id = target->getDeployId();
            if( _track_target ) {
                _target_pos = target->getHitPos();
            }
            else {
                _target_pos = target->getPosition();
            }
            break;
        }
    }while( true );
    this->shoot( emit_pos_type );
}

void BulletNode::shootAtPosition( const cocos2d::Point& pos ) {
    _target_pos = pos;
    this->shoot();
}

void BulletNode::updateFrame( float delta ) {
    _accumulator += delta;
    
    Point last_pos = this->getPosition();
    TargetNode* target_unit = _battle_layer->getAliveTargetByDeployId( _target_id );
    
    
    if( target_unit && _track_target ) {
        _target_pos = target_unit->getHitPos();
    }
    
    if( this->doesHitTarget( last_pos, _target_pos, delta ) ) {
        //does hit target
        
        cocos2d::Vector<TargetNode*> hit_targets;
        
        if( _damage_radius > 0 ) {
            Vector<UnitNode*> unit_candidates = _battle_layer->getAliveOpponentsInRange( _source_unit->getTargetCamp(), _target_pos, _damage_radius );
            
            for( auto unit : unit_candidates ) {
                hit_targets.pushBack( unit );
            }
            
            Vector<TowerNode*> tower_candidates = _battle_layer->getAliveTowersInRange( _source_unit->getTargetCamp(), _target_pos, _damage_radius );
            
            for( auto tower : tower_candidates ) {
                hit_targets.pushBack( tower );
            }
        }
        else {
            if( target_unit ) {
                hit_targets.pushBack( target_unit );
            }
        }
        
        if( _show_bomb_effect ) {
            //show range effect
            std::string hit_resource = Utils::stringFormat( "effects/bullets/%s_hit", _bomb_name.c_str() );
            spine::SkeletonAnimation* bomb_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
            UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( bomb_effect, Utils::stringFormat( "bullet_%d_hit", _bullet_id ), true );
            _battle_layer->addToEffectLayer( component, _target_pos, 0 );
            component->setAnimation( 0, "animation", false );
            
            AudioManager::getInstance()->playEffect( "common/explode.mp3" );
        }
        
        for( TargetNode* u : hit_targets ) {
            //take damage
            this->hitTarget( u );
        }
        
        this->setShouldRecycle( true );
    }
    else {
        //not hit yet, update bullet position
        Point dir = _target_pos - last_pos;
        dir.normalize();
        float angle = -dir.getAngle();
        Point new_pos;
        float new_angle = 0;
        if( _is_paracurve ) {
            Point sp = _init_pos;
            Point ep = _target_pos;
            float cos = cosf( angle );
            float sin = sinf( angle );
            float h = 200.0f * cos;
            Point cp1 = sp;
            Point cp2 = Point( sp.x + 0.5f * ( ep.x - sp.x ) - h * sin, sp.y + 0.5f * ( ep.y - sp.y ) + h * cos );
            float t = _accumulator;
            new_pos = Math::bezierTo( t, sp, cp1, cp2, ep );
            Point dpos = new_pos - last_pos;
            new_angle = -dpos.getAngle();
        }
        else {
            new_pos = last_pos + dir * ( _speed * delta );
            new_angle = angle;
        }
        
        this->setPosition( new_pos );
        
        if( _need_rotate ) {
            _bullet->setRotation( CC_RADIANS_TO_DEGREES( new_angle ) );
        }
        
        if( _streak ) {
            _streak->setPosition( new_pos );
        }
    }
}

void BulletNode::hitTarget( class TargetNode *target_unit, bool with_buff ) {
    ValueMap result;
    if( _will_miss ) {
        result = _damage_calculator->calculateDamage( _source_unit->getTargetData(), target_unit->getTargetData() );
    }
    else {
        result = _damage_calculator->calculateDamageWithoutMiss( _source_unit->getTargetData(), target_unit->getTargetData() );
    }
    target_unit->takeDamage( result, _source_unit );
    
    UnitNode* unit = dynamic_cast<UnitNode*>( target_unit );
    if( unit && with_buff && !_buff_data.empty() ) {
        Buff* buff = Buff::create( unit, _buff_data );
        unit->addBuff( buff->getBuffId(), buff );
        buff->begin();
    }
    
    //show hit effect
    if( _show_hit_effect ) {
        std::string hit_resource = Utils::stringFormat( "effects/bullets/%s_hit", _hit_name.c_str() );
        spine::SkeletonAnimation* hit_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
        if( _hit_scale != 1.0f ) {
            hit_effect->setScale( _hit_scale );
        }
        UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( hit_effect, Utils::stringFormat( "bullet_%d_hit", _bullet_id ), true );
        component->setPosition( target_unit->getLocalHitPos() );
        target_unit->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
        component->setAnimation( 0, "animation", false );
    }
}

void BulletNode::setSourceUnit( class TargetNode* source_unit ) {
    CC_SAFE_RETAIN( source_unit );
    CC_SAFE_RELEASE( _source_unit );
    _source_unit = source_unit;
}

void BulletNode::setDamageCalculator( DamageCalculate* calculator ) {
    CC_SAFE_RETAIN( calculator );
    CC_SAFE_RELEASE( _damage_calculator );
    _damage_calculator = calculator;
}

void BulletNode::setShouldRecycle( bool b ) {
    _should_recycle = b;
    if( _should_recycle && _streak ) {
        _streak->removeFromParent();
    }
}

//private methods
void BulletNode::shoot( int emit_pos_type ) {
    std::string body_type = _bullet_data.at( "body_type" ).asString();
    Node* bullet = nullptr;
    if( body_type == "png" ) {
        std::string resource = _bullet_data.at( "name" ).asString() + ".png";
        bullet = Sprite::createWithSpriteFrameName( resource );
    }
    else if( body_type == "cocos" ) {
        std::string resource = _bullet_data.at( "name" ).asString();
        cocostudio::Armature* armature = cocostudio::Armature::create( resource );
        armature->getAnimation()->play( "Fly" );
        bullet = armature;
    }
    else if( body_type == "spine" ) {
        std::string resource = Utils::stringFormat( "effects/bullets/%s_body", _bullet_data.at( "name" ).asString().c_str() );
        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
        skeleton->setAnimation( 0, "animation", true );
        bullet = skeleton;
    }
    auto itr = _bullet_data.find( "scale" );
    if( itr != _bullet_data.end() ) {
        bullet->setScale( itr->second.asFloat() );
    }
    this->addChild( bullet );
    _bullet = bullet;
    
    switch( emit_pos_type ) {
        case 1:
            _init_pos = _source_unit->getBonePos( "shou-l" );
            break;
        case 2:
            _init_pos = _source_unit->getBonePos( "shou-r" );
            break;
        case 3:
            _init_pos = _source_unit->getBonePos( "tou" );
            break;
        case 0:
        default:
            _init_pos = _source_unit->getEmitPos();
            break;
    }
    itr = _bullet_data.find( "streak_color" );
    if( itr != _bullet_data.end() ) {
        const ValueMap& streak_rgb = itr->second.asValueMap();
        _streak = MotionStreak::create( 0.5, 3, _bullet_data.at( "streak_width" ).asFloat(), Color3B( streak_rgb.at( "r" ).asInt(), streak_rgb.at( "g" ).asInt(), streak_rgb.at( "b" ).asInt() ), "effects/tuowei.png" );
        _battle_layer->addToEffectLayer( _streak, _init_pos, 0 );
    }
    
    this->setPosition( _init_pos );
    this->setLocalZOrder( _source_unit->getLocalZOrder() );
    _battle_layer->addBullet( _bullet_id, this );
    Point full_dir = _target_pos - _init_pos;
    full_dir.normalize();
    
    _is_paracurve = ( _bullet_data.at( "move_type" ).asString() == "paracurve" ) && ( fabsf( full_dir.x / full_dir.y ) > 0.2 );
}

bool BulletNode::doesHitTarget( const cocos2d::Point& source_pos, const cocos2d::Point& target_pos, float delta ) {
    float distance = source_pos.distance( target_pos );
    if( _is_paracurve ) {
        return ( distance < HIT_DISTANCE );
    }
    else {
        return ( distance < _speed * delta );
    }
}


//directional bullet node
DirectionalBulletNode::DirectionalBulletNode()
{
    
}

DirectionalBulletNode::~DirectionalBulletNode() {
    
}

DirectionalBulletNode* DirectionalBulletNode::create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    DirectionalBulletNode* ret = new DirectionalBulletNode();
    if( ret && ret->init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DirectionalBulletNode::init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    if( !BulletNode::init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        return false;
    }
    _accumulator = 0;
    
    return true;
}

void DirectionalBulletNode::shootAlong( const cocos2d::Point& dir, float duration ) {
    _init_pos = this->getPosition();
    _dir = dir;
    _duration = duration;
    BulletNode::shoot();
    _bullet->setRotation( -_dir.getAngle() * 180.0f / M_PI );
}

void DirectionalBulletNode::updateFrame( float delta ) {
    _accumulator += delta;
    if( _accumulator > _duration ) {
        this->setShouldRecycle( true );
    }
    else {
        Point new_pos = this->getPosition() + _dir * delta * _speed;
        this->setPosition( new_pos );
        if( Terrain::getInstance()->isBlocked( _init_pos, new_pos ) ) {
            this->setShouldRecycle( true );
        }
        else {
            if( _streak ) {
                _streak->setPosition( new_pos );
            }
            
            cocos2d::Vector<UnitNode*> alive_opponents = _battle_layer->getAliveOpponentsInRange( _source_unit->getTargetCamp(), new_pos, _damage_radius );
            for( auto itr = alive_opponents.begin(); itr != alive_opponents.end(); ++itr ) {
                UnitNode* target_unit = *itr;
                int unit_id = target_unit->getDeployId();
                if( _excluded_targets.find( unit_id ) == _excluded_targets.end() ) {
                    _excluded_targets.insert( std::make_pair( unit_id, Value( true ) ) );
                    this->hitTarget( target_unit );
                }
            }
        }
    }
}

//directional lasting bullet node
DirectionalLastingBulletNode::DirectionalLastingBulletNode() {
    
}

DirectionalLastingBulletNode::~DirectionalLastingBulletNode() {
    
}

DirectionalLastingBulletNode* DirectionalLastingBulletNode::create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    DirectionalLastingBulletNode* ret = new DirectionalLastingBulletNode();
    if( ret && ret->init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DirectionalLastingBulletNode::init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    if( !BulletNode::init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        return false;
    }
    
    _reach_to_pos = false;
    _damage_interval = bullet_data.at( "interval" ).asFloat();
    _damage_elapse = 0;
    _accumulator = 0;
    
    return true;
}

void DirectionalLastingBulletNode::shootTo( const cocos2d::Point& from_pos, const cocos2d::Point& to_pos ) {
    BulletNode::shoot();
    
    _dir = to_pos - from_pos;
    _dir.normalize();
    _to_pos = to_pos;
    
    _last_distance_to_pos = from_pos.distance( to_pos );
    _init_pos = from_pos;
    this->setPosition( _init_pos );
}

void DirectionalLastingBulletNode::updateFrame( float delta ) {
    if( !_should_recycle ) {
        if( !_reach_to_pos ) {
            Point new_pos = this->getPosition() + _dir * _speed * delta;
            float distance = new_pos.distance( _to_pos );
            if( distance <= HIT_DISTANCE || distance >= _last_distance_to_pos ) {
                _reach_to_pos = true;
                this->setPosition( _to_pos );
            }
            else {
                _last_distance_to_pos = distance;
                this->setPosition( new_pos );
            }
        }
        else {
            _accumulator += delta;
            if( _accumulator > _duration ) {
                this->setShouldRecycle( true );
            }
            else {
                _damage_elapse += delta;
                if( _damage_elapse >= _damage_interval ) {
                    _damage_elapse = 0;
                    _excluded_targets.clear();
                }
            }
        }
        Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsInRange( _source_unit->getTargetCamp(), this->getPosition(), _damage_radius );
        for( auto itr = candidates.begin(); itr != candidates.end(); ++itr ) {
            UnitNode* target_unit = *itr;
            int target_id = target_unit->getDeployId();
            if( _excluded_targets.find( target_id ) == _excluded_targets.end() ) {
                _excluded_targets.insert( std::make_pair( target_id, Value( target_id ) ) );
                this->hitTarget( target_unit, true );
            }
        }
    }
}

//fixed position bullet node
FixedPosBulletNode::FixedPosBulletNode() {
    
}

FixedPosBulletNode::~FixedPosBulletNode() {
    
}

FixedPosBulletNode* FixedPosBulletNode::create( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    FixedPosBulletNode* ret = new FixedPosBulletNode();
    if( ret && ret->init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool FixedPosBulletNode::init( class TargetNode* unit_node, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    if( !BulletNode::init( unit_node, bullet_data, damage_calculator, buff_data ) ) {
        return false;
    }
    
    return true;
}

void FixedPosBulletNode::shootAtPosition( const cocos2d::Point& pos, int layer ) {
    std::string resource = Utils::stringFormat( "effects/bullets/%s_body", _bullet_data.at( "name" ).asString().c_str() );
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    skeleton->setEventListener( CC_CALLBACK_2( FixedPosBulletNode::onSkeletonAnimationEvent, this ) );
    skeleton->setCompleteListener( CC_CALLBACK_1( FixedPosBulletNode::onSkeletonAnimationCompleted, this ) );
    skeleton->setAnimation( 0, "animation", false );
    auto itr = _bullet_data.find( "scale" );
    if( itr != _bullet_data.end() ) {
        skeleton->setScale( itr->second.asFloat() );
    }
    this->addChild( skeleton );
    this->setPosition( pos );
    _battle_layer->addBullet( BulletNode::getNextBulletId(), this, (eBattleSubLayer)layer );
}

void FixedPosBulletNode::updateFrame( float delta ) {
}

void FixedPosBulletNode::onSkeletonAnimationEvent( int track_index, spEvent* event ) {
    Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsInRange( _source_unit->getTargetCamp(), this->getPosition(), _damage_radius );
    for( auto u : candidates ) {
        this->hitTarget( u, true );
    }
}

void FixedPosBulletNode::onSkeletonAnimationCompleted( int track_index ) {
     this->setShouldRecycle( true );
}


//bomb bullet node
BombBulletNode::BombBulletNode() {
    
}

BombBulletNode::~BombBulletNode() {
    
}

BombBulletNode* BombBulletNode::create( class TargetNode* shooter, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    BombBulletNode* ret = new BombBulletNode();
    if( ret && ret->init( shooter, bullet_data, damage_calculator, buff_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BombBulletNode::init( class TargetNode* shooter, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    if( !DirectionalLastingBulletNode::init( shooter, bullet_data, damage_calculator, buff_data ) ) {
        return false;
    }
    
    auto itr = bullet_data.find( "lasting_damage" );
    if( itr != bullet_data.end() ) {
        _lasting_damage = bullet_data.at( "lasting_damage" ).asFloat();
    }
    else {
        _lasting_damage = 0;
    }
    
    return true;
}

void BombBulletNode::updateFrame( float delta ) {
    if( !_should_recycle ) {
        if( !_reach_to_pos ) {
            Point new_pos = this->getPosition() + _dir * _speed * delta;
            float distance = new_pos.distance( _to_pos );
            if( distance <= HIT_DISTANCE || distance >= _last_distance_to_pos ) {
                _reach_to_pos = true;
                this->setPosition( _to_pos );
                //hit effect
                if( _show_bomb_effect ) {
                    //show range effect
                    std::string hit_resource = Utils::stringFormat( "effects/bullets/%s_hit", _bomb_name.c_str() );
                    spine::SkeletonAnimation* bomb_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
                    UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( bomb_effect, Utils::stringFormat( "bullet_%d_hit", BulletNode::getNextBulletId() ), true );
                    _battle_layer->addToEffectLayer( component, _to_pos, 0 );
                    component->setAnimation( 0, "animation", false );
                }
                
                //add gas effect
                auto itr = _bullet_data.find( "gas_resource" );
                if( itr != _bullet_data.end() ) {
                    this->setVisible( false );
                    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( itr->second.asString() );
                    std::string name = Utils::stringFormat( "BombBullet_%d", BulletNode::getNextBulletId() );
                    TimeLimitSpineComponent* component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
                    component->setAnimation( 0, "animation", true );
                    _battle_layer->addToLayer( component, eBattleSubLayer::EffectLayer, _to_pos, 0 );
                    _damage_calculator->setCalculatorName( BULLET_NAME_GAS );
                    _damage_calculator->setBaseDamage( _lasting_damage );
                }
                
                Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsInRange( _source_unit->getTargetCamp(), _to_pos, _damage_radius );
                for( auto itr = candidates.begin(); itr != candidates.end(); ++itr ) {
                    this->hitTarget( *itr, true );
                }
            }
            else {
                _last_distance_to_pos = distance;
                this->setPosition( new_pos );
            }
        }
        else {
            _accumulator += delta;
            if( _accumulator > _duration ) {
                this->setShouldRecycle( true );
            }
            else {
                _damage_elapse += delta;
                if( _damage_elapse >= _damage_interval ) {
                    _damage_elapse = 0;
                    
                    Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsInRange( _source_unit->getTargetCamp(), _to_pos, _damage_radius );
                    for( auto itr = candidates.begin(); itr != candidates.end(); ++itr ) {
                        this->hitTarget( *itr, true );
                    }
                }
            }
        }
    }
}

//circle bullet ndoe
CircleBulletNode::CircleBulletNode() {
    
}

CircleBulletNode::~CircleBulletNode() {
    
}

CircleBulletNode* CircleBulletNode::create( class TargetNode* shooter, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    CircleBulletNode* ret = new CircleBulletNode();
    if( ret && ret->init( shooter, bullet_data, damage_calculator, buff_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CircleBulletNode::init( class TargetNode* shooter, const cocos2d::ValueMap& bullet_data, DamageCalculate* damage_calculator, const cocos2d::ValueMap& buff_data ) {
    if( !BulletNode::init( shooter, bullet_data, damage_calculator, buff_data ) ) {
        return false;
    }
    
    _elapse = 0;
    
    return true;
}

void CircleBulletNode::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        _angle += _speed * delta;
        if( _elapse > _duration ) {
            this->setShouldRecycle( true );
        }
        else {
            Point center = _source_unit->getHitPos() + _center_bias;
            Point new_pos = center + Point( _radius * cosf( _angle ), _radius * sinf( _angle ) );
            this->setPosition( new_pos );
            Vector<UnitNode*> candidates = _battle_layer->getAliveOpponentsInRoundRange( _source_unit->getTargetCamp(), this->getPosition(), this->getPosition(), _radius );
            for( auto unit : candidates ) {
                int unit_id = unit->getDeployId();
                auto itr = _excluded_targets.find( unit_id );
                if( itr == _excluded_targets.end() ) {
                    _excluded_targets.insert( std::make_pair( unit_id, Value( unit_id ) ) );
                    this->hitTarget( unit );
                }
            }
        }
    }
}

void CircleBulletNode::shootToward( const cocos2d::Point& dir, float radius ) {
    _radius = radius;
    Point center_dir = Geometry::clockwiseRotate45( dir );
    center_dir.normalize();
    _center_bias = center_dir * radius;
    center_dir = -center_dir;
    _angle = center_dir.getAngle();
    
    std::string resource = Utils::stringFormat( "effects/bullets/%s_body", _bullet_data.at( "name" ).asString().c_str() );
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    skeleton->setAnimation( 0, "animation", true );
    auto itr = _bullet_data.find( "scale" );
    if( itr != _bullet_data.end() ) {
        skeleton->setScale( itr->second.asFloat() );
    }
    this->addChild( skeleton );
    this->setPosition( _source_unit->getHitPos() );
    _battle_layer->addBullet( BulletNode::getNextBulletId(), this );
}