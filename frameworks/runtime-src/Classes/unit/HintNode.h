//
//  HintNode.h
//  Boids
//
//  Created by chenyanjie on 6/12/15.
//
//

#ifndef __Boids__HintNode__
#define __Boids__HintNode__

#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"

class BattleLayer;

class BattleHintNode : public cocos2d::Node {
private:
    cocos2d::Node* _nd_arrow;
    
    bool _is_active;
    
public:
    BattleHintNode();
    virtual ~BattleHintNode();
    
    static BattleHintNode* create( const std::string& hint_name );
    virtual bool init( const std::string& hint_name );
    
    void updateHintPos( BattleLayer* battle_layer, const cocos2d::Point& pos );
    
    void setActive( bool b );
    bool isActive();
};

class HintNode : public cocos2d::Node {
private:
    std::string _name;
    std::string _type;
    cocos2d::Sprite* _icon;
    
    BattleHintNode* _hint_icon;
    
    BattleLayer* _battle_layer;
    
    spine::SkeletonAnimation* _effect;
    
public:
    HintNode();
    virtual ~HintNode();
    
    static HintNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& obj_properties, const cocos2d::ValueMap& grid_properties );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& obj_properties, const cocos2d::ValueMap& grid_properties );
    
    void setHintType( const std::string& type ) { _type = type; }
    const std::string& getHintType() { return _type; }
    
    void setHintName( const std::string& name ) { _name = name; }
    const std::string& getHintName() { return _name; }
    
    void updateFrame( float delta );
    
    virtual void setVisible( bool b );
};

#endif /* defined(__Boids__HintNode__) */
