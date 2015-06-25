//
//  DarkIllusion.cpp
//  Boids
//
//  Created by chenyanjie on 5/27/15.
//
//

#include "DarkIllusion.h"
#include "../../Utils.h"
#include "../../scene/BattleLayer.h"
#include "../../AI/Terrain.h"

using namespace cocos2d;

DarkIllusion::DarkIllusion() {
    
}

DarkIllusion::~DarkIllusion() {
    
}

DarkIllusion* DarkIllusion::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    DarkIllusion* ret = new DarkIllusion();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DarkIllusion::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _elase = 0;
    _range = data.at( "range" ).asFloat();
    _radius = data.at( "radius" ).asFloat();
    _stun_duration = data.at( "stun_duration" ).asFloat();
    _speed = _range / _duration;
    _dir = _owner->getUnitDirection();
    _dir.normalize();
    
    return true;
}

void DarkIllusion::updateFrame( float delta ) {
    if( !_should_recycle ) {
        if( _elase == 0 ) {
            std::string resource = "effects/durarara_skill_1";
            spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
            std::string name = Utils::stringFormat( "%s_effect", SKILL_NAME_DARK_ILLUSION );
            _component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
            skeleton->getSkeleton()->flipX = ( _dir.x > 0 );
            _component->setScale( _owner->getUnitScale() );
            _component->setPosition( _owner->getLocalEmitPos() );
            _component->setAnimation( 0, "animation", true );
            _owner->addUnitComponent( _component, _component->getName(), eComponentLayer::OverObject );
        }
        
        _elase += delta;
        if( _elase > _duration ) {
            this->end();
        }
        else {
            Point d_pos = _dir * ( _speed * delta );
            Point new_pos = _owner->getPosition() + d_pos;
           
            std::vector<Collidable*> collidables;
            Terrain::getInstance()->getMeshByUnitRadius( _owner->getTargetData()->collide )->getNearbyBorders( _owner->getPosition(), _speed * delta, collidables );
            bool collide = false;
            for( auto c : collidables ) {
                if( c->willCollide( _owner, new_pos ) ) {
                    collide = true;
                    break;
                }
            }
            
            if( !collide ) {
                _owner->setPosition( _owner->getPosition() + d_pos );
            }
            
            Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getTargetCamp(), _owner->getPosition(), _radius );
            if( candidates.size() > 0 ) {
                ValueMap buff_data;
                buff_data["duration"] = Value( _stun_duration );
                buff_data["buff_type"] = Value( BUFF_TYPE_STUN );
                buff_data["buff_name"] = Value( "DarkIllusion" );
                
                DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_DARK_ILLUSION, _damage );
                for( auto unit : candidates ) {
                    int deployed_id = unit->getDeployId();
                    bool excluded = false;
                    for( auto v : _exclude_targets ) {
                        if( v.asInt() == deployed_id ) {
                            excluded = true;
                            break;
                        }
                    }
                    if( !excluded ) {
                        //push
                        _exclude_targets.push_back( Value( deployed_id ) );
                        Point push_dir = unit->getPosition() - _owner->getPosition();
                        float cross = _dir.cross( push_dir );
                        if( cross > 0 ) {
                            push_dir = Geometry::clockwisePerpendicularVecToLine( _dir );
                        }
                        else {
                            push_dir = Geometry::anticlockwisePerpendicularVecToLine( _dir );
                        }
                        float push_d = _owner->getTargetData()->collide + unit->getTargetData()->collide - Geometry::distanceToLine( unit->getPosition(), _owner->getPosition(), _owner->getPosition() + _dir * 1000.0f );
                        unit->pushToward( push_dir, push_d );
                        
                        //stun
                        StunBuff* buff = StunBuff::create( unit, buff_data );
                        unit->addBuff( buff->getBuffId(), buff );
                        buff->begin();
                        
                        //damage
                        ValueMap result = calculator->calculateDamageWithoutMiss( _owner->getTargetData(), unit->getTargetData() );
                        unit->takeDamage( result, _owner );
                    }
                }
            }
        }
    }
}

void DarkIllusion::begin() {
    SkillNode::begin();
}

void DarkIllusion::end() {
    _component->setDuration( 0 );
    _owner->endCast();
    SkillNode::end();
}