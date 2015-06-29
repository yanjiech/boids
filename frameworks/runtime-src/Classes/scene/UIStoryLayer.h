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
#include "spine/spine-cocos2dx.h"

class UIStoryLayer : public cocos2d::Layer {
public:
    typedef std::function<void(UIStoryLayer* story)> StoryEndCallback;
    
private:
    std::string _story_name;
    
    bool _is_enabled;
    cocos2d::ValueVector _story_data;
    
    float _elapse;
    float _interval;
    
    int _current_line_index;
    int _current_letter_pos;
    
    std::u16string _current_line;
    int _max_letters_per_row;
    
    cocos2d::Sprite* _sp_speaker;
    cocos2d::Label* _lb_line;
    
    spine::SkeletonAnimation* _prompt;

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
    
    const std::string& getStoryName() { return _story_name; }
    void setStoryName( const std::string& name ) { _story_name = name; }
    
    void showLetters( int count );
    
    bool isEnabled();
    void setEnabled( bool b );
    
    float getInterval();
    void setInterval( float interval );
    
    int getMaxLettersPerRow();
    void setMaxLettersPerRow( int count );

    void setStoryData( const cocos2d::ValueMap& data );
    
    void setStoryEndCallback( const StoryEndCallback& callback );
    
    void setSpeaker( const std::string& name, bool left_or_right );
};

#endif /* defined(__Boids__UIStoryLayer__) */
