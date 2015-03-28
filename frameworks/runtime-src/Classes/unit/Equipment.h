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

class Equipment : public cocos2d::Ref {
public:
    Equipment();
    virtual ~Equipment();
    
    static Equipment* create( const cocos2d::ValueMap& data );
    virtual bool init( const cocos2d::ValueMap& data );
};

#endif /* defined(__Boids__Equipment__) */
