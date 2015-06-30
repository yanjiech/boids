//
//  TutorialLayer.h
//  Boids
//
//  Created by chenyanjie on 6/29/15.
//
//

#ifndef __Boids__TutorialLayer__
#define __Boids__TutorialLayer__

#include "cocos2d.h"
#include "BattleLayer.h"
#include "UIHomeLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

typedef enum {
    TutorialStateUnstarted = 0,
    TutorialStateWaiting = 1,
    TutorialStatePlaying = 2
}eTutorialState;

typedef enum {
    TutorialBattle = 0,
    TutorialPrepare = 1
}eTutorialType;

class TutorialLayer : public cocos2d::Layer {
private:
    int _step;
    eTutorialState _state;
    eTutorialType _type;
    
    spine::SkeletonAnimation* _prompt;
    
    cocos2d::Rect _touchable_rect;
    
    cocos2d::Node* _root_node;
    
public:
    TutorialLayer();
    virtual ~TutorialLayer();
    
    static TutorialLayer* create( eTutorialType type );
    virtual bool init( eTutorialType type );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );
    
    void updateTutorial( float delta );
    
    virtual void onEnter();
    
    void onStoryEnd( UIStoryLayer* story );
    
    void gotoNextStep();
};

#endif /* defined(__Boids__TutorialLayer__) */
