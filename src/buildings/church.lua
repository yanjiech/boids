local Building = require "buildings/building"
local Church = class("Church", Building)

Church.Resources = {
    bubble = "buildings/church_bubble",
    light = "buildings/church_light",
    star = "buildings/church_star",
}

function Church:init(data)
    self.super.init(self, data)
    self._bubble = ue.ArmatureManager:getInstance():createArmature(Church.Resources.bubble)
    self._light = ue.ArmatureManager:getInstance():createArmature(Church.Resources.light)
    self._star = ue.ArmatureManager:getInstance():createArmature(Church.Resources.star)
    local p = cc.p(self._data.center_x, self._data.center_y)
    self._bubble:setPosition(p)
    self._light:setPosition(p)
    self._star:setPosition(p)
    self:addChild(self._bubble)
    self:addChild(self._light)
    self:addChild(self._star)
    self._bubble:setAnimation(0, "animation", true)
    self._star:setAnimation(0, "animation", true)
    local function onAnimationEnded(event)
        if event.animation == "animation" then
            self._star:setVisible(true)
            self._light:setVisible(false)
        end
    end
    self._light:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
end

function Church:setState(state)
    self.super.setState(self, state)
    if state == "normal" then
        self._bubble:setVisible(false)
        self._light:setVisible(true)
        self._light:setAnimation(0, "animation", false)
    elseif state == "unnormal" then
        self._bubble:setVisible(true)
        self._light:setVisible(false)
        self._star:setVisible(false)
    end
end

return Church
