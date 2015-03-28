//
//  BEUIBase.h
//  Boids
//
//  Created by Xin Xu on 11/8/14.
//
//

#ifndef __Boids__BEUIBase__
#define __Boids__BEUIBase__

#include "BEHeaders.h"
#include "BETypes.h"
#include "EditorModel.h"
#include "EditorModelDefines.h"

extern const char *kInputNamePanel;
extern const char *kTriggerOptionsPanel;
extern const char *kActionOptionsPanel;
extern const char *kUnitActionPanel;
extern const char *kUnitTriggerPanel;
extern const char* kUnitStayTriggerPanel;
extern const char *kEventChangePanel;
extern const char *kGameChangePanel;
extern const char *kTaskChangePanel;
extern const char *kNewGroupPanel;
extern const char *kEventListPanel;
extern const char *kEventEditPanel;
extern const char *kVisionChangePanel;
extern const char *kWaveActionPanel;
extern const char *kGameConditionPanel;
extern const char *kConversationActionPanel;
extern const char *kConversationChangePanel;

enum class EditorPopupEventType {
    OK = 0,
    Cancel,
    AnotherTrigger,
    AnotherAction,
    NewPosition,
    SelectPosition,
    EditEvent,
    PopupEventTypeMax
};

enum class EditorCommandType {
    NewPositon = 0,
    NewEvent,
    ListEvent,
    EditCondition,
    Exit
};

enum class MainCommandType {
    EditMap,
    PlayMap,
    Exit
};

typedef std::function<void(cocos2d::Ref *)> BEClickHandler;
typedef std::function<void(EditorPopupEventType, class BEUIBase *, cocos2d::Ref *)> BEPopupEventHandler;
typedef std::function<void(EditorCommandType, cocos2d::Ref *)> BEEditorCommandHandler;
typedef std::function<void(MainCommandType, const std::string&, cocos2d::Ref *)> BEMainCommandHandler;

class BEListButton : public cocos2d::ui::Button {
public:
    static BEListButton *create(const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const std::string& name, int index, cocos2d::Size size, float fontSize);
    bool init(const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const std::string& name, int index,
              cocos2d::Size size, float fontSize);
    int getItemIndex() const { return _index; }
    void setItemIndex(int index) { _index = index; }
    const std::string& getItemName() const { return _name; }
    void setSelected(bool selected);
private:
    cocos2d::Color3B _titleColor;
    cocos2d::Color3B _hlColor;
    std::string _name;
    int _index;
    bool _selected;
};

class BEListView : public cocos2d::ui::ListView {
public:
    static BEListView *create(cocos2d::Size contentSize, cocos2d::Size buttonSize, float fontSize, float margin, const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const BEClickHandler& buttonClickHandler);
    bool init(cocos2d::Size contentSize, cocos2d::Size buttonSize, float fontSize, float margin, const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const BEClickHandler& buttonClickHandler);
    void addItem(const std::string& name);
    virtual void addItems(const std::vector<std::string>& names);
    int getCurrentIndex() { return _currentIndex; }
    const std::string& getCurrentName();
    void reset();
    void cleanAndReset();
    void selectIndex(int index);
    void removeIndex(int index);
    void removeCurrentIndex();
protected:
    cocos2d::Color3B _titleColor;
    cocos2d::Color3B _hlColor;
    int _currentIndex;
    float _fontSize;
    cocos2d::Size _buttonSize;
    BEClickHandler _itemClickedHandler;
};

class BEDefaultListView : public BEListView {
public:
    static BEDefaultListView *create(cocos2d::Size contentSize, const BEClickHandler& clickHandler);
    bool init(cocos2d::Size contentSize, const BEClickHandler& clickHandler);
};

class BETypeListView : public BEDefaultListView {
public:
    static BETypeListView *create(cocos2d::Size contentSize, EditorTypePtr editorType, const BEClickHandler& clickHandler);
    bool init(cocos2d::Size contentSize, EditorTypePtr editorType, const BEClickHandler& clickHandler);
    EditorTypePtr getEditorType() { return _editorType; }
    const std::string& getCurrentType() { return _editorType->getTypeVector()[_currentIndex]; }
protected:
    EditorTypePtr _editorType;
};

class BEFilterListViewDelegate {
public:
    virtual std::string searchNameAtIndex(int index) = 0;
    virtual std::string displayNameAtIndex(int index) = 0;
    virtual int itemCount() = 0;
};

class BEFilterListView : public BEDefaultListView {
public:
    BEFilterListView(): _delegate(nullptr) {}
    static BEFilterListView *create(cocos2d::Size contentSize, const BEClickHandler& clickHandler, cocos2d::ui::TextField *filterField);
    bool init(cocos2d::Size contentSize, const BEClickHandler& clickHandler, cocos2d::ui::TextField *filterField);
    void setDelegate(BEFilterListViewDelegate *delegate);
    void updateData();
    int getCurrentIndexForFullItems();
protected:
    BEFilterListViewDelegate *_delegate;
    cocos2d::ui::TextField *_filterField;
    std::vector<int> _currentDisplayIndexes;
    void onFilterTextChanged(cocos2d::Ref *sender, cocos2d::ui::TextField::EventType et);
};

class BEUIBase {
public:
    BEUIBase() = delete;
    explicit BEUIBase(cocos2d::Node *root);
    explicit BEUIBase(cocos2d::Node *root, const BEPopupEventHandler& handler);
    cocos2d::ui::Layout *getPanelFrom(const std::string& name, cocos2d::Node *parent);
    cocos2d::ui::Button *getButtonFrom(const std::string& name, cocos2d::Node *parent);
    cocos2d::ui::CheckBox *getCheckBoxFrom(const std::string& name, cocos2d::Node *parent);
    cocos2d::ui::TextField *getTextFieldFrom(const std::string& name, cocos2d::Node *parent);
    cocos2d::ui::Text *getLabelFrom(const std::string& name, cocos2d::Node *parent);
    cocos2d::ui::ListView *getListViewFrom(const std::string& name, cocos2d::Node *parent);
    virtual void didGetPosition(EditorPositionPtr pos, cocos2d::Ref *sender) {}
    void setButtonEnabled(cocos2d::ui::Button *btn, bool enabled);
    BETypeListView *popupTypeListFromButton(cocos2d::ui::Button *btn, EditorTypePtr editorType, const BEClickHandler& listItemClickedHandler);
    BEListView *popupListFromButton(cocos2d::ui::Button *btn, const BEClickHandler& handler);
    void setVisible(bool visible);
    virtual void reset() {}
    std::string getRootName();
protected:
    cocos2d::Node *_root;
    BEPopupEventHandler _eventHandler;
    
    void bindOKHandler(cocos2d::ui::Button *okButton);
    void bindCancelHandler(cocos2d::ui::Button *cancelButton);
    void bindNewPositionHandler(cocos2d::ui::Button *newPositionButton);
    void bindSelectPositionHandler(cocos2d::ui::Button *selectPositionButton);
    void bindButton(cocos2d::ui::Button *btn, EditorPopupEventType et);
};

#endif /* defined(__Boids__BEUIBase__) */
