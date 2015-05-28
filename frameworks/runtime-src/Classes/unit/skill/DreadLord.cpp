//
//  DreadLord.cpp
//  Boids
//
//  Created by chenyanjie on 5/27/15.
//
//

#include "DreadLord.h"
#include "../UnitNode.h"
#include "../../Utils.h"

using namespace cocos2d;

DreadLord::DreadLord() {
    
}

DreadLord::~DreadLord() {
    
}

DreadLord* DreadLord::create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    DreadLord* ret = new DreadLord();
    if( ret && ret->init( owner, data, params ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool DreadLord::init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params ) {
    if( !SkillNode::init( owner ) ) {
        return false;
    }
    
    int level = data.at( "level" ).asInt();
    _extra_atk = data.at( "extra_atk" ).asValueVector().at( level - 1 ).asFloat();
    _extra_dod = data.at( "extra_atk" ).asValueVector().at( level - 1 ).asFloat();
    _duration = data.at( "duration" ).asFloat();
    _elapse = 0;
    
    return true;
}

void DreadLord::updateFrame( float delta ) {
    if( !_should_recycle ) {
        _elapse += delta;
        if( _elapse > _duration ) {
            this->end();
        }
    }
}

void DreadLord::begin() {
    SkillNode::begin();
    UnitData* unit_data = _owner->getUnitData();
    unit_data->atk += _extra_atk;
    unit_data->dodge += _extra_dod;
    _owner->setUnitScale( _owner->getUnitScale() * 1.5f );
    std::string resource = "effects/durarara_skill_2";
    std::string name = Utils::stringFormat( "%s_effect", SKILL_NAME_DREAD_LORD );
    spine::SkeletonAnimation* skeleton = ArmatureManager::getInstance()->createArmature( resource );
    TimeLimitSpineComponent* component = TimeLimitSpineComponent::create( _duration, skeleton, name, true );
    component->setPosition( Point( 0, 0 ) );
    component->setAnimation( 0, "animation", true );
    component->setScaleX( 1.74 );
    _owner->addUnitComponent( component, component->getName(), eComponentLayer::BelowObject );
    
    _owner->endCast();
}

void DreadLord::end() {
    UnitData* unit_data = _owner->getUnitData();
    unit_data->atk -= _extra_atk;
    unit_data->dodge -= _extra_dod;
    _owner->setUnitScale( _owner->getUnitScale() / 1.5f );
    SkillNode::end();
}