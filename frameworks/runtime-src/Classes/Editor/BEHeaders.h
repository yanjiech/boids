//
//  BEHeaders.h
//  Boids
//
//  Created by Xin Xu on 11/8/14.
//
//

#ifndef Boids_BEHeaders_h
#define Boids_BEHeaders_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#if defined(CC_TARGET_OS_MAC) || (defined(_WIN32) && defined(_WINDOWS))
#define SUPPORT_EXTERNAL_MAP 1
#else
#undef SUPPORT_EXTERNAL_MAP
#endif

#endif
