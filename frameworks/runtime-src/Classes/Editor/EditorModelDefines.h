//
//  EditorModelDefines.h
//  Boids
//
//  Created by Xin Xu on 11/13/14.
//
//

#ifndef Boids_EditorModelDefines_h
#define Boids_EditorModelDefines_h

#include "EditorModel.h"

typedef std::shared_ptr<EditorTask> EditorTaskPtr;
typedef std::shared_ptr<EditorEvent> EditorEventPtr;
typedef std::shared_ptr<EditorUnit> EditorUnitPtr;
typedef std::shared_ptr<EditorWave> EditorWavePtr;
typedef std::shared_ptr<EditorSpeech> EditorSpeechPtr;
typedef std::shared_ptr<EditorPosition> EditorPositionPtr;
typedef std::shared_ptr<EditorMapInitTrigger> EditorMapInitTriggerPtr;
typedef std::shared_ptr<EditorUnitTrigger> EditorUnitTriggerPtr;
typedef std::shared_ptr<EditorUnitStayTrigger> EditorUnitStayTriggerPtr;
typedef std::shared_ptr<EditorEventTrigger> EditorEventTriggerPtr;
typedef std::shared_ptr<EditorCustomTrigger> EditorCustomTriggerPtr;
typedef std::shared_ptr<EditorUnitAction> EditorUnitActionPtr;
typedef std::shared_ptr<EditorGameAction> EditorGameActionPtr;
typedef std::shared_ptr<EditorTaskAction> EditorTaskActionPtr;
typedef std::shared_ptr<EditorTriggerMeta> EditorTriggerMetaPtr;
typedef std::shared_ptr<EditorActionMeta> EditorActionMetaPtr;
typedef std::shared_ptr<EditorEventAction> EditorEventActionPtr;
typedef std::shared_ptr<EditorCustomAction> EditorCustomActionPtr;
typedef std::shared_ptr<EditorVisionAction> EditorVisionActionPtr;
typedef std::shared_ptr<EditorGameTrigger> EditorGameTriggerPtr;
typedef std::shared_ptr<EditorVisionTrigger> EditorVisionTriggerPtr;
typedef std::shared_ptr<EditorWaveAction> EditorWaveActionPtr;
typedef std::shared_ptr<EditorGameCondition> EditorGameConditionPtr;
typedef std::shared_ptr<EditorConversationAction> EditorConversationActionPtr;
//typedef std::shared_ptr<EditorConversationChangeTrigger> EditorConversationChangeTriggerPtr;

#endif
