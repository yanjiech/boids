//
//  EditorVisionNode.cpp
//  Boids
//
//  Created by Xin Xu on 1/7/15.
//
//

#include "EditorVisionNode.h"
#include "EditorHelper.h"
#include "Utils.h"

/*
 children从底向上:
 vision图片,
 nameTextField,
 选中色块
 */

using namespace cocos2d;

EditorVisionNode *EditorVisionNode::create(const ValueMap& GIDProperty, Vec2 position, const std::string& name) {
    auto ret = new EditorVisionNode();
    if (ret->init(GIDProperty, position, name)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EditorVisionNode::init(const ValueMap& GIDProperty, Vec2 position, const std::string& name) {
    //super init
    if (!Node::init()) {
        return false;
    }
    assert(name.length() > 0); // vision object必须要有名字，为了meta.json和tmx进行匹配
    
    this->setPosition(position);
    _visionImageName = GIDProperty.at("source").asString();
    _visionImageName = _visionImageName.substr(0, _visionImageName.length() - 4); // remove .png
    
    this->_visionSprite = createSpriteOfState(_state);
    auto contentSize = _visionSprite->getTextureRect().size;
    auto centerPoint = Vec2(contentSize.width / 2, contentSize.height / 2);
    this->setContentSize(contentSize);
    _visionSprite->setAnchorPoint(Vec2::ZERO);
    _visionSprite->setPosition(Vec2::ZERO);
    this->addChild(_visionSprite, 0);
    
    this->_nameTextField = ui::TextField::create();
    _nameTextField->setFontSize(20);
    _nameTextField->setString(name);
    _nameTextField->setPosition(centerPoint);
    _nameTextField->setTextColor(Color4B::RED);
    this->addChild(_nameTextField, 1);
    
    this->_selectingSprite = EditorHelper::spriteWithColor(Color4F(0, 0, 1, 0.5), contentSize);
    _selectingSprite->setPosition(centerPoint);
    this->addChild(_selectingSprite, 2);
    
    this->setSelected(_selected);
    
    return true;
}

void EditorVisionNode::setState(const std::string& state) {
    if (_state != state) {
        _state = state;
        if (_visionSprite != nullptr) {
            _visionSprite->removeFromParent();
        }
        _visionSprite = createSpriteOfState(_state);
        _visionSprite->setAnchorPoint(Vec2::ZERO);
        _visionSprite->setPosition(Vec2::ZERO);
        this->addChild(_visionSprite, 0);
    }
}

void EditorVisionNode::setSelected(bool selected) {
    _selected = selected;
    _selectingSprite->setVisible(_selected);
}

Sprite *EditorVisionNode::createSpriteOfState(const std::string& state) {
    auto spriteName = Utils::stringFormat("%s_%s.png", _visionImageName.c_str(), state.c_str());
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName);
    if (spriteFrame == nullptr) {
        spriteName = Utils::stringFormat("%s.png", _visionImageName.c_str());
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName);
    }
    return Sprite::createWithSpriteFrame(spriteFrame);
}
