local Poison = class( "Poison", nil )
local UnitNodeComponet = require( "UnitNodeComponet" )

function Poison:ctor( ... )
    local args = { ... }
    self._duration = args[2]
    self._damage_per_sec = args[3]
    self._accumulator = 0
end

function Poison:onAttach( logic_unit )
    local unit_node = logic_unit._unitNode
    unit_node:setGLProgramState( cc.SHADER_POSITION_TEXTURE_COLOR_POISON )
    local resource = "effects/poison"
    local poison_effect = ue.ArmatureManager:getInstance():createArmature( resource )
    local hit_pos = unit_node:_getHitPosition()
    poison_effect:setPosition( cc.p( hit_pos.x, hit_pos.y - 40.0 ) )
    poison_effect:setAnimation( 0, "animation", true ) 
    poison_effect:setScale( 0.7 )
    unit_node:addComponet( poison_effect, UnitNodeComponet.LayerType.SelfAbove, "buff_poison" )
end

function Poison:onDetach( logic_unit )
    local unit_node = logic_unit._unitNode
    unit_node:setGLProgramState( cc.SHADER_POSITION_TEXTURE_COLOR )
    unit_node:removeComponent( "buff_poison" )
end

function Poison:update( dt )
    self._accumulator = self._accumulator + dt
end

function Poison:shouldRecycle()
    return self._accumulator >= self._duration
end

return Poison
