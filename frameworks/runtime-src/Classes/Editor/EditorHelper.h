//
//  EditorHelper.h
//  Boids
//
//  Created by Xin Xu on 11/6/14.
//
//

#ifndef __Boids__EditorHelper__
#define __Boids__EditorHelper__

#include "BEHeaders.h"

class EditorHelper {
public:
    static cocos2d::Rect rectFromTwoPoint(cocos2d::Point p1, cocos2d::Point p2);
    static cocos2d::Sprite *spriteWithColor(cocos2d::Color4F color, cocos2d::Size size);
};

#endif /* defined(__Boids__EditorHelper__) */
