//
//  EditorPositionNode.cpp
//  Boids
//
//  Created by Xin Xu on 11/13/14.
//
//

#include "EditorPositionNode.h"
#include "EditorHelper.h"
#include "ui/UITextField.h"

using namespace cocos2d;

EditorPositionNode *EditorPositionNode::create(EditorPositionPtr pos, const Color4F& color) {
    auto ret = new EditorPositionNode();
    if (ret->init(pos, color)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EditorPositionNode::init(EditorPositionPtr pos, const Color4F& color) {
    if (!Node::init()) {
        return false;
    }
    this->setContentSize(pos->Rect.size);
    this->setAnchorPoint(Vec2::ZERO);
    _normalSprite = EditorHelper::spriteWithColor(color, pos->Rect.size);
    _selectedSprite = EditorHelper::spriteWithColor(Color4F(0, 0, 1, 0.5), pos->Rect.size);
    auto center = Vec2(pos->Rect.size.width / 2, pos->Rect.size.height / 2);
    _normalSprite->setPosition(center);
    _selectedSprite->setPosition(center);
    
    this->addChild(_normalSprite);
    this->addChild(_selectedSprite);
    auto tf = ui::TextField::create();
    tf->setFontSize(12);
    tf->setString(pos->Name);
    tf->setPosition(center);
    tf->setTextColor(Color4B::BLACK);
    this->addChild(tf);
    this->setPosition(pos->Rect.origin);
    _pos = pos;
    setSelected(false);
    return true;
}

bool EditorPositionNode::containsPoint(Point point) {
    return _pos->Rect.containsPoint(point);
}

void EditorPositionNode::setSelected(bool selected) {
    _selected = selected;
    if (selected) {
        _selectedSprite->setVisible(true);
        _normalSprite->setVisible(false);
    } else {
        _selectedSprite->setVisible(false);
        _normalSprite->setVisible(true);
    }
}