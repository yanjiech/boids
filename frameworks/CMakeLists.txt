cmake_minimum_required(VERSION 2.8)

set(APP_NAME Boids)
set(CLASS_PATH runtime-src/Classes)
project (${APP_NAME})

include(cocos2d-x/cmake/BuildHelpers.CMakeLists.txt)

option(DEBUG_MODE "Debug or release?" ON)

if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE DEBUG)
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wno-deprecated-declarations -Wno-reorder")
ADD_DEFINITIONS(-DLINUX)

# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "64-bit")
else()
set(ARCH_DIR "32-bit")
endif()

file(GLOB_RECURSE GAME_SRC ${CLASS_PATH} "*.cpp")
list(APPEND GAME_SRC runtime-src/proj.linux/main.cpp)

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d-x)

include_directories(
  /usr/include
  /usr/include/GLFW
  /usr/local/include/GLFW
  ${CLASS_PATH}
  ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/manual
  ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/auto
  ${COCOS2D_ROOT}/external/lua/lua
  ${COCOS2D_ROOT}/external/lua/tolua
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/platform/desktop
  ${COCOS2D_ROOT}/cocos/platform/linux
  ${COCOS2D_ROOT}/cocos/editor-support
  ${COCOS2D_ROOT}/extensions
  ${COCOS2D_ROOT}/external
  ${COCOS2D_ROOT}/external/edtaa3func
  ${COCOS2D_ROOT}/external/jpeg/include/linux
  ${COCOS2D_ROOT}/external/tiff/include/linux
  ${COCOS2D_ROOT}/external/webp/include/linux
  ${COCOS2D_ROOT}/external/websockets/include/linux
  ${COCOS2D_ROOT}/external/tinyxml2
  ${COCOS2D_ROOT}/external/unzip
  ${COCOS2D_ROOT}/external/chipmunk/include/chipmunk
  ${COCOS2D_ROOT}/external/freetype2/include/linux
  ${COCOS2D_ROOT}/external/linux-specific/fmod/include/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/xxhash
  ${COCOS2D_ROOT}/external/xxtea
)

file(GLOB children RELATIVE ${CLASS_PATH} ${CLASS_PATH}/*)
foreach(child ${children})
    if(IS_DIRECTORY ${CLASS_PATH}/${child})
        list(APPEND ${INCLUDE_DIRECTORIES} ${CLASS_PATH}/${child})
    endif()
endforeach()

link_directories(
  /usr/local/lib
  ${COCOS2D_ROOT}/external/jpeg/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/tiff/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/webp/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/websockets/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/freetype2/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/linux-specific/fmod/prebuilt/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/chipmunk/prebuilt/linux/${ARCH_DIR}
)

# build for 3rd party libraries
if(LINUX)
add_subdirectory(${COCOS2D_ROOT}/external/Box2D)
add_subdirectory(${COCOS2D_ROOT}/external/unzip)
add_subdirectory(${COCOS2D_ROOT}/external/xxhash)
add_subdirectory(${COCOS2D_ROOT}/external/tinyxml2)
endif()

# libcocos2d
add_subdirectory(${COCOS2D_ROOT}/cocos)

# libluacocos2d
add_subdirectory(${COCOS2D_ROOT}/cocos/scripting/lua-bindings)

# add the executable
add_executable(${APP_NAME} ${GAME_SRC})

if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(FMOD_LIB "fmodex64")
else()
set(FMOD_LIB "fmodex")
endif()

target_link_libraries(${APP_NAME} cocos2d luacocos2d)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

pre_build(${APP_NAME}
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../res ${APP_BIN_DIR}/Resources/res
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../src ${APP_BIN_DIR}/Resources/src
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/cocos2d ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/cocosdenshion ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/cocosbuilder ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/cocostudio ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/extension ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/network ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/script/ui ${APP_BIN_DIR}/Resources
  )
  
