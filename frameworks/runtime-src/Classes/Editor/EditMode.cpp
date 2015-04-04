//
//  EditMode.cpp
//  Boids
//
//  Created by Xin Xu on 11/6/14.
//
//

#include "EditMode.h"
#include "Terrain.h"
#include "EditorHelper.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Utils.h"
#include "EditorPositionNode.h"
#include "LuaFunc.h"
#include "BETypes.h"
#include <math.h>
//#include "AI.h"
#include "MapDebugger.h"
#include "../constant/DebugMacros.h"

using namespace cocos2d;
using namespace cocostudio;

const Color4F kColorDrawingPosition = Color4F(0, 1, 0, 0.5);

EditMode *EditMode::_instance = nullptr;

EditMode *EditMode::getInstance() {
    if (_instance == nullptr) {
        _instance = new EditMode();
    }
    return _instance;
}

EditMode::EditMode(): _drawNode(nullptr), _map(nullptr), _touchListener(nullptr), _draftDrawNode(nullptr), _positionLayer(nullptr), _inSubContext(false), _inDragMode(false) {
}

void EditMode::enterMain(std::function<void()> onExit) {
    _onExit = onExit;
    _mainRootNode = CSLoader::createNode("Main.csb");
    _mainUI = std::shared_ptr<BEMainUI>(new BEMainUI(_mainRootNode, CC_CALLBACK_3(EditMode::onMainCommand, this)));
    runSceneWithNode(_mainRootNode);
}

void EditMode::backMain() {
    _mainRootNode = CSLoader::createNode("Main.csb");
    _mainUI = std::shared_ptr<BEMainUI>(new BEMainUI(_mainRootNode, CC_CALLBACK_3(EditMode::onMainCommand, this)));
    runSceneWithNode(_mainRootNode);
}

void EditMode::enterEdit(const std::string& mapFolder) {
    loadUnits();
    _mapData = std::shared_ptr<MapData>(new MapData(mapFolder));
//    _mapData->loadImagesToCache();
    _map = _mapData->generateTiledMapWithFlags(7);
    loadVisionObjects();
    _data.loadJson(_mapData->getMetaData());
    fetchTags();
    fetchConversations();
    fetchPositionTags();
	if (PAINT_MESH)
	{
		std::set<float> collides;
		collides.insert(PAINT_MESH);
		Terrain::getInstance()->parseMap(_mapData->getMapData(), collides);
		Terrain::getInstance()->paintDebugLayer(_map);
	}
    _draftDrawNode = DrawNode::create();
    _map->addChild(_draftDrawNode);
    _positionLayer = Layer::create();
    _map->addChild(_positionLayer);
    for (auto pos : _data.Positions) {
        createPositionOnMap(pos);
    }
    _editRootNode = CSLoader::createNode("EditMode.csb");
    _editRootNode->addChild(_map, -1);
    _editUI = std::shared_ptr<BEEditorMainUI>(new BEEditorMainUI(_editRootNode, CC_CALLBACK_3(EditMode::onPopupEvent, this), CC_CALLBACK_2(EditMode::onMenuCommand, this)));
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = CC_CALLBACK_2(EditMode::onTouchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(EditMode::onTouchMoved, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(EditMode::onTouchEnded, this);
    _touchListener->onTouchCancelled = CC_CALLBACK_2(EditMode::onTouchEnded, this);
    _map->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, _map);
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_1(EditMode::onKeyPressed, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_1(EditMode::onKeyReleased, this);
    _map->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, _map);
    runSceneWithNode(_editRootNode);
}

void EditMode::leaveEdit() {
    _mapData->dumpMetaData(_data.getString());
    _mapData->removeImagesFromCache();
    _mapData = nullptr;
    _editUI = nullptr;
    backMain();
}

void EditMode::loadUnits() {
    _units.clear();
    auto jsonStr = FileUtils::getInstance()->getStringFromFile("unit.json");
    rapidjson::Document doc;
    doc.Parse<0>(jsonStr.c_str());
    for (auto it = doc.MemberonBegin(); it != doc.MemberonEnd(); ++it) {
        auto name = it->name.GetString();
        _units.push_back(name);
    }
    jsonStr = FileUtils::getInstance()->getStringFromFile("item.json");
    doc.Parse<0>(jsonStr.c_str());
    for (auto it = doc.MemberonBegin(); it != doc.MemberonEnd(); ++it) {
        auto name = it->name.GetString();
        _units.push_back(name);
    }
}

void EditMode::loadVisionObjects() {
    _visionObjects.clear();
    _visionTags.clear();
    std::set<std::string> tags;
    auto visionObjectGroup = _map->getObjectGroup("vision");
    for (auto object : visionObjectGroup->getObjects()) {
        auto vm = object.asValueMap();
        auto name = vm["name"].asString();
        auto type = vm["type"].asString();
        if (name.length() && type.length()) {
            _visionObjects.push_back(name);
        }
        auto tag = vm["tag"].asString();
        if (tag.length()) {
            tags.insert(tag);
        }
    }
    
    auto blockObjectGroup = _map->getObjectGroup("block");
    for (auto object : blockObjectGroup->getObjects()) {
        auto vm = object.asValueMap();
        auto name = vm["name"].asString();
        if (name.length() ) {
            _visionObjects.push_back(name);
        }
        auto tag = vm["tag"].asString();
        if (tag.length()) {
            tags.insert(tag);
        }
    }
    
    _visionTags.assign(tags.begin(), tags.end());
}

std::vector<std::pair<std::string, std::string>> EditMode::getActionSources(bool hasTriggerUnit) {
    std::vector<std::pair<std::string, std::string>> ret;
    auto unitTypeNames = EditorTypeManager::getInstance()->getUnitType()->getTypeVector();
    for (auto name : unitTypeNames) {
        ret.push_back(std::make_pair("type_source", name));
    }
    if (hasTriggerUnit) {
        ret.push_back(std::make_pair("trigger_source", "trigger_unit"));
    }
    for (auto tag : _tags) {
        ret.push_back(std::make_pair("tag_source", tag));
    }
    for (auto name : _units) {
        ret.push_back(std::make_pair("name_source", name));
    }
    return ret;
}

std::vector<std::pair<std::string, std::string>> EditMode::getTriggerSources() {
    std::vector<std::pair<std::string, std::string>> ret;
    auto unitTypeNames = EditorTypeManager::getInstance()->getUnitType()->getTypeVector();
    for (auto name : unitTypeNames) {
        ret.push_back(std::make_pair("type_source", name));
    }
    for (auto tag : _tags) {
        ret.push_back(std::make_pair("tag_source", tag));
    }
    for (auto name : _units) {
        ret.push_back(std::make_pair("name_source", name));
    }
    return ret;
}

std::vector<std::pair<std::string, std::string>> EditMode::getConversationSources() {
    std::vector<std::pair<std::string, std::string>> ret;
    for (auto tag : _tags) {
        ret.push_back( std::make_pair("tag_source", tag) );
    }
    for (auto name : _units) {
        ret.push_back( std::make_pair("name_source", name) );
    }
    ret.push_back(std::make_pair("player_source", "player"));
    return ret;
}

std::vector<std::pair<std::string, std::string>> EditMode::getVisionSources() {
    std::vector<std::pair<std::string, std::string>> ret;
    for (auto name : _visionObjects) {
        ret.push_back( std::make_pair("name_source", name) );
    }
    for (auto tag : _visionTags) {
        ret.push_back(std::make_pair("tag_source", tag));
    }
    
    return ret;
}

std::vector<EditorEventPtr> EditMode::getEditableEvents() {
    std::vector<EditorEventPtr> ret;
    for (auto event : _data.Events) {
        for (auto action : event->Actions) {
            if (action->ActionType == "unit_change") {
                ret.push_back(event);
                break;
            }
        }
    }
    return ret;
}

void EditMode::fetchTags() {
    _tags.clear();
    std::set<std::string> tags;
    for (auto event : _data.Events) {
        for (auto action : event->Actions) {
            if (action->ActionType == "unit_change") {
                auto unitAction = static_pointer_cast<EditorUnitAction>(action);
                if (unitAction->TagChanged) {
                    std::vector<std::string> names;
                    Utils::split(unitAction->TagName, names, ',');
                    for (auto name : names) {
                        tags.insert(name);
                    }
                }
            }
        }
    }
    _tags.assign(tags.begin(), tags.end());
}

void EditMode::fetchConversations() {
    _conversations.clear();
    for (auto event : _data.Events) {
        for (auto action : event->Actions) {
            if (action->ActionType == "conversation_action") {
                auto conversationAction = static_pointer_cast<EditorConversationAction>(action);
                _conversations.push_back(conversationAction->Name);
            }
        }
    }
}

void EditMode::fetchPositionTags() {
    _positionTags.clear();
    std::set<std::string> tags;
    for (auto position : _data.Positions) {
        if (position->Tag.length() > 0) {
            tags.insert(position->Tag);
        }
    }
    _positionTags.assign(tags.begin(), tags.end());
}

void EditMode::createPositionOnMap(EditorPositionPtr pos) {
    auto node = EditorPositionNode::create(pos, kColorDrawingPosition);
    _positionLayer->addChild(node);
}

EditorPositionPtr EditMode::getPositionForTouch(Touch *touch) {
    auto touchPos = _map->convertTouchToNodeSpace(touch);
    for (auto pos : _data.Positions) {
        if (pos->Rect.containsPoint(touchPos)) {
            return pos;
        }
    }
    return nullptr;
}

void EditMode::setMapPosition(Point position) {
    /*float minX = Director::getInstance()->getWinSize().width - _map->getContentSize().width;
    float minY = Director::getInstance()->getWinSize().height - _map->getContentSize().height;
    float x = position.x < minX ? minX : (position.x < 0 ? position.x : 0);
    float y = position.y < minY ? minY : (position.y < 0 ? position.y : 0);
    _map->setPosition(x, y);*/
	//modified by weiyuemin, allow move outside map
	_map->setPosition(position.x, position.y);
}

void EditMode::deleteEventByIndexes(std::vector<int>& indexes) {
    if (indexes.size() == 0) {
        return;
    }
    std::sort(indexes.begin(), indexes.end());
    std::vector<EditorEventPtr> ret;
    int k = 0;
    for (int i = 0; i < _data.Events.size(); ++i) {
        if (i < indexes[k] || k >= indexes.size()) {
            ret.push_back(_data.Events[i]);
        } else {
            ++k;
        }
    }
    _data.Events = ret;
    fetchTags();
}

void EditMode::deletePositionByName(const std::string& name) {
    for (auto it = _data.Positions.begin(); it != _data.Positions.end(); ++it) {
        if ((*it)->Name == name) {
            _data.Positions.erase(it);
            break;
        }
    }
    fetchPositionTags();
    _mapData->dumpMetaData(_data.getString());
}

void EditMode::runSceneWithNode(Node *node) {
    if (node->getParent()) {
        node->removeFromParent();
    }
    auto layer = Layer::create();
    layer->addChild(node);
    auto scene = Scene::create();
    scene->addChild(layer);
    if (Director::getInstance()->getRunningScene()) {
        Director::getInstance()->replaceScene(scene);
    } else {
        Director::getInstance()->runWithScene(scene);
    }
}

void EditMode::onMenuCommand(EditorCommandType ct, Ref *sender) {
    auto btn = (ui::Button *)sender;
    if (ct == EditorCommandType::NewEvent) {
        if (_context.currentOperation() == EditorOperation::None) {
            btn->setTitleText("Cancel Creating");
            _context.addOperation(EditorOperation::InputEventName);
            moveStart();
        } else {
            btn->setTitleText("Create Event");
            _context.reset();
            _editUI->reset();
        }
    } else if (ct == EditorCommandType::NewPositon) {
        if (_context.currentOperation() == EditorOperation::None) {
            btn->setTitleText("Cancel Creating");
            _context.addOperation(EditorOperation::ChoosingRect);
            moveStart();
        } else {
            btn->setTitleText("Create Position");
            _context.reset();
            _editUI->reset();
        }
    } else if (ct == EditorCommandType::ListEvent) {
        if (_context.currentOperation() == EditorOperation::None) {
            _context.addOperation(EditorOperation::ListEvent);
            moveStart();
        } else {
            _editUI->reset();
            _context.reset();
        }
    } else if (ct == EditorCommandType::Exit) {
        leaveEdit();
    } else if (ct == EditorCommandType::EditCondition) {
        if (_context.currentOperation() == EditorOperation::None) {
            _context.addOperation(EditorOperation::EditCondition);
            moveStart();
        } else {
            _editUI->reset();
            _context.reset();
        }
    }
}

void EditMode::onPopupEvent(EditorPopupEventType et, BEUIBase *popup, Ref *sender) {
    auto popupName = popup->getRootName();
    auto ctx = currentContext();
    auto op = ctx->currentOperation();
    if (et == EditorPopupEventType::Cancel) {
        moveBack();
        return;
    } else if (et == EditorPopupEventType::OK) {
        if (op == EditorOperation::InputPositionName) {
            BEUIInputName *ui = (BEUIInputName *)popup;
            ctx->Position->Name = ui->getInputName();
            ctx->Position->Tag = ui->getInputTag();
            ctx->addOperation(EditorOperation::CreatePosition);
        } else if (op == EditorOperation::SetCustomTrigger) {
            BEUIInputName *ui = (BEUIInputName *)popup;
            auto customTrigger = EditorCustomTriggerPtr(new EditorCustomTrigger());
            customTrigger->TriggerName = ui->getInputName();
            ctx->Event->Triggers.push_back(customTrigger);
            ctx->addOperation(EditorOperation::SetTrigger);
        } else if (op == EditorOperation::InputEventName) {
            BEUIInputName *ui = (BEUIInputName *)popup;
            ctx->Event->Name = ui->getInputName();
            ctx->addOperation(EditorOperation::SetTrigger);
        } else if (op == EditorOperation::SetTrigger) {
            BEUITriggerOptions *ui = (BEUITriggerOptions *)popup;
            if (ctx->Event->Triggers.size() == 0) {
                ctx->Event->TriggerMeta->IsRepeated = ui->isRepeat();
                ctx->Event->TriggerMeta->TriggerRelation = ui->getRelationType();
                auto triggerType = ui->getType();
                if (triggerType == "map_init") {
                    ctx->Event->Triggers.push_back(EditorMapInitTriggerPtr(new EditorMapInitTrigger()));
                }
            }
            if (ctx->Event->Triggers.size() > 0) {
                ctx->addOperation(EditorOperation::SetAction);
            } else {
                return;
            }
        } else if (op == EditorOperation::SetAction) {
            if (ctx->Event->Actions.size() > 0) {
                ctx->addOperation(EditorOperation::CreateEvent);
            } else {
                return;
            }
        } else if (op == EditorOperation::SetUnitTrigger) {
            ctx->Event->Triggers.push_back(_editUI->unitTriggerUI->getTrigger());
            ctx->addOperation(EditorOperation::SetTrigger);
        }
        else if( op == EditorOperation::SetUnitStayTrigger ) {
            ctx->Event->Triggers.push_back(_editUI->unitstayTriggerUI->getTrigger());
            ctx->addOperation(EditorOperation::SetTrigger);
        }
        else if (op == EditorOperation::SetEventTrigger) {
            auto trigger = EditorEventTriggerPtr(new EditorEventTrigger());
            trigger->EventName = _editUI->eventChangeUI->getEventName();
            trigger->EventState = _editUI->eventChangeUI->getEventState();
            ctx->Event->Triggers.push_back(trigger);
            ctx->addOperation(EditorOperation::SetTrigger);
        } else if (op == EditorOperation::SetGameTrigger) {
            auto trigger = _editUI->gameStateChangeUI->getTrigger();
            ctx->Event->Triggers.push_back(trigger);
            ctx->addOperation(EditorOperation::SetTrigger);
        } else if (op == EditorOperation::SetVisionTrigger) {
            auto trigger = _editUI->visionChangeUI->getTrigger();
            ctx->Event->Triggers.push_back(trigger);
            ctx->addOperation(EditorOperation::SetTrigger);
        } else if (op == EditorOperation::ListEvent) {
            deleteEventByIndexes(_editUI->eventListUI->getDeletedEventIndexs());
        } else if (op == EditorOperation::SetUnitAction) {
            auto actions = _editUI->unitActionUI->getActions();
            for (auto action : actions) {
                action->ActionMeta = ctx->ActionMeta;
                ctx->Event->Actions.push_back(action);
            }
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::SetMissionAction) {
            auto action = _editUI->taskStateChangeUI->getAction();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(action);
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::SetGameAction) {
            auto action = _editUI->gameStateChangeUI->getAction();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(_editUI->gameStateChangeUI->getAction());
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::SetEventAction) {
            auto action = EditorEventActionPtr(new EditorEventAction());
            action->EventName = _editUI->eventChangeUI->getEventName();
            action->EventState = _editUI->eventChangeUI->getEventState();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(action);
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::SetCustomAction) {
            BEUIInputName *ui = (BEUIInputName *)popup;
            auto action = EditorCustomActionPtr(new EditorCustomAction());
            action->ActionName = ui->getInputName();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(action);
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::SetVisionAction) {
            auto action = _editUI->visionChangeUI->getAction();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(action);
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::SetWaveAction) {
            auto action = _editUI->waveActionUI->getAction();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(action);
            ctx->addOperation(EditorOperation::SetAction);
        } else if (op == EditorOperation::EditCondition) {
            _data.Conditions = _editUI->gameConditionUI->getConditions();
        } else if (op == EditorOperation::SetConverSationAction) {
            auto action = _editUI->conversationActionUI->getAction();
            action->ActionMeta = ctx->ActionMeta;
            ctx->Event->Actions.push_back(action);
            ctx->addOperation(EditorOperation::SetAction);
        }
//        else if (op == EditorOperation::SetConverSationTrigger) {
//            auto trigger = _editUI->conversationChangeUI->getTrigger();
//            ctx->Event->Triggers.push_back(trigger);
//            ctx->addOperation(EditorOperation::SetTrigger);
//        }
        else if (op == EditorOperation::EditEvent) {
            _editUI->unitActionUI->saveEvent();
        }
        moveOn();
    } else if (et == EditorPopupEventType::NewPosition) {
        _inSubContext = true;
        _preUI = popup;
        _preSender = sender;
        _subContext.addOperation(EditorOperation::ChoosingRect);
        moveStart();
    } else if (et == EditorPopupEventType::SelectPosition) {
        _inSubContext = true;
        _preUI = popup;
        _preSender = sender;
        _subContext.addOperation(EditorOperation::ChossingPosition);
        moveStart();
    } else if (et == EditorPopupEventType::EditEvent) {
        ctx->Event = _editUI->eventListUI->getCurrentEvent();
        ctx->addOperation(EditorOperation::EditEvent);
        moveOn();
    } else if (et == EditorPopupEventType::AnotherTrigger) {
        BEUITriggerOptions *ui = (BEUITriggerOptions *)popup;
        auto triggerType = ui->getType();
        if (ctx->Event->Triggers.size() == 0) {
            ctx->Event->TriggerMeta->IsRepeated = ui->isRepeat();
        }
        if (triggerType == "map_init") {
            ctx->Event->Triggers.push_back(EditorMapInitTriggerPtr(new EditorMapInitTrigger()));
            ctx->addOperation(EditorOperation::SetAction);
        } else if (triggerType == "unit_change") {
            ctx->addOperation(EditorOperation::SetUnitTrigger);
        }
        else if( triggerType == "unit_stay" ) {
            ctx->addOperation(EditorOperation::SetUnitStayTrigger);
        }
        else if (triggerType == "event_change") {
            ctx->addOperation(EditorOperation::SetEventTrigger);
        } else if (triggerType == "custom") {
            ctx->addOperation(EditorOperation::SetCustomTrigger);
        } else if (triggerType == "game_change") {
            ctx->addOperation(EditorOperation::SetGameTrigger);
        } else if (triggerType == "vision_change") {
            ctx->addOperation(EditorOperation::SetVisionTrigger);
        }
        moveOn();
    } else if (et == EditorPopupEventType::AnotherAction) {
        BEUIActionOptions *ui = (BEUIActionOptions *)popup;
        auto type = ui->getType();
        ctx->ActionMeta = ui->getMeta();
        if (type == "unit_change") {
            ctx->addOperation(EditorOperation::SetUnitAction);
        } else if (type == "game_change") {
            ctx->addOperation(EditorOperation::SetGameAction);
        } else if (type == "task_change") {
            ctx->addOperation(EditorOperation::SetMissionAction);
        } else if (type == "custom") {
            ctx->addOperation(EditorOperation::SetCustomAction);
        } else if (type == "event_change") {
            ctx->addOperation(EditorOperation::SetEventAction);
        } else if (type == "vision_change") {
            ctx->addOperation(EditorOperation::SetVisionAction);
        } else if (type == "wave_action") {
            ctx->addOperation(EditorOperation::SetWaveAction);
        } else if (type == "conversation_action") {
            ctx->addOperation(EditorOperation::SetConverSationAction);
        }
        moveOn();
    }
}

void EditMode::onMainCommand(MainCommandType ct, const std::string& mapFolder, Ref *sender) {
    _mainUI->setVisible(false);
    if (ct == MainCommandType::PlayMap) {
//        LuaFunc::runBattleSceneWithMapPath(mapFolder);
    } else if (ct == MainCommandType::EditMap) {
        enterEdit(mapFolder);
    } else if (ct == MainCommandType::Exit) {
        _onExit();
    }
}

#pragma mark -- touch event --

void EditMode::onStep(int step) {
    _editUI->hideAllPopups();
    auto ctx = currentContext();
    if (step < 0 || step >= ctx->Pipeline.size()) {
        if (_inSubContext) {
            if (ctx->Position->Name.length()) {
                _preUI->didGetPosition(ctx->Position, _preSender);
            }
            _preUI->setVisible(true);
            _preUI = nullptr;
            _preSender = nullptr;
            _inSubContext = false;
        } else {
            _editUI->reset();
            _mapData->dumpMetaData(_data.getString());
        }
        ctx->reset();
        return;
    }
    auto op = ctx->currentOperation();
    switch (op) {
        case EditorOperation::ChoosingRect: {
            _draftDrawNode->clear();
            break;
        }
        case EditorOperation::InputEventName: {
            _editUI->inputNameUI->setVisible(true);
            _editUI->inputNameUI->reset();
            _editUI->inputNameUI->setShowTagInput(false);
            break;
        }
        case EditorOperation::SetCustomTrigger: {
            _editUI->inputNameUI->setVisible(true);
            _editUI->inputNameUI->reset();
            _editUI->inputNameUI->setShowTagInput(false);
            break;
        }
        case EditorOperation::SetCustomAction: {
            _editUI->inputNameUI->setVisible(true);
            _editUI->inputNameUI->reset();
            _editUI->inputNameUI->setShowTagInput(false);
            break;
        }
        case EditorOperation::InputPositionName: {
            _editUI->inputNameUI->setVisible(true);
            _editUI->inputNameUI->reset();
            _editUI->inputNameUI->setShowTagInput(true);
            break;
        }
        case EditorOperation::CreatePosition: {
            _draftDrawNode->clear();
            createPositionOnMap(ctx->Position);
            _data.Positions.push_back(ctx->Position);
            fetchPositionTags();
            moveOn();
            break;
        }
        case EditorOperation::SetTrigger: {
            _editUI->triggerOptionsUI->setVisible(true);
            _editUI->triggerOptionsUI->reset();
            bool showMetaUI = ctx->Event->Triggers.size() == 0;
            _editUI->triggerOptionsUI->setMetaUIVisible(showMetaUI);
            break;
        }
        case EditorOperation::SetAction: {
            _editUI->actionOptionsUI->setVisible(true);
            _editUI->actionOptionsUI->reset();
            break;
        }
        case EditorOperation::SetUnitTrigger: {
            _editUI->unitTriggerUI->setVisible(true);
            _editUI->unitTriggerUI->reset();
            _editUI->unitTriggerUI->loadSourceList(getTriggerSources());
            break;
        }
        case EditorOperation::SetUnitStayTrigger: {
            _editUI->unitstayTriggerUI->setVisible(true);
            _editUI->unitstayTriggerUI->reset();
            _editUI->unitstayTriggerUI->loadSourceList(getTriggerSources());
            break;
        }
        case EditorOperation::SetEventTrigger: {
            _editUI->eventChangeUI->setVisible(true);
            _editUI->eventChangeUI->reset();
            _editUI->eventChangeUI->loadEventList(_data.Events);
            _editUI->eventChangeUI->setIsTrigger(true);
            break;
        }
        case EditorOperation::SetMissionAction: {
            _editUI->taskStateChangeUI->setVisible(true);
            _editUI->taskStateChangeUI->reset();
            _editUI->taskStateChangeUI->loadTaskList(_data.Conditions);
            break;
        }
        case EditorOperation::SetGameTrigger:
        case EditorOperation::SetGameAction: {
            _editUI->gameStateChangeUI->setVisible(true);
            _editUI->gameStateChangeUI->reset();
            break;
        }
        case EditorOperation::SetUnitAction: {
            _editUI->unitActionUI->setVisible(true);
            _editUI->unitActionUI->reset();
            auto lastTrigger = ctx->Event->Triggers.back();
            bool hasTriggerUnit = lastTrigger->TriggerType == "unit_change";
            _editUI->unitActionUI->loadSourceList(getActionSources(hasTriggerUnit));
            _editUI->unitActionUI->loadPositionGroups(_positionTags);
            break;
        }
        case EditorOperation::SetEventAction: {
            _editUI->eventChangeUI->setVisible(true);
            _editUI->eventChangeUI->reset();
            _editUI->eventChangeUI->loadEventList(_data.Events);
            _editUI->eventChangeUI->setIsTrigger(false);
            break;
        }
        case EditorOperation::CreateEvent: {
            _data.Events.push_back(ctx->Event);
            fetchTags();
            fetchConversations();
            moveOn();
            break;
        }
        case EditorOperation::ChossingPosition: {
            break;
        }
        case EditorOperation::ListEvent: {
            _editUI->eventListUI->setVisible(true);
            _editUI->eventListUI->reset();
            _editUI->eventListUI->loadEventList(this->getEditableEvents());
            break;
        }
        case EditorOperation::SetVisionTrigger:
        case EditorOperation::SetVisionAction: {
            _editUI->visionChangeUI->setVisible(true);
            _editUI->visionChangeUI->reset();
            _editUI->visionChangeUI->loadVisionObjects(this->getVisionSources());
            break;
        }
        case EditorOperation::SetWaveAction: {
            _editUI->waveActionUI->setVisible(true);
            _editUI->waveActionUI->reset();
            _editUI->waveActionUI->loadUnitList(_units);
            _editUI->waveActionUI->loadPositionGroups(_positionTags);
            break;
        }
        case EditorOperation::EditCondition: {
            _editUI->gameConditionUI->setVisible(true);
            _editUI->gameConditionUI->reset();
            _editUI->gameConditionUI->loadConditions(_data.Conditions);
            break;
        }
        case EditorOperation::SetConverSationAction: {
            _editUI->conversationActionUI->setVisible(true);
            _editUI->conversationActionUI->reset();
            _editUI->conversationActionUI->loadConversationSource(this->getConversationSources());
            break;
        }
//        case EditorOperation::SetConverSationTrigger: {
//            _editUI->conversationChangeUI->setVisible(true);
//            _editUI->conversationChangeUI->reset();
//            _editUI->conversationChangeUI->loadConversations(_conversations);
//            break;
//        }
        case EditorOperation::EditEvent: {
            _editUI->unitActionUI->setVisible(true);
            _editUI->unitActionUI->reset();
            _editUI->unitActionUI->loadEvent(ctx->Event);
            _editUI->unitActionUI->loadPositionGroups(_positionTags);
            break;
        }
        default:
            break;
    }
}

void EditMode::moveOn() {
    auto ctx = currentContext();
    ctx->CurrentStep += 1;
    onStep(ctx->CurrentStep);
}

void EditMode::moveBack() {
    auto ctx = currentContext();
    ctx->moveBack();
    onStep(ctx->CurrentStep);
}

void EditMode::moveStart() {
    auto ctx = currentContext();
    ctx->CurrentStep = 0;
    onStep(ctx->CurrentStep);
}

#pragma mark -- touch event --

void EditMode::onBlankPositionTouched(cocos2d::Point pt)
{
	map_dbg.onTouchEvent(pt);
}

bool EditMode::onTouchBegan(Touch *touch, Event *event) {
    auto ctx = currentContext();
    if (ctx->currentOperation() == EditorOperation::ChoosingRect && !_inDragMode) {
        ctx->TouchStartPosition = _map->convertTouchToNodeSpace(touch);
    } else if (ctx->currentOperation() == EditorOperation::ChossingPosition) {
        auto pos = getPositionForTouch(touch);
        if (pos != nullptr) {
            ctx->Position->Name = pos->Name;
            ctx->Position->Rect = pos->Rect;
            moveOn();
            return false;
        } else {
            return true;
        }
    } else if (ctx->currentOperation() == EditorOperation::None) {
        auto point = _positionLayer->convertTouchToNodeSpace(touch);
        EditorPositionNode *selectedNode = nullptr;
        for (auto node : _positionLayer->getChildren()) {
            auto positionNode = (EditorPositionNode *)node;
            if (positionNode->containsPoint(point)) {
                selectedNode = positionNode;
                break;
            }
        }
        if (selectedNode != nullptr) {
            for (auto node: _positionLayer->getChildren()) {
                auto positionNode = (EditorPositionNode *)node;
                if (positionNode == selectedNode) {
                    positionNode->setSelected(true);
                } else {
                    positionNode->setSelected(false);
                }
            }            
        }
		else
		{
			onBlankPositionTouched(point);
		}
    }
    return true;
}

void EditMode::onTouchMoved(Touch *touch, Event *event) {
    auto ctx = currentContext();
    if (ctx->currentOperation() == EditorOperation::ChoosingRect && !_inDragMode) {
        auto currentPosition = _map->convertTouchToNodeSpace(touch);
        _draftDrawNode->clear();
        _draftDrawNode->drawRect(ctx->TouchStartPosition, currentPosition, kColorDrawingPosition);
    } else {
        auto delta = touch->getDelta();
        auto dest = _map->getPosition() + delta;
        setMapPosition(dest);
    }
}

void EditMode::onTouchEnded(Touch *touch, Event *event) {
    auto ctx = currentContext();
    if (ctx->currentOperation() == EditorOperation::ChoosingRect && !_inDragMode) {
        auto currentPosition = _map->convertTouchToNodeSpace(touch);
        ctx->Position->Rect = EditorHelper::rectFromTwoPoint(ctx->TouchStartPosition, currentPosition);
        ctx->addOperation(EditorOperation::InputPositionName);
        moveOn();
    } else {
        
    }
}

void EditMode::onKeyPressed(EventKeyboard::KeyCode code) {
    auto ctx = currentContext();
    if (code == EventKeyboard::KeyCode::KEY_CTRL) {
        _inDragMode = true;
    }
    if (code == EventKeyboard::KeyCode::KEY_ESCAPE && (ctx->currentOperation() == EditorOperation::ChoosingRect|| ctx->currentOperation() == EditorOperation::ChossingPosition)) {
        moveBack();
    }
    if (code == EventKeyboard::KeyCode::KEY_BACKSPACE && ctx->currentOperation() == EditorOperation::None) {
        for (auto node : _positionLayer->getChildren()) {
            auto positionNode = (EditorPositionNode *)node;
            if (positionNode->isSelected()) {
                deletePositionByName(positionNode->getData()->Name);
                positionNode->removeFromParent();
            }
        }
    }
}

void EditMode::onKeyReleased(EventKeyboard::KeyCode code) {
    if (code == EventKeyboard::KeyCode::KEY_CTRL) {
        _inDragMode = false;
    }
}