local Building = require "buildings/building"
local HpBar = require "HpBar"
local AttackableBuilding = class("AttackableBuilding", Building)

local defaultHpSize = { height = 6, width = 100 }

function AttackableBuilding:init(data)
    self.super.init(self, data)
    self._hp = data.hp
    self._radius = data.radius
    self._hpBar = HpBar:new(defaultHpSize)
    local offsetY = self._hpBar:getContentSize().height * self._hpBar:getAnchorPoint().y
    offsetY = offsetY + self:getContentSize().height
    self._hpBar:setPosition(cc.p(self:getContentSize().width / 2, offsetY))
    self:addChild(self._hpBar)
end

function AttackableBuilding:onHpChanged(percentage)
    self._hpBar:setPercentage(percentage)
    if percentage <= 0 then
        self._hpBar:setVisible(false)
    end
end

return AttackableBuilding
