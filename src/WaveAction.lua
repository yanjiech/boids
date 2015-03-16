local EventAction = require "EventAction"
local LogicUnit = require "LogicUnit"
local WaveAction = class("WaveAction", nil)

function WaveAction:ctor(...)
    local arg = {...}
    self._waves = arg[2].waves
    self._tag = arg[3]
    self._logic = arg[4]
    self._currentWave = 1
end

function WaveAction:getTag()
    return self._tag
end

function WaveAction:excute()
    if self._currentWave <= #self._waves then
        local waveData = self._waves[self._currentWave]
        self._currentWave = self._currentWave + 1
        local function doExcute(finish)
            local areas = {}
            if waveData.position_name ~= nil then
                areas[1] = self._logic:getAreaByName(waveData.position_name)
            else
                areas = self._logic:getAreasByTag(waveData.position_tag)
            end
            for _, area in ipairs(areas) do
                local units = {}
                for _, unit in ipairs(waveData.units) do
                    if unit.class_name then
                        classname = require(string.format("units/%s", unit.class_name))
                    else
                        classname = LogicUnit
                    end
                    for i = 1, unit.count do
                        local logicUnit = classname:new()
                        logicUnit:init(unit.name, Boids.UnitType.Enemy, 0, false, "", self._logic._battleLayer)
                        logicUnit:addTag(self._tag)
                        logicUnit:setBoss(unit.is_boss)
                        units[#units + 1] = logicUnit
                    end
                end
                local sightGroup = string.format("%s_%s", area.name, self._tag)
                self._logic._battleLayer:deployUnits(units, area, sightGroup)
            end
        end
        local ea = EventAction:new(0, waveData.interval, 0)
        ea:schedule(doExcute, true)
    end
end

return WaveAction
