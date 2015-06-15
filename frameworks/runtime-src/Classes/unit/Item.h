//
//  Item.h
//  Boids
//
//  Created by chenyanjie on 4/27/15.
//
//

#ifndef __Boids__Item__
#define __Boids__Item__

#include "spine/spine-cocos2dx.h"

typedef enum {
    ItemStateFly = 1,
    ItemStateReady = 2,
    ItemStateDisappear = 3
}eItemState;

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

class DropItem : public cocos2d::Node {
private:
    std::string _item_id;
    int _count;
    eItemState _state;
    bool _should_recycle;
    spine::SkeletonAnimation* _effect;
    
public:
    DropItem();
    virtual ~DropItem();
    
    static DropItem* create( const cocos2d::ValueMap& item_data );
    virtual bool init( const cocos2d::ValueMap& item_data );
    
    const std::string& getItemId() { return _item_id; }
    void setItemId( const std::string& item_id ) { _item_id = item_id; }
    
    int getCount() { return _count; }
    void setCount( int count ) { _count = count; }
    
    eItemState getItemState() { return _state; }
    void setItemState( eItemState state ) { _state = state; }
    
    bool shouldRecycle() { return _should_recycle; }
    void setShouldRecycle( bool b ) { _should_recycle = b; }
    
    void dropTo( const cocos2d::Point& pos );
    void onDropCompleted();
};

#endif /* defined(__Boids__Item__) */
