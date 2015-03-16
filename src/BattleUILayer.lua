local BattleUILayer = class("BattleUILayer", function() return cc.LayerColor:create(cc.c4b(255, 255, 255, 255), 600, 600) end)

local playAgain = "再玩一次"
local startGame = "开始游戏"
local resumeGame = "恢复游戏"
local exitGame = "退出游戏"

function BattleUILayer:ctor(...)
    local arg = {...}
    self._battleLayer = arg[2]
    local lv = ccui.ListView:create()
    lv:setContentSize(cc.size(600, 500))
    lv:setDirection(ccui.ListViewDirection.vertical)
    lv:setTouchEnabled(true)
    lv:setItemsMargin(40)
    lv:setPosition(cc.p(0, 0))
    lv:setGravity(ccui.ListViewGravity.centerHorizontal)
    lv:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
    lv:setBackGroundColor(cc.c3b(255, 255, 255))
    self:addChild(lv)
    self._listview = lv
    local tl = cc.Label:createWithSystemFont("", "Helvetica", 60)
    tl:setAnchorPoint(cc.p(0.5, 0.5))
    tl:setPosition(cc.p(300, 550))
    tl:setTextColor(cc.c4b(0, 0, 0, 1))
    self:addChild(tl)
    self._titleLabel = tl
    self:setPosition(cc.p(660, 240))
end

function BattleUILayer:loadTasks(tasks)
    self._tasks = tasks
end

function BattleUILayer:setStage(stage)
    if stage == Boids.BattleStage.before then
        self:setVisible(true)
        self._titleLabel:setString("关卡目标")
        self._listview:removeAllItems()
        for _,task in ipairs(self._tasks) do
            self:_addItem(task.desc)
        end
        self:_addButton(startGame)
    elseif stage == Boids.BattleStage.ongoing then
        self:setVisible(false)
    elseif stage == Boids.BattleStage.win then
        self:setVisible(true)
        self._titleLabel:setString("胜利")
        self._listview:removeAllItems()
        self:_addButton(playAgain)
        self:_addButton(exitGame)
    elseif stage == Boids.BattleStage.lose then
        self:setVisible(true)
        self._titleLabel:setString("失败")
        self._listview:removeAllItems()
        self:_addButton(playAgain)
    elseif stage == Boids.BattleStage.pause then
        self:setVisible(true)
        self._titleLabel:setString("暂停")
        self._listview:removeAllItems()
        self:_addButton(resumeGame)
        self:_addButton(exitGame)
    end
end

-- private

function BattleUILayer:_addItem(title)
    local btn = ccui.Button:create()
    btn:setContentSize(cc.size(600, 120))
    btn:setTitleText(title)
    btn:setTitleColor(cc.c3b(51, 102, 51))
    btn:setTitleFontSize(60)
    self._listview:pushBackCustomItem(btn)
end

function BattleUILayer:_addButton(title)
    local btn = ccui.Button:create()
    btn:setContentSize(cc.size(600, 120))
    btn:setTitleText(title)
    btn:setTitleColor(cc.c3b(0, 179, 255))
    btn:setTitleFontSize(60)
    local function touchEventHandler(ref, eventType)
        if (eventType == 0) then
            self:_onTitleTouched(title)
        end
    end
    btn:addTouchEventListener(touchEventHandler)
    self._listview:pushBackCustomItem(btn)
end

function BattleUILayer:_onTitleTouched(title)
    if title == playAgain then
        self._battleLayer:onStageChanged(Boids.BattleStage.before)
    elseif title == startGame then
        self._battleLayer:onStageChanged(Boids.BattleStage.ongoing)
    elseif title == resumeGame then
        self:setVisible(false)
        self._battleLayer:resumeGame()
    elseif title == exitGame then
        self:setVisible(false)
        self._battleLayer:exitGame()
    end
end

return BattleUILayer
