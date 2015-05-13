//
//  BETypes.h
//  Boids
//
//  Created by Xin Xu on 11/27/14.
//
//

#ifndef __Boids__BETypes__
#define __Boids__BETypes__

#include <vector>
#include <string>
#include <memory>

class EditorType {
public:
    EditorType() {}
    virtual ~EditorType() {}
    const std::string& getType(size_t index) { return _typeVector[index]; }
    const std::vector<std::string>& getTypeVector() { return _typeVector; }
protected:
    std::vector<std::string> _typeVector;
    friend class EditorTypeManager;
};

typedef std::shared_ptr<EditorType> EditorTypePtr;

class EditorTypeManager {
public:
    EditorTypeManager();
    virtual ~EditorTypeManager() {}
    static EditorTypeManager *getInstance();
    EditorTypePtr getEventTriggerState() { return _eventTriggerState; }
    EditorTypePtr getEventActionState() { return _eventActionState; }
    EditorTypePtr getGameState() { return _gameState; }
    EditorTypePtr getTaskState() { return _taskState; }
    EditorTypePtr getUnitActionState() { return _unitActionState; }
    EditorTypePtr getUnitTriggerState() { return _unitTriggerState; }
    EditorTypePtr getUnitType() { return _unitType; }
    EditorTypePtr getUnitTriggerSourceType() { return _unitTriggerSourceType; }
    EditorTypePtr getUnitActionSourceType() { return _unitActionSourceType; }
    EditorTypePtr getTriggerType() { return _triggerType; }
    EditorTypePtr getActionType() { return _actionType; }
    EditorTypePtr getVisionObjectStateType() { return _visionObjectStateType; }
    EditorTypePtr getVisionObjectSourceType() { return _visionObjectSourceType; }
    EditorTypePtr getGameConditionType() { return _gameConditionType; }
    EditorTypePtr getGameConditionSourceType() { return _gameConditionSourceType; }
    EditorTypePtr getConversationState() { return _conversationState; }
    EditorTypePtr getUnitPopupType() { return _unitPopupType; }
    EditorTypePtr getTriggerRelationType() { return _triggerRelationType; }
    EditorTypePtr getStoryStateType() { return _storyStateType; }
protected:
    EditorTypePtr _eventTriggerState;
    EditorTypePtr _eventActionState;
    EditorTypePtr _gameState;
    EditorTypePtr _taskState;
    EditorTypePtr _unitActionState;
    EditorTypePtr _unitTriggerState;
    EditorTypePtr _unitType;
    EditorTypePtr _unitTriggerSourceType;
    EditorTypePtr _unitActionSourceType;
    EditorTypePtr _triggerType;
    EditorTypePtr _actionType;
    EditorTypePtr _visionObjectStateType;
    EditorTypePtr _visionObjectSourceType;
    EditorTypePtr _gameConditionType;
    EditorTypePtr _gameConditionSourceType;
    EditorTypePtr _conversationState;
    EditorTypePtr _unitPopupType;
    EditorTypePtr _triggerRelationType;
    EditorTypePtr _storyStateType;
    
    static EditorTypeManager *_instance;
};

#endif /* defined(__Boids__BETypes__) */
