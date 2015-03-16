local LogicBuilding = require "LogicBuilding"
local LogicShelter = class("LogicShelter", LogicBuilding)
local EventAction = require "EventAction"
local wave = 3

function LogicShelter:init(data, centerPos, source, battleLayer)
    LogicBuilding.init(self, data, centerPos, source, battleLayer)
    if self._data["type"] == "shelter" then
        self._total = self._battleLayer._logicJson.custom_data.small_church_count
        self._bonus = self._battleLayer._logicJson.custom_data.small_church_bonus
    else
        self._total = self._battleLayer._logicJson.custom_data.big_church_count
        self._bonus = self._battleLayer._logicJson.custom_data.big_church_bonus
    end
    self._triggered = false
end

function LogicShelter:deploy(deployId)
    LogicBuilding.deploy(self, deployId)
    local posName = string.format("%s_exit", self._data.name)
    self._area = self._battleLayer._logic:getAreaByName(posName)
end

function LogicShelter:onSeen()
    --if not self._triggered and self._state == "normal" then
        --print(string.format("%s triggered", self._data.name))
        --self._triggered = true
        --local ea = EventAction:new(wave - 1, 0, 8)
        --local countPerWave = self._total / wave
        --local function generateVampire(finish)
            --self:_generateVampire(countPerWave)
        --end
        --ea:schedule(generateVampire, true)
    --end
end

function LogicShelter:setState(state)
    LogicBuilding.setState(self, state)
    if state == "unnormal" then
        self:_generateVampire(self._bonus)
    end
end

function LogicShelter:_generateVampire(count)
    local units = {}
    for i = 1, count do
        local randomInt = ue.LuaUtils:randomNumber(2)
        local unitName = randomInt == 1 and "thin_vampire" or "fat_vampire"
        local classname = require "units/vampire"
        local unit = classname:new()
        unit:init(unitName, Boids.UnitType.Enemy, 0, false, "", self._battleLayer) --可以给生成个方向（最后个参数）
        units[#units + 1] = unit
    end
    self._battleLayer:deployUnits(units, self._area, "")
    print("deploy vampires")
end

return LogicShelter
