//
//  EditorModel.cpp
//  Boids
//
//  Created by Xin Xu on 11/6/14.
//
//

#include "EditorModel.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "Utils.h"
#include <fstream>
#include <memory>

rapidjson::Value& EditorBase::toJson(rapidjson::Document::AllocatorType& allocator) {
    _json.SetObject();
    return _json;
}

void EditorBase::loadJson(const rapidjson::Value& value) {
}

rapidjson::Value& EditorPosition::toJson(rapidjson::Document::AllocatorType& allocator) {
    _json.SetObject();
    _json.AddMember("name", this->Name.c_str(), allocator);
    rapidjson::Value rect;
    rect.SetObject();
    rect.AddMember("x", this->Rect.origin.x, allocator);
    rect.AddMember("y", this->Rect.origin.y, allocator);
    rect.AddMember("width", this->Rect.size.width, allocator);
    rect.AddMember("height", this->Rect.size.height, allocator);
    _json.AddMember("rect", rect, allocator);
    if (this->Tag.length() > 0) {
        _json.AddMember("tag", this->Tag.c_str(), allocator);
    }
    if( PrevPos != "" ) {
        _json.AddMember( "prev_pos", this->PrevPos.c_str(), allocator );
    }
    return _json;
}

void EditorPosition::loadJson(const rapidjson::Value& value) {
    this->Name = value["name"].GetString();
    const rapidjson::Value& rect = value["rect"];
    float x = rect["x"].GetDouble();
    float y = rect["y"].GetDouble();
    float width = rect["width"].GetDouble();
    float height = rect["height"].GetDouble();
    this->Rect.setRect(x, y, width, height);
    if (value.HasMember("tag")) {
        this->Tag = value["tag"].GetString();
    } else {
        this->Tag = "";
    }
    if( value.HasMember( "prev_pos" ) ) {
        this->PrevPos = value["prev_pos"].GetString();
    }
}

rapidjson::Value& EditorUnit::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("name", Name.c_str(), allocator);
    _json.AddMember("count", Count, allocator);
    _json.AddMember("level", Level, allocator);
    _json.AddMember("is_boss", IsBoss, allocator);
	_json.AddMember("unpushable", UnPushable, allocator);
    if (Class != "LogicUnit") {
        _json.AddMember("class_name", Class.c_str(), allocator);
    }
    return _json;
}

void EditorUnit::loadJson(const rapidjson::Value& value) {
    Name = value["name"].GetString();
    Count = value["count"].GetInt();
    Level = value["level"].GetInt();
    IsBoss = value["is_boss"].GetBool();
	UnPushable = value["unpushable"].GetBool();
    if (value.HasMember("class_name")) {
        Class = value["class_name"].GetString();
    } else {
        Class = "LogicUnit";
    }
}

void EditorWave::reset() {
    Units.clear();
    PositionName = "";
    PositionTag = "";
    Interval = 0;
}

rapidjson::Value& EditorWave::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    rapidjson::Value units;
    units.SetArray();
    for (auto u : Units) {
        units.PushBack(u->toJson(allocator), allocator);
    }
    _json.AddMember("units", units, allocator);
    if (PositionName.length() > 0) {
        _json.AddMember("position_name", PositionName.c_str(), allocator);
    } else if (PositionTag.length() > 0) {
        _json.AddMember("position_tag", PositionTag.c_str(), allocator);
    }
    _json.AddMember("interval", Interval, allocator);
    return _json;
}

void EditorWave::loadJson(const rapidjson::Value& value) {
    if (value.HasMember("position_name")) {
        PositionName = value["position_name"].GetString();
    }
    if (value.HasMember("position_tag")) {
        PositionTag = value["position_tag"].GetString();
    }
    Interval = value["interval"].GetDouble();
    const rapidjson::Value& units = value["units"];
    assert(units.IsArray());
    Units.clear();
    for (int i = 0; i < units.Size(); ++i) {
        auto unit = std::shared_ptr<EditorUnit>(new EditorUnit());
        unit->loadJson(units[i]);
        Units.push_back(unit);
    }
}

rapidjson::Value& EditorTask::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("name", this->Name.c_str(), allocator);
    _json.AddMember("desc", this->Description.c_str(), allocator);
    return _json;
}

void EditorTask::loadJson(const rapidjson::Value& value) {
    this->Name = value["name"].GetString();
    this->Description = value["desc"].GetString();
}

rapidjson::Value& EditorGameCondition::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("type", Type.c_str(), allocator);
    _json.AddMember("name", Name.c_str(), allocator);
    _json.AddMember("desc", Desc.c_str(), allocator);
    _json.AddMember( "title", Title.c_str(), allocator );
    return _json;
}

void EditorGameCondition::loadJson(const rapidjson::Value& value) {
    Type = value["type"].GetString();
    if (value.HasMember("name")) {
        Name = value["name"].GetString();
    }
    if( value.HasMember( "desc" ) ) {
        Desc = value["desc"].GetString();
    }
    if( value.HasMember( "title" ) ) {
        Title = value["title"].GetString();
    }
}

rapidjson::Value& EditorSpeech::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("content", Content.c_str(), allocator);
    _json.AddMember("duration", Duration, allocator);
    _json.AddMember( "interval", Interval, allocator );
    return _json;
}

void EditorSpeech::loadJson(const rapidjson::Value& value) {
    Content = value["content"].GetString();
    Duration = value["duration"].GetDouble();
    Interval = value["interval"].GetDouble();
}

rapidjson::Value& EditorTriggerMeta::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("is_repeated", this->IsRepeated, allocator);
    _json.AddMember("trigger_relation", this->TriggerRelation.c_str(), allocator);
    return _json;
}

void EditorTriggerMeta::loadJson(const rapidjson::Value& value) {
    this->IsRepeated = value["is_repeated"].GetBool();
    if (value.HasMember("trigger_relation")) {
        this->TriggerRelation = value["trigger_relation"].GetString();
    } else {
        this->TriggerRelation = "then";
    }
}

void EditorTriggerBase::loadJson(const rapidjson::Value& value) {
    this->TriggerType = value["trigger_type"].GetString();
}

rapidjson::Value& EditorTriggerBase::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("trigger_type", TriggerType.c_str(), allocator);
    return _json;
}

rapidjson::Value& EditorUnitTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("source_type", this->SourceType.c_str(), allocator);
    _json.AddMember("source_value", this->SourceValue.c_str(), allocator);
    _json.AddMember("unit_state", this->UnitState.c_str(), allocator);
//    if (this->PositionName != "") {
//        _json.AddMember("position_name", this->PositionName.c_str(), allocator);
//    }
    _json.AddMember("trigger_count", this->TriggerCount, allocator);
    return _json;
}

void EditorUnitTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    this->SourceType = value["source_type"].GetString();
    this->SourceValue = value["source_value"].GetString();
    this->UnitState = value["unit_state"].GetString();
    this->PositionName = value.HasMember("position_name") ? value["position_name"].GetString() : "";
    if (value.HasMember("trigger_count")) {
        this->TriggerCount = value["trigger_count"].GetInt();
    } else {
        this->TriggerCount = 1;
    }
}

rapidjson::Value& EditorUnitStayTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("source_type", this->SourceType.c_str(), allocator);
    _json.AddMember("source_value", this->SourceValue.c_str(), allocator);
    _json.AddMember("position_name", this->PositionName.c_str(), allocator);
    _json.AddMember("trigger_count", this->TriggerCount, allocator);
    _json.AddMember( "duration", this->Duration, allocator );
    return _json;
}

void EditorUnitStayTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    this->SourceType = value["source_type"].GetString();
    this->SourceValue = value["source_value"].GetString();
    this->PositionName = value["position_name"].GetString();
    this->Duration = (float)value["duration"].GetDouble();
    if (value.HasMember("trigger_count")) {
        this->TriggerCount = value["trigger_count"].GetInt();
    } else {
        this->TriggerCount = 1;
    }
}

rapidjson::Value& EditorEventTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("event_name", EventName.c_str(), allocator);
    _json.AddMember("event_state", EventState.c_str(), allocator);
    return _json;
}

void EditorEventTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    EventName = value["event_name"].GetString();
    EventState = value["event_state"].GetString();
}

rapidjson::Value& EditorGameTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("game_state", GameState.c_str(), allocator);
    return _json;
}

void EditorGameTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    this->GameState = value["game_state"].GetString();
}

rapidjson::Value& EditorCustomTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("trigger_name", TriggerName.c_str(), allocator);
    return _json;
}

void EditorCustomTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    TriggerName = value["trigger_name"].GetString();
}

rapidjson::Value& EditorVisionTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("source_type", SourceType.c_str(), allocator);
    _json.AddMember("source_value", SourceValue.c_str(), allocator);
    _json.AddMember("vision_state", VisionState.c_str(), allocator);
    return _json;
}

void EditorVisionTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    this->SourceType = value["source_type"].GetString();
    this->SourceValue = value["source_value"].GetString();
    VisionState = value["vision_state"].GetString();
}

//rapidjson::Value& EditorConversationChangeTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
//    EditorTriggerBase::toJson(allocator);
//    _json.AddMember("name", ConversationName.c_str(), allocator);
//    _json.AddMember("state", State.c_str(), allocator);
//    return _json;
//}
//
//void EditorConversationChangeTrigger::loadJson(const rapidjson::Value& value) {
//    EditorTriggerBase::loadJson(value);
//    ConversationName = value["name"].GetString();
//    State = value["state"].GetString();
//}

rapidjson::Value& EditorStoryChangeTrigger::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorTriggerBase::toJson(allocator);
    _json.AddMember("story_name", StoryName.c_str(), allocator);
    _json.AddMember("story_state", State.c_str(), allocator);
    return _json;
}

void EditorStoryChangeTrigger::loadJson(const rapidjson::Value& value) {
    EditorTriggerBase::loadJson(value);
    this->StoryName = value["story_name"].GetString();
    this->State = value["story_state"].GetString();
}

rapidjson::Value& EditorActionMeta::toJson(rapidjson::Document::AllocatorType& allocator) {
    int repeat = IsInfinite ? -1 : RepeatTimes;
    std::string value = Utils::stringFormat("%d,%.2f,%.2f", repeat, Delay, Interval);
    _json.SetString(value.c_str(), allocator);
    return _json;
}

void EditorActionMeta::loadJson(const rapidjson::Value& value) {
    std::string str = value.GetString();
    std::vector<std::string> ret;
    Utils::split(str, ret, ',');
    int repeat = atoi(ret[0].c_str());
    if (repeat < 0) {
        this->IsInfinite = true;
        this->RepeatTimes = 0;
    } else {
        this->IsInfinite = false;
        this->RepeatTimes = repeat;
    }
    this->Delay = atof(ret[1].c_str());
    this->Interval = atof(ret[2].c_str());
}

EditorActionBase::EditorActionBase(const std::string type): ActionType(type) {
    ActionMeta = std::shared_ptr<EditorActionMeta>(new EditorActionMeta());
}

rapidjson::Value& EditorActionBase::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson(allocator);
    _json.AddMember("meta", ActionMeta->toJson(allocator), allocator);
    _json.AddMember("action_type", ActionType.c_str(), allocator);
    return _json;
}

void EditorActionBase::loadJson(const rapidjson::Value& value) {
    this->ActionMeta->loadJson(value["meta"]);
    this->ActionType = value["action_type"].GetString();
}

rapidjson::Value& EditorCustomAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    _json.AddMember("action_name", ActionName.c_str(), allocator);
    return _json;
}

void EditorCustomAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    this->ActionName = value["action_name"].GetString();
}

rapidjson::Value& EditorUnitAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    _json.AddMember("source_type", this->SourceType.c_str(), allocator);
    _json.AddMember("source_value", this->SourceValue.c_str(), allocator);
    if (this->TypeChanged) {
        _json.AddMember("unit_type", UnitType.c_str(), allocator);
    }
    if (this->StateChanged) {
        _json.AddMember("unit_state", UnitState.c_str(), allocator);
    }
    _json.AddMember("show_hp", ShowHP, allocator);
    if (this->TagChanged) {
        _json.AddMember("tag_name", TagName.c_str(), allocator);
    }
    if (this->BuffChanged) {
        _json.AddMember("buff_name", BuffName.c_str(), allocator);
        _json.AddMember( "buff_type", BuffType.c_str(), allocator );
        _json.AddMember( "buff_params", BuffParams.c_str(), allocator );
    }
    if (this->PositionName != "") {
        _json.AddMember("position_name", PositionName.c_str(), allocator);
    }
    if (this->PositionTag != "") {
        _json.AddMember("position_tag", PositionTag.c_str(), allocator);
    }
    if (this->UnitCount > 0) {
        _json.AddMember("unit_count", UnitCount, allocator);
    }
    if (this->IsBoss) {
        _json.AddMember("is_boss", true, allocator);
    }
	if (this->UnPushable) {
		_json.AddMember("unpushable", true, allocator);
	}
    if (this->ClassName != "LogicUnit") {
        _json.AddMember("class_name", this->ClassName.c_str(), allocator);
    }
    if (this->CustomChange.length() > 0) {
        _json.AddMember("custom_change", CustomChange.c_str(), allocator);
    }
    _json.AddMember( "unit_level", this->UnitLevel, allocator );
    if (this->PopupType.length() > 0) {
        _json.AddMember("popup_type", PopupType.c_str(), allocator);
    }
    if( this->SkillOneLevel != 0 ) {
        _json.AddMember( "skill_1_level", SkillOneLevel, allocator );
    }
    if( this->SkillTwoLevel != 0 ) {
        _json.AddMember( "skill_2_level", SkillTwoLevel, allocator );
    }
    
    if( this->IsItemChanged ) {
        _json.AddMember( "item_get_or_lose", GetOrLoseItem, allocator );
        _json.AddMember( "item_name", ItemName.c_str(), allocator );
        _json.AddMember( "item_resource", ItemResource.c_str(), allocator );
    }
    
    return _json;
}

void EditorUnitAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    if (value.HasMember("state_changed")) {
        this->StateChanged = value["state_changed"].GetBool();
        this->UnitState = this->StateChanged ? value["unit_state"].GetString() : "";
    } else if (value.HasMember("unit_state")) {
        this->StateChanged = true;
        this->UnitState = value["unit_state"].GetString();
    } else {
        this->StateChanged = false;
        this->UnitState = "";
    }
    if (value.HasMember("type_changed")) {
        this->TypeChanged = value["type_changed"].GetBool();
        this->UnitType = this->TypeChanged ? value["unit_type"].GetString() : "";
    } else if (value.HasMember("unit_type")) {
        this->UnitType = value["unit_type"].GetString();
        this->TypeChanged = true;
    } else {
        this->TypeChanged = false;
        this->UnitType = "";
    }
    if (value.HasMember("show_hp")) {
        this->ShowHP = value["show_hp"].GetBool();
    }
    if (value.HasMember("tag_changed")) {
        this->TagChanged = value["tag_changed"].GetBool();
        this->TagName = this->TagChanged ? value["tag_name"].GetString() : "";
    } else if (value.HasMember("tag_name")) {
        this->TagName = value["tag_name"].GetString();
        this->TagChanged = true;
    } else {
        this->TagChanged = false;
        this->TagName = "";
    }
    if (value.HasMember("buff_name")) {
        this->BuffChanged = true;
        this->BuffName = value["buff_name"].GetString();
        this->BuffType = value["buff_type"].GetString();
        this->BuffParams = value["buff_params"].GetString();
        
    } else if (value.HasMember("buff_name")) {
        this->BuffName = value["buff_name"].GetString();
        this->BuffChanged = true;
    } else {
        this->BuffChanged = false;
        this->BuffName = "";
    }
    if (value.HasMember("is_boss")) {
        this->IsBoss = value["is_boss"].GetBool();
    } else {
        this->IsBoss = false;
    }
	if (value.HasMember("unpushable")) {
		this->UnPushable = value["unpushable"].GetBool();
	}
	else {
		this->UnPushable = false;
	}
    if (value.HasMember("class_name")) {
        this->ClassName = value["class_name"].GetString();
    } else {
        this->ClassName = "LogicUnit";
    }
    if (value.HasMember("custom_change")) {
        this->CustomChange = value["custom_change"].GetString();
    } else {
        this->CustomChange = "";
    }
    if( value.HasMember( "unit_level" ) ) {
        this->UnitLevel = value["unit_level"].GetInt();
    }
    else {
        this->UnitLevel = 1;
    }
    if (value.HasMember("popup_type")) {
        this->PopupType = value["popup_type"].GetString();
    } else {
        this->PopupType = "normal";
    }
    if( value.HasMember( "skill_1_level" ) ) {
        this->SkillOneLevel = value["skill_1_level"].GetInt();
    }
    else {
        this->SkillOneLevel = 0;
    }
    if( value.HasMember( "skill_2_level" ) ) {
        this->SkillTwoLevel = value["skill_2_level"].GetInt();
    }
    else {
        this->SkillTwoLevel = 0;
    }
    
    if( value.HasMember( "item_name" ) ) {
        this->IsItemChanged = true;
        this->GetOrLoseItem = value["item_get_or_lose"].GetBool();
        this->ItemName = value["item_name"].GetString();
        this->ItemResource = value["item_resource"].GetString();
    }
    else {
        this->IsItemChanged = false;
        this->GetOrLoseItem = true;
        this->ItemName = "";
        this->ItemResource = "";
    }
    
    this->UnitCount = value.HasMember("unit_count") ? value["unit_count"].GetInt() : 0;
    this->PositionName = value.HasMember("position_name") ? value["position_name"].GetString() : "";
    this->PositionTag = value.HasMember("position_tag") ? value["position_tag"].GetString() : "";
    this->SourceType = value["source_type"].GetString();
    this->SourceValue = value["source_value"].GetString();
}

rapidjson::Value& EditorTaskAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    _json.AddMember("task_name", TaskName.c_str(), allocator);
    _json.AddMember("task_state", TaskState.c_str(), allocator);
    if( TaskState == "task_progress" ) {
        _json.AddMember( "progress", TaskProgress, allocator );
    }
    return _json;
}

void EditorTaskAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    this->TaskName = value["task_name"].GetString();
    this->TaskState = value["task_state"].GetString();
    if( value.HasMember( "progress" ) ) {
        this->TaskProgress = (float)value["progress"].GetDouble();
    }
    else {
        this->TaskProgress = 0;
    }
}

rapidjson::Value& EditorGameAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    _json.AddMember("game_state", GameState.c_str(), allocator);
    return _json;
}

void EditorGameAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    this->GameState = value["game_state"].GetString();
}

rapidjson::Value& EditorEventAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    _json.AddMember("event_name", EventName.c_str(), allocator);
    _json.AddMember("event_state", EventState.c_str(), allocator);
    return _json;
}

void EditorEventAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    this->EventName = value["event_name"].GetString();
    this->EventState = value["event_state"].GetString();
}

rapidjson::Value& EditorVisionAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    _json.AddMember("source_type", this->SourceType.c_str(), allocator);
    _json.AddMember("source_value", this->SourceValue.c_str(), allocator);
    _json.AddMember("vision_state", VisionState.c_str(), allocator);
    return _json;
}

void EditorVisionAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    this->SourceType = value["source_type"].GetString();
    this->SourceValue = value["source_value"].GetString();
    this->VisionState = value["vision_state"].GetString();
}

rapidjson::Value& EditorWaveAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    rapidjson::Value waves;
    waves.SetArray();
    for (auto wave : Waves) {
        waves.PushBack(wave->toJson(allocator), allocator);
    }
    _json.AddMember("waves", waves, allocator);
    return _json;
}

void EditorWaveAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    const rapidjson::Value& waves  = value["waves"];
    assert(waves.IsArray());
    Waves.clear();
    for (int i = 0; i < waves.Size(); ++i) {
        auto wave = std::shared_ptr<EditorWave>(new EditorWave());
        wave->loadJson(waves[i]);
        Waves.push_back(wave);
    }
}

rapidjson::Value& EditorConversationAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson(allocator);
    rapidjson::Value speeches;
    speeches.SetArray();
    for (auto speech : Speeches) {
        speeches.PushBack(speech->toJson(allocator), allocator);
    }
    _json.AddMember("speeches", speeches, allocator);
    _json.AddMember("name", Name.c_str(), allocator);
    _json.AddMember( "source_type", SourceType.c_str(), allocator );
    _json.AddMember( "source_value", SourceValue.c_str(), allocator );
    _json.AddMember( "is_random_order", RandomOrder, allocator );
    _json.AddMember( "repeat_times", RepeatTimes, allocator );
    
    return _json;
}

void EditorConversationAction::loadJson(const rapidjson::Value& value) {
    EditorActionBase::loadJson(value);
    const rapidjson::Value& speeches = value["speeches"];
    assert(speeches.IsArray());
    Speeches.clear();
    for (int i = 0; i < speeches.Size(); ++i) {
        auto speech = std::shared_ptr<EditorSpeech>(new EditorSpeech());
        speech->loadJson(speeches[i]);
        Speeches.push_back(speech);
    }
    Name = value["name"].GetString();
    SourceType = value["source_type"].GetString();
    SourceValue = value["source_value"].GetString();
    RandomOrder = value["is_random_order"].GetBool();
    RepeatTimes = value["repeat_times"].GetInt();
}

EditorStory::EditorStory() :
LeftOrRight( true )
{
    
}

rapidjson::Value& EditorStory::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorBase::toJson( allocator );
    _json.AddMember( "left_or_right", LeftOrRight, allocator );
    _json.AddMember( "line", Line.c_str(), allocator );
    _json.AddMember( "speaker", SpeakerName.c_str(), allocator );
    return _json;
}

void EditorStory::loadJson(const rapidjson::Value& value) {
    LeftOrRight = value["left_or_right"].GetBool();
    Line = std::string( value["line"].GetString(), value["line"].GetStringLength() );
    SpeakerName = std::string( value["speaker"].GetString(), value["speaker"].GetStringLength() );
}

rapidjson::Value& EditorStoryAction::toJson(rapidjson::Document::AllocatorType& allocator) {
    EditorActionBase::toJson( allocator );
    rapidjson::Value stories;
    stories.SetArray();
    for( auto story : Stories ) {
        stories.PushBack( story->toJson( allocator), allocator );
    }
    _json.AddMember( "story_data", stories, allocator );
    _json.AddMember( "name", Name.c_str(), allocator );
    return _json;
}

void EditorStoryAction::loadJson( const rapidjson::Value& value ) {
    EditorActionBase::loadJson( value );
    Name = value["name"].GetString();
    const rapidjson::Value& stories = value["story_data"];
    assert( stories.IsArray() );
    Stories.clear();
    for( int i = 0; i < stories.Size(); i++ ) {
        auto story = std::shared_ptr<EditorStory>( new EditorStory() );
        story->loadJson( stories[i] );
        Stories.push_back( story );
    }
}

EditorEvent::EditorEvent() {
    TriggerMeta = std::shared_ptr<EditorTriggerMeta>(new EditorTriggerMeta());
    this->Enabled = true;
}

rapidjson::Value& EditorEvent::toJson(rapidjson::Document::AllocatorType& allocator) {
    _json.SetObject();
    _json.AddMember("name", this->Name.c_str(), allocator);
    _json.AddMember( "enabled", this->Enabled, allocator );
    _json.AddMember("trigger_meta", this->TriggerMeta->toJson(allocator), allocator);
    rapidjson::Value triggers;
    triggers.SetArray();
    for (int i = 0; i < Triggers.size(); i++) {
        triggers.PushBack(Triggers[i]->toJson(allocator), allocator);
    }
    _json.AddMember("triggers", triggers, allocator);
    rapidjson::Value actions;
    actions.SetArray();
    for (int i = 0; i < Actions.size(); i++) {
        actions.PushBack(Actions[i]->toJson(allocator), allocator);
    }
    _json.AddMember("actions", actions, allocator);
    return _json;
}

void EditorEvent::loadJson(const rapidjson::Value& value) {
    this->Name = value["name"].GetString();
    this->Enabled = value.HasMember( "enabled" ) ? value["enabled"].GetBool() : true;
    this->TriggerMeta->loadJson(value["trigger_meta"]);
    Triggers.clear();
    const rapidjson::Value& triggers = value["triggers"];
    if (!triggers.IsNull()) {
        assert(triggers.IsArray());
        for (int i = 0; i < triggers.Size(); ++i) {
            std::string triggerType = triggers[i]["trigger_type"].GetString();
            std::shared_ptr<EditorTriggerBase> trigger;
            if (triggerType == "unit_change") {
                trigger = std::shared_ptr<EditorUnitTrigger>(new EditorUnitTrigger());
            }
            else if( triggerType == "unit_stay" ) {
                trigger = std::shared_ptr<EditorUnitStayTrigger>( new EditorUnitStayTrigger() );
            }
            else if( triggerType == "story_change" ) {
                trigger = std::shared_ptr<EditorStoryChangeTrigger>( new EditorStoryChangeTrigger() );
            }
            else if (triggerType == "map_init") {
                trigger = std::shared_ptr<EditorMapInitTrigger>(new EditorMapInitTrigger());
            } else if (triggerType == "event_change") {
                trigger = std::shared_ptr<EditorEventTrigger>(new EditorEventTrigger());
            } else if (triggerType == "custom") {
                trigger = std::shared_ptr<EditorCustomTrigger>(new EditorCustomTrigger());
            } else if (triggerType == "game_change") {
                trigger = std::shared_ptr<EditorGameTrigger>(new EditorGameTrigger());
            } else if (triggerType == "vision_change") {
                trigger = std::shared_ptr<EditorVisionTrigger>(new EditorVisionTrigger());
            }
//            else if (triggerType == "conversation_change") {
//                trigger = std::shared_ptr<EditorConversationChangeTrigger>(new EditorConversationChangeTrigger());
//            }
            trigger->loadJson(triggers[i]);
            Triggers.push_back(trigger);
        }
    }
    Actions.clear();
    const rapidjson::Value& actions = value["actions"];
    if (!actions.IsNull()) {
        assert(actions.IsArray());
        for (int i = 0; i < actions.Size(); ++i) {
            std::string actionType = actions[i]["action_type"].GetString();
            std::shared_ptr<EditorActionBase> action;
            if (actionType == "unit_change") {
                action = std::shared_ptr<EditorUnitAction>(new EditorUnitAction());
            } else if (actionType == "game_change") {
                action = std::shared_ptr<EditorGameAction>(new EditorGameAction());
                
            } else if (actionType == "task_change") {
                action = std::shared_ptr<EditorTaskAction>(new EditorTaskAction());
                
            } else if (actionType == "custom") {
                action = std::shared_ptr<EditorActionBase>(new EditorCustomAction());
            } else if (actionType == "event_change") {
                action = std::shared_ptr<EditorEventAction>(new EditorEventAction());
            } else if (actionType == "vision_change") {
                action = std::shared_ptr<EditorVisionAction>(new EditorVisionAction());
            } else if (actionType == "wave_action") {
                action = std::shared_ptr<EditorWaveAction>(new EditorWaveAction());
            } else if (actionType == "conversation_action") {
                action = std::shared_ptr<EditorConversationAction>(new EditorConversationAction());
            } else if( actionType == "story_action" ) {
                action = std::shared_ptr<EditorStoryAction>( new EditorStoryAction() );
            }
            action->loadJson(actions[i]);
            Actions.push_back(action);
        }
    }
}


void EditorData::loadJson(const std::string& content) {
    loadJson(_doc.Parse<0>(content.c_str()));
}

std::string EditorData::getString() {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>> writer(buffer);
    toJson(_doc.GetAllocator());
    _doc.Accept(writer);
    return buffer.GetString();
}

void EditorData::loadJson(const rapidjson::Value& value) {
//    Tasks.clear();
    Positions.clear();
    Events.clear();
    Conditions.clear();
    if (!value.IsNull()) {
//        const rapidjson::Value& tasks = value["tasks"];
//        if (!tasks.IsNull()) {
//            assert(tasks.IsArray());
//            for (int i = 0; i < tasks.Size(); ++i) {
//                auto taskPtr = std::shared_ptr<EditorTask>(new EditorTask());
//                taskPtr->loadJson(tasks[i]);
//                Tasks.push_back(taskPtr);
//            }
//        }
        
        const rapidjson::Value& positions = value["positions"];
        if (!positions.IsNull()) {
            assert(positions.IsArray());
            for (int i = 0; i < positions.Size(); ++i) {
                auto positionPtr = std::shared_ptr<EditorPosition>(new EditorPosition());
                positionPtr->loadJson(positions[i]);
                Positions.push_back(positionPtr);
            }
        }
        
        const rapidjson::Value& events = value["events"];
        if (!events.IsNull()) {
            assert(events.IsArray());
            for (int i = 0; i < events.Size(); i++) {
                auto eventPtr = std::shared_ptr<EditorEvent>(new EditorEvent());
                eventPtr->loadJson(events[i]);
                Events.push_back(eventPtr);
            }
		}
        const rapidjson::Value& conditions = value["tasks"];
        if (!conditions.IsNull()) {
            assert(conditions.IsArray());
            for (int i = 0; i < conditions.Size(); i++) {
                auto conditionPtr = std::shared_ptr<EditorGameCondition>(new EditorGameCondition());
                conditionPtr->loadJson(conditions[i]);
                Conditions.push_back(conditionPtr);
            }
        }
    }
}

rapidjson::Value& EditorData::toJson(rapidjson::Document::AllocatorType& allocator) {
    _doc.RemoveMember("tasks");
    _doc.RemoveMember("positions");
    _doc.RemoveMember("events");
    _doc.RemoveMember("units");
    _doc.RemoveMember("conditions");
    rapidjson::Value tasks;
    tasks.SetArray();
//    for (int i = 0; i < Tasks.size(); i++) {
//        tasks.PushBack(Tasks[i]->toJson(allocator), allocator);
//    }
//    _doc.AddMember("tasks", tasks, allocator);
    rapidjson::Value positions;
    positions.SetArray();
    for (int i = 0; i < Positions.size(); i++) {
        positions.PushBack(Positions[i]->toJson(allocator), allocator);
    }
    _doc.AddMember("positions", positions, allocator);
    rapidjson::Value events;
    events.SetArray();
    for (int i = 0; i < Events.size(); i++) {
        events.PushBack(Events[i]->toJson(allocator), allocator);
    }
    _doc.AddMember("events", events, allocator);
    std::set<std::string> unitset;
    for (auto event : Events) {
        for (auto action : event->Actions) {
            if (action->ActionType == "unit_change") {
                std::shared_ptr<EditorUnitAction> unitAction = std::dynamic_pointer_cast<EditorUnitAction>(action);
                if (unitAction->SourceType == "name_source" && unitAction->StateChanged && unitAction->UnitState == "unit_appear" && unitAction->UnitType != "item") {
                    unitset.insert(unitAction->SourceValue);
                }
            } else if (action->ActionType == "waves_appear") {
                std::shared_ptr<EditorWaveAction> waveAction = std::dynamic_pointer_cast<EditorWaveAction>(action);
                for (auto wave : waveAction->Waves) {
                    for (auto unit : wave->Units) {
                        unitset.insert(unit->Name);
                    }
                }
            }
        }
    }
    rapidjson::Value units;
    units.SetArray();
    for (auto it = unitset.begin(); it != unitset.end(); ++it) {
        rapidjson::Value name;
        name.SetString(it->c_str(), allocator);
        units.PushBack(name, allocator);
    }
    _doc.AddMember("units", units, allocator);
    if (Conditions.size() > 0) {
        rapidjson::Value conditions;
        conditions.SetArray();
        for (auto cond : Conditions) {
            conditions.PushBack(cond->toJson(allocator), allocator);
        }
        _doc.AddMember("tasks", conditions, allocator);
    }
    return _doc;
}

EditorContext::EditorContext() {
    this->reset();
}

void EditorContext::moveBack() {
    Pipeline.erase(Pipeline.begin() + CurrentStep);
    CurrentStep -= 1;
}

void EditorContext::reset() {
    Pipeline.clear();
    CurrentStep = 0;
    Event = std::shared_ptr<EditorEvent>(new EditorEvent());
    Position = std::shared_ptr<EditorPosition>(new EditorPosition());
    ActionMeta = std::shared_ptr<EditorActionMeta>(new EditorActionMeta());
    TouchStartPosition = cocos2d::Point::ZERO;
}

EditorOperation EditorContext::currentOperation() {
    if (CurrentStep < 0 || CurrentStep >= Pipeline.size()) {
        return EditorOperation::None;
    }
    return Pipeline[CurrentStep];
}