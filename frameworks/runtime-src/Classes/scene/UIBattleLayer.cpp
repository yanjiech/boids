//
//  UIBattleLayer.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/2/15.
//
//

#include "UIBattleLayer.h"

using namespace cocos2d;

UISkillNode::UISkillNode() {
    
}

UISkillNode::~UISkillNode() {
}

UISkillNode* UISkillNode::create( BattleLayer* battle_layer, UnitNode* unit_node ) {
    UISkillNode* ret = new UISkillNode();
    if( ret && ret->init( battle_layer, unit_node ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UISkillNode::init( BattleLayer* battle_layer, UnitNode* unit_node ) {
    if( !Node::init() ) {
        return false;
    }
    _battle_layer = battle_layer;
    _unit_node = unit_node;
    
    return true;
}

void UISkillNode::activate( const cocos2d::Point& dir, float range_per ) {
    
}

void UISkillNode::showHint( const cocos2d::Point& dir, float range_per ) {
}

void UISkillNode::hideHint() {
    
}

UIBattleLayer::UIBattleLayer() {
    
}

UIBattleLayer::~UIBattleLayer() {
    
}

UIBattleLayer* UIBattleLayer::create( BattleLayer* battle_layer ) {
    UIBattleLayer* ret = new UIBattleLayer();
    if( ret && ret->init( battle_layer ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool UIBattleLayer::init( BattleLayer* battle_layer ) {
    _battle_layer = battle_layer;
    return true;
}

bool UIBattleLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event ) {
    if( _is_touch_began == false ) {
        
    }
    return false;
}

void UIBattleLayer::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleLayer::onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleLayer::onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event ) {
    
}

void UIBattleLayer::reset() {
    _is_touch_began = false;
    _selected_skill = nullptr;
}

void UIBattleLayer::addSkillNode( UnitNode* unit ) {
    
}

void UIBattleLayer::removeSkillNode( UnitNode* unit ) {
    
}

void UIBattleLayer::removeAllSkillNodes() {
    for( auto node : _skill_nodes ) {
        node->removeFromParent();
    }
    _skill_nodes.clear();
}

UISkillNode* UIBattleLayer::skillNodeForTouch( cocos2d::Touch* touch ) {
    for( auto node : this->getChildren() ) {
        UISkillNode* skill_node = dynamic_cast<UISkillNode*>( node );
        if( skill_node ) {
            
        }
    }
    return nullptr;
}

void UIBattleLayer::alignSkillNodesWithPadding( float padding ) {
    int count = (int)_skill_nodes.size();
    
}