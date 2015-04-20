//
//  BEUI.cpp
//  Boids
//
//  Created by Xin Xu on 11/11/14.
//
//

#include "BEUI.h"
#include "../../Utils.h"
#include "../BETypes.h"
#if defined( SUPPORT_EXTERNAL_MAP )
    #include <boost/filesystem.hpp>
    #include "../../ini/SimpleIni.h"
    #define EDITOR_SETTING_PATH ("editor_setting.ini")
#endif

using namespace cocos2d;
using namespace cocostudio;

#ifdef SUPPORT_EXTERNAL_MAP
namespace fs = boost::filesystem;
#endif 

BEUIInputName::BEUIInputName(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler) {
    _ok = getButtonFrom("button_ok", _root);
    _cancel = getButtonFrom("button_cancel", _root);
    _name = getTextFieldFrom("input_name", _root);
    _tag = getTextFieldFrom("input_tag", _root);
    bindOKHandler(_ok);
    bindCancelHandler(_cancel);
}

void BEUIInputName::reset() {
    _name->setString("");
    _tag->setString("");
    _tag->setVisible(false);
}

void BEUIInputName::setShowTagInput(bool showTagInput) {
    _tag->setVisible(showTagInput);
}

const std::string& BEUIInputName::getInputName() const {
    return _name->getString();
}

const std::string& BEUIInputName::getInputTag() const {
    return _tag->getString();
}

BEUICreatePosition::BEUICreatePosition(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler) :
BEUIInputName( root, handler )
{
    _btn_prev_pos = getButtonFrom("btn_prev_pos", _root);
    _tf_prev_pos = getTextFieldFrom("tf_prev_pos", _root);
    
    bindSelectPositionHandler( _btn_prev_pos );
}

void BEUICreatePosition::reset() {
    BEUIInputName::reset();
    _tf_prev_pos->setString( "" );
}

const std::string& BEUICreatePosition::getPrevPosName() const {
    return _tf_prev_pos->getString();
}

void BEUICreatePosition::didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender) {
    _tf_prev_pos->setString( pos->Name );
}

//trigger options
BEUITriggerOptions::BEUITriggerOptions(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler) {
    _ok = getButtonFrom("button_ok", _root);
    _done = getButtonFrom("button_done", _root);
    _cancel = getButtonFrom("button_cancel", _root);
    _relationButton = getButtonFrom("button_relation", _root);
    bindOKHandler(_done);
    bindButton(_ok, EditorPopupEventType::AnotherTrigger);
    bindCancelHandler(_cancel);
    _isRepeat = getCheckBoxFrom("checkbox_isRepeat", _root);
    _isRepeatLabel = getLabelFrom("text_isRepeat", _root);
    
    Size contentSize = Size(400, 300);
    _list = BETypeListView::create(contentSize, EditorTypeManager::getInstance()->getTriggerType(), nullptr);
    _list->setPosition(Vec2(0, 300));
    _root->addChild(_list);
    _relationButton->addClickEventListener(CC_CALLBACK_1(BEUITriggerOptions::onRelationClicked, this));
    _relationList = popupTypeListFromButton(_relationButton, EditorTypeManager::getInstance()->getTriggerRelationType(), CC_CALLBACK_1(BEUITriggerOptions::onRelationItemClicked, this));
}

void BEUITriggerOptions::reset() {
    _isRepeat->setSelected(false);
    _relationList->reset();
    _relationList->setVisible(false);
    _relationButton->setTitleText("then");
    _list->reset();
}

void BEUITriggerOptions::setMetaUIVisible(bool isVisible) {
    _isRepeat->setVisible(isVisible);
    _isRepeatLabel->setVisible(isVisible);
    _relationButton->setVisible(isVisible);
}

const std::string& BEUITriggerOptions::getType() {
    return _list->getCurrentType();
}

bool BEUITriggerOptions::isRepeat() const {
    return _isRepeat->isSelected();
}

std::string BEUITriggerOptions::getRelationType() const {
    return _relationButton->getTitleText();
}

void BEUITriggerOptions::onRelationClicked(Ref *sender) {
    _relationList->setVisible(true);
}

void BEUITriggerOptions::onRelationItemClicked(Ref *sender) {
    _relationButton->setTitleText(_relationList->getCurrentType());
    _relationList->setVisible(false);
}

BEUIActionOptions::BEUIActionOptions(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler) {
    _ok = getButtonFrom("button_ok", _root);
    _cancel = getButtonFrom("button_cancel", _root);
    _done = getButtonFrom("button_done", _root);
    bindOKHandler(_done);
    bindCancelHandler(_cancel);
    bindButton(_ok, EditorPopupEventType::AnotherAction);
    _isInfinite = getCheckBoxFrom("checkbox_forever", _root);
    _times = getTextFieldFrom("input_repeat", _root);
    _inteval = getTextFieldFrom("input_interval", _root);
    _delay = getTextFieldFrom("input_delay", _root);
    
    Size contentSize = Size(400, 300);
    _list = BETypeListView::create(contentSize, EditorTypeManager::getInstance()->getActionType(), nullptr);
    _list->setPosition(Vec2(0, 400));
    _root->addChild(_list);
}

void BEUIActionOptions::reset() {
    _isInfinite->setSelected(false);
    _times->setString("0");
    _delay->setString("0");
    _inteval->setString("0");
    _list->reset();
}

const std::string& BEUIActionOptions::getType() {
    return _list->getCurrentType();
}

EditorActionMetaPtr BEUIActionOptions::getMeta() {
    EditorActionMetaPtr meta = EditorActionMetaPtr(new EditorActionMeta());
    meta->IsInfinite = _isInfinite->isSelected();
    meta->Delay = atof(_delay->getString().c_str());
    meta->RepeatTimes = atoi(_times->getString().c_str());
    meta->Interval = atof(_inteval->getString().c_str());
    return meta;
}

BEUIUnitAction::BEUIUnitAction(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler), _groupListView(nullptr), _typeListView(nullptr), _stateListView(nullptr) {
    
    _infoPanel = getPanelFrom("panel_info", _root);
    _groupListPanel = getPanelFrom("panel_groupList", _root);
    _ok = getButtonFrom("button_ok", _infoPanel);
    _cancel = getButtonFrom("button_cancel", _infoPanel);
    
    _typeButton = getButtonFrom("button_chooseType", _infoPanel);
    _typeTileLabel = getLabelFrom("text_type", _infoPanel);
    
    _stateButton = getButtonFrom("button_chooseState", _infoPanel);
    _stateTitleLabel = getLabelFrom("text_state", _infoPanel);
    
    _newPositionButton = getButtonFrom("button_newPosition", _infoPanel);
    _selectPositionButton = getButtonFrom("button_selectPosition", _infoPanel);
    _selectPositionGroupButton = getButtonFrom("button_selectPositionGroup", _infoPanel);
    _positionLabel = getLabelFrom("text_position", _infoPanel);
    
    _showHPCheckBox = getCheckBoxFrom("checkbox_showHP", _infoPanel);
    _changeTypeCheckBox = getCheckBoxFrom("checkbox_changeType", _infoPanel);
    _changeStateCheckBox = getCheckBoxFrom("checkbox_changeState", _infoPanel);
    _changeShowHPCheckBox = getCheckBoxFrom("checkbox_changeShowHP", _infoPanel);
    _showHPTitleLabel = getLabelFrom("text_showHP", _infoPanel);
    _countLabel = getLabelFrom("text_count", _infoPanel);
    _countTextField = getTextFieldFrom("input_count", _infoPanel);
    _tagCheckBox = getCheckBoxFrom("checkbox_tag", _infoPanel);
    _tagLabel = getLabelFrom("text_tag", _infoPanel);
    _tagTextField = getTextFieldFrom("input_tag", _infoPanel);
    _buffCheckBox = getCheckBoxFrom("checkbox_buff", _infoPanel);
    _buffLabel = getLabelFrom("text_buff", _infoPanel);
    
    //buff
    _tf_buff_name = getTextFieldFrom("tf_buff_name", _infoPanel);
    _tf_buff_type = getTextFieldFrom( "tf_buff_type", _infoPanel );
    _tf_buff_params = getTextFieldFrom( "tf_buff_params", _infoPanel );
    
    _addButton = getButtonFrom("button_add", _infoPanel);
    _deleteButton = getButtonFrom("button_delete", _root);
    _addButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitAction::onAddButtonClicked, this));
    _deleteButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitAction::onDeleteButtonClicked, this));
    _filterTextField = getTextFieldFrom("input_filter", _groupListPanel);
    _bossCheckBox = getCheckBoxFrom("checkbox_boss", _infoPanel);
    _bossLabel = getLabelFrom("text_boss", _infoPanel);
    _classTextField = getTextFieldFrom("input_class", _infoPanel);
    _customTextField = getTextFieldFrom("input_custom", _infoPanel);
    
    _levelTextField = getTextFieldFrom( "tf_level", _infoPanel );
    
    _groupListView = BEFilterListView::create(Size(500, 600), CC_CALLBACK_1(BEUIUnitAction::onGroupItemClicked, this), _filterTextField);
    _groupListView->setDelegate(this);
    _groupListView->setPosition(Vec2(0, 100));
    _groupListPanel->addChild(_groupListView);
    
    _doneListView = BEDefaultListView::create(Size(500, 600), nullptr);
    _doneListView->setPosition(Vec2(1100, 100));
    _root->addChild(_doneListView);
    
    _positionGroupListView = popupListFromButton(_selectPositionGroupButton, CC_CALLBACK_1(BEUIUnitAction::onPositionGroupItemClicked, this));
    
    _typeListView = popupTypeListFromButton(_typeButton, EditorTypeManager::getInstance()->getUnitType(), CC_CALLBACK_1(BEUIUnitAction::onTypeListItemClicked, this));
    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getUnitActionState(), CC_CALLBACK_1(BEUIUnitAction::onStateListItemClicked, this));
    
    _typeButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitAction::onTypeButtonClicked, this));
    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitAction::onStateButtonClicked, this));
    _selectPositionGroupButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitAction::onSelectPositionGroupButtonClicked, this));
    bindNewPositionHandler(_newPositionButton);
    bindSelectPositionHandler(_selectPositionButton);
    bindOKHandler(_ok);
    bindCancelHandler(_cancel);
    _showHPCheckBox->addEventListener(CC_CALLBACK_2(BEUIUnitAction::onToggleShowHP, this));
    _popupButton = getButtonFrom("button_choosePopup", _infoPanel);
    _popupButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitAction::onPopupButtonClicked, this));
    _popupListView = popupTypeListFromButton(_popupButton, EditorTypeManager::getInstance()->getUnitPopupType(), CC_CALLBACK_1(BEUIUnitAction::onPopupItemClicked, this));
    
    togglePositionUI(false);
    toggleCountUI(false);
    reset();
}

void BEUIUnitAction::reset() {
    _actions.clear();
    _action = EditorUnitActionPtr(new EditorUnitAction());
    
    _doneListView->cleanAndReset();
    _groupListView->cleanAndReset();
    _typeListView->reset();
    _stateListView->reset();
    _popupListView->reset();
    _positionGroupListView->cleanAndReset();
    _typeListView->setVisible(false);
    _stateListView->setVisible(false);
    _popupListView->setVisible(false);
    _positionGroupListView->setVisible(false);
    
    _changeTypeCheckBox->setSelected(false);
    _typeButton->setTitleText("player");
    _changeStateCheckBox->setSelected(false);
    _stateButton->setTitleText("unit_appear");
    togglePositionUI(false);
    _positionName = "";
    _positionTag = "";
    toggleCountUI(false);
    _countTextField->setString("1");
    _customTextField->setString("");
    _levelTextField->setString( "1" );
    _changeShowHPCheckBox->setSelected(false);
    _showHPCheckBox->setSelected(false);
    _buffCheckBox->setSelected(false);
    _tf_buff_name->setString("");
    _tf_buff_type->setString( "" );
    _tf_buff_params->setString( "" );
    _tagCheckBox->setSelected(false);
    _tagTextField->setString("");
    _tagTextField->setPlaceHolder("Tag Name");
    _filterTextField->setString("");
    _bossCheckBox->setSelected(false);
    toggleBossUI(false);
    _popupButton->setTitleText("normal");
}

const std::vector<EditorUnitActionPtr>& BEUIUnitAction::getActions() {
    return _actions;
}

void BEUIUnitAction::loadSourceList(const std::vector<std::pair<std::string, std::string>>& sources) {
    _editMode = false;
    _sourceList = sources;
    _groupListView->updateData();
}

void BEUIUnitAction::loadEvent(EditorEventPtr event) {
    _editMode = true;
    _editEvent = event;
    _editableActions.clear();
    _editDataList.clear();
    for (auto action : event->Actions) {
        if (action->ActionType == "unit_change") {
            auto unitAction = std::dynamic_pointer_cast<EditorUnitAction>(action);
            _editableActions.push_back(unitAction);
        }
    }
    _groupListView->updateData();
    renderAction(_editableActions[0]);
}

void BEUIUnitAction::saveEvent() {
    for (auto data : _editDataList) {
        auto it = std::find(_editEvent->Actions.begin(), _editEvent->Actions.end(), data.originalAction);
        *it = data.newAction;
    }
}

int BEUIUnitAction::itemCount() {
    if (_editMode) {
        return (int)_editableActions.size();
    } else {
        return (int)_sourceList.size();
    }
}

std::string BEUIUnitAction::searchNameAtIndex(int index) {
    if (_editMode) {
        auto action = _editableActions[index];
        return action->SourceValue;
    } else {
        auto pair = _sourceList[index];
        return pair.second;
    }
}

std::string BEUIUnitAction::displayNameAtIndex(int index) {
    std::string sourceType, sourceValue;
    if (_editMode) {
        auto action = _editableActions[index];
        sourceType = action->SourceType;
        sourceValue = action->SourceValue;
     } else {
        auto pair = _sourceList[index];
         sourceType = pair.first;
         sourceValue = pair.second;
    }
    if (sourceType == "name_source") {
        return Utils::stringFormat("name:%s", sourceValue.c_str());
    } else if (sourceType == "tag_source") {
        return Utils::stringFormat("tag:%s", sourceValue.c_str());
    } else if (sourceType == "type_source") {
        return Utils::stringFormat("type:%s", sourceValue.c_str());
    } else {
        return sourceValue;
    }
}

void BEUIUnitAction::loadPositionGroups(const std::vector<std::string>& groups) {
    if (groups.size() > 0) {
        _positionGroupListView->addItems(groups);
    }
}

void BEUIUnitAction::didGetPosition(EditorPositionPtr pos, Ref *sender) {
    std::string text = Utils::stringFormat("%s:(%.1f,%.1f)", pos->Name.c_str(), pos->Rect.origin.x, pos->Rect.origin.y);
    _positionName = pos->Name;
    _positionTag = "";
    _positionLabel->setString(text);
}

void BEUIUnitAction::renderAction(EditorUnitActionPtr action) {
    if (action->StateChanged) {
        _changeStateCheckBox->setSelected(true);
        _stateButton->setTitleText(action->UnitState);
        this->adjustUIForState(action->UnitState);
    }
    if (action->ShowHPChanged) {
        _changeShowHPCheckBox->setSelected(true);
        _showHPCheckBox->setSelected(action->ShowHP);
    }
    if (action->TypeChanged) {
        _changeTypeCheckBox->setSelected(true);
        _typeButton->setTitleText(action->UnitType);
    }
    if (action->TagChanged) {
        _tagCheckBox->setSelected(true);
        _tagTextField->setString(action->TagName);
    }
    if (action->BuffChanged) {
        _buffCheckBox->setSelected(true);
        _tf_buff_name->setString( action->BuffName );
        _tf_buff_type->setString( action->BuffType );
        _tf_buff_params->setString( action->BuffParams );
    }
    _bossCheckBox->setSelected(action->IsBoss);
    _customTextField->setString(action->CustomChange);
    _levelTextField->setString( Utils::stringFormat( "%d", action->UnitLevel ) );
    if (action->PositionName.length() > 0) {
        _positionLabel->setString(action->PositionName);
    }
    if (action->PositionTag.length() > 0) {
        _positionLabel->setString(action->PositionTag);
    }
    _countTextField->setString(Utils::stringFormat("%d", action->UnitCount));
    _classTextField->setString(action->ClassName);
    _popupButton->setTitleText(action->PopupType);
}

void BEUIUnitAction::onAddButtonClicked(Ref *sender) {
    _action->UnitType = _typeListView->getCurrentType();
    _action->UnitState = _stateListView->getCurrentType();
    _action->PositionName = _positionName;
    _action->PositionTag = _positionTag;
    _action->StateChanged = _changeStateCheckBox->isSelected();
    _action->TypeChanged = _changeTypeCheckBox->isSelected();
    _action->ShowHPChanged = _changeShowHPCheckBox->isSelected();
    _action->TagChanged = _tagCheckBox->isSelected();
    _action->BuffChanged = _buffCheckBox->isSelected();
    _action->PopupType = _popupListView->getCurrentType();
    int idx = _groupListView->getCurrentIndexForFullItems();
    if (_editMode) {
        auto action = _editableActions[idx];
        _action->SourceType = action->SourceType;
        _action->SourceValue = action->SourceValue;
    } else {
        auto pair = _sourceList[idx];
        _action->SourceType = pair.first;
        _action->SourceValue = pair.second;
    }
    _action->IsBoss = _bossCheckBox->isSelected();
    _action->CustomChange = _customTextField->getString();
    _action->UnitLevel = Utils::toInt( _levelTextField->getString() );
    if (_action->BuffChanged) {
        _action->BuffName = _tf_buff_name->getString();
        _action->BuffType = _tf_buff_type->getString();
        _action->BuffParams = _tf_buff_params->getString();
    }
    if (_action->TagChanged) {
        _action->TagName = _tagTextField->getString();
    }
    bool hasCount = false;
    if (_action->StateChanged) {
        auto state = _action->UnitState;
        if (state == "unit_appear") {
            _action->UnitCount = atoi(_countTextField->getString().c_str());
            hasCount = true;
        }
    }
    if (_classTextField->getString().length() > 0) {
        _action->ClassName = _classTextField->getString();
    }
    std::string str;
    auto btn = (BEListButton *)_groupListView->getItem(_groupListView->getCurrentIndex());
    if (hasCount) {
        str = Utils::stringFormat("%s(%d)", btn->getItemName().c_str(), _action->UnitCount);
    } else {
        str = btn->getItemName().c_str();
    }
    if (_editMode) {
        UnitActionEditData data;
        data.originalAction = _editableActions[idx];
        data.newAction = _action;
        _editDataList.push_back(data);
    } else {
        _actions.push_back(_action);
    }
    _doneListView->addItem(str);
    _action = EditorUnitActionPtr(new EditorUnitAction());
}

void BEUIUnitAction::onSelectPositionGroupButtonClicked(Ref *sender) {
    _positionGroupListView->setVisible(true);
}

void BEUIUnitAction::onPositionGroupItemClicked(Ref *sender) {
    _positionGroupListView->setVisible(false);
    auto name = _positionGroupListView->getCurrentName();
    _positionLabel->setString(name);
    _positionTag = name;
    _positionName = "";
}

void BEUIUnitAction::onDeleteButtonClicked(Ref *sender) {
    int index = _doneListView->getCurrentIndex();
    if (_editMode) {
        _editDataList.erase(_editDataList.begin() + index);
    } else {
        _actions.erase(_actions.begin() + index);
    }
    _doneListView->removeCurrentIndex();
}

void BEUIUnitAction::onTypeButtonClicked(Ref *sender) {
    _typeListView->setVisible(true);
}

void BEUIUnitAction::onStateButtonClicked(Ref *sender) {
    _stateListView->setVisible(true);
}

void BEUIUnitAction::onTypeListItemClicked(Ref *sender) {
    _typeButton->setTitleText(_typeListView->getCurrentType());
    _typeListView->setVisible(false);
}
                                    
void BEUIUnitAction::onStateListItemClicked(Ref *sender) {
    auto name = _stateListView->getCurrentType();
    _stateListView->setVisible(false);
    _stateButton->setTitleText(name);
    this->adjustUIForState(name);
}

void BEUIUnitAction::adjustUIForState(const std::string& state) {
    if (state == "unit_move" || state == "unit_patrol") {
        togglePositionUI(true);
        _selectPositionGroupButton->setVisible(false);
        toggleCountUI(false);
        toggleBossUI(false);
    } else if (state == "unit_appear" || state == "unit_hold_appear") {
        togglePositionUI(true);
        toggleCountUI(true);
        toggleBossUI(true);
    } else {
        togglePositionUI(false);
        toggleCountUI(false);
        toggleBossUI(false);
    }
}

void BEUIUnitAction::onGroupItemClicked(Ref *sender) {
    if (_editMode) {
        int index = _groupListView->getCurrentIndex();
        renderAction(_editableActions[index]);
    }
}

void BEUIUnitAction::onPopupItemClicked(Ref *sender) {
    _popupButton->setTitleText(_popupListView->getCurrentType());
    _popupListView->setVisible(false);
}

void BEUIUnitAction::onPopupButtonClicked(Ref *sender) {
    _popupListView->setVisible(true);
}

void BEUIUnitAction::onToggleShowHP(Ref *sender, ui::CheckBox::EventType et) {
    if (et == ui::CheckBox::EventType::SELECTED) {
        _action->ShowHP = true;
    } else {
        _action->ShowHP = false;
    }
}

void BEUIUnitAction::togglePositionUI(bool visible) {
    _positionLabel->setVisible(visible);
    _newPositionButton->setVisible(visible);
    _selectPositionButton->setVisible(visible);
    _selectPositionGroupButton->setVisible(visible);
}

void BEUIUnitAction::toggleCountUI(bool visible) {
    _countLabel->setVisible(visible);
    _countTextField->setVisible(visible);
}

void BEUIUnitAction::toggleBossUI(bool visible) {
    _bossLabel->setVisible(visible);
    _bossCheckBox->setVisible(visible);
}

//unit change trigger
BEUIUnitTrigger::BEUIUnitTrigger(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler), _groupListView(nullptr), _stateListView(nullptr) {
    
    _infoPanel = getPanelFrom("panel_info", _root);
    _groupListPanel = getPanelFrom("panel_groupList", _root);
    _ok = getButtonFrom("button_ok", _infoPanel);
    _cancel = getButtonFrom("button_cancel", _infoPanel);
    _trigger = EditorUnitTriggerPtr(new EditorUnitTrigger());
    
    
    _stateButton = getButtonFrom("button_chooseState", _infoPanel);
    _stateTitleLabel = getLabelFrom("text_state", _infoPanel);
    _filterTextField = getTextFieldFrom("input_filter", _groupListPanel);
    
    _positionTitleLabel1 = getLabelFrom("text_positionTitle1", _infoPanel);
    _newPositionButton1 = getButtonFrom("button_newPosition1", _infoPanel);
    _selectPositionButton1 = getButtonFrom("button_selectPosition1", _infoPanel);
    _positionLabel1 = getLabelFrom("text_postion1", _infoPanel);
    _triggerCountTextField = getTextFieldFrom("input_count", _infoPanel);
    
    _lb_tag = getTextFieldFrom( "lb_tag", _infoPanel );
    
    _groupListView = BEFilterListView::create(Size(500, 600), nullptr, _filterTextField);
    _groupListView->setDelegate(this);
    _groupListView->setPosition(Vec2(0, 100));
    _groupListPanel->addChild(_groupListView);
    
    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getUnitTriggerState(), CC_CALLBACK_1(BEUIUnitTrigger::onStateListItemClicked, this));
    
    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUIUnitTrigger::onStateButtonClicked, this));
    bindNewPositionHandler(_newPositionButton1);
    bindSelectPositionHandler(_selectPositionButton1);
    bindOKHandler(_ok);
    bindCancelHandler(_cancel);
    
    togglePositionUI(false);
}

void BEUIUnitTrigger::reset() {
    togglePositionUI(false);
    _groupListView->cleanAndReset();
    _stateListView->reset();
    _stateListView->setVisible(false);
    _stateButton->setTitleText("unit_appear");
    _triggerCountTextField->setString("");
    _positionName = "";
    _positionLabel1->setString("位置1");
    _lb_tag->setString( "1" );
    _trigger = EditorUnitTriggerPtr(new EditorUnitTrigger());
    _filterTextField->setString("");
}

void BEUIUnitTrigger::loadSourceList(const std::vector<std::pair<std::string, std::string>>& sources) {
    _sourceList.clear();
    _sourceList.push_back( std::make_pair( "tag_source", "custom" ) );
    _sourceList.insert( _sourceList.end(), sources.begin(), sources.end() );
    _groupListView->updateData();
}

int BEUIUnitTrigger::itemCount() {
    return (int)_sourceList.size();
}

std::string BEUIUnitTrigger::searchNameAtIndex(int index) {
    auto pair = _sourceList[index];
    return pair.second;
}

std::string BEUIUnitTrigger::displayNameAtIndex(int index) {
    auto pair = _sourceList[index];
    if (pair.first == "name_source") {
        return Utils::stringFormat("name:%s", pair.second.c_str());
    } else if (pair.first == "tag_source") {
        return Utils::stringFormat("tag:%s", pair.second.c_str());
    } else if (pair.first == "type_source") {
        return Utils::stringFormat("type:%s", pair.second.c_str());
    } else {
        return pair.second;
    }
}

void BEUIUnitTrigger::didGetPosition(EditorPositionPtr pos, Ref *sender) {
    std::string text = Utils::stringFormat("%s:(%.1f,%.1f)", pos->Name.c_str(), pos->Rect.origin.x, pos->Rect.origin.y);
    _positionName = pos->Name;
    _positionLabel1->setString(text);
}

void BEUIUnitTrigger::onStateButtonClicked(Ref *sender) {
    _stateListView->setVisible(true);
}

EditorUnitTriggerPtr BEUIUnitTrigger::getTrigger() {
    _trigger->UnitState = _stateListView->getCurrentType();
    _trigger->PositionName = _positionName;
    int idx = _groupListView->getCurrentIndexForFullItems();
    auto pair = _sourceList[idx];
    _trigger->SourceType = pair.first;
    if( pair.first == "tag_source" ) {
        _trigger->SourceValue = _lb_tag->getString();
    }
    else {
        _trigger->SourceValue = pair.second;
    }
    if (_triggerCountTextField->getString() == "") {
        _trigger->TriggerCount = 1;
    } else {
        _trigger->TriggerCount = atoi(_triggerCountTextField->getString().c_str());
    }
    return _trigger;
}

void BEUIUnitTrigger::onStateListItemClicked(Ref *sender) {
    auto name = _stateListView->getCurrentType();
    _stateButton->setTitleText(name);
    _stateListView->setVisible(false);
    if (name == "unit_dead") {
        togglePositionUI( false );
        _positionTitleLabel1->setString("移动至");
    } else if (name == "unit_appear") {
        togglePositionUI(false);
    } else if (name == "unit_disappear") {
        togglePositionUI(false);
    }
}

void BEUIUnitTrigger::togglePositionUI(bool visible) {
    _positionLabel1->setVisible(visible);
    _positionTitleLabel1->setVisible(visible);
    _newPositionButton1->setVisible(visible);
    _selectPositionButton1->setVisible(visible);
}

//unit stay trigger
BEUIUnitStayTrigger::BEUIUnitStayTrigger(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler) {
    _infoPanel = getPanelFrom("Panel_info", _root);
    _btn_ok = getButtonFrom("btn_ok", _infoPanel);
    _btn_cancel = getButtonFrom("btn_cancel", _infoPanel);
    _trigger = EditorUnitStayTriggerPtr(new EditorUnitStayTrigger());
    
    _lb_pos_name = getLabelFrom("text_pos", _infoPanel);
    _btn_new_pos = getButtonFrom("btn_create_pos", _infoPanel);
    _btn_select_pos = getButtonFrom("btn_select_pos", _infoPanel);
    _tf_count = getTextFieldFrom("tf_count", _infoPanel);
    _tf_duration = getTextFieldFrom( "tf_duration", _infoPanel );
    _tf_tag = getTextFieldFrom( "tf_tag", _infoPanel );
    
    bindNewPositionHandler( _btn_new_pos );
    bindSelectPositionHandler( _btn_select_pos );
    bindOKHandler( _btn_ok );
    bindCancelHandler( _btn_cancel );
    
    _groupListPanel = getPanelFrom("panel_groupList", _root);
    _filterTextField = getTextFieldFrom("input_filter", _groupListPanel);
   
    _groupListView = BEFilterListView::create(Size(500, 600), nullptr, _filterTextField);
    _groupListView->setDelegate(this);
    _groupListView->setPosition(Vec2(0, 100));
    _groupListPanel->addChild(_groupListView);
    
//    togglePositionUI(true);
}

void BEUIUnitStayTrigger::reset() {
//    togglePositionUI(false);
    _groupListView->cleanAndReset();
    _filterTextField->setString( "" );
    _tf_count->setString("1");
    _positionName = "";
    _lb_pos_name->setString("位置1");
    _tf_duration->setString( "0" );
    _tf_tag->setString( "" );
    _trigger = EditorUnitStayTriggerPtr( new EditorUnitStayTrigger() );
}

void BEUIUnitStayTrigger::didGetPosition(EditorPositionPtr pos, Ref *sender) {
    std::string text = Utils::stringFormat("%s:(%.1f,%.1f)", pos->Name.c_str(), pos->Rect.origin.x, pos->Rect.origin.y);
    _positionName = pos->Name;
    _lb_pos_name->setString(text);
}

EditorUnitStayTriggerPtr BEUIUnitStayTrigger::getTrigger() {
    _trigger->PositionName = _positionName;
    _trigger->SourceType = "tag_source";
    _trigger->SourceValue = _tf_tag->getString();
    if (_tf_count->getString() == "") {
        _trigger->TriggerCount = 1;
    } else {
        _trigger->TriggerCount = atoi(_tf_count->getString().c_str());
    }
    if (_tf_duration->getString() == "") {
        _trigger->Duration = 0;
    } else {
        _trigger->Duration = atof(_tf_duration->getString().c_str());
    }
    return _trigger;
}

void BEUIUnitStayTrigger::togglePositionUI(bool visible) {
    _lb_pos_name->setVisible(visible);
    _btn_new_pos->setVisible(visible);
    _btn_select_pos->setVisible(visible);
}

void BEUIUnitStayTrigger::loadSourceList(const std::vector<std::pair<std::string, std::string>>& sources) {
    _sourceList.clear();
    _sourceList.push_back( std::make_pair( "tag_source", "custom" ) );
    _sourceList.insert( _sourceList.end(), sources.begin(), sources.end() );
    _groupListView->updateData();
}

std::string BEUIUnitStayTrigger::searchNameAtIndex(int index) {
    auto pair = _sourceList[index];
    return pair.second;
}

std::string BEUIUnitStayTrigger::displayNameAtIndex(int index) {
    auto pair = _sourceList[index];
    if (pair.first == "name_source") {
        return Utils::stringFormat("name:%s", pair.second.c_str());
    } else if (pair.first == "tag_source") {
        return Utils::stringFormat("tag:%s", pair.second.c_str());
    } else if (pair.first == "type_source") {
        return Utils::stringFormat("type:%s", pair.second.c_str());
    } else {
        return pair.second;
    }
}

int BEUIUnitStayTrigger::itemCount() {
     return (int)_sourceList.size();
}

//event change trigger
BEUIEventChange::BEUIEventChange(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler),
_eventListView(nullptr), _eventTypeListView(nullptr) {
    _eventTypeButton = getButtonFrom("button_eventType", _root);
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
    _eventListView = BEDefaultListView::create(Size(400, 400), nullptr);
    _eventListView->setPosition(Vec2(0, 200));
    _root->addChild(_eventListView);
    
    _eventTypeButton->addClickEventListener(CC_CALLBACK_1(BEUIEventChange::onEventTypeButtonClicked, this));
    
}

void BEUIEventChange::reset() {
    _eventListView->cleanAndReset();
    if (_eventTypeListView != nullptr) {
        _eventTypeListView->removeFromParent();
        _eventTypeListView = nullptr;
    }
    _eventTypeButton->setTitleText("event_trigger");
}

void BEUIEventChange::setIsTrigger(bool isTrigger) {
    if (isTrigger) {
        _eventTypeListView = popupTypeListFromButton(_eventTypeButton, EditorTypeManager::getInstance()->getEventTriggerState(), CC_CALLBACK_1(BEUIEventChange::onEventTypeItemClicked, this));
    } else {
        _eventTypeListView = popupTypeListFromButton(_eventTypeButton, EditorTypeManager::getInstance()->getEventActionState(), CC_CALLBACK_1(BEUIEventChange::onEventTypeItemClicked, this));
    }
    _eventTypeListView->setVisible(false);
}

std::string BEUIEventChange::getEventName() {
    int index = _eventListView->getCurrentIndex();
    return _eventList[index]->Name;
}

std::string BEUIEventChange::getEventState() {
    return _eventTypeListView->getCurrentType();
}

void BEUIEventChange::loadEventList(const std::vector<EditorEventPtr>& eventList) {
    _eventListView->removeAllItems();
    std::vector<std::string> names;
    for (auto e : eventList) {
        names.push_back(e->Name);
    }
    _eventList = eventList;
    _eventListView->addItems(names);
}

void BEUIEventChange::onEventTypeButtonClicked(Ref *sender) {
    _eventTypeListView->setVisible(true);
}

void BEUIEventChange::onEventTypeItemClicked(Ref *sender) {
    _eventTypeButton->setTitleText(_eventTypeListView->getCurrentType());
    _eventTypeListView->setVisible(false);
}

BEUIGameStateChange::BEUIGameStateChange(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler), _stateListView(nullptr) {
    _stateButton = getButtonFrom("button_gameState", _root);
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getGameState(), CC_CALLBACK_1(BEUIGameStateChange::onStateItemClicked, this));
    _stateListView->setVisible(false);
    
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUIGameStateChange::onStateButtonClicked, this));
    reset();
}

void BEUIGameStateChange::reset() {
    _stateListView->reset();
    _stateListView->setVisible(false);
    _stateButton->setTitleText("game_start");
}

EditorGameActionPtr BEUIGameStateChange::getAction() {
    auto action = EditorGameActionPtr(new EditorGameAction());
    action->GameState = _stateListView->getCurrentType();
    return action;
}

EditorGameTriggerPtr BEUIGameStateChange::getTrigger() {
    auto trigger = EditorGameTriggerPtr(new EditorGameTrigger());
    trigger->GameState = _stateListView->getCurrentType();
    return trigger;
}

void BEUIGameStateChange::onStateButtonClicked(Ref *sender) {
    _stateListView->setVisible(true);
}

void BEUIGameStateChange::onStateItemClicked(Ref *sender) {
    _stateButton->setTitleText(_stateListView->getCurrentType());
    _stateListView->setVisible(false);
}

BEUITaskStateChange::BEUITaskStateChange(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler), _stateListView(nullptr), _taskListView(nullptr) {
    _action = EditorTaskActionPtr(new EditorTaskAction());
    _stateButton = getButtonFrom("button_taskState", _root);
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getTaskState(), CC_CALLBACK_1(BEUITaskStateChange::onStateListItemClicked, this));
    _stateListView->setVisible(false);
    
    _taskListView = BEDefaultListView::create(Size(400, 400), nullptr);
    _taskListView->setPosition(Vec2(0, 300));
    _root->addChild(_taskListView);
    
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUITaskStateChange::onStateButtonClicked, this));
}

void BEUITaskStateChange::reset() {
    _taskListView->cleanAndReset();
    _stateListView->reset();
    _stateListView->setVisible(false);
    _stateButton->setTitleText("task_start");
    _action = EditorTaskActionPtr(new EditorTaskAction());
}

EditorTaskActionPtr BEUITaskStateChange::getAction() {
    _action->TaskState = _stateListView->getCurrentType();
    _action->TaskName = _taskListView->getCurrentName();
    return _action;
}

void BEUITaskStateChange::loadTaskList(const std::vector<EditorGameConditionPtr>& taskList) {
    std::vector<std::string> names;
    for (auto task : taskList) {
        names.push_back(task->Name);
    }
    _taskListView->addItems(names);
}

void BEUITaskStateChange::onStateButtonClicked(Ref *sender) {
    _stateListView->setVisible(true);
}

void BEUITaskStateChange::onStateListItemClicked(Ref *sender) {
    _stateListView->setVisible(false);
    _stateButton->setTitleText(_stateListView->getCurrentType());
}

BEUIEventList::BEUIEventList(ui::Layout *root, const BEPopupEventHandler& handler):BEUIBase(root, handler), _eventListView(nullptr) {
    _editButton = getButtonFrom("button_edit", _root);
    _deleteButton = getButtonFrom("button_delete", _root);
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    bindCancelHandler(_cancelButton);
    bindOKHandler(_okButton);
    bindButton(_editButton, EditorPopupEventType::EditEvent);
    _deleteButton->addClickEventListener(CC_CALLBACK_1(BEUIEventList::onDeleteButtonClicked, this));
    
    _eventListView = BEDefaultListView::create(Size(400, 500), nullptr);
    _eventListView->setPosition(Vec2(0, 200));
    _root->addChild(_eventListView);
}

void BEUIEventList::reset() {
    _eventListView->cleanAndReset();
    _deletedEventIndexs.clear();
}

void BEUIEventList::loadEventList(const std::vector<EditorEventPtr>& eventList) {
    std::vector<std::string> names;
    _eventList = eventList;
    for (auto event : eventList) {
        names.push_back(event->Name);
    }
    _eventListView->addItems(names);
}

void BEUIEventList::onDeleteButtonClicked(Ref *sender) {
    int index = _eventListView->getCurrentIndex();
    if (index >= 0 && index < _eventListView->getItems().size()) {
        _deletedEventIndexs.push_back(index);
        _eventListView->removeCurrentIndex();
    }
}

EditorEventPtr BEUIEventList::getCurrentEvent() {
    auto index = _eventListView->getCurrentIndex();
    return _eventList[index];
}

BEUIVisionChange::BEUIVisionChange(Node *root, const BEPopupEventHandler& handler): BEUIBase(root, handler) {
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    _stateButton = getButtonFrom("button_state", _root);
    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getVisionObjectStateType(), CC_CALLBACK_1(BEUIVisionChange::onStateItemClicked, this));
    _stateListView->setVisible(false);
    
    _groupListPanel = getPanelFrom("panel_groupList", _root);
    _filterTextField = getTextFieldFrom("input_filter", _groupListPanel);
    
    _groupListView = BEFilterListView::create(Size(500, 600), nullptr, _filterTextField);
    _groupListView->setDelegate(this);
    _groupListView->setPosition(Vec2(0, 100));
    _groupListPanel->addChild(_groupListView);
    
    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUIVisionChange::onStateButtonClicked, this));
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
}

void BEUIVisionChange::reset() {
    _groupListView->cleanAndReset();
    _stateListView->reset();
    _stateButton->setTitleText("vision_change");
}

std::string BEUIVisionChange::searchNameAtIndex(int index) {
    auto pair = _sourceList[index];
    return pair.second;
}

std::string BEUIVisionChange::displayNameAtIndex(int index) {
    auto pair = _sourceList[index];
    if (pair.first == "name_source") {
        return Utils::stringFormat("name:%s", pair.second.c_str());
    } else if (pair.first == "tag_source") {
        return Utils::stringFormat("tag:%s", pair.second.c_str());
    } else {
        return pair.second;
    }
}

int BEUIVisionChange::itemCount() {
    return (int)_sourceList.size();
}

EditorVisionActionPtr BEUIVisionChange::getAction() {
    auto action = EditorVisionActionPtr(new EditorVisionAction());
    int index = _groupListView->getCurrentIndex();
    auto p = _sourceList[index];
    action->SourceType = p.first;
    action->SourceValue = p.second;
    action->VisionState = _stateListView->getCurrentType();
    return action;
}

EditorVisionTriggerPtr BEUIVisionChange::getTrigger() {
    auto trigger = EditorVisionTriggerPtr(new EditorVisionTrigger());
    int index = _groupListView->getCurrentIndex();
    auto p = _sourceList[index];
    trigger->SourceType = p.first;
    trigger->SourceValue = p.second;
    trigger->VisionState = _stateListView->getCurrentType();
    return trigger;
}

void BEUIVisionChange::loadVisionObjects(const std::vector<std::pair<std::string, std::string>>& visionObjects) {
    _sourceList = visionObjects;
    _groupListView->updateData();
}

void BEUIVisionChange::onStateButtonClicked(Ref *sender) {
    _stateListView->setVisible(true);
}

void BEUIVisionChange::onStateItemClicked(Ref *sender) {
    _stateButton->setTitleText(_stateListView->getCurrentType());
    _stateListView->setVisible(false);
}

BEUIWaveAction::BEUIWaveAction(Node *root, const BEPopupEventHandler& handler): BEUIBase(root, handler) {
    _infoPanel = getPanelFrom("panel_info", _root);
    _unitListPanel = getPanelFrom("panel_unitList", _root);
    _okButton = getButtonFrom("button_ok", _infoPanel);
    _cancelButton = getButtonFrom("button_cancel", _infoPanel);
    _addButton = getButtonFrom("button_add", _unitListPanel);
    _addWaveButton = getButtonFrom("button_addWave", _infoPanel);
    _deleteWaveButton = getButtonFrom("button_deleteWave", _root);
    _filterTextField = getTextFieldFrom("input_filter", _unitListPanel);
    _numberTextField = getTextFieldFrom("input_number", _unitListPanel);
    _levelTextField = getTextFieldFrom("input_level", _unitListPanel);
    _bossCheckBox = getCheckBoxFrom("checkbox_boss", _unitListPanel);
    _newPositionButton = getButtonFrom("button_newPosition", _infoPanel);
    _choosePositionButton = getButtonFrom("button_selectPosition", _infoPanel);
    _choosePositionGroupButton = getButtonFrom("button_selectPositionGroup", _infoPanel);
    _positionLabel = getLabelFrom("text_position", _infoPanel);
    _intervalTextField = getTextFieldFrom("input_interval", _infoPanel);
    _classTextField = getTextFieldFrom("input_class", _unitListPanel);
    _unitListView = BEFilterListView::create(Size(500, 500), nullptr, _filterTextField);
    _unitListView->setDelegate(this);
    _unitListView->setPosition(Vec2(0, 200));
    _unitListPanel->addChild(_unitListView);
    _waveListView = BEDefaultListView::create(Size(500, 600), nullptr);
    _waveListView->setPosition(Vec2(1100, 100));
    _root->addChild(_waveListView);
    _chosenUnitListView = BEDefaultListView::create(Size(600, 450), nullptr);
    _chosenUnitListView->setPosition(Vec2(0, 150));
    _infoPanel->addChild(_chosenUnitListView);
    _positionGroupListView = popupListFromButton(_choosePositionGroupButton, CC_CALLBACK_1(BEUIWaveAction::onPositionGroupItemClicked, this));
    _positionGroupListView->setVisible(false);
    _choosePositionGroupButton->addClickEventListener(CC_CALLBACK_1(BEUIWaveAction::onPositionGroupButtonClicked, this));
    bindNewPositionHandler(_newPositionButton);
    bindSelectPositionHandler(_choosePositionButton);
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
    _addButton->addClickEventListener(CC_CALLBACK_1(BEUIWaveAction::onAddButtonClicked, this));
    _addWaveButton->addClickEventListener(CC_CALLBACK_1(BEUIWaveAction::onAddWaveButtonClicked, this));
    _deleteWaveButton->addClickEventListener(CC_CALLBACK_1(BEUIWaveAction::onDeleteWaveButtonClicked, this));
    _currentWave = EditorWavePtr(new EditorWave());
}

void BEUIWaveAction::reset() {
    _action = EditorWaveActionPtr(new EditorWaveAction());
    _unitListView->cleanAndReset();
    _chosenUnitListView->cleanAndReset();
    _positionGroupListView->cleanAndReset();
    _waveListView->cleanAndReset();
    _positionName = "";
    _positionTag = "";
    _currentWave = EditorWavePtr(new EditorWave());
}

void BEUIWaveAction::loadUnitList(const std::vector<std::string>& unitList) {
    _unitList = unitList;
    _unitListView->updateData();
}

void BEUIWaveAction::loadPositionGroups(const std::vector<std::string>& groups) {
    if (groups.size() > 0) {
        _positionGroupListView->addItems(groups);
    }
}

int BEUIWaveAction::itemCount() {
    return (int)_unitList.size();
}

std::string BEUIWaveAction::searchNameAtIndex(int index) {
    return _unitList[index];
}

std::string BEUIWaveAction::displayNameAtIndex(int index) {
    return _unitList[index];
}

void BEUIWaveAction::onAddButtonClicked(Ref *sender) {
    EditorUnitPtr unit = EditorUnitPtr(new EditorUnit());
    int number = atoi(_numberTextField->getString().c_str());
    unit->Count = number == 0 ? 1 : number;
    int level = atoi(_levelTextField->getString().c_str());
    unit->Level = level == 0 ? 1 : level;
    unit->IsBoss = _bossCheckBox->isSelected();
    int idx = _unitListView->getCurrentIndexForFullItems();
    unit->Name = _unitList[idx];
    std::string className = _classTextField->getString();
    unit->Class = className.length() == 0 ? "LogicUnit" : className;
    _currentWave->Units.push_back(unit);
    std::string itemName = Utils::stringFormat("%s(%d)", unit->Name.c_str(), unit->Count);
    _chosenUnitListView->addItem(itemName);
}

void BEUIWaveAction::onAddWaveButtonClicked(Ref *sender) {
    _currentWave->PositionName = _positionName;
    _currentWave->PositionTag = _positionTag;
    _currentWave->Interval = atof(_intervalTextField->getString().c_str());
    _action->Waves.push_back(_currentWave);
    _chosenUnitListView->cleanAndReset();
    std::string itemName = "";
    for (int i = 0; i < _currentWave->Units.size(); ++i) {
        auto unit = _currentWave->Units[i];
        std::string name;
        if (i == _currentWave->Units.size() - 1) {
            name = Utils::stringFormat("%s(%d)", unit->Name.c_str(), unit->Count);
        } else {
            name = Utils::stringFormat("%s(%d),", unit->Name.c_str(), unit->Count);
        }
        itemName += name;
    }
    _waveListView->addItem(itemName);
    _currentWave = EditorWavePtr(new EditorWave());
}

void BEUIWaveAction::onDeleteWaveButtonClicked(Ref *sender) {
    int index = _waveListView->getCurrentIndex();
    _action->Waves.erase(_action->Waves.begin() + index);
    _waveListView->removeCurrentIndex();
}

void BEUIWaveAction::onPositionGroupButtonClicked(Ref *sender) {
    _positionGroupListView->setVisible(true);
}

void BEUIWaveAction::didGetPosition(EditorPositionPtr pos, Ref *sender) {
    std::string text = Utils::stringFormat("%s:(%.1f,%.1f)", pos->Name.c_str(), pos->Rect.origin.x, pos->Rect.origin.y);
    _positionName = pos->Name;
    _positionTag = "";
    _positionLabel->setString(text);
}

void BEUIWaveAction::onPositionGroupItemClicked(Ref *sender) {
    _positionGroupListView->setVisible(false);
    auto name = _positionGroupListView->getCurrentName();
    _positionLabel->setString(name);
    _positionTag = name;
    _positionName = "";
}

BEUIGameCondition::BEUIGameCondition(Node *root, const BEPopupEventHandler& handler): BEUIBase(root, handler) {
    _chooseTypeButton = getButtonFrom("button_type", _root);
    _nameTextField = getTextFieldFrom("tf_name", _root);
    _descTextField = getTextFieldFrom("tf_desc", _root);
    _titleTextField = getTextFieldFrom( "tf_title", _root );
    _addButton = getButtonFrom("button_add", _root);
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    _deleteButton = getButtonFrom("button_delete", _root);
    _resultListView = BEDefaultListView::create(Size(500, 550), nullptr);
    _resultListView->setPosition(Vec2(500, 150));
    _root->addChild(_resultListView);
    _typeListView = popupTypeListFromButton(_chooseTypeButton, EditorTypeManager::getInstance()->getGameConditionSourceType(), CC_CALLBACK_1(BEUIGameCondition::onTypeItemClicked, this));
    _typeListView->setVisible(false);
    _chooseTypeButton->addClickEventListener(CC_CALLBACK_1(BEUIGameCondition::onChooseTypeButtonClicked, this));
    _addButton->addClickEventListener(CC_CALLBACK_1(BEUIGameCondition::onAddButtonClicked, this));
    _deleteButton->addClickEventListener(CC_CALLBACK_1(BEUIGameCondition::onDeleteButtonClicked, this));
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
}

void BEUIGameCondition::reset() {
    _condition = EditorGameConditionPtr(new EditorGameCondition());
    _conditions.clear();
    _resultListView->cleanAndReset();
    _typeListView->reset();
}

void BEUIGameCondition::loadConditions(const std::vector<EditorGameConditionPtr>& conditions) {
    _conditions = conditions;
    _resultListView->removeAllItems();
    std::vector<std::string> names;
    for (auto condition : _conditions) {
        std::string name = Utils::stringFormat("%s:%s", condition->Type.c_str(), condition->Name.c_str());
        names.push_back(name);
    }
    _resultListView->addItems(names);
}

void BEUIGameCondition::onAddButtonClicked(Ref *sender) {
    _condition->Type = _typeListView->getCurrentType();
    _condition->Title = _titleTextField->getString();
    _condition->Name = _nameTextField->getString();
    _condition->Desc = _descTextField->getString();
    _conditions.push_back(_condition);
    std::string name = Utils::stringFormat("%s:%s", _condition->Type.c_str(), _condition->Name.c_str());
    _resultListView->addItem(name);
    _condition = EditorGameConditionPtr(new EditorGameCondition());
}

void BEUIGameCondition::onDeleteButtonClicked(Ref *sender) {
    int index = _resultListView->getCurrentIndex();
    _conditions.erase(_conditions.begin() + index);
    _resultListView->removeCurrentIndex();
}

void BEUIGameCondition::onChooseTypeButtonClicked(Ref *sender) {
    _typeListView->setVisible(true);
}

void BEUIGameCondition::onConditionItemClicked(Ref *sender) {
    _nameTextField->setString("");
    _descTextField->setString("");
    _titleTextField->setString( "" );
}

void BEUIGameCondition::onTypeItemClicked(Ref *sender) {
    _chooseTypeButton->setTitleText(_typeListView->getCurrentType());
    _typeListView->setVisible(false);
}

//conversation action ui
BEUIConversationAction::BEUIConversationAction(Node *root, const BEPopupEventHandler& handler): BEUIBase(root, handler) {
    _listPanel = getPanelFrom("panel_list", _root);
    _infoPanel = getPanelFrom("panel_info", _root);
    _filterTextField = getTextFieldFrom("input_filter", _listPanel);
    _contentTextField = getTextFieldFrom("input_content", _infoPanel);
    _nameTextField = getTextFieldFrom("input_conversationName", _infoPanel);
    _tf_tag = getTextFieldFrom("tf_speaker_tag", _infoPanel);
    _tf_repeat = getTextFieldFrom("tf_repeat", _infoPanel);
    _tf_duration = getTextFieldFrom("tf_duration", _infoPanel);
    _tf_interval = getTextFieldFrom("tf_interval", _infoPanel);
    _cb_random = getCheckBoxFrom( "cb_random", _infoPanel );
    _addButton = getButtonFrom("button_add", _infoPanel);
    _okButton = getButtonFrom("button_ok", _infoPanel);
    _cancelButton = getButtonFrom("button_cancel", _infoPanel);
    _deleteButton = getButtonFrom("button_delete", _root);
    _sourceListView = BEFilterListView::create(Size(500, 700), nullptr, _filterTextField);
    _sourceListView->setDelegate(this);
    _sourceListView->setPosition(Vec2(0, 50));
    _speechListView= BEDefaultListView::create(Size(500, 600), nullptr);
    _speechListView->setPosition(Vec2(1100, 100));
    _root->addChild(_speechListView);
    _listPanel->addChild(_sourceListView);
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
    _addButton->addClickEventListener(CC_CALLBACK_1(BEUIConversationAction::onAddButtonClicked, this));
    _deleteButton->addClickEventListener(CC_CALLBACK_1(BEUIConversationAction::onDeleteButtonClicked, this));
}

void BEUIConversationAction::reset() {
    _sourceListView->cleanAndReset();
    _speechListView->cleanAndReset();
    _action = EditorConversationActionPtr(new EditorConversationAction());
    _currentSpeech = EditorSpeechPtr(new EditorSpeech());
}

void BEUIConversationAction::loadConversationSource(const std::vector<std::pair<std::string, std::string>>& sources) {
    _sources.clear();
    _sources.push_back( std::make_pair( "tag_source", "custom" ) );
    _sources.insert( _sources.end(), sources.begin(), sources.end() );
    _sourceListView->updateData();
}

EditorConversationActionPtr BEUIConversationAction::getAction() {
    _action->Name = _nameTextField->getString();
    _action->RandomOrder = _cb_random->isSelected();
    _action->RepeatTimes = atoi( _tf_repeat->getString().c_str() );
    int index = _sourceListView->getCurrentIndexForFullItems();
    _action->SourceType = _sources[index].first;
    if( _action->SourceType == "tag_source" ) {
        _action->SourceValue = _tf_tag->getString();
    }
    else {
        _action->SourceValue = _sources[index].second;
    }
    return _action;
}

std::string BEUIConversationAction::searchNameAtIndex(int index) {
    auto pair = _sources[index];
    return pair.second;
}

std::string BEUIConversationAction::displayNameAtIndex(int index) {
    auto pair = _sources[index];
    if (pair.first == "name_source") {
        return Utils::stringFormat("name:%s", pair.second.c_str());
    } else if (pair.first == "tag_source") {
        return Utils::stringFormat("tag:%s", pair.second.c_str());
    } else {
        return pair.second;
    }
}

int BEUIConversationAction::itemCount() {
    return (int)_sources.size();
}

void BEUIConversationAction::onAddButtonClicked(Ref *sender) {
    _currentSpeech->Content = _contentTextField->getString();
    _currentSpeech->Duration = atof( _tf_duration->getString().c_str());
    _currentSpeech->Interval = atof( _tf_interval->getString().c_str() );
    _action->Speeches.push_back(_currentSpeech);
    _speechListView->addItem(_currentSpeech->Content);
    _currentSpeech = EditorSpeechPtr(new EditorSpeech());
}

void BEUIConversationAction::onDeleteButtonClicked(Ref *sender) {
    int index = _speechListView->getCurrentIndex();
    _action->Speeches.erase(_action->Speeches.begin() + index);
    _speechListView->removeCurrentIndex();
}


//story action ui
BEUIStoryAction::BEUIStoryAction(Node *root, const BEPopupEventHandler& handler): BEUIBase(root, handler) {
    _listPanel = getPanelFrom("panel_list", _root);
    _infoPanel = getPanelFrom("panel_info", _root);
    _filterTextField = getTextFieldFrom("input_filter", _listPanel);
    _contentTextField = getTextFieldFrom("input_content", _infoPanel);
    _tf_line = getTextFieldFrom("tf_line", _infoPanel);
    _tf_story_name = getTextFieldFrom( "tf_story_name", _infoPanel );
    _cb_left_or_right = getCheckBoxFrom( "cb_left_or_right", _infoPanel );
    _addButton = getButtonFrom("button_add", _infoPanel);
    _okButton = getButtonFrom("button_ok", _infoPanel);
    _cancelButton = getButtonFrom("button_cancel", _infoPanel);
    _deleteButton = getButtonFrom("button_delete", _root);
    _sourceListView = BEFilterListView::create(Size(500, 700), nullptr, _filterTextField);
    _sourceListView->setDelegate(this);
    _sourceListView->setPosition(Vec2(0, 50));
    _storiesListView= BEDefaultListView::create(Size(500, 600), nullptr);
    _storiesListView->setPosition(Vec2(1100, 100));
    _root->addChild(_storiesListView);
    _listPanel->addChild(_sourceListView);
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
    _addButton->addClickEventListener(CC_CALLBACK_1(BEUIStoryAction::onAddButtonClicked, this));
    _deleteButton->addClickEventListener(CC_CALLBACK_1(BEUIStoryAction::onDeleteButtonClicked, this));
}

void BEUIStoryAction::reset() {
    _sourceListView->cleanAndReset();
    _storiesListView->cleanAndReset();
    _action = EditorStoryActionPtr(new EditorStoryAction());
    _currentStory = EditorStoryPtr(new EditorStory());
}

void BEUIStoryAction::loadStorySource(const std::vector<std::pair<std::string, std::string>>& sources) {
    _sources = sources;
    _sources.push_back( std::make_pair( "name_source", "goblin_mom" ) );
    _sourceListView->updateData();
}

EditorStoryActionPtr BEUIStoryAction::getAction() {
    _action->Name = _tf_story_name->getString();
    return _action;
}

std::string BEUIStoryAction::searchNameAtIndex(int index) {
    auto pair = _sources[index];
    return pair.second;
}

std::string BEUIStoryAction::displayNameAtIndex(int index) {
    auto pair = _sources[index];
    if (pair.first == "name_source") {
        return Utils::stringFormat("name:%s", pair.second.c_str());
    } else if (pair.first == "tag_source") {
        return Utils::stringFormat("tag:%s", pair.second.c_str());
    } else {
        return pair.second;
    }
}

int BEUIStoryAction::itemCount() {
    return (int)_sources.size();
}

void BEUIStoryAction::onAddButtonClicked(Ref *sender) {
    int index = _sourceListView->getCurrentIndexForFullItems();
    _currentStory->SpeakerName = _sources[index].second;
    _currentStory->Line = _tf_line->getString();
    _currentStory->LeftOrRight = _cb_left_or_right->isSelected();
    _action->Stories.push_back(_currentStory);
    _storiesListView->addItem(_currentStory->Line);
    _currentStory = EditorStoryPtr(new EditorStory());
}

void BEUIStoryAction::onDeleteButtonClicked(Ref *sender) {
    int index = _storiesListView->getCurrentIndex();
    _action->Stories.erase(_action->Stories.begin() + index);
    _storiesListView->removeCurrentIndex();
}

//BEUIConversationTrigger::BEUIConversationTrigger(Node *root, const BEPopupEventHandler& handler): BEUIBase(root, handler) {
//    _stateButton = getButtonFrom("button_state", _root);
//    _okButton = getButtonFrom("button_ok", _root);
//    _cancelButton = getButtonFrom("button_cancel", _root);
//    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getConversationState(), CC_CALLBACK_1(BEUIConversationTrigger::onStateItemClicked, this));
//    _stateListView->setVisible(false);
//    _conversationListView = BEDefaultListView::create(Size(400, 400), nullptr);
//    _conversationListView->setPosition(Vec2(0, 200));
//    _root->addChild(_conversationListView);
//    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUIConversationTrigger::onStateButtonClicked, this));
//    bindOKHandler(_okButton);
//    bindCancelHandler(_cancelButton);
//}
//
//void BEUIConversationTrigger::reset() {
//    _conversationListView->cleanAndReset();
//    _stateListView->reset();
//    _stateButton->setTitleText("conversation_start");
//}
//
//void BEUIConversationTrigger::loadConversations(const std::vector<std::string>& conversations) {
//    _conversationListView->addItems(conversations);
//}
//
//EditorConversationChangeTriggerPtr BEUIConversationTrigger::getTrigger() {
//    auto trigger = EditorConversationChangeTriggerPtr(new EditorConversationChangeTrigger());
//    trigger->ConversationName = _conversationListView->getCurrentName();
//    trigger->State = _stateListView->getCurrentType();
//    return trigger;
//}
//
//void BEUIConversationTrigger::onStateButtonClicked(Ref *sender) {
//    _stateListView->setVisible(true);
//}
//
//void BEUIConversationTrigger::onStateItemClicked(Ref *sender) {
//    _stateButton->setTitleText(_stateListView->getCurrentType());
//    _stateListView->setVisible(false);
//}

//story change trigger
BEUIStoryChangeTrigger::BEUIStoryChangeTrigger(cocos2d::Node *root, const BEPopupEventHandler& handler) :  BEUIBase(root, handler) {
    _stateButton = getButtonFrom("button_state", _root);
    _okButton = getButtonFrom("button_ok", _root);
    _cancelButton = getButtonFrom("button_cancel", _root);
    _stateListView = popupTypeListFromButton(_stateButton, EditorTypeManager::getInstance()->getStoryStateType(), CC_CALLBACK_1(BEUIStoryChangeTrigger::onStateItemClicked, this));
    _stateListView->setVisible(false);
    _storyListView = BEDefaultListView::create(Size(400, 400), nullptr);
    _storyListView->setPosition(Vec2(0, 200));
    _root->addChild(_storyListView);
    _stateButton->addClickEventListener(CC_CALLBACK_1(BEUIStoryChangeTrigger::onStateButtonClicked, this));
    bindOKHandler(_okButton);
    bindCancelHandler(_cancelButton);
}

void BEUIStoryChangeTrigger::reset() {
    _storyListView->cleanAndReset();
    _stateListView->reset();
    _stateButton->setTitleText("story_end");
}

void BEUIStoryChangeTrigger::loadStories(const std::vector<std::string>& stories) {
    _storyListView->addItems(stories);
}

EditorStoryChangeTriggerPtr BEUIStoryChangeTrigger::getTrigger() {
    auto trigger = EditorStoryChangeTriggerPtr(new EditorStoryChangeTrigger());
    trigger->StoryName = _storyListView->getCurrentName();
    trigger->State = _stateListView->getCurrentType();
    return trigger;
}

void BEUIStoryChangeTrigger::onStateButtonClicked(cocos2d::Ref *sender) {
    _stateListView->setVisible(true);
}

void BEUIStoryChangeTrigger::onStateItemClicked(cocos2d::Ref *sender) {
    _stateButton->setTitleText(_stateListView->getCurrentType());
    _stateListView->setVisible(false);
}


//editor main ui
BEEditorMainUI::BEEditorMainUI(Node *root, const BEPopupEventHandler& handler, const BEEditorCommandHandler& commandHandler):BEUIBase(root, handler) {
    _menuPanel = getPanelFrom("panel_menu", _root);
    _popupContainer = _root->getChildByName("node_popupContainer");
    _newPositionButton = getButtonFrom("button_newPosition", _menuPanel);
    _newEventButton = getButtonFrom("button_newEvent", _menuPanel);
    _listEventButton = getButtonFrom("button_listEvent", _menuPanel);
    _exitButton = getButtonFrom("button_exit", _menuPanel);
    _editConditionButton = getButtonFrom("button_editCondition", _menuPanel);
    _inputNamePanel = getPanelFrom(kInputNamePanel, _popupContainer);
    _createPositionPanel = getPanelFrom( kCreatePositionPanel, _popupContainer );
    _triggerOptionsPanel = getPanelFrom(kTriggerOptionsPanel, _popupContainer);
    _actionOptionsPanel = getPanelFrom(kActionOptionsPanel, _popupContainer);
    _unitActionPanel = getPanelFrom(kUnitActionPanel, _popupContainer);
    _unitTriggerPanel = getPanelFrom(kUnitTriggerPanel, _popupContainer);
    _unitstayTriggerPanel = getPanelFrom(kUnitStayTriggerPanel, _popupContainer);
    _eventChangePanel = getPanelFrom(kEventChangePanel, _popupContainer);
    _gameChangePanel = getPanelFrom(kGameChangePanel, _popupContainer);
    _taskChangePanel = getPanelFrom(kTaskChangePanel, _popupContainer);
    _eventListPanel = getPanelFrom(kEventListPanel, _popupContainer);
    _visionChangePanel = getPanelFrom(kVisionChangePanel, _popupContainer);
    _waveActionPanel = getPanelFrom(kWaveActionPanel, _popupContainer);
    _gameConditionPanel = getPanelFrom(kGameConditionPanel, _popupContainer);
    _conversationActionPanel = getPanelFrom(kConversationActionPanel, _popupContainer);
    _storyActionPanel = getPanelFrom( kStoryActionPanel, _popupContainer );
//    _conversationChangePanel = getPanelFrom(kConversationChangePanel, _popupContainer);
    _storyChangePanel = getPanelFrom( kStoryChangePanel, _popupContainer );
    _commandHandler = commandHandler;
    _newEventButton->addClickEventListener([this](Ref *sender) {
        this->_commandHandler(EditorCommandType::NewEvent, sender);
    });
    _newPositionButton->addClickEventListener([this](Ref *sender) {
        this->_commandHandler(EditorCommandType::NewPositon, sender);
    });
    _listEventButton->addClickEventListener([this](Ref *sender) {
        this->_commandHandler(EditorCommandType::ListEvent, sender);
    });
    _editConditionButton->addClickEventListener([this](Ref *sender) {
        this->_commandHandler(EditorCommandType::EditCondition, sender);
    });
    _exitButton->addClickEventListener([this](Ref *sender) {
        this->_commandHandler(EditorCommandType::Exit, sender);
    });
    
    inputNameUI = new BEUIInputName(_inputNamePanel, handler);
    createPositionUI = new BEUICreatePosition( _createPositionPanel, handler );
    triggerOptionsUI = new BEUITriggerOptions(_triggerOptionsPanel, handler);
    actionOptionsUI = new BEUIActionOptions(_actionOptionsPanel, handler);
    unitActionUI = new BEUIUnitAction(_unitActionPanel, handler);
    unitTriggerUI = new BEUIUnitTrigger(_unitTriggerPanel, handler);
    unitstayTriggerUI = new BEUIUnitStayTrigger(_unitstayTriggerPanel, handler);
    eventChangeUI = new BEUIEventChange(_eventChangePanel, handler);
    gameStateChangeUI = new BEUIGameStateChange(_gameChangePanel, handler);
    taskStateChangeUI = new BEUITaskStateChange(_taskChangePanel, handler);
    eventListUI = new BEUIEventList(_eventListPanel, handler);
    visionChangeUI = new BEUIVisionChange(_visionChangePanel, handler);
    waveActionUI = new BEUIWaveAction(_waveActionPanel, handler);
    gameConditionUI = new BEUIGameCondition(_gameConditionPanel, handler);
    conversationActionUI = new BEUIConversationAction(_conversationActionPanel, handler);
    storyActionUI = new BEUIStoryAction( _storyActionPanel, handler );
    storyChangeTriggerUI = new BEUIStoryChangeTrigger( _storyChangePanel, handler );
//    conversationChangeUI = new BEUIConversationTrigger(_conversationChangePanel, handler);
    hideAllPopups();
}

BEEditorMainUI::~BEEditorMainUI() {
    delete inputNameUI;
    delete createPositionUI;
    delete triggerOptionsUI;
    delete actionOptionsUI;
    delete unitActionUI;
    delete unitTriggerUI;
    delete unitstayTriggerUI;
    delete eventChangeUI;
    delete gameStateChangeUI;
    delete taskStateChangeUI;
    delete eventListUI;
    delete visionChangeUI;
    delete waveActionUI;
    delete gameConditionUI;
    delete conversationActionUI;
    delete storyActionUI;
    delete storyChangeTriggerUI;
//    delete conversationChangeUI;
}

void BEEditorMainUI::hideAllPopups() {
    inputNameUI->setVisible(false);
    createPositionUI->setVisible( false );
    triggerOptionsUI->setVisible(false);
    actionOptionsUI->setVisible(false);
    unitActionUI->setVisible(false);
    unitTriggerUI->setVisible(false);
    unitstayTriggerUI->setVisible( false );
    eventChangeUI->setVisible(false);
    gameStateChangeUI->setVisible(false);
    taskStateChangeUI->setVisible(false);
    eventListUI->setVisible(false);
    visionChangeUI->setVisible(false);
    waveActionUI->setVisible(false);
    gameConditionUI->setVisible(false);
    conversationActionUI->setVisible(false);
    storyActionUI->setVisible( false );
    storyChangeTriggerUI->setVisible( false );
//    conversationChangeUI->setVisible(false);
}

void BEEditorMainUI::reset() {
    hideAllPopups();
    _newEventButton->setTitleText("Create Event");
    _newPositionButton->setTitleText("Create Position");
    _listEventButton->setTitleText("Event List");
}

BEMainUI::BEMainUI(Node *root, const BEMainCommandHandler& handler): BEUIBase(root) {
    _handler = handler;
    _panel = getPanelFrom("root", _root);
    _loadButton = getButtonFrom("button_load", _panel);
    _editButton = getButtonFrom("button_edit", _panel);
    _playButton = getButtonFrom("button_play", _panel);
    _exitButton = getButtonFrom("button_exit", _panel);
    _mapPathTextField = getTextFieldFrom("input_mapPath", _panel);
#ifdef SUPPORT_EXTERNAL_MAP
	CSimpleIniA ini;
	if (ini.LoadFile(EDITOR_SETTING_PATH) == SI_OK)
	{
		const char* map_path = ini.GetValue("Setting", "MapPath");
		if (map_path)
		{
			_mapPathTextField->setString(map_path);
		}
	}
	else
	{
		ini.SaveFile(EDITOR_SETTING_PATH);
	}
#endif
    _mapListView = BEDefaultListView::create(Size(600, 800), nullptr);
    _mapListView->setPosition(Vec2(0, 100));
    _panel->addChild(_mapListView);
    _loadButton->addClickEventListener(CC_CALLBACK_1(BEMainUI::onLoadButtonClicked, this));
    _editButton->addClickEventListener(CC_CALLBACK_1(BEMainUI::onEditButtonClicked, this));
    _playButton->addClickEventListener(CC_CALLBACK_1(BEMainUI::onPlayButtonClicked, this));
    _exitButton->addClickEventListener(CC_CALLBACK_1(BEMainUI::onExitButtonClicked, this));
}

void BEMainUI::onLoadButtonClicked(Ref *sender) {
#ifdef SUPPORT_EXTERNAL_MAP
    _mapFolders.clear();
    _mapListView->removeAllItems();
    auto path = _mapPathTextField->getString();
    fs::path rootFolder = fs::path(path);
    std::vector<std::string> names;
    if (fs::exists(rootFolder) && fs::is_directory(rootFolder)) {

		CSimpleIniA ini;
		if (ini.LoadFile(EDITOR_SETTING_PATH) == SI_OK)
		{
			ini.SetValue("Setting", "MapPath", path.c_str());
			ini.SaveFile(EDITOR_SETTING_PATH);
		}

        fs::directory_iterator endIt;
        for (fs::directory_iterator it(rootFolder); it != endIt; ++it) {
            if (fs::is_directory(it->status())) {
                names.push_back(it->path().filename().string());
                _mapFolders.push_back(std::make_pair(it->path().string(), it->path().filename().string()));
            }
        }
    }
    if (_mapFolders.size() > 0) {
        _mapListView->addItems(names);
    }
#endif
}

void BEMainUI::onExitButtonClicked(Ref *sender) {
    _handler(MainCommandType::Exit, "", sender);
}

void BEMainUI::onEditButtonClicked(Ref *sender) {
    int index = _mapListView->getCurrentIndex();
    if (index >= 0 ) {
        std::string path = _mapFolders[index].first;
        _handler(MainCommandType::EditMap, path, sender);
    }
}

void BEMainUI::onPlayButtonClicked(Ref *sender) {
    int index = _mapListView->getCurrentIndex();
    if (index >= 0 ) {
        std::string path = _mapFolders[index].first;
        _handler(MainCommandType::PlayMap, path, sender);
    }
}
