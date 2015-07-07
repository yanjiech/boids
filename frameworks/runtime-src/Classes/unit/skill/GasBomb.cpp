//
//  GasBomb.cpp
//  Boids
//
//  Created by chenyanjie on 4/23/15.
//
//

#include "GasBomb.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

GasBomb::GasBomb() {
    
}

GasBomb::~GasBomb() {
    
}

GasBomb* GasBomb::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    GasBomb* ret = new GasBomb();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool GasBomb::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _damage_radius = data.at( "damage_radius" ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = 0;
    _damage_radius = data.at( "damage_radius" ).asFloat();
    _gas_duratoin = data.at( "gas_duration" ).asFloat();;
    _gas_damage = data.at( "gas_damage" ).asValueVector().at( level - 1 ).asFloat();;
    _range = data.at( "range" ).asFloat();
    _warning_duration = data.at( "warning_duration" ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _stage = 0;
    _shoot_elapse = 0;
    
    return true;
}

void GasBomb::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->end();
        }
        else {
            _shoot_elapse += delta;
            switch( _stage ) {
                case 0: //load
                {
                    if( _shoot_elapse > _interval ) {
                        _shoot_elapse = 0;
                        _stage = 1;
                        //add warning effect
//                        _shoot_pos = Utils::randomPositionInRange( _owner->getPosition(), _range );
                        _shoot_pos = _owner->getBattleLayer()->getLeaderUnit()->getPosition();
                        std::string resource = "effects/skeleton_king_skill_1/cross";
                        std::string name = Utils::stringFormat( "%s_%d", SKILL_NAME_GAS_BOMB, BulletNode::getNextBulletId() );
                        spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
                        skeleton->setScale( _damage_radius / 60.0f );
                        TimeLimitSpineComponent* component = TimeLimitSpineComponent::create( _warning_duration, skeleton, name, true );
                        component->setAnimation( 0, "animation", true );
                        _owner->getBattleLayer()->addToLayer( component, eBattleSubLayer::OnGroundLayer, _shoot_pos, 0 );
                    }
                    break;
                }
                case 1:
                {
                    if( _shoot_elapse > _warning_duration ) {
                        _shoot_elapse = 0;
                        _stage = 0;
                        //shoot
                        
                        Point to_pos = _shoot_pos;
                        Point from_pos = Point( _shoot_pos.x, _shoot_pos.y + 1080.0f );
                        
                        ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "gas_bomb" );
                        bullet_data["gas_resource"] = Value( "effects/bullets/gas_body" );
                        bullet_data["damage_radius"] = Value( _damage_radius );
                        bullet_data["interval"] = Value( 0.5f );
                        bullet_data["lasting_damage"] = Value( _gas_damage );
                        bullet_data["duration"] = Value( _gas_duratoin );
                        
                        DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_GAS_BOMB, _damage );
                        
                        BombBulletNode* bullet = BombBulletNode::create( _owner, bullet_data, calculator, ValueMap() );
                        bullet->shootTo( from_pos, to_pos );
                        
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void GasBomb::begin() {
    SkillNode::begin();
    _owner->setAttackable( false );
    BattleLayer* battle_layer = _owner->getBattleLayer();
    battle_layer->clearChasingTarget( _owner );
    _owner->setCollidable( false );
    _owner->setVisible( false );
}

void GasBomb::end() {
    _owner->endCast();
    _owner->setAttackable( true );
    _owner->setCollidable( true );
    _owner->setVisible( true );
    
    float radius = _owner->getTargetData()->collide;
    
    Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), radius );
    for( auto itr = candidates.begin(); itr != candidates.end(); ++itr ) {
        UnitNode* unit = *itr;
        
        Point push_dir = unit->getPosition() - _owner->getPosition();
        push_dir.normalize();
        unit->pushToward( push_dir, radius );
    }
    
    SkillNode::end();
}