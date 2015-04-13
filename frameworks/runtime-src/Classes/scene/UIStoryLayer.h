//
//  UIStoryLayer.h
//  Boids
//
//  Created by chenyanjie on 4/13/15.
//
//

#ifndef __Boids__UIStoryLayer__
#define __Boids__UIStoryLayer__

#include "cocos2d.h"

class UIStoryLayer : public cocos2d::Layer {
public:
    typedef std::function<void()> StoryEndCallback;
    
private:
    bool _is_enabled;
    cocos2d::ValueVector _story_data;
    
    float _elapse;
    float _interval;
    
    int _current_line_index;
    int _current_letter_pos;
    
    std::string _current_line;
    int _max_letters_per_row;
    
    cocos2d::Sprite* _sp_speaker;
    cocos2d::Label* _lb_line;

    StoryEndCallback _callback;
    
    bool gotoNextLine();
    
public:
    UIStoryLayer();
    virtual ~UIStoryLayer();
    
    static UIStoryLayer* create( const StoryEndCallback& callback );
    virtual bool init( const StoryEndCallback& callback );
    
    virtual bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* unused_event );
    virtual void onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* unused_event );
    virtual void onTouchCancelled( cocos2d::Touch* touch, cocos2d::Event* unused_event );
    virtual void onTouchEnded( cocos2d::Touch* touch, cocos2d::Event* unused_event );
    
    virtual void updateFrame( float delta );
    
    void reset();
    
    void showLetters( int count );
    
    bool isEnabled();
    void setEnabled( bool b );
    
    float getInterval();
    void setInterval( float interval );
    
    int getMaxLettersPerRow();
    void setMaxLettersPerRow( int count );

    void setStoryData( const cocos2d::ValueMap& data );
    
    void setStoryEndCallback( const StoryEndCallback& callback );
};

#endif /* defined(__Boids__UIStoryLayer__) */
