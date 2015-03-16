local BattleFloatLayer = class("BattleFloatLayer", function() return cc.Layer:create() end)

local topMargin = 12

function BattleFloatLayer:ctor(...)
    local arg = {...}
    self._battleLayer = arg[2]
    self:setContentSize(self._battleLayer:getContentSize())
    local size = self:getContentSize()
    local killCounterBackground = cc.Sprite:createWithSpriteFrameName("battle_killcounter.png")
    local clockBackground = cc.Sprite:createWithSpriteFrameName("battle_clock.png")
    local origin = cc.Director:getInstance():getVisibleOrigin()
    killCounterBackground:setPosition(cc.p(size.width - 144, size.height - 47))
    clockBackground:setPosition(cc.p(size.width - 393, size.height - 47))
    self:addChild(killCounterBackground)
    self:addChild(clockBackground)
    self._timeLabel = cc.Label:createWithCharMap("ui/ui_numberfont.png", 27, 35, 48)
    self._killCountLabel = cc.Label:createWithCharMap("ui/ui_numberfont.png", 27, 35, 48)
    self._timeLabel:setAlignment(2, 1)
    self._killCountLabel:setAlignment(2, 1)
    self._killCountLabel:setAnchorPoint(cc.p(0.5, 0.5))
    self._timeLabel:setAnchorPoint(cc.p(0.5, 0.5))
    self._timeLabel:setPosition(cc.p(150, 38))
    self._killCountLabel:setPosition(cc.p(167, 38))
    killCounterBackground:addChild(self._killCountLabel)
    clockBackground:addChild(self._timeLabel)
    -- setup pause button
    local pauseButton = ccui.Button:create()
    pauseButton:loadTextureNormal("pause_normal.png", 1)
    pauseButton:loadTexturePressed("pause_push.png", 1)
    pauseButton:setPosition(cc.p(110, size.height - 80))
    self:addChild(pauseButton)
    local function onPauseClicked(sender, eventType)
        if eventType == 2 then
            self._battleLayer:onPause()
        end
    end
    pauseButton:addTouchEventListener(onPauseClicked)
end

function BattleFloatLayer:reset()
    self._second = 0
    self._minute = 0
    self._killCount = 0
    self._timeLabel:setString("00:00")
    self._killCountLabel:setString("0")
end

function BattleFloatLayer:addDayNightUI()
    local size = self:getContentSize()
    local clippingNode = cc.ClippingNode:create()
    local maskSprite = cc.Sprite:createWithSpriteFrameName("day_night_mask.png")
    clippingNode:setAlphaThreshold(0.5)
    clippingNode:setStencil(maskSprite)
    clippingNode:setPosition(cc.p(size.width / 2, size.height - 77))
    self._dayNightArmature = ue.ArmatureManager:getInstance():createArmature("ui/day_and_night")
    self._dayNightArmature:setTimeScale(1.0/30)
    clippingNode:addChild(self._dayNightArmature)
    self:addChild(clippingNode)
end

function BattleFloatLayer:cleanup()
    ue.ArmatureManager:getInstance():unloadArmatureData("ui/day_and_night")
end

function BattleFloatLayer:update(dt)
    self._second = self._second + dt
    if self._second >= 60 then
        self._second = self._second - 60
        self._minute = self._minute + 1
    end
    self._timeLabel:setString(string.format("%02d:%02d", self._minute, self._second))
end

function BattleFloatLayer:onGameStart()
    if self._dayNightArmature ~= nil then
        self:startDayNightAnimation()
    end
end

function BattleFloatLayer:addKillCount()
    self._killCount = self._killCount + 1
    self._killCountLabel:setString(string.format("%d", self._killCount))
end

function BattleFloatLayer:startDayNightAnimation()
    self._dayNightArmature:clearTracks()
    self._dayNightArmature:setAnimation(0, "begin", false)
    self._dayNightArmature:addAnimation(0, "night_to_day", false)
    local function onAnimationEnded(event)
        if event.animation == "day_to_night" then
            self._dayNightArmature:clearTracks()
            self._dayNightArmature:setAnimation(0, "night_to_day", false)
        elseif event.animation == "night_to_day" then
            self._dayNightArmature:clearTracks()
            self._dayNightArmature:setAnimation(0, "day_to_night", false)
        end
    end
    self._dayNightArmature:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
end

return BattleFloatLayer
