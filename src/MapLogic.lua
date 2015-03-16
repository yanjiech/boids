local MapLogic = class("MapLogic", nil)
local json = cjson
local EventTrigger = require("EventTrigger")
local EventAction = require("EventAction")
local LogicUnit = require "LogicUnit"
local MapToken = require "MapToken"
local WaveAction = require "WaveAction"
require("AI")

function MapLogic.mapPath2Class(path)
    if path:endWith("maps/forest02") then
        return require "maps/forest02"
    elseif path:endWith( "maps/forest02_2" ) then
        return require( "maps/forest02_2" )
    elseif path:endWith( "maps/forest08" ) then
        return require( "maps/forest08" )
    elseif path:endWith( "maps/map07" ) then
        return require( "maps/map07" )
    else
        return MapLogic
    end
end

function MapLogic:initWithLogicJson(logicJson, battleLayer)
    self._logic = logicJson
    self._battleLayer = battleLayer
    self._tasks = {}
    self._triggers = {}
    self._items = {}
    self._waveActions = {}
    self._unitDieCountByName = {}
    self._unitDieCountByCamp = {}
    self._unitDieCountByTag = {}
    self._unitAppearCountByName = {}
    self._unitAppearCountByCamp = {}
    self._unitAppearCountByTag = {}
    self._itemData = ResourceManager.itemData
    self._second = 0
    self._winConditionCount = 0
    self._playerDieCount = 0
    self._hasDayNight = false
    for _, event in ipairs(self._logic.events) do
        event.isEnable = true
        local trigger = EventTrigger:new(event)
        self._triggers[#self._triggers + 1] = trigger
        for _, action in ipairs(event.actions) do
            if action.action_type == "custom" and action.action_name == "toggle_daynight" then
                self._hasDayNight = true
            end
        end
    end
    for _, task in ipairs(self._logic.tasks) do
        local t = {}
        t.name = task.name
        t.description = task.desc
        t.state = Boids.TaskState.UnStart
        self._tasks[t.name] = t
    end
    self._units_script = {}
    if self._logic.custom_data then
        local custom_data = self._logic.custom_data
        if custom_data.map_script then
            self._map_script = require("maps/"..custom_data.map_script):new(self._battleLayer)
            AI._map_script = self._map_script
        end
        if custom_data.custom_unit_script then
            for _, unit in ipairs(custom_data.custom_unit_script) do
                self._units_script[unit] = require("units/"..unit)
            end
        end
    end
    if logicJson.conditions then
        for _, condition in ipairs(logicJson.conditions) do
            if condition["type"] == "primary_win" then
                self._primaryWin = condition
            end
        end
    end
    self:postInit()
end

function MapLogic:postInit()
end

function MapLogic:update(dt)
    self._second = self._second + dt
    if self._primaryWin and self._primaryWin.condition == "time_limit" and self._second >= self._primaryWin.number then
        self:primaryWin()
    end
    if self._logic.conditions then
        for _,condition in ipairs(self._logic.conditions) do
            if condition["type"] == "time_limit" and self._second >= condition.number then
                self:onGameStateChanged(Boids.GameState.Lose)
            end
        end
    end
end

function MapLogic:cleanup()
    EventAction.unscheduleAll()
    for _, item in pairs(self._items) do
        self:removeItem(item)
    end
    self._items = {}
    self._currentConversation = nil
    self._currentSpeechIndex = 0
end

function MapLogic:getAreaByName(name)
    if (self._startArea == nil) then
        for _, pos in ipairs(self._logic.positions) do
            if (pos.name == name) then
                self._startArea = pos
                break
            end
        end
    end
    return self._startArea
end

function MapLogic:getAreas()
    return self._logic.positions
end

function MapLogic:getTasks()
    return self._logic.tasks
end

function MapLogic:getAreaByPosition(position)
    for _, v in ipairs(self._logic.positions) do
        if (cc.rectContainsPoint(v.rect, position)) then
            return v
        end
    end
    return v
end

function MapLogic:getAreaByName(name)
    if (name == nil) then return nil end
    for _, area in ipairs(self._logic.positions) do
        if area.name == name then
            return area
        end
    end
    return nil
end

function MapLogic:getAreasByNamePrefix(name_prefix)
    local result = {}
    for _, area in ipairs(self._logic.positions) do
        if string.starts(area.name, name_prefix) then
            result[#result + 1] = area
        end
    end
    return result
end

function MapLogic:getAreasByTag(tag)
    local result = {}
    for _, area in ipairs(self._logic.positions) do
        if (area.tag ~= nil and area.tag == tag) then
            result[#result + 1] = area
        end
    end
    return result
end

function MapLogic:getEventByName(name)
    for _, v in ipairs(self._logic.events) do
        if (v.name == name) then
            return v
        end
    end
    return nil
end

function MapLogic:getTaskByName(name)
    return self._tasks[name]
end

function MapLogic:primaryWin()
    self._winConditionCount = 1
    if self._logic.conditions ~= nil then
        for _, condition in ipairs(self._logic.conditions) do
            if condition["type"] == "additional_win" then
                if condition.condition == "player_nodie" and self._playerDieCount == 0 then
                    self._winConditionCount = self._winConditionCount + 1
                elseif condition.condition == "unit_alive" then
                    local units = self._battleLayer:getUnitsByName(condition.name)
                    if #units >= condition.number then
                        self._winConditionCount = self._winConditionCount + 1
                    end
                elseif condition.condition == "time_limit" then
                    if self._second < condition.number then
                        self._winConditionCount = self._winConditionCount + 1
                    end
                elseif condition.condition == "unit_die" then
                    local currentCount = self._unitDieCountByName[condition.name]
                    if currentCount ~= nil and currentCount . condition.number then
                        self._winConditionCount = self._winConditionCount + 1
                    end
                elseif condition.condition == "clear_enemy" then
                    local enemies = self._battleLayer:getUnitsByCamp(Boids.UnitType.Enemy)
                    if #enemies == 0 then
                        self._winConditionCount = self._winConditionCount + 1
                    end
                end
            end
        end
    end
    self:onGameStateChanged(Boids.GameState.Win)
end

function MapLogic:testUnitEvent(unit, eventType, area)
    for _, et in ipairs(self._triggers) do
        if (et:valid()) then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if (self:isUnitCanTrigger(unit, trigger, eventType, area)) then
                    et:onSingleTriggerPass(trigger, self, unit)
                end
            end
        end
    end
end

function MapLogic:onUnitAppear(unit)
    local name = unit:getName()
    local camp = unit:getCamp()
    local tags = unit:getTags()
    if self._unitAppearCountByName[name] == nil then
        self._unitAppearCountByName[name] = 1
    else
        self._unitAppearCountByName[name] = self._unitAppearCountByName[name] + 1
    end
    if self._unitAppearCountByCamp[camp] == nil then
        self._unitAppearCountByCamp[camp] = 1
    else
        self._unitAppearCountByCamp[camp] = self._unitAppearCountByCamp[camp] + 1
    end
    for tag, _ in pairs(tags) do
        if self._unitAppearCountByTag[tag] == nil then
            self._unitAppearCountByTag[tag] = 1
        else
            self._unitAppearCountByTag[tag] = self._unitAppearCountByTag[tag] + 1
        end
    end
    self:testUnitEvent(unit, Boids.UnitState.Appear)
end

function MapLogic:onUnitDisappear(unit, isDie)
    local name = unit:getName()
    local camp = unit:getCamp()
    local tags = unit:getTags()
    if self._unitDieCountByName[name] == nil then
        self._unitDieCountByName[name] = 1
    else
        self._unitDieCountByName[name] = self._unitDieCountByName[name] + 1
    end
    if self._unitDieCountByCamp[camp] == nil then
        self._unitDieCountByCamp[camp] = 1
    else
        self._unitDieCountByCamp[camp] = self._unitDieCountByCamp[camp] + 1
    end
    for tag, _ in pairs(tags) do
        if self._unitDieCountByTag[tag] == nil then
            self._unitDieCountByTag[tag] = 1
        else
            self._unitDieCountByTag[tag] = self._unitDieCountByTag[tag] + 1
        end
    end
    self:testUnitEvent(unit, Boids.UnitState.Disappear)
    if unit:getCamp() == Boids.UnitType.Player then
        local playerUnits = self._battleLayer:getUnitsByCamp(Boids.UnitType.Player)
        self._playerDieCount = self._playerDieCount + 1
        if #playerUnits == 0 then
            self:onGameStateChanged(Boids.GameState.Lose)
        end
    elseif unit:getCamp() == Boids.UnitType.Opponent then
        local opponents = self._battleLayer:getUnitsByCamp(Boids.UnitType.Opponent)
        if #opponents == 0 then
            self:onGameStateChanged(Boids.GameState.Win)
        end
    end
    for _, waveAction in ipairs(self._waveActions) do
        local tag = waveAction:getTag()
        if unit:hasTag(tag) then
            local units = self._battleLayer:getUnitsByTag(tag)
            if #units == 0 then
                waveAction:excute()
            end
        end
    end
    if self._logic.conditions then
        if self._primaryWin.condition == "unit_die" then
            local currentCount = self._unitDieCountByName[self._primaryWin.name]
            if currentCount ~= nil and currentCount >= self._primaryWin.number then
                self:primaryWin()
            end
        end
        if self._primaryWin.condition == "clear_enemy" then
            local enemies = self._battleLayer:getUnitsByCamp(Boids.UnitType.Enemy)
            if #enemies == 0 then
                self:primaryWin()
            end
        end
        for _,condition in ipairs(self._logic.conditions) do
            if condition["type"] == "lose" and condition.condition == "unit_die" then
                local currentCount = self._unitDieCountByName[condition.name]
                if currentCount ~= nil and currentCount >= condition.number then
                    self:onGameStateChanged(Boids.GameState.Lose)
                end
            end
        end
    end
end

function MapLogic:onUnitArrive(unit, area)
    self:testUnitEvent(unit, Boids.UnitState.MoveTo, area)
    if self._primaryWin and self._primaryWin.condition == "arrive_area" and unit:getCamp() == Boids.UnitType.Player and area.name == self._primaryWin.name then
        self:primaryWin()
    end
end

function MapLogic:onMapPositionChanged()
    for tag, item in pairs(self._items) do
        local data = item.data
        if (data.type == "token") then
            self._battleLayer:adjustFloatNode(item.node, item.pos)
        end
    end
end

function MapLogic:onVisionChanged(visionName, visionTag, visionState)
    for _, et in ipairs(self._triggers) do
        if et:valid() then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if trigger.trigger_type == Boids.TriggerType.VisionChange then
                    if trigger.source_type == "name_source" and trigger.source_value == visionName and trigger.vision_state == visionState then
                        et:onSingleTriggerPass(trigger, self, nil)
                    end
                elseif trigger.source_type == "tag_source" and trigger.source_value == visionTag then
                    local visions = self._battleLayer:getVisisonsByTag(visionTag)
                    local canTrigger = true
                    for _, v in ipairs(visions) do
                        if v._state ~= visionState then
                            canTrigger = false
                            break
                        end
                    end
                    if canTrigger then
                        et:onSingleTriggerPass(trigger, self, nil)
                    end
                end
            end
        end
    end
end

function MapLogic:onPlayerUnitMove(unit, pos)
    for index, item in pairs(self._items) do
        local data = item.data
        if (data.type == "health") then
            if (cc.pGetDistance(item.pos, pos) <= data.radius) then
                local units = self._battleLayer:getUnitsByCamp(unit:getCamp())
                for _,unit in pairs(units) do
                    unit:takeHealingByPercentage(data.heal_percetage)
                end
                self:removeItem(item)
            end
        end
    end
end

function MapLogic:onMapInit()
    for _, et in ipairs(self._triggers) do
        if (et:valid()) then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if (trigger.trigger_type == Boids.TriggerType.MapInit) then
                    et:onSingleTriggerPass(trigger, self, nil)
                end
            end
        end
    end
end

function MapLogic:onConversationStateChanged(conversationName, state)
    for _, et in ipairs(self._triggers) do
        if (et:valid()) then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if (trigger.trigger_type == Boids.TriggerType.ConversationChange and trigger.state == state and conversationName == trigger.name) then
                    et:onSingleTriggerPass(trigger, self, nil)
                end
            end
        end
    end
end

function MapLogic:onCustomTrigger(triggerName)
    for _, et in ipairs(self._triggers) do
        if (et:valid()) then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if trigger.trigger_type == Boids.TriggerType.Custom and trigger.trigger_name == triggerName then
                    et:onSingleTriggerPass(trigger, self, nil)
                end
            end
        end
    end
end

function MapLogic:onEventChange(eventName, state)
    for _, et in ipairs(self._triggers) do
        if (et:valid()) then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if (trigger.trigger_type == Boids.TriggerType.EventChange and
                    trigger.event_name == eventName and trigger.event_state == state) then
                    et:onSingleTriggerPass(trigger, self, nil)
                end
            end
        end
    end
end

function MapLogic:onGameStateChanged(state)
    ue.LuaUtils:logf(string.format("onGameStateChanged: %s", state))
    if self._battleLayer._replay_to_save and (state == Boids.GameState.Lose or state == Boids.GameState.Win) then
        ue.LuaUtils:logf("finished. saving replay")
        self._battleLayer._replay_to_save:save()
    end
    if (state == Boids.GameState.Lose) then
        self._battleLayer:onStageChanged(Boids.BattleStage.lose)
    elseif (state == Boids.GameState.Win) then
        self._battleLayer:onStageChanged(Boids.BattleStage.win)
    elseif (state == Boids.GameState.Start) then
    end

    for _, et in ipairs(self._triggers) do
        if (et:valid()) then
            local triggers = et:triggersForTest()
            for _, trigger in ipairs(triggers) do
                if (trigger.trigger_type == Boids.TriggerType.GameChange and trigger.game_state == state) then
                    et:onSingleTriggerPass(trigger, self, nil)
                end
            end
        end
    end
end

function MapLogic:onTaskStateChanged(task, state)
    local stateString = ""
    if (state == Boids.TaskState.Start) then
        stateString = "开始"
    elseif (state == Boids.TaskState.Finish) then
        stateString = "完成"
    else
        stateString = "失败"
    end
    local toast = task.name .. stateString
    self._battleLayer:showToast(toast)
end

function MapLogic:onCustomEvent( action )
    self._battleLayer:onCustomEvent( action.action_name )
end

function MapLogic:onVisionChangedEvent( action )
    if action.source_type == "name_source" then
        local vision = self._battleLayer:getVisisonByName(action.source_value)
        vision:setState(action.vision_state)
        self:onVisionChanged( vision._data.name, vision._data.tag, action.vision_state )
    elseif action.source_type == "tag_source" then
        local visions = self._battleLayer:getVisisonsByTag(action.source_value)
        for _,v in ipairs(visions) do
            v:setState(action.vision_state)
            self:onVisionChanged( v._data.name, v._data.tag, action.vision_state)
        end
    end
end

function MapLogic:excuteEvent(event, unit)
    ue.LuaUtils:logf(string.format("excute event: %s", event.name))
    for idx, action in ipairs(event.actions) do
        local ea = EventAction:new(action.meta)
        local function onEventTriggered(finish)
            self:onEventChange(event.name, Boids.EventState.Trigger)
            if not event.isEnable then return end
            local actionType = action.action_type
            if (actionType == Boids.ActionType.UnitChange) then -- unit change
                self:excuteUnitAction(action, unit)
            elseif (actionType == Boids.ActionType.TaskChange) then -- task change
                local taskName = action.task_name
                local taskState = action.task_state
                local task = self:getTaskByName(taskName)
                if (task ~= nil) then
                    task.state = taskState
                    self:onTaskStateChanged(task, taskState)
                end
            elseif (actionType == Boids.ActionType.GameChange) then -- game change
                local gameState = action.game_state
                if gameState == Boids.GameState.Win then
                    self:primaryWin()
                else
                    self:onGameStateChanged(gameState)
                end
            elseif (actionType == Boids.ActionType.Custom) then
                self:onCustomEvent( action )
            elseif actionType == Boids.ActionType.EventChange then
                local eventName = action.event_name
                local enable = action.event_state == "event_enable"
                for _, et in ipairs(self._triggers) do
                    if (et._event.name == eventName) then
                        et:setEnable(enable)
                        break
                    end
                end
            elseif actionType == Boids.ActionType.VisionChange then
                self:onVisionChangedEvent( action )
            elseif actionType == Boids.ActionType.WaveAction then
                local waveTag = string.format("%s_%d", event.name, idx)
                self:excuteWaveAction(action, waveTag)
            elseif actionType == Boids.ActionType.ConversationAction then
                self:excuteConversationAction(action)
            end
            if (finish) then
                ea:unschedule()
                self:onEventChange(event.name, Boids.EventState.Finish)
            end
        end
        ea:schedule(onEventTriggered, false)
    end
end

function MapLogic:endCurrentConversation()
    if self._currentConversation then
        self:onConversationStateChanged(self._currentConversation.name, "conversation_end")
        self._battleLayer:setCameraMode(Boids.CameraMode.Follow)
        self._currentConversation = nil
        self._currentSpeechIndex = 0
    end
end

function MapLogic:onChatTimeOut()
    if self._currentConversation ~= nil then
        self._currentSpeechIndex = self._currentSpeechIndex + 1
        if self._currentSpeechIndex <= #self._currentConversation.speeches then
            self:excuteSpeech(self._currentConversation.speeches[self._currentSpeechIndex])
        else
            self:endCurrentConversation()
        end
    end
end


function MapLogic:excuteWaveAction(action, waveActionTag)
    local waveAction = WaveAction:new(action, waveActionTag, self)
    self._waveActions[#self._waveActions + 1] = waveAction
    waveAction:excute()
end

function MapLogic:excuteConversationAction(action)
    if self._currentConversation ~= nil then
        print(string.format("error:has conversation %s", self._currentConversation.name))
        return
    end
    self._currentConversation = action
    if #self._currentConversation.speeches > 0 then
        self._currentSpeechIndex = 1
        self:excuteSpeech(self._currentConversation.speeches[self._currentSpeechIndex])
        self:onConversationStateChanged(action.name, "conversation_start")
    end
end

function MapLogic:excuteSpeech(speech)
    local talker = nil
    if speech.source_type == "tag_source" then
        units = self._battleLayer:getUnitsByTag(speech.source)
        if #units > 0 then
            talker = units[1]
        end
    elseif speech.source_type == "name_source" then
        units = self._battleLayer:getUnitsByName(speech.source)
        if #units > 0 then
            talker = units[1]
        end
    elseif speech.source_type == "player_source" then
        talker = self._battleLayer:getLeaderPlayer()
    end
    if talker ~= nil then
        if speech.camera_move and speech.source_type ~= "player_source" then
            self._battleLayer:lockCameraTo(talker:getPos())
        end
        talker:say(speech.content, speech.duration)
    else
        self:endCurrentConversation()
    end
end

function MapLogic:excuteUnitAction(action, unit)
    local sourceUnits = {}
    local shouldPerformChanges = true;
    local sourceType = action.source_type
    local sourceValue = action.source_value
    local state = action.unit_state
    local buffsToAdd = {}
    local buffsToRemove = {}
    if action.buff_name ~= nil then
        local buffs = action.buff_name:split(",")
        for _, v in ipairs(buffs) do
            local buffName = string.sub(v, 2, string.len(v))
            local operator = string.sub(v, 1, 1)
            if operator == "+" then
                buffsToAdd[#buffsToAdd + 1] = buffName
            elseif operator == "-" then
                buffsToRemove[#buffsToRemove + 1] = buffName
            end
        end
    end

    if (sourceType == Boids.UnitSource.Type) then --UnitType
        sourceUnits = self._battleLayer:getUnitsByCamp(sourceValue)
    elseif (sourceType == Boids.UnitSource.Triggered) then --TriggerUnit
        if (unit ~= nil) then
            sourceUnits[1] = unit
        end
    elseif (sourceType == Boids.UnitSource.Tag) then -- UnitTag
        local item = self._items[sourceValue]
        if item ~= nil then
            if (state == Boids.UnitState.Disappear) then
                self:removeItem(item)
            end
        else
            sourceUnits = self._battleLayer:getUnitsByTag(sourceValue)
        end
    elseif (sourceType == Boids.UnitSource.Name) then --UnitName
        if state ~= nil and (state == Boids.UnitState.Appear or state == Boids.UnitState.HoldAppear) then
            shouldPerformChanges = false
            local areas = {}
            if action.position_name ~= nil then
                areas[1] = self:getAreaByName(action.position_name)
            else
                areas = self:getAreasByTag(action.position_tag)
            end
            for _, area in ipairs(areas) do
                if (action.unit_type == "item") then
                    local tagName = action.tag_name ~= nil and action.tag_name or area.name
                    self:deployItem(sourceValue, G.center(area.rect), tagName)
                else
                    local tag = action.tag_name ~= nil and action.tag_name or ""
                    local unitType = action.unit_type
                    local buff = action.buff_name ~= nil and action.buff_name or ""
                    local showHp = action.show_hp ~= nil and action.show_hp or false
                    local units = {}
                    local unitCount = action.unit_count ~= nil and action.unit_count or 1
                    local isBoss = action.is_boss ~= nil and action.is_boss == true
                    local unPushable = 0
                    if action.unpushable ~= nil then
                        if action.unpushable then
	                        unPushable = 1
	                    else
	                        unPushable = -1
	                    end
	                end
                    local classname = nil
                    if action.class_name then
                        classname = require(string.format("units/%s", action.class_name))
                    else
                        classname = LogicUnit
                    end
                    local mid_points = nil
                    if action.mid_points then
                        mid_points = {}
                        for i,v in pairs( action.mid_points ) do
                            local ar = self:getAreaByName( v )  
                            table.insert( mid_points, cc.p( ar.rect.x, ar.rect.y ) )
                        end
                    end

                    if action.buff_name ~= nil then
                    end

                    if self._battleLayer._my_force_id and self._battleLayer._my_force_id ~= 1 then --有force_id的就是pvp了，不是1的话表示是右边。如果有3方的话阵营和这里都还要再改
                        if unitType == Boids.Camp.Enemy then
                            unitType = Boids.Camp.Ally
                        elseif unitType == Boids.Camp.Ally then
                            unitType = Boids.Camp.Enemy
                        end
                    end
                    if unitCount > 0 then
                        for i = 1, unitCount do
                            local unit = classname:new()
                            unit:init(sourceValue, unitType, 0, showHp, tag, self._battleLayer)
                            units[#units + 1] = unit
                            unit:setBoss(isBoss)
                            --print("unpushable:", sourceValue, unPushable)
                            if unPushable ~= 0 then
                                if unPushable == 1 then
                                    unit:setForceUnpushable(true)
                                else
                                    unit:setForceUnpushable(false)
                                end
                            end
                            if state == Boids.UnitState.HoldAppear then
                                unit._hold_position = true
                            else
                                unit._hold_position = false
                            end
                            if action.custom_change ~= nil then
                                unit:setCustomChange(action.custom_change)
                            end
                            unit._mid_points = mid_points
                        end
                        local sightGroup = action.sight_group or string.format("%s_%s", area.name, action.meta)
                        self._battleLayer:deployUnits(units, area, sightGroup)
                        for _, unit in ipairs(units) do
                            unit:setPopup(action.popup_type)
                            for _, buffName in ipairs(buffsToAdd) do
                                unit:addBuff(buffName)
                            end
                        end
                    end
                end
            end
        else -- existed units
            sourceUnits = self._battleLayer:getUnitsByName(sourceValue)
        end
    end
    -- perform states changes
    if (shouldPerformChanges and #sourceUnits > 0) then
        for _, unit in pairs(sourceUnits) do
            if (state ~= nil) then
                if (state == Boids.UnitState.MoveTo) then
                    local area = self:getAreaByName(action.position_name)
                    unit:setState(state, area)
                elseif (state == Boids.UnitState.Patrol) then
                    local area = self:getAreaByName(action.position_name)
                    unit:setState(state, area)
                else
                    unit:setState(state, nil)
                end
            end
            if (action.show_hp ~= nil) then
                unit:setShowHP(action.show_hp)
            end
            if (action.tag_name ~= nil) then
                unit:setTag(action.tag_name)
            end
            if (action.unit_type ~= nil) then
                unit:setCamp(action.unit_type)
            end
            for _, buffName in ipairs(buffsToAdd) do
                unit:addBuff(buffName)
            end
            for _, buffName in ipairs(buffsToRemove) do
                unit:removeBuff(buffName)
            end
            unit:setPopup(action.popup_type)
        end
    end
end

function MapLogic:isUnitCanTrigger(unit, trigger, state, area)
    local camp = unit:getCamp()
    if (trigger.trigger_type == Boids.TriggerType.UnitChange and trigger.unit_state == state) then
        local sourceType = trigger.source_type
        local sourceValue = trigger.source_value
        local positionName = trigger.position_name
        local triggerCount = trigger.trigger_count
        if triggerCount == nil then
            triggerCount = 1
        end
        if (sourceType == Boids.UnitSource.Type and camp == sourceValue) then
            if (state == Boids.UnitState.Appear) then
                if triggerCount == 0 then
                    return false
                elseif triggerCount == 1 then
                    return true
                else
                    return self._unitAppearCountByCamp[sourceValue] == triggerCount
                end
            elseif (state == Boids.UnitState.Disappear) then
                if triggerCount == 0 then
                    local units = self._battleLayer:getUnitsByCamp(sourceValue)
                    return #units == 0
                elseif triggerCount == 1 then
                    return true
                else
                    return self._unitDieCountByCamp[camp] == triggerCount
                end
            elseif (state == Boids.UnitState.MoveTo) then
                local inAreaCount = 0
                local units = self._battleLayer:getUnitsByCamp(sourceValue)
                if #units == 0 then return false end
                for _, unit in ipairs(units) do
                    local unitArea = self:getAreaByPosition(unit:getPos())
                    if unitArea and unitArea.name == positionName then
                        inAreaCount = inAreaCount + 1
                    end
                end
                if triggerCount == 0 then
                    return inAreaCount == #units
                elseif triggerCount == 1 then
                    return area.name == positionName
                else
                    return inAreaCount == triggerCount
                end
            end
        elseif sourceType == Boids.UnitSource.Tag and unit:hasTag(sourceValue) then
            if (state == Boids.UnitState.Appear) then
                if triggerCount == 0 then
                    return false
                elseif triggerCount == 1 then
                    return true
                else
                    return self._unitAppearCountByTag[sourceValue] == triggerCount
                end
            elseif (state == Boids.UnitState.Disappear) then
                if triggerCount == 0 then
                    local units = self._battleLayer:getUnitsByTag(sourceValue)
                    return #units == 0
                elseif triggerCount == 1 then
                    return true
                else
                    return self._unitDieCountByTag[sourceValue] == triggerCount
                end
            elseif (state == Boids.UnitState.MoveTo) then
                local inAreaCount = 0
                local units = self._battleLayer:getUnitsByTag(sourceValue)
                if #units == 0 then return false end
                for _, unit in ipairs(units) do
                    local unitArea = self:getAreaByPosition(unit:getPos())
                    if unitArea and unitArea.name == positionName then
                        inAreaCount = inAreaCount + 1
                    end
                end
                if triggerCount == 0 then
                    return inAreaCount == #units
                elseif triggerCount == 1 then
                    return area.name == positionName
                else
                    return inAreaCount == triggerCount
                end
            end
        elseif (sourceType == Boids.UnitSource.Name and unit:getName() == sourceValue) then
            if (state == Boids.UnitState.Appear) then
                if triggerCount == 0 then
                    return false
                elseif triggerCount == 1 then
                    return true
                else
                    return self._unitAppearCountByName[sourceValue] == triggerCount
                end
            elseif (state == Boids.UnitState.Disappear) then
                if triggerCount == 0 then
                    local units = self._battleLayer:getUnitsByName(sourceValue)
                    return #units == 0
                elseif triggerCount == 1 then
                    return true
                else
                    return self._unitDieCountByName[sourceValue] == triggerCount
                end
            elseif (state == Boids.UnitState.MoveTo) then
                local inAreaCount = 0
                local units = self._battleLayer:getUnitsByName(sourceValue)
                if #units == 0 then return false end
                for _, unit in ipairs(units) do
                    local unitArea = self:getAreaByPosition(unit:getPos())
                    if unitArea and unitArea.name == positionName then
                        inAreaCount = inAreaCount + 1
                    end
                end
                if triggerCount == 0 then
                    return inAreaCount == #units
                elseif triggerCount == 1 then
                    return area.name == positionName
                else
                    return inAreaCount == triggerCount
                end
            end
        end
    end
    return false
end

function MapLogic:deployItem(itemName, pos, tag)
    local t = {}
    t.pos = pos
    t.data = self._itemData[itemName]
    t.tag = tag
    if (t.data.type == "health") then
        t.node = cc.Sprite:createWithSpriteFrameName(t.data.res)
        self._battleLayer:addToObjectLayerByPos(t.node, t.pos)
    elseif (t.data.type == "token") then
        t.node = MapToken:new(t.data.res)
        self._battleLayer:addToFloatLayer(t.node, pos)
    end
    self._items[tag] = t
    return t
end

function MapLogic:removeItem(item)
    if (item.node ~= nil) then
        item.node:removeFromParent()
    end
    self._items[item.tag] = nil
end

return MapLogic
