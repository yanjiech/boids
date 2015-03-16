local BattleLayer = class("BattleLayer", function() return cc.Layer:create() end)
local LogicUnit = require "LogicUnit"
local JoyStick = require "JoyStick"
local json = cjson
local MapLogic = require("MapLogic")
local UISkillLayer = require "UISkillLayer"
local AIAction = require("AIAction")
local ToastLayer = require("ToastLayer")
local BattleUILayer = require("BattleUILayer")
local UIShadeNode = require("ui/UIShadeNode")
local Trace = require("Trace")
local LogicBuilding = require "LogicBuilding"
local BattleFloatLayer = require "BattleFloatLayer"
require("AI").init()
require("PlayerSkillExecuter")

-- Layers(从底至顶)
--[[
_tmxMap: tmxmap
_belowObjectLayer: 血渍
_objectLayer: 单位,装饰物,技能
_overObjectLayer: 血条
_floatLayer: token
_toastLayer: 各种消息
_uiLayer: 弹窗
]]

function BattleLayer:ctor(...)
    self:setTag(Boids.KnownLayer.battleLayer)
    AI._battleLayer = self
    self._current_frame = 0
end

function BattleLayer:initWithMapPath(mapPath)
    self._mapPath = mapPath
    self._mapData = ue.MapData:new(mapPath)
    self._tmxMap = self._mapData:generateTiledMapWithFlags(3)
    local logicString = self._mapData:getMetaData()
    self._logicJson = json.decode(logicString)
    self:_init()
end

function BattleLayer:initWithData(logicJson, map, mapPath, pvp)
    self._mapPath = mapPath
    self._tmxMap = map
    self._logicJson = logicJson
    if pvp then
        self._net_ad = ue.NetworkAdapter:getInstance()
    else
        if Boids.playReplay then --要播录像
            self._replay_to_play = ue.ReplayToPlay:new(Boids.playReplay, Boids.checkSnapshot or false)
            G.setFrame(self._replay_to_play:getFrameRate())
        end
    end
    if Boids.saveReplay then --要存录像
        self._replay_to_save = ue.ReplayToSave:new(Boids.frameRate, Boids.saveSnapshotInterval or 0)
        self.save_replay_this_frame = false
    end
    self:_init()
end

function BattleLayer:getInfoFromReplayAndNet()
    if self._net_ad then
        local moves, skills = ue.NetworkAdapter.getOps() --TODO: 理论上技能这里要判断是否cd到，到时候再改
        for user_id, dir in pairs(moves) do
            self.direction[self._userid2camp[user_id]] = dir
        end
        for unit_id, skill_data in pairs(skills) do
            PlayerSkill.data[tonumber(unit_id)] = skill_data
        end
    else
        if self._replay_to_play then
            if Boids.checkSnapshot then --update的时候才真的检查snapshot，这里只是把附加的信息(hp)存到c里以待检查
                for _, unit in pairs(AI._unitTable) do
                    self._replay_to_play:checkUnitHpWithSnapshot(unit:getId(), unit._currentHP)
                end
            end
            self._replay_to_play:updateFrame() --在update的时候会检查snapshot。相当于是上一帧的结束。相应的，snapshot也是在每帧开始的时候保存的

            self.direction[Boids.Camp.Player] = cc.p(self._replay_to_play:getJoystickX(), self._replay_to_play:getJoystickY())
            if self.direction[Boids.Camp.Player].x == 0 and self.direction[Boids.Camp.Player].y == 0 then
                self.direction[Boids.Camp.Player] = nil
            end
            
            skill_player_in_replay = self._replay_to_play:getSkillPlayer()
            if skill_player_in_replay ~= 0 then
                ue.LuaUtils:logf(string.format("replay_skill: %d", skill_player_in_replay))
                PlayerSkill.data[skill_player_in_replay] = { ["dir"] = cc.p(self._replay_to_play:getSkillDirX(), self._replay_to_play:getSkillDirY()),
                                                             ["range"] = self._replay_to_play:getSkillRange() }
            end
        end
    end
end

function BattleLayer:saveInfoToReplay()
    if self._replay_to_save then
        self._replay_to_save:updateFrame() --在update的时候会顺便保存snapshot
        if Boids.saveSnapshotInterval then --和放replay不同，保存的时候是先update，再存hp到当前snapshot的。只是写法不同而已
            for _, unit in pairs(AI._unitTable) do
                self._replay_to_save:setUnitHpToSnapshot(unit:getId(), unit._currentHP)
            end
        end
        local dir = self.direction[Boids.Camp.Player]
        if dir then
            self._replay_to_save:setJoystick(dir.x, dir.y)
        end
        --技能的存录像是在LogicUnit:useSkillById里面
    end
end

function BattleLayer:_init()
    local size = cc.Director:getInstance():getVisibleSize()
    local origin = cc.Director:getInstance():getVisibleOrigin()
    self:setPosition(origin)
    self:setContentSize(size)
    self:addChild(self._tmxMap, 0, Boids.KnownLayer.mapLayer)
    self._onGroundLayer = cc.Layer:create()
    self._belowObjectLayer = cc.Layer:create()
    self._tmxMap:addChild(self._onGroundLayer)
    self._tmxMap:addChild(self._belowObjectLayer)
    self._objectLayer = cc.Layer:create()
    self._tmxMap:addChild(self._objectLayer)
    self._overObjectLayer = cc.Layer:create()
    self._tmxMap:addChild(self._overObjectLayer)
    --skill effect layer
    self._effectLayer = cc.Layer:create()
    self._tmxMap:addChild( self._effectLayer )
    self._floatLayer = BattleFloatLayer:new(self)
    self:addChild(self._floatLayer)
    self._toastLayer = ToastLayer:new()
    self:addChild(self._toastLayer)
    self._uiLayer = BattleUILayer:new(self)
    self._uiLayer:loadTasks(self._logicJson.tasks)
    self:addChild(self._uiLayer)
    if (Boids.debugPosition) then
        self:_createDebugPosition()
    end

    -- setup joystick
    self._joyStick = JoyStick:new()
    self:addChild(self._joyStick)

    self:setTouchEnabled(true)
    local function onTouchBegan(touch, event)
        if self._stage == Boids.BattleStage.ongoing and not self._replay_to_play then
            self._joyStick:onTouchBegan(touch, event)
        end
        return true
    end
    local function onTouchMoved(touch, event)
        if self._stage == Boids.BattleStage.ongoing and not self._replay_to_play then
            self._joyStick:onTouchMoved(touch, event)
        end
        if self._cameraMode == Boids.CameraMode.Free then
            local delta = touch:getDelta()
            local dest = cc.pAdd(G.getPosition(self._tmxMap), delta)
            self:setMapPosition(dest)
        end
    end
    local function onTouchEnded(touch, event)
        if self._stage == Boids.BattleStage.ongoing and not self._replay_to_play then
            self._joyStick:onTouchEnded(touch, event)
        end
    end
    local listener = cc.EventListenerTouchOneByOne:create()
    listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
    listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
    listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_CANCELLED)
    self._touchListener = listener
    local eventDispatcher = self:getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener, self)
    if (Boids.editMode) then
        local function onSecretKeyPressed(code, data)
            if (cc.KeyCodeKey[code] == "KEY_UNDERSCORE") then
                self:exit()
            end
        end
        local keyBoardListener = cc.EventListenerKeyboard:create()
        keyBoardListener:registerScriptHandler(onSecretKeyPressed, cc.Handler.EVENT_KEYBOARD_PRESSED)
        eventDispatcher:addEventListenerWithSceneGraphPriority(keyBoardListener, self)
    end

    -- schedule update
    local function update(dt)
        if not (self._stage == Boids.BattleStage.before or self._stage == Boids.BattleStage.ongoing) then  
            --我现在改成init就相当于游戏开始，因为那时候我就处理了服务器发过来的游戏信息，服务器以那个包围游戏开始
            print("game haven't start or already finished")
            return 
        end

        if (self._current_frame == 0) then -- just start
            gTrace:beginAccumulate()
            cAI:paintDebugLayer()
            AudioManager.playMusic( "bg002_d", "common", true ) --第一帧的时候再放背景音乐
            cc.SimpleAudioEngine:getInstance():setMusicVolume( 0.2 )
        else
            gTrace:accumulate("outsideUpdate")
        end

        self._frameStage = Boids.FrameStage.Begin

        self._current_frame = self._current_frame + 1
        
        cAI:updateFrame() --其实这行就是把c里的current_frame进1，主要影响到打出来的log

        self.direction = {}
        self:getInfoFromReplayAndNet() --验证snapshot，然后把摇杆啊，技能信息啊取出来

        if self._joyStick:isVisible() then
            local current_direction = self._joyStick:getDirection()
            if self._net_ad then
                self._net_ad:setMove(current_direction.x * 1e6, current_direction.y * 1e6)
            else
                self.direction[Boids.Camp.Player] = current_direction
            end
        end

        self:saveInfoToReplay() --保存snapshot，然后把摇杆信息存进去
        
        --不管是通过按icon放的还是通过录像放的技能，都要到这时候才执行
        PlayerSkill.execute()

        AI.updateFrame(self.direction, dt)

        self._frameStage = Boids.FrameStage.PostAI

        if self:getPlayerUnitCount() > 0 then
            local leader = self:getLeaderPlayer()
            local playerPosition = leader:getPos()
            if self._cameraMode == Boids.CameraMode.Follow then
                self:centerMapPosition(playerPosition)
            end
        end

        gTrace:accumulate("center position")

        local objects = self._objectLayer:getChildren()
        for _, object in pairs(objects) do
            if object.updateFrame ~= nil then
                object:updateFrame(dt)
            end
        end
        gTrace:accumulate("object_updateFrame")

        local objects = self._overObjectLayer:getChildren()
        for _,object in pairs(objects) do
            if object.shouldRecycle ~= nil and object.shouldRecycle == true then
                object:removeFromParent()
            end
        end
        gTrace:accumulate("shouldRecycle")
        self._floatLayer:update(dt)
        gTrace:accumulate("floatLayer")
        self._logic:update(dt)

        if Boids.printTraceStatisticsInterval then
            if self._current_frame % Boids.printTraceStatisticsInterval == 0 then
                gTrace:printStatistics(true)
            end
        end

        gTrace:accumulate("postAI")
        self._frameStage = Boids.FrameStage.PostUpdate
    end
    self:scheduleUpdateWithPriorityLua(update, -1) --这里一定得是-1，要保证比其它update先执行。

    self._uiSkillLayer = UISkillLayer:new( self )
    self:addChild( self._uiSkillLayer, 0, Boids.KnownLayer.skillUILayer )

    -- init
    if self._net_ad and G._game_init_data then
        self._game_init_data = G._game_init_data
        self._userid2camp = {}
        self._my_force_id = 1
        --print("game_init_data")
        --printtable(game_init_data)
        --printtable(self._userid2camp)
        for user_id, force_data in pairs(self._game_init_data) do
            --print("user_id:", user_id)
            if user_id == ue.Net:getUserId() then
                --print("my_user_id:", user_id)
                self._my_force_id = force_data.force_id
                self._userid2camp[user_id] = Boids.Camp.Player
            else
                --print("his_user_id:", user_id)
                self._userid2camp[user_id] = Boids.Camp.Opponent
            end
        end
    end
    self:onStageChanged(Boids.BattleStage.before)
end

function BattleLayer:cleanup()
    self._logic:cleanup()
    self._floatLayer:cleanup()
end

function BattleLayer:lockCameraTo(pos)
    self._cameraMode = Boids.CameraMode.Lock
    self:centerMapPosition(pos)
end

function BattleLayer:setCameraMode(mode)
    self._cameraMode = mode
end

function BattleLayer:onStageChanged(stage)
    print("onGameStateChanged:", stage)
    self._stage = stage
    if stage == Boids.BattleStage.before then
        self._cameraMode = Boids.CameraMode.Free
        self:_setup()
    elseif stage == Boids.BattleStage.ongoing then
        self._cameraMode = Boids.CameraMode.Follow
        self:showToast("游戏开始")
        self._floatLayer:onGameStart()
        self._logic:onGameStateChanged(Boids.GameState.Start)
    elseif stage == Boids.BattleStage.win then
        self._logic:cleanup()
    elseif stage == Boids.BattleStage.lose then
        self._logic:cleanup()
    elseif stage == Boids.BattleStage.pause then
        cc.Director:getInstance():pause()
    end
    self._uiLayer:setStage(stage)
end

function BattleLayer:getUnitsByName(name)
    local ret = {}
    for _, unit in pairs(self._unitTable) do
        if (unit:getName() == name) then
            ret[#ret + 1] = unit
        end
    end
    return ret
end

function BattleLayer:getUnitsByTag(tag)
    local ret = {}
    for _, unit in pairs(self._unitTable) do
        if (unit:hasTag(tag)) then
            ret[#ret + 1] = unit
        end
    end
    return ret
end

function BattleLayer:getUnitById(id)
    return self._unitTable[id]
end

function BattleLayer:getPlayerUnits()
    return self._units.player
end

function BattleLayer:getLeaderPlayer()
    for _, unit in pairs(self:getPlayerUnits()) do
        return unit
    end
end

function BattleLayer:getPlayerUnitCount()
    local count = 0
    for _, unit in pairs(self:getPlayerUnits()) do
        count = count + 1
    end
    return count
end

function BattleLayer:getUnitByCampInRange( camp, pos, range )
    return self:getUnitInRangeFromTable(self._units[camp])
end

function BattleLayer:getUnitInRangeFromTable( tbl, pos, range )
    local candidates = {}
    for k,v in pairs( tbl ) do
        local upos = v:getPos()
        --if cc.pDistanceSQ( pos, upos ) < range * range then
        if MathExtend.isPositionInRange( pos, range, upos ) then
            table.insert( candidates, v )
        end
    end
    if #candidates == 0 then
        return nil
    else
        local i = ue.LuaUtils:randomNumber( #candidates )
        --ue.LuaUtils:logf(string.format("getUnitByCampInRange; index:%d, id:%d", i, candidates[i]:getId()))
        return candidates[i]
    end
end

function BattleLayer:getUnitsByCamp(unitCamp)
    local ret = {}
    for _, unit in pairs(self._units[unitCamp]) do
        ret[#ret + 1] = unit
    end
    return ret
end

function BattleLayer:getVisisonByName(name)
    for _, v in pairs(self._buildings) do
        if v._data.name == name then
            return v
        end
    end
end

function BattleLayer:getVisisonsByTag(tag)
    local ret = {}
    for _, v in pairs(self._buildings) do
        if v._data.tag == tag then
            ret[#ret + 1] = v
        end
    end
    return ret
end

function BattleLayer:changeUnitCamp(unit, newCamp)
    local uid = unit:getId()
    self._units[unit:getCamp()][uid] = nil
    self._units[newCamp][uid] = unit
    if (newCamp == Boids.UnitType.Player) then
        self:_onAddPlayerUnit(unit)
        unit:setSightGroup("player_start")
    end
end

function BattleLayer:deployUnits(units, area, sightGroup)
    local region = area.rect
    local radiusList = {}
    local infoList = {}
    for i = 1, #units do
        units[i]:setSightGroup(sightGroup)
        radiusList[i] = units[i]:getData("collide")
    end
    ue.LuaUtils:logr(string.format("deploy %d units", #units))
    local positions = cAI:getAvailablePosition(radiusList, region)
    for i = 1, #positions do
        self:_deployUnit(units[i], positions[i])
    end
end

function BattleLayer:postDeploy(unit)
    if self._logic._map_script and self._logic._map_script.postDeploy then
        self._logic._map_script:postDeploy(unit)
    end
    if unit.postDeploy then
        unit:postDeploy()
    end
end

function BattleLayer:patrolTo(units, region)
    for _, unit in ipairs(units) do
    end
end

function BattleLayer:onUnitDisappear(unit, isDie)
    self._units[unit:getCamp()][unit._deployId] = nil
    self._unitTable[unit._deployId] = nil
    if (unit:getCamp() == Boids.UnitType.Player) then
        self._uiSkillLayer:removeSkillNode(unit)
    end
    if unit:getCamp() == Boids.UnitType.Enemy and isDie then
        self._floatLayer:addKillCount()
    end
    self._logic:onUnitDisappear(unit, isDie)
end

function BattleLayer:onBuildingDisappear(building)
    self._buildings[building:getName()] = nil
    self._logic:onBuildingDisappear(building)
end

function BattleLayer:onUnitMove(unit, position)
    local area = self._logic:getAreaByPosition(position)
    if (area ~= nil) then
        self._logic:onUnitArrive(unit, area)
    end
    if unit:isPlayerOrOpponent() then
        self._logic:onPlayerUnitMove(unit, position)
    end
end

function BattleLayer:centerMapPosition(position)
    local size = self:getContentSize()
    local offsetX = position.x - size.width / 2
    local offsetY = position.y - size.height / 2
    local mapPositionX = offsetX > 0 and -offsetX or 0
    local mapPositionY = offsetY > 0 and -offsetY or 0
    self:setMapPosition(cc.p(mapPositionX, mapPositionY))
end

function BattleLayer:setMapPosition(position)
    local size = self:getContentSize()
    local minX = size.width - self._tmxMap:getContentSize().width
    local minY = size.height - self._tmxMap:getContentSize().height
    local x = position.x < minX and minX or (position.x < 0 and position.x or 0)
    local y = position.y < minY and minY or (position.y < 0 and position.y or 0)
    local orig_x, orig_y = self._tmxMap:getPosition()
    if orig_x ~= x or orig_y ~= y then
        self._tmxMap:setPosition(x, y)
        self._logic:onMapPositionChanged()
        for _, building in pairs(self._buildings) do
            local p = building._data:getCenterPos()
            if self:isPositionInVision(p) and building.onSeen then
                building:onSeen()
            end
        end
    end
end

function BattleLayer:showToast(toast)
    self._toastLayer:showToast(toast)
end

function BattleLayer:addEffect( node, pos, zorder )
    node:setPosition( pos )
    self._effectLayer:addChild( node, zorder )
end

function BattleLayer:addGroundEffect( node, pos, zorder )
    node:setPosition( pos )
    self._belowObjectLayer:addChild( node, zorder )
end

function BattleLayer:addEffectToObjectLayer( node, pos, zorder )
    node:setPosition( pos )
    self._objectLayer:addChild( node, zorder )
end

function BattleLayer:addToOverObjectLayer(node, pos)
    node:setPosition(pos)
    node:setLocalZOrder(self:zOrderForPosition(pos))
    self._overObjectLayer:addChild(node)
end

function BattleLayer:addToBelowObjectLayer(node, pos)
    node:setPosition(pos)
    node:setLocalZOrder(self:zOrderForPosition(pos))
    self._belowObjectLayer:addChild(node)
end

function BattleLayer:addToObjectLayerByArea(object, area)
    local pos = G.center(area.rect)
    self:addToObjectLayerByPos(pos)
end

function BattleLayer:addToObjectLayerByPos(object, pos)
    self:addToObjectLayer(object, pos, pos)
end

function BattleLayer:addToObjectLayer(object, pos, center)
    object:setLocalZOrder(self:zOrderForPosition(center))
    object:setPosition(pos)
    self._objectLayer:addChild(object)
end

function BattleLayer:addToOnGroundLayer(object, pos, center)
    object:setLocalZOrder(self:zOrderForPosition(center))
    object:setPosition(pos)
    self._onGroundLayer:addChild(object)
end

function BattleLayer:addToStencil(node, pos)
    node:setPosition(pos)
    local zOrder = self:zOrderForPosition(pos)
    self:_getShadeNode():addStencilComponent(node, zOrder, 0)
end

function BattleLayer:zOrderForPosition(pos)
    return -pos.x - pos.y * self._tmxMap:getContentSize().width
end

function BattleLayer:isPositionInVision(pos)
    local size = self:getContentSize()
    local mapPos = G.getPosition(self._tmxMap)
    local x = pos.x + mapPos.x
    local y = pos.y + mapPos.y
    return x > 0 and x < size.width and y > 0 and y < size.height
end

function BattleLayer:addToFloatLayer(object, pos)
    self._floatLayer:addChild(object)
    self:adjustFloatNode(object, pos)
end

function BattleLayer:adjustFloatNode(node, pos)
    if (self:isPositionInVision(pos)) then
        node:setVisible(false)
    else
        local size = self:getContentSize()
        local mapPos = G.getPosition(self._tmxMap)
        local centerPos = cc.p(size.width / 2 - mapPos.x, size.height / 2 - mapPos.y)
        local deltaX = pos.x - centerPos.x
        local deltaY = pos.y - centerPos.y
        local ns = node:getContentSize()
        local x, y
        if (math.abs(deltaX) / math.abs(deltaY) > size.width / size.height) then
            y = (size.width / 2 - ns.width / 2) / math.abs(deltaX) * deltaY + size.height / 2
            x = deltaX > 0 and size.width - ns.width / 2 or ns.width / 2
        else
            y = deltaY > 0 and size.height - ns.height / 2 or ns.height / 2
            x = (size.height / 2 - ns.height / 2) / math.abs(deltaY) * deltaX + size.width / 2
        end
        node:setPosition(cc.p(x, y))
        node:setArrowPosition(deltaX, deltaY)
        node:setVisible(true)
    end
end

function BattleLayer:onCustomEvent(eventName)
    if eventName == "toggle_daynight" then
        self:_toggleDayNight(not self._isDay)
    end
end

function BattleLayer:onPause()
    if self._stage ~= Boids.BattleStage.pause and not self._net_ad then
        self:pauseGame()
    end
end

function BattleLayer:pauseGame()
    self:onStageChanged(Boids.BattleStage.pause)
end

function BattleLayer:resumeGame()
    self._stage = Boids.BattleStage.ongoing
    cc.Director:getInstance():resume()
end

function BattleLayer:exitGame()
    AudioManager.stopMusic()
    G.runLevelChooseScene()
    self:removeAllChildren()
    cc.Director:getInstance():resume()
    ResourceManager.purgeMap(self._mapPath)
end

-- private

function BattleLayer:_getShadeNode()
    if (self._shadeNode == nil) then
        local size = self._tmxMap:getContentSize()
        local layer = cc.LayerColor:create(cc.c4b(0, 38, 69, 127), size.width, size.height)
        layer:setBlendFunc(gl.DST_COLOR, gl.ONE_MINUS_SRC_ALPHA)
        self._shadeNode = UIShadeNode:new(layer)
        self._overObjectLayer:addChild(self._shadeNode, -1)
        self._shadeNode:getBackgroundLayer():setOpacity((self._isDay and 0 or 100))
        self._shadeNode._stencil:setVisible(not self._isDay)
    end
    return self._shadeNode
end

function BattleLayer:_toggleDayNight(isDay)
    local function trigger()
        self:_setDayNight(isDay)
        self:_getShadeNode()._stencil:setVisible(not isDay)
    end
    local fade = cc.FadeTo:create(1.5, (isDay and 0 or 100))
    local func = cc.CallFunc:create(trigger)
    local seq = cc.Sequence:create(fade, func)
    self:_getShadeNode():getBackgroundLayer():runAction(seq)
    if isDay then
        AudioManager.stopMusic()
        AudioManager.playMusic( "bg002_d", "common", true )
    else
        AudioManager.stopMusic()
        AudioManager.playMusic( "bg002_n", "common", true )
    end
end

function BattleLayer:_deployUnit(unit, position)
    local camp = unit:getCamp()
    local deployId = self:_fetchDeployId()
    unit:deploy(deployId, position)
    self._unitTable[deployId] = unit
    self._units[camp][deployId] = unit
    if (camp == Boids.UnitType.Player) then
        self:_onAddPlayerUnit(unit)
    end
    self._logic:onUnitAppear(unit)
end

function BattleLayer:_onAddPlayerUnit(unit)
    self._uiSkillLayer:addSkillNode(unit)
end

function BattleLayer:_createDebugPosition()
    local drawNode = cc.DrawNode:create()
    self._tmxMap:addChild(drawNode)
    local areas = self._logic:getAreas()
    for _, area in ipairs(areas) do
        local r = area.rect
        drawNode:drawRect(cc.p(r.x, r.y), cc.p(cc.rectGetMaxX(r), cc.rectGetMaxY(r)), cc.c4f(0, 1, 0, 0.5))
    end
end

function BattleLayer:_parseObjects()
    local namedObjects = {}
    local visionObjects = self._tmxMap:getObjectGroup("vision"):getObjects()
    for _, object in ipairs(visionObjects) do
        local property = self._tmxMap:getPropertiesForGID(object.gid)
        local resName = string.sub(property.source, 1, -5)
        local centerData = ResourceManager.centerData[resName]
        local centerPos
        if centerData then
            centerPos = cc.p(centerData.x, centerData.y)
        else
            centerPos = cc.p(property.width / 2, math.min(property.height, property.width) / 3.4641)
        end
        if object.name ~= nil then
            local classname
            if object.logic_class ~= nil then
                classname = require(string.format("logic_buildings/%s", object.logic_class))
            else
                classname = LogicBuilding
            end
            local logicBuilding = classname:new()
            logicBuilding:init(object, centerPos, resName, self)
            logicBuilding:deploy(self:_fetchDeployId())
            self._buildings[object.name] = logicBuilding
        else
            local objectSprite = cc.Sprite:createWithSpriteFrameName(property.source)
            objectSprite:setAnchorPoint(0, 0)
            objectSprite:setPosition(cc.p(object.x, object.y))
            self:addToObjectLayer(objectSprite, cc.p(object.x, object.y), cc.p(object.x + centerPos.x, object.y + centerPos.y))
        end
    end
end

function BattleLayer:_deployPlayerUnits(area)
    ue.LuaUtils:logf("_deployPlayerUnits")
    if not area then
        ue.LuaUtils:logf("player_start NOT FOUND!!")
    end
    local units = {}
    local team = PlayerInfo.getPlayerUnits()
    for _, name in ipairs(team) do
        local classname = cc.FileUtils:getInstance():isFileExist( name .. ".lua" ) and require( name ) or LogicUnit
        local unit = classname:new()
        --print("player init")
        unit:init(name, Boids.Camp.Player, 0, true, "", self)
        units[#units + 1] = unit
    end
    self:deployUnits(units, area, Boids.Camp.Player)
end

function BattleLayer:_deployPlayerUnitsFromGameInitData()
    ue.LuaUtils:logf("_deployPlayerUnits(pvp)")
    local area1 = self._logic:getAreaByName("player_start")
    if not area1 then
        ue.LuaUtils:logf("player_start NOT FOUND!!")
    end
    local area2 = self._logic:getAreaByName("player_start2")
    if not area2 then
        ue.LuaUtils:logf("player_start2 NOT FOUND!!")
    end
    local my_area
    for _, force_data in pairs(self._game_init_data) do
        local current_area
        if force_data.force_id == 1 then
            current_area = area1
        else
            current_area = area2
        end
        local current_camp
        if force_data.force_id == self._my_force_id then
            current_camp = Boids.Camp.Player
            my_area = current_area
        else
            current_camp = Boids.Camp.Opponent
        end

        local units = {}
        for unit_name, level in pairs(force_data.units) do            
            local classname = cc.FileUtils:getInstance():isFileExist( unit_name .. ".lua" ) and require( unit_name ) or LogicUnit
            local unit = classname:new()
            ue.LuaUtils:logf(string.format("init_%s_unit %s", current_camp, unit_name))
            unit:init(unit_name, current_camp, 0, true, "", self)
            units[#units + 1] = unit
        end
        self:deployUnits(units, current_area, current_camp)
    end
    self:centerMapPosition(G.center(my_area.rect))
end

function BattleLayer:_setup()
    self:_resetData()
    AI.reset()
    print( self._mapPath )
    local logicClass = MapLogic.mapPath2Class(self._mapPath)
    self._logic = logicClass:new()
    self._logic:initWithLogicJson(self._logicJson, self)
    self._effectLayer:removeAllChildren()
    self._belowObjectLayer:removeAllChildren()
    self._uiSkillLayer:clearSkillNodes()
    self:_clearUnits()
    self._objectLayer:removeAllChildren()
    self._overObjectLayer:removeAllChildren()
    self._buildings = {}
    self:_parseObjects()
    self._floatLayer:reset()
    if self._replay_to_save then
        self._replay_to_save:reset()
    end
    if self._replay_to_play then
        self._replay_to_play:reset()
    end
    if self._net_ad then
        self:_deployPlayerUnitsFromGameInitData()
    else
        local area = self._logic:getAreaByName("player_start")
        self:_deployPlayerUnits(area) --deploy之前AI必须reset过了
        self:centerMapPosition(G.center(area.rect))
    end
    self._logic:onMapInit()
    if self._logic._hasDayNight then
        self:_setDayNight(true)
        self._floatLayer:addDayNightUI()
    end
    if self._net_ad then 
        self._net_ad:start() --先把兵都刷完再连GameServer，免得开局那会儿会卡；这个调了之后，不收到包画面就不走了
    end
end

function BattleLayer:_clearUnits()
    if (self._unitTable ~= nil) then
        for _, unit in pairs(self._unitTable) do
            if (unit ~= nil) then
                unit._unitNode:removeFromBattle()
            end
        end
    end
end

function BattleLayer:_resetData()
    self._unitTable = {}
    self._units = {}
    for _, camp_value in pairs(Boids.Camp) do
        self._units[camp_value] = {}
    end
    self._currentDeployId = 1
end

function BattleLayer:_setDayNight(isDay)
    self._isDay = isDay
    local triggerName = isDay and "into_day" or "into_night"
    self._logic:onCustomTrigger(triggerName)
end

function BattleLayer:_fetchDeployId()
    local deployId = self._currentDeployId
    self._currentDeployId = self._currentDeployId + 1
    return deployId
end

return BattleLayer
