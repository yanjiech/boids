//
//  Item.h
//  Boids
//
//  Created by chenyanjie on 4/27/15.
//
//

#ifndef __Boids__Item__
#define __Boids__Item__

#include "cocos2d.h"

class Item : public cocos2d::Ref {
private:
    int _item_id;
    cocos2d::ValueMap _item_data;
    
public:
    Item();
    virtual ~Item();
    
    static Item* create( const cocos2d::ValueMap& item_data );
    virtual bool init( const cocos2d::ValueMap& item_data );
    
    int getItemId() { return _item_id; }
    void setItemId( int i ) { _item_id = i; }
    
    std::string getName();
    std::string getResource();
    
    void removeFromUnit( class UnitNode* owner );
};

#endif /* defined(__Boids__Item__) */
