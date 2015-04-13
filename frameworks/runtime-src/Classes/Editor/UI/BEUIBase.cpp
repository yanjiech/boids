//
//  BEUIBase.cpp
//  Boids
//
//  Created by Xin Xu on 11/8/14.
//
//

#include "BEUIBase.h"

using namespace cocos2d;
using namespace cocostudio;

const char *kInputNamePanel = "popup_inputName";
const char *kCreatePositionPanel = "popup_new_position";
const char *kTriggerOptionsPanel = "popup_triggerOptions";
const char *kActionOptionsPanel = "popup_actionOptions";
const char *kUnitActionPanel = "popup_unitAction";
const char *kUnitTriggerPanel = "popup_unitTrigger";
const char *kUnitStayTriggerPanel = "popup_unitstayTrigger";
const char *kEventChangePanel = "popup_eventChange";
const char *kGameChangePanel = "popup_gameChange";
const char *kTaskChangePanel = "popup_taskChange";
const char *kNewGroupPanel = "popup_newGroup";
const char *kEventListPanel = "popup_eventList";
const char *kEventEditPanel = "popup_editEvent";
const char *kVisionChangePanel = "popup_visionChange";
const char *kWaveActionPanel = "popup_waveAction";
const char *kGameConditionPanel = "popup_gameCondition";
const char *kConversationActionPanel = "popup_conversationAction";
const char* kStoryActionPanel = "popup_storyAction";
//const char *kConversationChangePanel = "popup_conversationChange";

#define LIST_BUTTON_HEIGHT    60
#define LIST_BUTTON_MARGIN    20
#define LIST_BUTTON_FONTSIZE  20
#define LIST_BUTTON_COLOR     (Color3B(0, 179, 255))
#define LIST_BUTTON_HLCOLOR   (Color3B::RED)

BEListButton *BEListButton::create(const Color3B& titleColor, const Color3B& hlColor, const std::string& name, int index, Size size, float fontSize) {
    auto ret = new BEListButton();
    if (ret->init(titleColor, hlColor, name, index, size, fontSize)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BEListButton::init(const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const std::string& name, int index, cocos2d::Size size, float fontSize) {
    if (!ui::Button::init()) {
        return false;
    }
    this->setContentSize(size);
    this->setTitleText(name);
    this->setTitleColor(titleColor);
    this->setTitleFontSize(fontSize);
    _titleColor = titleColor;
    _hlColor = hlColor;
    _index = index;
    _name = name;
    _selected = false;
    return true;
}

void BEListButton::setSelected(bool selected) {
    _selected = selected;
    if (selected) {
        this->setTitleColor(_hlColor);
    } else {
        this->setTitleColor(_titleColor);
    }
}

BEListView *BEListView::create(cocos2d::Size contentSize, cocos2d::Size buttonSize, float fontSize, float margin, const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const BEClickHandler& buttonClickHandler) {
    auto ret = new BEListView();
    if (ret->init(contentSize, buttonSize, fontSize, margin, titleColor, hlColor, buttonClickHandler)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BEListView::init(cocos2d::Size contentSize, cocos2d::Size buttonSize, float fontSize, float margin, const cocos2d::Color3B& titleColor, const cocos2d::Color3B& hlColor, const BEClickHandler& buttonClickHandler) {
    if (!ui::ListView::init()) {
        return false;
    }
    _titleColor = titleColor;
    _hlColor = hlColor;
    _buttonSize = buttonSize;
    _fontSize = fontSize;
    _currentIndex = -1;
    _itemClickedHandler = buttonClickHandler;
    this->setContentSize(contentSize);
    this->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->setTouchEnabled(true);
    this->setItemsMargin(margin);
    this->setAnchorPoint(Vec2::ZERO);
    this->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    return true;
}

void BEListView::addItem(const std::string& name) {
    int index = getItems().size();
    auto btn = BEListButton::create(_titleColor, _hlColor, name, index, _buttonSize, _fontSize);
    if (index == 0) {
        btn->setSelected(true);
        _currentIndex = 0;
    }
    btn->addClickEventListener([this](Ref *sender) {
        for (auto widget : this->getItems()) {
            auto btn = (BEListButton *)widget;
            btn->setSelected(btn == sender);
        }
        auto b = (BEListButton *)sender;
        this->_currentIndex = b->getItemIndex();
        if (_itemClickedHandler != nullptr) {
            _itemClickedHandler(sender);
        }
    });
    this->pushBackCustomItem(btn);
}

void BEListView::addItems(const std::vector<std::string>& names) {
    for (auto& name : names) {
        addItem(name);
    }
}

void BEListView::selectIndex(int index) {
    int itemCount = this->getItems().size();
    if (index >= 0 && index < itemCount) {
        for (int i = 0; i < itemCount; ++i) {
            auto btn = (BEListButton *)this->getItem(i);
            btn->setSelected(i == index);
        }
        _currentIndex = index;
    }
}

void BEListView::removeIndex(int index) {
    this->removeItem(index);
    int itemCount = this->getItems().size();
    for (int i = 0; i < itemCount; ++i) {
        auto btn = (BEListButton *)this->getItem(i);
        btn->setItemIndex(i);
    }
}

void BEListView::removeCurrentIndex() {
    this->removeIndex(_currentIndex);
    if (_currentIndex == this->getItems().size()) {
        this->selectIndex(0);
    } else {
        this->selectIndex(_currentIndex);
    }
}

void BEListView::reset() {
    for (auto widget : this->getItems()) {
        auto btn = (BEListButton *)widget;
        btn->setSelected(btn->getItemIndex() == 0);
    }
    _currentIndex = 0;
}

void BEListView::cleanAndReset() {
    removeAllItems();
    _currentIndex = -1;
}

const std::string& BEListView::getCurrentName() {
    auto btn = (BEListButton *)getItem(_currentIndex);
    return btn->getItemName();
}

BEDefaultListView *BEDefaultListView::create(Size contentSize, const BEClickHandler& clickHandler) {
    auto ret = new BEDefaultListView();
    if (ret->init(contentSize, clickHandler)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BEDefaultListView::init(Size contentSize, const BEClickHandler& clickHandler) {
    Size buttonSize = Size(contentSize.width, LIST_BUTTON_HEIGHT);
    if (!BEListView::init(contentSize, buttonSize, LIST_BUTTON_FONTSIZE, LIST_BUTTON_MARGIN, LIST_BUTTON_COLOR, LIST_BUTTON_HLCOLOR, clickHandler)) {
        return false;
    }
    return true;
}

BETypeListView *BETypeListView::create(Size contentSize, EditorTypePtr editorType, const BEClickHandler& handler) {
    auto ret = new BETypeListView();
    if (ret->init(contentSize, editorType, handler)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BETypeListView::init(Size contentSize, EditorTypePtr editorType, const BEClickHandler& handler) {
    if (!BEDefaultListView::init(contentSize, handler)) {
        return false;
    }
    _editorType = editorType;
    this->addItems(editorType->getTypeVector());
    return true;
}

BEFilterListView *BEFilterListView::create(Size contentSize, const BEClickHandler& clickHandler, ui::TextField *filterField) {
    auto ret = new BEFilterListView();
    if (ret->init(contentSize, clickHandler, filterField)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BEFilterListView::init(Size contentSize, const BEClickHandler& clickHandler, ui::TextField *filterField) {
    if (!BEDefaultListView::init(contentSize, clickHandler)) {
        return false;
    }
    _filterField = filterField;
    _filterField->addEventListener(CC_CALLBACK_2(BEFilterListView::onFilterTextChanged, this));
    return true;
}

void BEFilterListView::setDelegate(BEFilterListViewDelegate *delegate) {
    _delegate = delegate;
}

void BEFilterListView::updateData() {
    if (_delegate != nullptr) {
        _currentDisplayIndexes.clear();
        int itemCount = _delegate->itemCount();
        for (int i = 0; i < itemCount; i++) {
            _currentDisplayIndexes.push_back(i);
            addItem(_delegate->displayNameAtIndex(i));
        }
    }
}

int BEFilterListView::getCurrentIndexForFullItems() {
    int index = this->getCurrentIndex();
    return _currentDisplayIndexes[index];
}

void BEFilterListView::onFilterTextChanged(Ref *sender, ui::TextField::EventType et) {
    if (et == ui::TextField::EventType::DETACH_WITH_IME || et == ui::TextField::EventType::ATTACH_WITH_IME) {
        return;
    }
    if (_delegate != nullptr) {
        auto text = _filterField->getString();
        _currentDisplayIndexes.clear();
        std::vector<std::string> filterResult;
        int itemCount = _delegate->itemCount();
        for (int i = 0; i < itemCount; i++) {
            std::string searchName = _delegate->searchNameAtIndex(i);
            std::string displayName = _delegate->displayNameAtIndex(i);
            if (text.length() > 0) {
                if (searchName.find(text) == 0) {
                    filterResult.push_back(displayName);
                    _currentDisplayIndexes.push_back(i);
                }
            } else {
                filterResult.push_back(displayName);
                _currentDisplayIndexes.push_back(i);
            }
        }
        this->removeAllItems();
        this->addItems(filterResult);
    }
}

BEUIBase::BEUIBase(Node *root): BEUIBase(root, nullptr) {
}

BEUIBase::BEUIBase(Node *root, const BEPopupEventHandler& handler): _root(root), _eventHandler(handler) {
    auto layout = dynamic_cast<ui::Layout *>(_root);
    if (layout) {
        layout->setBackGroundImageScale9Enabled(true);
        layout->setBackGroundImage("Default/Button_Disable.png");
    }
}

ui::Layout *BEUIBase::getPanelFrom(const std::string& name, Node *parent) {
    return (ui::Layout *)parent->getChildByName(name);
}

ui::Button *BEUIBase::getButtonFrom(const std::string& name, Node *parent) {
    return (ui::Button *)parent->getChildByName(name);
}

ui::CheckBox *BEUIBase::getCheckBoxFrom(const std::string& name, Node *parent) {
    return (ui::CheckBox *)parent->getChildByName(name);
}

ui::TextField *BEUIBase::getTextFieldFrom(const std::string& name, Node *parent) {
    return (ui::TextField *)parent->getChildByName(name);
}

ui::Text *BEUIBase::getLabelFrom(const std::string& name, Node *parent) {
    return (ui::Text *)parent->getChildByName(name);
}

ui::ListView *BEUIBase::getListViewFrom(const std::string& name, Node *parent) {
    return (ui::ListView *)parent->getChildByName(name);
}

void BEUIBase::setButtonEnabled(ui::Button *btn, bool enabled) {
    btn->setEnabled(enabled);
    if (enabled) {
        btn->setTitleColor(Color3B(0, 179, 255));
    } else {
        btn->setTitleColor(Color3B(51, 51, 51));
    }
}

std::string BEUIBase::getRootName() {
    return _root->getName();
}

void BEUIBase::setVisible(bool visible) {
    _root->setVisible(visible);
}

BETypeListView *BEUIBase::popupTypeListFromButton(cocos2d::ui::Button *btn, EditorTypePtr editorType, const BEClickHandler& listItemClickedHandler) {
    auto buttonSize = btn->getContentSize();
    auto typeVector = editorType->getTypeVector();
    int marginSize = typeVector.size() > 0 ? typeVector.size() - 1 : 0;
    Size contentSize = Size(200, buttonSize.height * typeVector.size() + 10 * marginSize);
    auto listView = BETypeListView::create(contentSize, editorType, listItemClickedHandler);
    listView->setAnchorPoint(Vec2(0.5, 1));
    listView->setPosition(Vec2(btn->getPosition().x, btn->getPosition().y - buttonSize.height / 2));
    btn->getParent()->addChild(listView);
    return listView;
}

BEListView *BEUIBase::popupListFromButton(cocos2d::ui::Button *btn, const BEClickHandler& handler) {
    auto buttonSize = btn->getContentSize();
    Size contentSize = Size(200, 600);
    auto listView = BEDefaultListView::create(contentSize, handler);
    listView->setAnchorPoint(Vec2(0.5, 1));
    listView->setPosition(Vec2(btn->getPosition().x, btn->getPosition().y - buttonSize.height / 2));
    btn->getParent()->addChild(listView);
    return listView;
}

void BEUIBase::bindOKHandler(ui::Button *okButton) {
    bindButton(okButton, EditorPopupEventType::OK);
}

void BEUIBase::bindCancelHandler(ui::Button *cancelButton) {
    bindButton(cancelButton, EditorPopupEventType::Cancel);
}

void BEUIBase::bindNewPositionHandler(ui::Button *newPositionButton) {
    bindButton(newPositionButton, EditorPopupEventType::NewPosition);
}

void BEUIBase::bindSelectPositionHandler(ui::Button *selectPositionButton) {
    bindButton(selectPositionButton, EditorPopupEventType::SelectPosition);
}

void BEUIBase::bindButton(ui::Button *btn, EditorPopupEventType et) {
    btn->addClickEventListener([this, et](Ref *sender) {
        if (_eventHandler) {
            _eventHandler(et, this, sender);
        }
    });
}
