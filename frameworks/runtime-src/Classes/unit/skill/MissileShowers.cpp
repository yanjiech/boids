//
//  MissileShowers.cpp
//  Boids
//
//  Created by chenyanjie on 4/1/15.
//
//

#include "MissileShowers.h"
#include "../../scene/BattleLayer.h"
#include "../../manager/ResourceManager.h"
#include "../../Utils.h"
#include "../../AI/Terrain.h"

using namespace cocos2d;

MissileShowers::MissileShowers() {
    
}

MissileShowers::~MissileShowers() {
    
}

MissileShowers* MissileShowers::create( UnitNode* owner, const cocos2d::ValueMap& data ) {
    MissileShowers* ret = new MissileShowers();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MissileShowers::init( UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _range = data.at( "range" ).asFloat();
    _radius = data.at( "radius" ).asFloat();
    _count = data.at( "count" ).asValueVector().at( level - 1 ).asInt();
    _interval = data.at( "interval" ).asFloat();
    _current_count = 0;
    
    return true;
}

void MissileShowers::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse >= _interval ) {
            _elapse = 0;
            ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "rattletrap_bullet" );
            DamageCalculate* calculator = DamageCalculate::create( SKILL_NAME_MISSILE_SHOWERS, _damage );
            bullet_data["damage_radius"] = Value( _radius );
            Vector<UnitNode*> candidates = _owner->getBattleLayer()->getAliveOpponentsInRange( _owner->getUnitCamp(), _owner->getPosition(), _owner->getPosition(), _range );
            int size = candidates.size();
            if( size > 0 ) {
                UnitNode* target_unit = candidates.at( Utils::randomNumber( size ) - 1 );
                BulletNode* bullet = BulletNode::create( _owner, bullet_data, calculator, nullptr );
                bullet->shootAt( _owner, target_unit );
            }
            else {
                bullet_data["track_target"] = Value( false );
                BulletNode* bullet = BulletNode::create( _owner, bullet_data, calculator, nullptr );
                float rand_angle = Utils::randomFloat() * M_PI * 2;
                float range = _range;
                Point center = _owner->getPosition();
                Point dir = Point( cosf( rand_angle ), sinf( rand_angle ) / 1.74f );
                Point pos;
                do {
                    pos = center + dir * range;
                    range /= 2.0f;
                } while( Terrain::getInstance()->isBlocked( center, pos ) );
                bullet->shootAtPosition( _owner, pos );
            }
            
            if( ++_current_count >= _count ) {
                this->end();
            }
        }
    }
}

void MissileShowers::begin() {
    SkillNode::begin();
}

void MissileShowers::end() {
    _owner->endCast();
    SkillNode::end();
}