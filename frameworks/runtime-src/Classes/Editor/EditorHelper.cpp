//
//  EditorHelper.cpp
//  Boids
//
//  Created by Xin Xu on 11/6/14.
//
//

#include "EditorHelper.h"

using namespace cocos2d;

Rect EditorHelper::rectFromTwoPoint(Point p1, Point p2) {
    float width = fabsf(p2.x - p1.x);
    float height = fabsf(p2.y - p1.y);
    float x = fminf(p1.x, p2.x);
    float y = fminf(p1.y, p2.y);
    return Rect(x, y, width, height);
}

Sprite *EditorHelper::spriteWithColor(Color4F color, Size size) {
    auto rt = RenderTexture::create(size.width, size.height);
    rt->beginWithClear(color.r, color.g, color.b, color.a);
    rt->end();
    return Sprite::createWithTexture(rt->getSprite()->getTexture());
}