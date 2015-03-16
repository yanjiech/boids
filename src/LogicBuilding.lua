local LogicTarget = require "LogicTarget"
local Building = require "buildings/building"
local BuildingData = require "BuildingData"
local LogicBuilding = class("LogicBuilding", LogicTarget)

function LogicBuilding:init(data, centerPos, source, battleLayer)
    self._data = BuildingData.new(data, centerPos, source)
    if self:attackable() then
        LogicTarget.init(self, data.name, data.camp, data.radius)
        self._fullHp = data.hp
        self._hp = data.hp
    end
    self._battleLayer = battleLayer
    self._state = self._data.state
    self._category = "building"
end

function LogicBuilding:takeDamage(value, srcId)
    if self:attackable() then
        self._hp = self._hp - value
        if self._hp < 0 then self._hp = 0 end
        if self._node.onHpChanged ~= nil then
            self._node:onHpChanged(self._hp / self._fullHp)
        end
        if self._hp <= 0 then
            self:setState("unnormal")
            self:_deleteSelf()
        end
    end
end

function LogicBuilding:takeNormalAttack(attackerData)
    self:takeDamage(attackerData.atk,0)
end

function LogicBuilding:deploy(deployId)
    local pos = self._data:getCenterPos()
    if self:attackable() then
        LogicTarget.deploy(self, deployId, pos)
    end
    local class
    if self._data.vision_class ~= nil then
        class = require(string.format("buildings/%s", self._data.vision_class))
    elseif self:attackable() then
        class = require "buildings/AttackableBuilding"
    else
        class = Building
    end
    local building = class:new()
    building:init(self._data)
    building:setState(self._state)
    self._battleLayer:addToObjectLayer(building, self._data:getPos(), pos)
    self._node = building
end

function LogicBuilding:setState(state)
    if _state ~= state then
        self._state = state
        if self._node ~= nil then
            self._node:setState(state)
        end
        self._battleLayer._logic:onVisionChanged(self._data.name, self._data.tag, state)
    end
end

function LogicBuilding:attackable()
    return self._data.is_target ~= nil
end

function LogicBuilding:canAttack()
end

return LogicBuilding
