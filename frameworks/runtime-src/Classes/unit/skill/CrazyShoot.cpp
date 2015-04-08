//
//  CrazyShoot.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/20/15.
//
//

#include "CrazyShoot.h"
#include "../../scene/BattleLayer.h"
#include "../../Utils.h"
#include "../../manager/ResourceManager.h"

using namespace cocos2d;

CrazyShoot::CrazyShoot() {
    
}

CrazyShoot::~CrazyShoot() {
    
}

CrazyShoot* CrazyShoot::create(  UnitNode* owner, const cocos2d::ValueMap& data ) {
    CrazyShoot* ret = new CrazyShoot();
    if( ret && ret->init( owner, data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool CrazyShoot::init(  UnitNode* owner, const cocos2d::ValueMap& data ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _base_damage = data.at( "damage" ).asValueVector().at( level - 1 ).asFloat();
    _interval = data.at( "interval" ).asFloat();
    _elapse = _interval;
    _speed = data.at( "speed" ).asFloat();
    _count = data.at( "count" ).asInt();
    _current_count = 0;
    _range = data.at( "range" ).asFloat();
    
    return true;
}

void CrazyShoot::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        
        if( _elapse >= _interval ) {
            _elapse = 0;
            BattleLayer* battle_layer = _owner->getBattleLayer();
            
            cocos2d::Vector<UnitNode*> candidates = battle_layer->getAliveOpponentsInRange( _owner->getUnitCamp(), _owner->getPosition(), _owner->getPosition(), _range );
            int size = (int)candidates.size();
            if( size > 0 ) {
                int i = Utils::randomNumber( size ) - 1;
                UnitNode* target_unit = candidates.at( i );
                ValueMap bullet_data = ResourceManager::getInstance()->getBulletData( "vanhelsing_bullet" );
                bullet_data["speed"] = Value( _speed );
                bullet_data["will_miss"] = Value( false );
                DamageCalculate* calculator = DamageCalculate::create( "CrazyShoot", _base_damage );
                BulletNode* bullet = BulletNode::create( _owner, bullet_data, calculator, ValueMap() );
                battle_layer->addBullet( bullet->getBulletId(), bullet );
                bullet->shootAt( _owner, target_unit );
            }
            
            if( ++_current_count >= _count ) {
                this->end();
            }
        }
    }
}

void CrazyShoot::begin() {
    SkillNode::begin();
}

void CrazyShoot::end() {
    SkillNode::end();
}