//
//  GameTask.h
//  Boids
//
//  Created by Yanjie Chen on 3/23/15.
//
//

#ifndef __Boids__GameTask__
#define __Boids__GameTask__

#include "cocos2d.h"

class MapLogic;

class GameTask : public cocos2d::Ref {
protected:
    MapLogic* _map_logic;
    bool _is_primary;
    
    std::string _task_name;
    std::string _task_title;
    std::string _task_desc;
    
    std::string _state;
    
    bool _is_active;
    
    float _progress;
    
public:
    GameTask();
    virtual ~GameTask();
    
    //factory method
    static GameTask* create( const cocos2d::ValueMap& data, MapLogic* map_logic );
    virtual bool init( const cocos2d::ValueMap& data, MapLogic* map_logic );
    
    bool isPrimary() { return _is_primary; }
    void setPrimary( bool b ) { _is_primary = b; }
    
    const std::string& getTaskName() { return _task_name; }
    void setTaskName( const std::string& name ) { _task_name = name; }
    
    const std::string& getTaskTitle() { return _task_title; }
    void setTaskTitle( const std::string& title ) { _task_title = title; }
    
    const std::string& getTaskDesc() { return _task_desc; }
    void setTaskDesc( const std::string& desc ) { _task_desc = desc; }
    
    const std::string& getTaskState() { return _state; }
    void setTaskState( const std::string& new_state );
    
    bool isActive() { return _is_active; }
    void setActive( bool b ) { _is_active = b; }
    
    void addProgress( float p );
    float getProgress() { return _progress; }
    void setProgress( float p ) { _progress = p; }
};

#endif /* defined(__Boids__GameTask__) */
