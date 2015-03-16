//
//  LuaPlatform.cpp
//  Boids
//
//  Created by Xin Xu on 11/18/14.
//
//

#include "LuaPlatform.h"

bool LuaPlatform::isPlatformIOS() {
    return CC_TARGET_PLATFORM == CC_PLATFORM_IOS;
}

bool LuaPlatform::isPlatformAndroid() {
    return CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID;
}

bool LuaPlatform::isPlatformMac() {
    return CC_TARGET_PLATFORM == CC_PLATFORM_MAC;
}

bool LuaPlatform::isPlatformWin32() {
    return CC_TARGET_PLATFORM == CC_PLATFORM_WIN32;
}