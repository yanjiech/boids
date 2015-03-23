//
//  SkillNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/18/15.
//
//

#include "SkillNode.h"
#include "UnitNode.h"

using namespace cocos2d;

SkillNode::SkillNode() :
_should_recycle( false ),
_owner( nullptr )
{
    
}

SkillNode::~SkillNode() {
    CC_SAFE_RELEASE( _owner );
}

SkillNode* SkillNode::create( UnitNode* owner ) {
    SkillNode* ret = new SkillNode();
    if( ret && ret->init( owner ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool SkillNode::init( UnitNode* owner ) {
    this->setOwner( owner );
    
    return true;
}

void SkillNode::updateFrame( float delta ) {
    
}

void SkillNode::setOwner( UnitNode* owner ) {
    CC_SAFE_RELEASE( _owner );
    _owner = owner;
    CC_SAFE_RETAIN( _owner );
}

UnitNode* SkillNode::getOwner() {
    return _owner;
}

void SkillNode::begin() {
    
}

void SkillNode::end() {
    
}