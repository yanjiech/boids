//
//  Equipment.cpp
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#include "Equipment.h"

using namespace cocos2d;

Equipment::Equipment() :
_equip_data( nullptr )
{
    
}

Equipment::~Equipment() {
    CC_SAFE_RELEASE( _equip_data );
}

Equipment* Equipment::create( EquipmentData* equip_data ) {
    Equipment* ret = new Equipment();
    if( ret && ret->init( equip_data ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool Equipment::init( EquipmentData* equip_data ) {
    this->setEquipData( equip_data );
    return true;
}

EquipmentData* Equipment::getEquipData() {
    return _equip_data;
}

void Equipment::setEquipData( EquipmentData* data ) {
    CC_SAFE_RETAIN( data );
    CC_SAFE_RELEASE( _equip_data );
    _equip_data = data;
}