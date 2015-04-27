//
//  BEUI.h
//  Boids
//
//  Created by Xin Xu on 11/11/14.
//
//

#ifndef __Boids__BEUI__
#define __Boids__BEUI__

#include "BEUIBase.h"
#include "../EditorModel.h"
#include "../EditorModelDefines.h"

class BEUIInputName : public BEUIBase {
public:
    explicit BEUIInputName(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    void setShowTagInput(bool showTagInput);
    virtual void reset();
    const std::string& getInputName() const;
    const std::string& getInputTag() const;
protected:
    cocos2d::ui::Button *_ok;
    cocos2d::ui::Button *_cancel;
    cocos2d::ui::TextField *_name;
    cocos2d::ui::TextField *_tag;
};

class BEUICreatePosition : public BEUIInputName {
public:
    explicit BEUICreatePosition(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    const std::string& getPrevPosName() const;
    
    virtual void didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender);
    
private:
    cocos2d::ui::Button* _btn_prev_pos;
    cocos2d::ui::TextField* _tf_prev_pos;
};

class BEUITriggerOptions : public BEUIBase {
public:
    explicit BEUITriggerOptions(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    bool isRepeat() const;
    bool isEnabled();
    std::string getRelationType() const;
    const std::string& getType();
    void setMetaUIVisible(bool isVisible);
private:
    cocos2d::ui::Button *_ok;
    cocos2d::ui::Button *_done;
    cocos2d::ui::Button *_cancel;
    cocos2d::ui::Button *_relationButton;
    cocos2d::ui::CheckBox *_isRepeat;
    cocos2d::ui::CheckBox *_cb_enabled;
    cocos2d::ui::Text *_isRepeatLabel;
    BETypeListView *_list;
    BETypeListView *_relationList;
    void onRelationClicked(cocos2d::Ref *sender);
    void onRelationItemClicked(cocos2d::Ref *sender);
};

class BEUIActionOptions : public BEUIBase {
public:
    explicit BEUIActionOptions(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    const std::string& getType();
    EditorActionMetaPtr getMeta();
private:
    cocos2d::ui::Button *_ok;
    cocos2d::ui::Button *_done;
    cocos2d::ui::Button *_cancel;
    cocos2d::ui::CheckBox *_isInfinite;
    cocos2d::ui::TextField *_times;
    cocos2d::ui::TextField *_inteval;
    cocos2d::ui::TextField *_delay;
    BETypeListView *_list;
};

class BEUIUnitAction : public BEUIBase, public BEFilterListViewDelegate {
public:
    explicit BEUIUnitAction(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadSourceList(const std::vector<std::pair<std::string, std::string>>& sources);
    void loadPositionGroups(const std::vector<std::string>& positionGroups);
    const std::vector<EditorUnitActionPtr>& getActions();
    virtual void didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender);
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
    
    // for edit mode
    void loadEvent(EditorEventPtr event);
    void saveEvent();
    
private:
    class UnitActionEditData {
    public:
        EditorUnitActionPtr originalAction;
        EditorUnitActionPtr newAction;
    };
    cocos2d::ui::Button *_typeButton;
    cocos2d::ui::Text *_typeTileLabel;
    cocos2d::ui::Button *_stateButton;
    cocos2d::ui::Text *_stateTitleLabel;
    cocos2d::ui::Button *_newPositionButton;
    cocos2d::ui::Button *_selectPositionButton;
    cocos2d::ui::Button *_selectPositionGroupButton;
    cocos2d::ui::Text *_positionLabel;
    cocos2d::ui::Text *_showHPTitleLabel;
    cocos2d::ui::CheckBox *_showHPCheckBox;
    cocos2d::ui::CheckBox *_changeTypeCheckBox;
    cocos2d::ui::CheckBox *_changeStateCheckBox;
    cocos2d::ui::Button *_ok;
    cocos2d::ui::Button *_cancel;
    cocos2d::ui::Text *_countLabel;
    cocos2d::ui::TextField* _levelTextField;
    cocos2d::ui::TextField *_countTextField;
    cocos2d::ui::CheckBox *_tagCheckBox;
    cocos2d::ui::Text *_tagLabel;
    cocos2d::ui::TextField *_tagTextField;
    cocos2d::ui::CheckBox *_buffCheckBox;
    cocos2d::ui::Text *_buffLabel;
    cocos2d::ui::TextField* _tf_buff_name;
    cocos2d::ui::TextField* _tf_buff_type;
    cocos2d::ui::TextField* _tf_buff_params;
    
    //skill
    cocos2d::ui::CheckBox* _cb_skill_1;
    cocos2d::ui::CheckBox* _cb_skill_2;
    cocos2d::ui::TextField* _tf_skill_1_level;
    cocos2d::ui::TextField* _tf_skill_2_level;
    
    //item
    cocos2d::ui::CheckBox* _cb_item;
    cocos2d::ui::CheckBox* _cb_item_get_or_lose;
    cocos2d::ui::TextField* _tf_item_name;
    cocos2d::ui::TextField* _tf_item_resource;
    
    cocos2d::ui::Button *_addButton;
    cocos2d::ui::Button *_deleteButton;
    cocos2d::ui::TextField *_filterTextField;
    cocos2d::ui::CheckBox *_bossCheckBox;
    cocos2d::ui::Text *_bossLabel;
    cocos2d::ui::TextField *_classTextField;
    cocos2d::ui::TextField *_customTextField;
    cocos2d::ui::Button *_popupButton;
    BEFilterListView *_groupListView;
    BEListView *_doneListView;
    BEListView *_positionGroupListView;
    BETypeListView *_typeListView;
    BETypeListView *_stateListView;
    BETypeListView *_popupListView;
    cocos2d::ui::Layout *_infoPanel;
    cocos2d::ui::Layout *_groupListPanel;
    std::vector<std::pair<std::string, std::string>> _sourceList;
    EditorUnitActionPtr _action;
    EditorUnitTrigger _trigger;
    std::vector<EditorUnitActionPtr> _actions;
    std::string _positionName;
    std::string _positionTag;
    bool _editMode;
    
    // for edit mode
    std::vector<EditorUnitActionPtr> _editableActions;
    std::vector<UnitActionEditData> _editDataList;
    EditorEventPtr _editEvent;
    void renderAction(EditorUnitActionPtr action);
    // end
    
    void adjustUIForState(const std::string& state);
    void onTypeButtonClicked(cocos2d::Ref *sender);
    void onStateButtonClicked(cocos2d::Ref *sender);
    void togglePositionUI(bool visible);
    void toggleCountUI(bool visible);
    void toggleBossUI(bool visible);
    void onAddButtonClicked(cocos2d::Ref *sender);
    void onDeleteButtonClicked(cocos2d::Ref *sender);
    void onTypeListItemClicked(cocos2d::Ref *sender);
    void onStateListItemClicked(cocos2d::Ref *sender);
    void onSelectPositionGroupButtonClicked(cocos2d::Ref *sender);
    void onPositionGroupItemClicked(cocos2d::Ref *sender);
    void onGroupItemClicked(cocos2d::Ref *sender);
    void onPopupItemClicked(cocos2d::Ref *sender);
    void onPopupButtonClicked(cocos2d::Ref *sender);
};

class BEUIUnitTrigger : public BEUIBase, public BEFilterListViewDelegate {
public:
    explicit BEUIUnitTrigger(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadSourceList(const std::vector<std::pair<std::string, std::string>>& sources);
    EditorUnitTriggerPtr getTrigger();
    virtual void didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender);
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
    
private:
    cocos2d::ui::Button *_stateButton;
    cocos2d::ui::Text *_stateTitleLabel;
    cocos2d::ui::Text *_positionTitleLabel1;
    cocos2d::ui::Button *_newPositionButton1;
    cocos2d::ui::Button *_selectPositionButton1;
    cocos2d::ui::Text *_positionLabel1;
    cocos2d::ui::Button *_ok;
    cocos2d::ui::Button *_cancel;
    cocos2d::ui::TextField *_triggerCountTextField;
    cocos2d::ui::TextField *_filterTextField;
    cocos2d::ui::TextField* _lb_tag;
    BEFilterListView *_groupListView;
    BETypeListView *_stateListView;
    cocos2d::ui::Layout *_infoPanel;
    cocos2d::ui::Layout *_groupListPanel;
    std::vector<std::pair<std::string, std::string>> _sourceList;
    EditorUnitTriggerPtr _trigger;
    std::string _positionName;
    
    void onStateButtonClicked(cocos2d::Ref *sender);
    void onStateListItemClicked(cocos2d::Ref *sender);
    void togglePositionUI(bool visible);
};

class BEUIUnitStayTrigger : public BEUIBase, public BEFilterListViewDelegate {
public:
    explicit BEUIUnitStayTrigger(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    EditorUnitStayTriggerPtr getTrigger();
    virtual void didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender);
    
    void loadSourceList(const std::vector<std::pair<std::string, std::string>>& sources);
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
    
private:
    cocos2d::ui::Button* _stateButton;
    cocos2d::ui::Text* _lb_pos_name;
    cocos2d::ui::Button* _btn_new_pos;
    cocos2d::ui::Button* _btn_select_pos;
    cocos2d::ui::Button* _btn_ok;
    cocos2d::ui::Button* _btn_cancel;
    cocos2d::ui::TextField* _tf_tag;
    cocos2d::ui::TextField* _tf_count;
    cocos2d::ui::TextField* _tf_duration;
    cocos2d::ui::Layout *_infoPanel;
    EditorUnitStayTriggerPtr _trigger;
    std::string _positionName;
    
    cocos2d::ui::TextField *_filterTextField;
    cocos2d::ui::Layout *_groupListPanel;
    BEFilterListView *_groupListView;
    std::vector<std::pair<std::string, std::string>> _sourceList;
    
    void togglePositionUI(bool visible);
};

class BEUIEventChange : public BEUIBase {
public:
    explicit BEUIEventChange(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadEventList(const std::vector<EditorEventPtr>& eventList);
    std::string getEventName();
    std::string getEventState();
    void setIsTrigger(bool isTrigger);
private:
    cocos2d::ui::Button *_eventTypeButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    BEListView *_eventListView;
    BETypeListView *_eventTypeListView;
    std::vector<EditorEventPtr> _eventList;
    
    void onEventTypeButtonClicked(cocos2d::Ref *sender);
    void onEventTypeItemClicked(cocos2d::Ref *sender);
    
};

class BEUIGameStateChange : public BEUIBase {
public:
    explicit BEUIGameStateChange(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    EditorGameActionPtr getAction();
    EditorGameTriggerPtr getTrigger();
private:
    cocos2d::ui::Button *_stateButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    BETypeListView *_stateListView;
    
    void onStateButtonClicked(cocos2d::Ref *sender);
    void onStateItemClicked(cocos2d::Ref *sender);
};

class BEUITaskStateChange : public BEUIBase {
public:
    explicit BEUITaskStateChange(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadTaskList(const std::vector<EditorGameConditionPtr>& taskList);
    EditorTaskActionPtr getAction();
private:
    cocos2d::ui::Button *_stateButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    BETypeListView *_stateListView;
    BEListView *_taskListView;
    EditorTaskActionPtr _action;
    
    void onStateButtonClicked(cocos2d::Ref *sender);
    void onStateListItemClicked(cocos2d::Ref *sender);
};

class BEUIEventList : public BEUIBase {
public:
    explicit BEUIEventList(cocos2d::ui::Layout *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadEventList(const std::vector<EditorEventPtr>& eventList);
    std::vector<int>& getDeletedEventIndexs() { return _deletedEventIndexs; }
    EditorEventPtr getCurrentEvent();
private:
    cocos2d::ui::Button *_editButton;
    cocos2d::ui::Button *_deleteButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    BEListView *_eventListView;
    std::vector<int> _deletedEventIndexs;
    std::vector<EditorEventPtr> _eventList;
    
    void onDeleteButtonClicked(cocos2d::Ref *sender);
};

class BEUIVisionChange : public BEUIBase, public BEFilterListViewDelegate {
public:
    BEUIVisionChange(cocos2d::Node *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadVisionObjects(const std::vector<std::pair<std::string, std::string>>& visionObjects);
    EditorVisionActionPtr getAction();
    EditorVisionTriggerPtr getTrigger();
private:
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    cocos2d::ui::Button *_stateButton;
    BETypeListView *_stateListView;
    std::vector<std::pair<std::string, std::string>> _sourceList;
    
    cocos2d::ui::Layout *_groupListPanel;
    cocos2d::ui::TextField *_filterTextField;
    BEFilterListView *_groupListView;
    
    void onStateButtonClicked(cocos2d::Ref *sender);
    void onStateItemClicked(cocos2d::Ref *sender);
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
};

class BEUIWaveAction : public BEUIBase, public BEFilterListViewDelegate {
public:
    BEUIWaveAction(cocos2d::Node *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadUnitList(const std::vector<std::string>& unitList);
    void loadPositionGroups(const std::vector<std::string>& groups);
    EditorWaveActionPtr getAction() { return _action; }
    virtual void didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender);
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
    
private:
    cocos2d::ui::Layout *_infoPanel;
    cocos2d::ui::Layout *_unitListPanel;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    cocos2d::ui::Button *_addButton;
    cocos2d::ui::Button *_addWaveButton;
    cocos2d::ui::Button *_deleteWaveButton;
    cocos2d::ui::TextField *_filterTextField;
    cocos2d::ui::TextField *_numberTextField;
    cocos2d::ui::TextField *_levelTextField;
    cocos2d::ui::CheckBox *_bossCheckBox;
    cocos2d::ui::Button *_newPositionButton;
    cocos2d::ui::Button *_choosePositionButton;
    cocos2d::ui::Button *_choosePositionGroupButton;
    cocos2d::ui::Text *_positionLabel;
    cocos2d::ui::TextField *_intervalTextField;
    cocos2d::ui::TextField *_classTextField;
    BEFilterListView *_unitListView;
    BEListView *_chosenUnitListView;
    BEListView *_positionGroupListView;
    BEListView *_waveListView;
    std::vector<std::string> _unitList;
    std::string _positionName;
    std::string _positionTag;
    EditorWaveActionPtr _action;
    EditorWavePtr _currentWave;
    void onPositionGroupItemClicked(cocos2d::Ref *sender);
    void onPositionGroupButtonClicked(cocos2d::Ref *sender);
    void onAddButtonClicked(cocos2d::Ref *sender);
    void onAddWaveButtonClicked(cocos2d::Ref *sender);
    void onDeleteWaveButtonClicked(cocos2d::Ref *sender);
};

class BEUIGameCondition : public BEUIBase {
public:
    BEUIGameCondition(cocos2d::Node *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadConditions(const std::vector<EditorGameConditionPtr>& conditions);
    std::vector<EditorGameConditionPtr> getConditions() { return _conditions; }
private:
    cocos2d::ui::Button *_chooseTypeButton;
    cocos2d::ui::TextField *_nameTextField;
    cocos2d::ui::TextField *_titleTextField;
    cocos2d::ui::TextField *_descTextField;
    cocos2d::ui::Button *_addButton;
    cocos2d::ui::Button *_deleteButton;
    BEListView *_resultListView;
    BETypeListView *_typeListView;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    void onAddButtonClicked(cocos2d::Ref *sender);
    void onChooseTypeButtonClicked(cocos2d::Ref *sender);
    void onConditionItemClicked(cocos2d::Ref *sender);
    void onTypeItemClicked(cocos2d::Ref *sender);
    void onDeleteButtonClicked(cocos2d::Ref *sender);
    EditorGameConditionPtr _condition;
    std::vector<EditorGameConditionPtr> _conditions;
};

class BEUIConversationAction : public BEUIBase, public BEFilterListViewDelegate {
public:
    BEUIConversationAction(cocos2d::Node *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadConversationSource(const std::vector<std::pair<std::string, std::string>>& sources);
    EditorConversationActionPtr getAction();
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
private:
    
    // model
    EditorConversationActionPtr _action;
    EditorSpeechPtr _currentSpeech;
    std::vector<std::pair<std::string, std::string>> _sources;
    
    // ui controls
    cocos2d::ui::Layout *_listPanel;
    cocos2d::ui::Layout *_infoPanel;
    cocos2d::ui::TextField *_filterTextField;
    cocos2d::ui::TextField *_contentTextField;
    cocos2d::ui::TextField* _tf_duration;
    cocos2d::ui::TextField* _tf_tag;
    cocos2d::ui::TextField* _tf_interval;
    cocos2d::ui::TextField* _tf_repeat;
    cocos2d::ui::CheckBox* _cb_random;
    cocos2d::ui::TextField *_nameTextField;
    cocos2d::ui::Button *_addButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    cocos2d::ui::Button *_deleteButton;
    BEFilterListView *_sourceListView;
    BEDefaultListView *_speechListView;
    void onAddButtonClicked(cocos2d::Ref *sender);
    void onDeleteButtonClicked(cocos2d::Ref *sender);
};

class BEUIStoryAction : public BEUIBase, public BEFilterListViewDelegate {
public:
    BEUIStoryAction(cocos2d::Node *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadStorySource(const std::vector<std::pair<std::string, std::string>>& sources);
    EditorStoryActionPtr getAction();
    
    virtual std::string searchNameAtIndex(int index);
    virtual std::string displayNameAtIndex(int index);
    virtual int itemCount();
private:
    
    // model
    EditorStoryActionPtr _action;
    EditorStoryPtr _currentStory;
    std::vector<std::pair<std::string, std::string>> _sources;
    
    // ui controls
    cocos2d::ui::Layout *_listPanel;
    cocos2d::ui::Layout *_infoPanel;
    cocos2d::ui::TextField *_filterTextField;
    cocos2d::ui::TextField *_contentTextField;
    
    cocos2d::ui::TextField* _tf_story_name;
    
    cocos2d::ui::TextField* _tf_line;
    cocos2d::ui::CheckBox* _cb_left_or_right;
    
    cocos2d::ui::Button *_addButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    cocos2d::ui::Button *_deleteButton;
    
    BEFilterListView *_sourceListView;
    
    BEDefaultListView *_storiesListView;
    void onAddButtonClicked(cocos2d::Ref *sender);
    void onDeleteButtonClicked(cocos2d::Ref *sender);
};


//class BEUIConversationTrigger : public BEUIBase {
//public:
//    BEUIConversationTrigger(cocos2d::Node *root, const BEPopupEventHandler& handler);
//    virtual void reset();
//    void loadConversations(const std::vector<std::string>& conversations);
//    EditorConversationChangeTriggerPtr getTrigger();
//private:
//    cocos2d::ui::Button *_stateButton;
//    cocos2d::ui::Button *_okButton;
//    cocos2d::ui::Button *_cancelButton;
//    BETypeListView *_stateListView;
//    BEListView *_conversationListView;
//    
//    void onStateButtonClicked(cocos2d::Ref *sender);
//    void onStateItemClicked(cocos2d::Ref *sender);
//};

class BEUIStoryChangeTrigger : public BEUIBase {
public:
    BEUIStoryChangeTrigger(cocos2d::Node *root, const BEPopupEventHandler& handler);
    virtual void reset();
    void loadStories(const std::vector<std::string>& conversations);
    EditorStoryChangeTriggerPtr getTrigger();
private:
    cocos2d::ui::Button *_stateButton;
    cocos2d::ui::Button *_okButton;
    cocos2d::ui::Button *_cancelButton;
    BETypeListView *_stateListView;
    BEListView *_storyListView;

    void onStateButtonClicked(cocos2d::Ref *sender);
    void onStateItemClicked(cocos2d::Ref *sender);
};

class BEEditorMainUI : public BEUIBase {
public:
    BEEditorMainUI(cocos2d::Node *root, const BEPopupEventHandler& handler, const BEEditorCommandHandler& commandHandler);
    virtual ~BEEditorMainUI();
    BEUIInputName *inputNameUI;
    BEUICreatePosition* createPositionUI;
    BEUITriggerOptions *triggerOptionsUI;
    BEUIActionOptions *actionOptionsUI;
    BEUIUnitAction *unitActionUI;
    BEUIUnitTrigger *unitTriggerUI;
    BEUIUnitStayTrigger* unitstayTriggerUI;
    BEUIEventChange *eventChangeUI;
    BEUIGameStateChange *gameStateChangeUI;
    BEUITaskStateChange *taskStateChangeUI;
    BEUIEventList *eventListUI;
    BEUIVisionChange *visionChangeUI;
    BEUIWaveAction *waveActionUI;
    BEUIGameCondition *gameConditionUI;
    BEUIConversationAction *conversationActionUI;
    BEUIStoryAction* storyActionUI;
//    BEUIConversationTrigger *conversationChangeUI;
    BEUIStoryChangeTrigger* storyChangeTriggerUI;
    virtual void reset();
    void hideAllPopups();
    
private:
    cocos2d::ui::Layout *_menuPanel;
    cocos2d::Node *_popupContainer;
    
    cocos2d::ui::Button *_newPositionButton;
    cocos2d::ui::Button *_newEventButton;
    cocos2d::ui::Button *_listEventButton;
    cocos2d::ui::Button *_exitButton;
    cocos2d::ui::Button *_editConditionButton;
    
    cocos2d::ui::Layout *_inputNamePanel;
    cocos2d::ui::Layout *_createPositionPanel;
    cocos2d::ui::Layout *_triggerOptionsPanel;
    cocos2d::ui::Layout *_actionOptionsPanel;
    cocos2d::ui::Layout *_unitActionPanel;
    cocos2d::ui::Layout *_unitTriggerPanel;
    cocos2d::ui::Layout *_unitstayTriggerPanel;
    cocos2d::ui::Layout *_eventChangePanel;
    cocos2d::ui::Layout *_gameChangePanel;
    cocos2d::ui::Layout *_taskChangePanel;
    cocos2d::ui::Layout *_eventListPanel;
    cocos2d::ui::Layout *_visionChangePanel;
    cocos2d::ui::Layout *_waveActionPanel;
    cocos2d::ui::Layout *_gameConditionPanel;
    cocos2d::ui::Layout *_conversationActionPanel;
    cocos2d::ui::Layout *_storyActionPanel;
    cocos2d::ui::Layout *_storyChangePanel;
    
    BEEditorCommandHandler _commandHandler;
};

class BEMainUI : public BEUIBase {
public:
    BEMainUI(cocos2d::Node *root, const BEMainCommandHandler& commandHandler);
private:
    cocos2d::ui::Layout *_panel;
    cocos2d::ui::Button *_loadButton;
    cocos2d::ui::Button *_editButton;
    cocos2d::ui::Button *_playButton;
    cocos2d::ui::Button *_exitButton;
    cocos2d::ui::TextField *_mapPathTextField;
    BEListView *_mapListView;
    BEMainCommandHandler _handler;
    std::vector<std::pair<std::string, std::string>> _mapFolders;
    
    void onLoadButtonClicked(cocos2d::Ref *sender);
    void onEditButtonClicked(cocos2d::Ref *sender);
    void onPlayButtonClicked(cocos2d::Ref *sender);
    void onExitButtonClicked(cocos2d::Ref *sender);
};


#endif /* defined(__Boids__BEUI__) */
