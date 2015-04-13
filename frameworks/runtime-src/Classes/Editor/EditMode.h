//
//  EditMode.h
//  Boids
//
//  Created by Xin Xu on 11/6/14.
//
//

#ifndef __Boids__EditMode__
#define __Boids__EditMode__

#include "BEHeaders.h"
#include "EditorModel.h"
#include "BEUI.h"
#include "EditorModelDefines.h"
#include "MapData.h"

class EditMode {
public:
    virtual ~EditMode() {}
    static EditMode *getInstance();
    void enterMain(std::function<void()> onExit);
    void backMain();
    
private:
    static EditMode *_instance;
    EditMode();
    cocos2d::TMXTiledMap *_map;
    cocos2d::DrawNode *_drawNode;
    cocos2d::DrawNode *_draftDrawNode;
    std::shared_ptr<BEEditorMainUI> _editUI;
    std::shared_ptr<BEMainUI> _mainUI;
    cocos2d::Node *_editRootNode;
    cocos2d::Node *_mainRootNode;
    cocos2d::EventListenerTouchOneByOne *_touchListener;
    cocos2d::EventListenerKeyboard *_keyboardListener;
    EditorContext _context;
    EditorContext _subContext;
    std::string _mapName;
    bool _inSubContext;
    std::vector<std::string> _units;
    cocos2d::Layer *_positionLayer;
    BEUIBase *_preUI;
    cocos2d::Ref *_preSender;
    bool _inDragMode;
    EditorData _data;
    std::string _jsonPath;
    std::function<void()> _onExit;
    std::shared_ptr<MapData> _mapData;
    std::vector<std::string> _tags;
    std::vector<std::string> _positionTags;
    std::vector<std::string> _visionObjects;
    std::vector<std::string> _visionTags;
    std::vector<std::string> _conversations;
    
    void enterEdit(const std::string& mapFolder);
    void leaveEdit();
    
    EditorContext* currentContext() { return _inSubContext ? &_subContext : &_context; }
    void loadUnits();
    void loadVisionObjects();
    std::vector<std::pair<std::string, std::string>> getActionSources(bool hasTriggerUnit);
    std::vector<std::pair<std::string, std::string>> getTriggerSources();
    std::vector<std::pair<std::string, std::string>> getVisionSources();
    std::vector<std::pair<std::string, std::string>> getConversationSources();
    std::vector<std::pair<std::string, std::string>> getStorySources();
    std::vector<EditorEventPtr> getEditableEvents();
    void fetchTags();
    void fetchConversations();
    void createPositionOnMap(EditorPositionPtr pos);
    EditorPositionPtr getPositionForTouch(cocos2d::Touch *touch);
    void setMapPosition(cocos2d::Point position);
    void deleteEventByIndexes(std::vector<int>& indexes);
    void deletePositionByName(const std::string& name);
    void runSceneWithNode(cocos2d::Node *node);
    void fetchPositionTags();
    
    void onMenuCommand(EditorCommandType ct, cocos2d::Ref *sender);
    void onPopupEvent(EditorPopupEventType et, BEUIBase *popup, cocos2d::Ref *sender);
    void onMainCommand(MainCommandType ct, const std::string& mapFolder, cocos2d::Ref *sender);
    
    void onStep(int step);
    void moveOn();
    void moveBack();
    void moveStart();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode code);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode code);

	void onBlankPositionTouched(cocos2d::Point pt);
};

#endif /* defined(__Boids__EditMode__) */
