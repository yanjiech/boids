local LogicUnit = require "LogicUnit"
local CommonLogicUnit = class("CommonLogicUnit", LogicUnit)

function CommonLogicUnit:ctor(...)
    local arg = {...}
    local name = arg[2]
    self._data = ResourceManager.getUnitData(name)
    self:set_unit_camp(arg[3])
    local guardRadius = arg[4]
    if guardRadius ~= 0 then
        self._guardRadius = guardRadius
    else
        self._guardRadius = self._data.guard_radius
    end
    local showHp = arg[5]
    self._tag = arg[6]
    self._buff = arg[7]
    self._battleLayer = arg[8]
    self._currentHP = self._data.hp
    self._state = Boids.UnitState.unknown
    self._castSkillId = 0
end

return CommonLogicUnit
