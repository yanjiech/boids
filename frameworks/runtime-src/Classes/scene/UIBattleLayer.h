//
//  UIBattleLayer.h
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#ifndef __Boids__UIBattleLayer__
#define __Boids__UIBattleLayer__

#include "cocos2d.h"
#include <list>

class UnitNode;
class BattleLayer;

class UISkillNode : public cocos2d::Node {
private:
    BattleLayer* _battle_layer;
    UnitNode* _unit_node;
    
public:
    UISkillNode();
    virtual ~UISkillNode();
    
    static UISkillNode* create( BattleLayer* battle_layer, UnitNode* unit_node );
    virtual bool init( BattleLayer* battle_layer, UnitNode* unit_node );
    
    UnitNode* getOwner() { return _unit_node; }
    
    void activate( const cocos2d::Point& dir, float range_per );
    
    void showHint( const cocos2d::Point& dir, float range_per );
    void hideHint();
};

class UIBattleLayer : public cocos2d::Layer {
private:
    BattleLayer* _battle_layer;
    bool _is_touch_began;
    UISkillNode* _selected_skill;
    
    std::list<UISkillNode*> _skill_nodes;
    
public:
    UIBattleLayer();
    virtual ~UIBattleLayer();
    
    static UIBattleLayer* create( BattleLayer* battle_layer );
    virtual bool init( BattleLayer* battle_layer );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
    
    void reset();
    
    void addSkillNode( UnitNode* unit );
    void removeSkillNode( UnitNode* unit );
    
    void removeAllSkillNodes();
    
private:
    UISkillNode* skillNodeForTouch( cocos2d::Touch* touch );
    void alignSkillNodesWithPadding( float padding );
};

#endif /* defined(__Boids__UIBattleLayer__) */
