//
//  Equipment.h
//  Boids
//
//  Created by chenyanjie on 3/27/15.
//
//

#ifndef __Boids__Equipment__
#define __Boids__Equipment__

#include "cocos2d.h"
#include "ElementData.h"

class Equipment : public cocos2d::Ref {
private:
    EquipmentData* _equip_data;
    
public:
    Equipment();
    virtual ~Equipment();
    
    static Equipment* create( EquipmentData* equip_data );
    virtual bool init( EquipmentData* equip_data );
    
    EquipmentData* getEquipData();
    void setEquipData( EquipmentData* data );
};

#endif /* defined(__Boids__Equipment__) */
