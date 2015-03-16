local LevelChooseLayer = class("LevelChooseLayer", function() return cc.Layer:create() end)
local json = cjson

local levels = {
    {
        name = "第一排第一关",
        path = "maps/level_1"
    },
    {
        name = "第一排第二关",
        path = "maps/level_2"
    },
    {
        name = "第一排第三关",
        path = "maps/level_3",
    },
    {
        name = "第一排第四关",
        path = "maps/level_4",
    },
    {
        name = "第一排第五关",
        path = "maps/level_5",
    },
    {
        name = "第一排第六关",
        path = "maps/level_6"
    },
    {
        name = "第一排第七关",
        path = "maps/level_7"
    },
    {
        name = "第一排第八关",
        path = "maps/map06",
    },
    {
        name = "第一排第九关",
        path = "maps/eg2",
    },
    {
        name = "第二排第一关",
        path = "maps/demo2",
    },
    {
        name = "第二排第二关",
        path = "maps/map107",
        pvp = true
    },
    {
        name = "第二排第三关",
        path = "maps/map106",
        pvp = true
    },
    {
        name = "第二排第四关",
        path = "maps/forest08",
    },
    {
        name = "第二排第五关",
        path = "maps/forest02_2",
    },
    {
        name = "第二排第六关",
        path = "maps/small03",
    },
    {
        name = "第二排第七关",
        path = "maps/demo2",
    },
    {
        name = "第二排第八关",
        path = "maps/forest03",
    },
    {
        name = "第二排第九关",
        path = "maps/small02",
        pvp = true
    },
    {
        name = "第三排第一关",
        path = "maps/eg7",
    },
    {
        name = "第三排第二关",
        path = "maps/eg6",
    },
    {
        name = "第三排第三关",
        path = "maps/eg1",
    },
    {
        name = "第三排第四关",
        path = "maps/eg5",
    },
    {
        name = "第三排第五关",
        path = "maps/eg6",
    },
    {
        name = "第三排第六关",
        path = "maps/eg8",
    },
    {
        name = "第三排第七关",
        path = "maps/eg8",
    },
    {
        name = "第三排第八关",
        path = "maps/eg8",
    },
    {
        name = "第三排第九关",
        path = "maps/eg8",
    }
}

function LevelChooseLayer:ctor(...)
    local maincsb = "level_choose/main/main.csb"
    local levelcsb = "level_choose/level/level.csb"
    local mainNode = cc.CSLoader:createNode(maincsb)
    local levelNode = cc.CSLoader:createNode(levelcsb)
    self:addChild(mainNode)
    self:addChild(levelNode)
    levelNode:setVisible(false)
    local scrollView = mainNode:getChildByName("scrollView")
    local background = levelNode:getChildByName("background")
    local backButton = background:getChildByName("backButton")
    local startButton = background:getChildByName("battleButton")
    local task1Label = background:getChildByName("mission1")
    local task2Label = background:getChildByName("mission2")
    local task3Label = background:getChildByName("mission3")
    local levelInfoLabel = background:getChildByName("missionText")
    local labels = {task1Label, task2Label, task3Label}
    local function onLevelButtonClicked(sender, eventType)
        if eventType == 2 then
            local tag = sender:getTag()
            self._currentLevel = levels[tag]
            mainNode:setVisible(false)
            levelNode:setVisible(true)
            self._mapData = ue.MapData:new(self._currentLevel.path)
            local metaStr = self._mapData:getMetaData()
            self._logicJson = json.decode(metaStr)
            if self._logicJson.tasks then
                for i, task in pairs(self._logicJson.tasks) do
                    labels[i]:setString(task.desc)
                end
            else
                for _, label in pairs(labels) do
                    label:setString("")
                end
            end
            if self._logicJson.desc then
                levelInfoLabel:setString(self._logicJson.desc)
            else
                levelInfoLabel:setString("")
            end

            if self._currentLevel.pvp then
                local function autoMatchResponse(ret_value, ret_info, game_init_data)
                    if ret_value == 0 then
                        --匹配成功
                        print("autoMatch success!")
                        self._match_success = true
                        G._game_init_data = game_init_data
                    else
                        print("autoMatch failed.", ret_value, ret_info)
                    end
                end
                ue.Net.autoMatch(autoMatchResponse, self._currentLevel.path)
            end
        end
    end
    local levels_count = table.maxn(levels)
    for i=1, levels_count do
        local level_i_Button = scrollView:getChildByName("level"..i.."Button")
        if level_i_Button then
            level_i_Button:addTouchEventListener(onLevelButtonClicked)
        end
    end
    local function onBackButtonClicked(sender, eventType)
        if eventType == 2 then
            mainNode:setVisible(true)
            levelNode:setVisible(false)
        end
    end
    local function onStartButtonClicked(sender, eventType)
        if eventType == 2 then
            self:_onLevelClicked(self._currentLevel)
        end
    end
    backButton:addTouchEventListener(onBackButtonClicked)
    startButton:addTouchEventListener(onStartButtonClicked)
end

-- private

function LevelChooseLayer:_addLevels()
    for _, level in ipairs(levels) do
        self:_addLevel(level)
    end
end

function LevelChooseLayer:_addLevel(level)
    local btn = ccui.Button:create()
    btn:setContentSize(cc.size(600, 120))
    btn:setTitleText(level.name)
    btn:setTitleColor(cc.c3b(0, 179, 255))
    btn:setTitleFontSize(60)
    local function touchEventHandler(ref, eventType)
        if (eventType == 2) then
            self:_onLevelClicked(level)
        end
    end
    btn:addTouchEventListener(touchEventHandler)
    self._listview:pushBackCustomItem(btn)
end

function LevelChooseLayer:_onLevelClicked(level)
    if level.pvp then
        if not self._match_success then
            return
        end
    end
    G.runeBattleScene(level.path, self._mapData, self._logicJson, level.pvp)
end

return LevelChooseLayer
