local Stunned = class("Stunned", nil)
local UnitNodeComponet = require "UnitNodeComponet"

function Stunned:ctor(...)
    local arg = {...}
    self._duration = arg[2]
    self._currentTime = 0
end

function Stunned:onAttach(logicUnit)
    local unitNode = logicUnit._unitNode
    local resource = "effects/stunned"
    local effect = ue.ArmatureManager:getInstance():createArmature(resource)
    effect:setAnimation(0, "animation", true)
    effect:setPosition(cc.p(0, unitNode:_getMaxY()))
    effect:setScale(0.75)
    unitNode:addComponet(effect, UnitNodeComponet.LayerType.SelfAbove, "buff_stunned")
    logicUnit:setState(Boids.UnitState.Stunned)
end

function Stunned:onDetach(logicUnit)
    local unitNode = logicUnit._unitNode
    unitNode:removeComponent("buff_stunned")
    ue.LuaUtils:logf(string.format("%d stun detached", logicUnit:getId()))
    logicUnit:setState(Boids.UnitState.Idle)
end

function Stunned:update(dt)
    self._currentTime = self._currentTime + dt
end

function Stunned:shouldRecycle()
    return self._currentTime >= self._duration
end

return Stunned
