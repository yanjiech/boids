local Vampire = class("LogicUnit", require "LogicUnit")

function Vampire:init(...)
    self.super.init(self, ...)
    self._speed_scale = 0.7
    self._low_enemy = true
end

function Vampire:postDeploy()
    self:setSightGroup('')
end

function Vampire:getOpponents()
    local p = self._currentHP / self._fullHP
    if p > 0.25 then
        return {}
    else
        return AI._players
    end
end

function Vampire:beforeBehavior()
    if not self._chasing_target and not self.c_unit:hasPath() then
        local destRegion = self._battleLayer._logic:getAreaByName("vampire_refuge")
        local destPosition = G.center(destRegion.rect)
        self:walkToDestination(destPosition, nil, false)
    end
end

function Vampire:takeDamage(value, srcId)
    if self._currentHP <= 0 then return end
    self._currentHP = self._currentHP - value
    self._unitNode:takeDamage(value)
    if self._currentHP / self._fullHP < 0.25 then
        self._speed_scale = 1
    end
    if (self._currentHP <= 0) then
        self:unitDie()
    end
end

return Vampire
