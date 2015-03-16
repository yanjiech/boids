//
//  EditorPositionNode.h
//  Boids
//
//  Created by Xin Xu on 11/13/14.
//
//

#ifndef __Boids__EditorPositionNode__
#define __Boids__EditorPositionNode__

#include "cocos2d.h"
#include "EditorModel.h"
#include "EditorModelDefines.h"

class EditorPositionNode : public cocos2d::Node {
public:
    static EditorPositionNode *create(EditorPositionPtr pos, const cocos2d::Color4F& color);
    bool init(EditorPositionPtr pos, const cocos2d::Color4F& color);
    bool containsPoint(cocos2d::Point point);
    void setSelected(bool selected);
    bool isSelected() const { return _selected; }
    EditorPositionPtr getData() const { return _pos; }
private:
    EditorPositionPtr _pos;
    cocos2d::Sprite *_normalSprite;
    cocos2d::Sprite *_selectedSprite;
    bool _selected;
};

#endif /* defined(__Boids__EditorPositionNode__) */
