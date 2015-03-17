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
#include "../BoidsMath.h"

using namespace cocos2d;

int BulletNode::_global_bullet_id = 0;

BulletNode::BulletNode() :
_buff( nullptr ),
_unit_data( nullptr ),
_damage_calculator( nullptr ),
_target_id( -1 ),
_source_id( -1 ),
_streak( nullptr ) {
    
}

BulletNode::~BulletNode() {
    if( _unit_data ) {
        _unit_data->release();
    }
    if( _damage_calculator ) {
        _damage_calculator->release();
    }
    if( _buff ) {
        _buff->release();
    }
}


int BulletNode::getNextBulletId() {
    if( ++_global_bullet_id == 0x00ffffff ) {
        _global_bullet_id = 0;
    }
    return _global_bullet_id;
}

BulletNode* BulletNode::create( class UnitNode* unit_node, const rapidjson::Value& bullet_data, DamageCalculate* damage_calculator, Buff* buff ) {
    BulletNode* ret = new BulletNode();
    if( ret && ret->init( unit_node, bullet_data, damage_calculator, buff ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BulletNode::init( class UnitNode* unit_node, const rapidjson::Value& bullet_data, DamageCalculate* damage_calculator, Buff* buff ) {
    if( !Node::init() ) {
        return false;
    }
    
    _bullet_data = CocosUtils::jsonObjectToValueMap( bullet_data );
    
    this->setBulletId( BulletNode::getNextBulletId() );
    _battle_layer = unit_node->getBattleLayer();
    this->setSourceId( unit_node->getDeployId() );
    this->setTargetId( -1 );
    
    if( _bullet_data.find( "speed" ) != _bullet_data.end() ) {
        _speed = _bullet_data.at( "speed" ).asFloat();
    }
    else {
        _speed = DEFAULT_BULLET_SPEED;
    }
    
    this->setUnitData( unit_node->getUnitData() );
    this->setDamageCalculator( damage_calculator );
    this->setBuff( buff );
    
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
    
    _should_recycle = false;
    return true;
}

void BulletNode::shootAt( class UnitNode* source, class TargetNode* target ) {
    do {
        UnitNode* unit_node = dynamic_cast<UnitNode*>( target );
        if( unit_node ) {
            _target_id = unit_node->getDeployId();
            _target_pos = unit_node->getHitPos();
            break;
        }
    }while( true );
    this->shoot( source );
}

void BulletNode::shootAtPosition( class UnitNode* source, const cocos2d::Point& pos ) {
    _target_pos = pos;
    this->shoot( source );
}

void BulletNode::updateFrame( float delta ) {
    _accumulator += delta;
    
    Point last_pos = this->getPosition();
    UnitNode* target_unit = _battle_layer->getAliveUnitByDeployId( _target_id );
    
    if( target_unit && _track_target ) {
        _target_pos = target_unit->getHitPos();
    }
    
    if( this->doesHitTarget( last_pos, _target_pos, delta ) ) {
        //does hit target
        
        std::list<UnitNode*> hit_targets;
        
        if( _damage_radius > 0 ) {
            hit_targets = _battle_layer->getAliveOpponentsInRange( (eUnitCamp)_source_camp, _target_pos, _damage_radius );
        }
        else {
            if( target_unit ) {
                hit_targets.push_back( target_unit );
            }
        }
        
        bool show_hit_effect = false;
        std::string hit_type = "";
        std::string hit_name = "";
        
        auto itr = _bullet_data.find( "hit_type" );
        if( itr != _bullet_data.end() ) {
            hit_type = itr->second.asString();
            hit_name = _bullet_data.at( "hit_name" ).asString();
        }
        
        float hit_effect_scale = 1.0f;
        itr = _bullet_data.find( "hit_scale" );
        if( itr != _bullet_data.end() ) {
            hit_effect_scale = itr->second.asFloat();
        }
        
        if( _damage_radius <= 0 && hit_type != "" ) {
            show_hit_effect = true;
        }
        else if( _damage_radius > 0 && hit_type != "" ) {
            //show range effect
            std::string hit_resource = Utils::stringFormat( "effects/bullets/%s_hit", hit_name.c_str() );
            spine::SkeletonAnimation* hit_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
            if( hit_effect_scale != 1.0f ) {
                hit_effect->setScale( hit_effect_scale );
            }
            UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( hit_effect, Utils::stringFormat( "bullet_%d_hit", _bullet_id ), true );
            _battle_layer->addToEffectLayer( component, _target_pos, this->getLocalZOrder() );
            component->setAnimation( 0, "animation", false );
        }
        
        for( UnitNode* u : hit_targets ) {
            //take damage
            ValueMap result = _damage_calculator->calculateDamage( 0, _unit_data, u->getUnitData() );
            u->takeDamage( result.at( "damage" ).asFloat(), result.at( "miss" ).asBool(), result.at( "cri" ).asBool(), _source_id );
            
            //show hit effect
            if( show_hit_effect ) {
                std::string hit_resource = Utils::stringFormat( "effects/bullets/%s_hit", hit_name.c_str() );
                spine::SkeletonAnimation* hit_effect = ArmatureManager::getInstance()->createArmature( hit_resource );
                if( hit_effect_scale != 1.0f ) {
                    hit_effect->setScale( hit_effect_scale );
                }
                UnitNodeSpineComponent* component = UnitNodeSpineComponent::create( hit_effect, Utils::stringFormat( "bullet_%d_hit", _bullet_id ), true );
                component->setPosition( u->getLocalHitPos() );
                u->addUnitComponent( component, component->getName(), eComponentLayer::OverObject );
                component->setAnimation( 0, "animation", false );
            }
        }
        
        if( _streak ) {
            _streak->removeFromParent();
        }
        _should_recycle = true;
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
            float t = _accumulator / _duration;
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

void BulletNode::setUnitData( class UnitData* unit_data ) {
    if( _unit_data ) {
        _unit_data->release();
    }
    _unit_data = unit_data;
    if( _unit_data ) {
        _unit_data->retain();
    }
}

void BulletNode::setDamageCalculator( DamageCalculate* calculator ) {
    if( _damage_calculator ) {
        _damage_calculator->release();
    }
    _damage_calculator = calculator;
    if( _damage_calculator ) {
        _damage_calculator->retain();
    }
}

void BulletNode::setBuff( class Buff* buff ) {
    if( _buff ) {
        _buff->release();
    }
    _buff = buff;
    if( _buff ) {
        _buff->retain();
    }
}

//private methods
void BulletNode::shoot( class UnitNode* source ) {
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
    
    _init_pos = source->getEmitPos();
    itr = _bullet_data.find( "streak_color" );
    if( itr != _bullet_data.end() ) {
        const ValueMap& streak_rgb = itr->second.asValueMap();
        _streak = MotionStreak::create( 0.5, 3, _bullet_data.at( "streak_width" ).asFloat(), Color3B( streak_rgb.at( "r" ).asInt(), streak_rgb.at( "g" ).asInt(), streak_rgb.at( "b" ).asInt() ), "effetcs/tuowei.png" );
        _battle_layer->addToEffectLayer( _streak, _init_pos, source->getLocalZOrder() );
    }
    
    this->setPosition( _init_pos );
    this->setLocalZOrder( source->getLocalZOrder() );
    _battle_layer->addBullet( _bullet_id, this );
    Point full_dir = _target_pos - _init_pos;
    full_dir.normalize();
    
    _is_paracurve = ( _bullet_data.at( "move_type" ).asString() == "paracurve" ) && ( fabsf( full_dir.x / full_dir.y ) > 0.2 );
}

bool BulletNode::doesHitTarget( const cocos2d::Point& source_pos, const cocos2d::Point& target_pos, float delta ) {
    float distance = source_pos.distance( target_pos );
    if( _is_paracurve ) {
        return ( distance < 60.0f );
    }
    else {
        return ( distance < _speed * delta );
    }
}