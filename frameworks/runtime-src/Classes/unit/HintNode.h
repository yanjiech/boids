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

class BattleHintNode : public cocos2d::Node {
private:
    cocos2d::Node* _nd_arrow;
    
public:
    BattleHintNode();
    virtual ~BattleHintNode();
    
    static BattleHintNode* create( const std::string& hint_name );
    virtual bool init( const std::string& hint_name );
    
    void setHintRotation( float angle );
    
    void updateHintPos( class BattleLayer* battle_layer, const cocos2d::Point& pos );
};

class HintNode : public cocos2d::Node {
private:
    std::string _name;
    cocos2d::Sprite* _icon;
    
    BattleHintNode* _hint_icon;
    
public:
    HintNode();
    virtual ~HintNode();
    
    static HintNode* create( const std::string& name, cocos2d::Sprite* icon );
    virtual bool init( const std::string& name, cocos2d::Sprite* icon );
    
    void setHintName( const std::string& name ) { _name = name; }
    const std::string& getHintName() { return _name; }
    
    void updateFrame( float delta );
};

#endif /* defined(__Boids__HintNode__) */
