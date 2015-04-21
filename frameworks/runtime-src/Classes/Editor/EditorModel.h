//
//  EditorModel.h
//  Boids
//
//  Created by Xin Xu on 11/6/14.
//
//

#ifndef __Boids__EditorModel__
#define __Boids__EditorModel__

#include "BEHeaders.h"
#include "json/rapidjson.h"
#include "json/document.h"

class EditorBase {
public:
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
    EditorBase(): _json(rapidjson::kObjectType) {}
protected:
    rapidjson::Value _json;
};

class EditorPosition : public EditorBase {
public:
    cocos2d::Rect Rect;
    std::string Name;
    std::string Tag;
    std::string PrevPos;
    EditorPosition(cocos2d::Rect rect, const std::string& name):Rect(rect), Name(name), Tag("") {}
    EditorPosition(const EditorPosition& pos):Rect(pos.Rect), Name(pos.Name), Tag(pos.Tag) {}
    EditorPosition() {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorUnit : public EditorBase {
public:
    std::string Name;
    std::string Class;
    int Count;
    int Level;
    bool IsBoss;
	bool UnPushable;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorWave : public EditorBase {
public:
    std::vector<std::shared_ptr<EditorUnit>> Units;
    std::string PositionName;
    std::string PositionTag;
    float Interval;
    void reset();
    EditorWave(): PositionName(""), PositionTag(""), Interval(0) {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorTask : public EditorBase {
public:
    std::string Name;
    std::string Description;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorGameCondition : public EditorBase {
public:
    int Number;
    std::string Name;
    std::string Type;
    std::string Title;
    std::string Desc;
    EditorGameCondition(): Number(0), Name(""), Desc( "" ) {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorSpeech : public EditorBase {
public:
    std::string Content;
    float Duration;
    float Interval;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorTriggerMeta : public EditorBase {
public:
    EditorTriggerMeta(): IsRepeated(false), TriggerRelation("then") {}
    bool IsRepeated;
    std::string TriggerRelation;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorTriggerBase : public EditorBase {
public:
    EditorTriggerBase(const std::string type): TriggerType(type) {}
    std::string TriggerType;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorMapInitTrigger : public EditorTriggerBase {
public:
    EditorMapInitTrigger(): EditorTriggerBase("map_init") {}
};

class EditorUnitTrigger : public EditorTriggerBase {
public:
    EditorUnitTrigger(): EditorTriggerBase("unit_change"), PositionName("") {}
    std::string SourceType;
    std::string SourceValue;
    std::string UnitState;
    std::string PositionName;
    int TriggerCount;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorUnitStayTrigger : public EditorTriggerBase {
public:
    EditorUnitStayTrigger(): EditorTriggerBase("unit_stay"), PositionName("") {}
    std::string SourceType;
    std::string SourceValue;
    std::string PositionName;
    int TriggerCount;
    float Duration;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorEventTrigger : public EditorTriggerBase {
public:
    EditorEventTrigger(): EditorTriggerBase("event_change") {}
    std::string EventName;
    std::string EventState;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorGameTrigger : public EditorTriggerBase {
public:
    std::string GameState;
    EditorGameTrigger(): EditorTriggerBase("game_change") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorCustomTrigger : public EditorTriggerBase {
public:
    EditorCustomTrigger(): EditorTriggerBase("custom") {}
    std::string TriggerName;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorVisionTrigger : public EditorTriggerBase {
public:
    EditorVisionTrigger(): EditorTriggerBase("vision_change") {}
    std::string SourceType;
    std::string SourceValue;
    std::string VisionState;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

//class EditorConversationChangeTrigger : public EditorTriggerBase {
//public:
//    EditorConversationChangeTrigger(): EditorTriggerBase("conversation_action") {}
//    std::string ConversationName;
//    std::string State;
//    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
//    virtual void loadJson(const rapidjson::Value& value);
//};

class EditorStoryChangeTrigger : public EditorTriggerBase {
public:
    EditorStoryChangeTrigger(): EditorTriggerBase("story_change") {}
    std::string StoryName;
    std::string State;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorActionMeta : public EditorBase {
public:
    bool IsInfinite;
    int RepeatTimes;
    float Delay;
    float Interval;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorActionBase : public EditorBase {
public:
    EditorActionBase(const std::string type);
    std::string ActionType;
    std::shared_ptr<EditorActionMeta> ActionMeta;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorCustomAction : public EditorActionBase {
public:
    std::string ActionName;
    EditorCustomAction(): EditorActionBase("custom") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorUnitAction : public EditorActionBase {
public:
    std::string SourceType;
    std::string SourceValue;
    std::string UnitState;
    std::string UnitType;
    std::string PositionName;
    std::string PositionTag;
    std::string TagName;
    std::string BuffName;
    std::string BuffType;
    std::string BuffParams;
    std::string ClassName;
    std::string CustomChange;
    std::string PopupType;
    bool ShowHP;
    bool StateChanged;
    bool TypeChanged;
    bool ShowHPChanged;
    bool TagChanged;
    bool BuffChanged;
    bool IsBoss;
	bool UnPushable;
    int UnitCount;
    int UnitLevel;
    EditorUnitAction(): EditorActionBase("unit_change"), StateChanged(false), TypeChanged(false), ShowHPChanged(false), TagChanged(false), BuffChanged(false), IsBoss(false), UnPushable(false), ClassName("LogicUnit"),
    SourceValue(""), PositionName(""), PositionTag(""), TagName(""), BuffName(""), UnitCount(0), PopupType("normal") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorTaskAction : public EditorActionBase {
public:
    std::string TaskName;
    std::string TaskState;
    EditorTaskAction(): EditorActionBase("task_change") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorGameAction : public EditorActionBase {
public:
    std::string GameState;
    EditorGameAction(): EditorActionBase("game_change") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorEventAction : public EditorActionBase {
public:
    std::string EventName;
    std::string EventState;
    EditorEventAction(): EditorActionBase("event_change") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorVisionAction : public EditorActionBase {
public:
    std::string SourceType;
    std::string SourceValue;
    std::string VisionState;
    EditorVisionAction(): EditorActionBase("vision_change") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorWaveAction : public EditorActionBase {
public:
    std::vector<std::shared_ptr<EditorWave>> Waves;
    EditorWaveAction(): EditorActionBase("waves_appear") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorConversationAction : public EditorActionBase {
public:
    std::vector<std::shared_ptr<EditorSpeech>> Speeches;
    std::string Name;
    std::string SourceType;
    std::string SourceValue;
    bool RandomOrder;
    int RepeatTimes;
    
    EditorConversationAction(): EditorActionBase("conversation_action") {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorStory : public EditorBase {
public:
    std::string SpeakerName;
    std::string Line;
    bool LeftOrRight;//true left, false right
    
    EditorStory();
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorStoryAction : public EditorActionBase {
public:
    std::vector<std::shared_ptr<EditorStory>> Stories;
    std::string Name;
    
    EditorStoryAction() : EditorActionBase( "story_action" ) {}
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorEvent : public EditorBase {
public:
    EditorEvent();
    std::string Name;
    std::shared_ptr<EditorTriggerMeta> TriggerMeta;
    std::vector<std::shared_ptr<EditorTriggerBase>> Triggers;
    std::vector<std::shared_ptr<EditorActionBase>> Actions;
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
};

class EditorData {
public:
//    std::vector<std::shared_ptr<EditorTask>> Tasks;
    std::vector<std::shared_ptr<EditorPosition>> Positions;
    std::vector<std::shared_ptr<EditorEvent>> Events;
    std::vector<std::shared_ptr<EditorGameCondition>> Conditions;
    std::string getString();
    void loadJson(const std::string& content);
    virtual rapidjson::Value& toJson(rapidjson::Document::AllocatorType& allocator);
    virtual void loadJson(const rapidjson::Value& value);
private:
    rapidjson::Document _doc;
};

enum class EditorOperation {
    None = 0,
    ChossingPosition,
    ChoosingRect,
    InputPositionName,
    CreatePosition,
    InputEventName,
    SetTrigger,
    SetAction,
    SetUnitTrigger,
    SetUnitStayTrigger,
    SetStoryChangeTrigger,
    SetEventTrigger,
    SetUnitAction,
    SetGameAction,
    SetMissionAction,
    SetEventAction,
    CreateEvent,
    ListEvent,
    SetCustomTrigger,
    SetCustomAction,
    SetVisionAction,
    SetGameTrigger,
    SetVisionTrigger,
    SetWaveAction,
    EditCondition,
    SetConverSationAction,
    SetStoryAction,
    EditEvent,
};

class EditorContext {
public:
    EditorContext();
    std::vector<EditorOperation> Pipeline;
    void moveBack();
    int CurrentStep;
    
    cocos2d::Point TouchStartPosition;
    
    std::shared_ptr<EditorPosition> Position;
    std::shared_ptr<EditorEvent> Event;
    std::shared_ptr<EditorActionMeta> ActionMeta;
    EditorOperation currentOperation();
    void reset();
    void addOperation(EditorOperation op) { Pipeline.push_back(op); }
};

#endif /* defined(__Boids__EditorModel__) */
