//
//  EditorVisionNode.h
//  Boids
//
//  Created by Xin Xu on 1/7/15.
//
//

#ifndef __Boids__EditorVisionNode__
#define __Boids__EditorVisionNode__

#include "cocos2d.h"
#include "ui/UITextField.h"

class EditorVisionNode : public cocos2d::Node {
public:
    EditorVisionNode(): _state("normal"), _selected(false), _visionSprite(nullptr) {}
    static EditorVisionNode *create(const cocos2d::ValueMap& GIDProperty, cocos2d::Vec2 position, const std::string& name);
    bool init(const cocos2d::ValueMap& GIDProperty, cocos2d::Vec2 position, const std::string& name);
    
    void setSelected(bool selected);
    bool isSelected() const { return _selected; }
    
    void setState(const std::string& state);
    const std::string& getState() const { return _state; }
private:
    cocos2d::Sprite *_visionSprite;
    cocos2d::ui::TextField *_nameTextField; // 显示vision name
    cocos2d::Sprite *_selectingSprite;
    bool _selected;
    std::string _state;
    std::string _visionImageName;
    cocos2d::Sprite *createSpriteOfState(const std::string& state);
};


#endif /* defined(__Boids__EditorVisionNode__) */
