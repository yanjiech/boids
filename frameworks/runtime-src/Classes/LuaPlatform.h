//
//  LuaPlatform.h
//  Boids
//
//  Created by Xin Xu on 11/18/14.
//
//

#ifndef __Boids__LuaPlatform__
#define __Boids__LuaPlatform__

#include "cocos2d.h"

class LuaPlatform {
public:
    static bool isPlatformIOS();
    static bool isPlatformAndroid();
    static bool isPlatformMac();
    static bool isPlatformWin32();
};


#endif /* defined(__Boids__LuaPlatform__) */
