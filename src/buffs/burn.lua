local Burn = class( "Burn", nil )
local UnitNodeComponet = require( "UnitNodeComponet" )

function Burn:ctor( ... )
    local args = { ... }
    self._duration = args[2]
    self._accumulator = 0
end

function Burn:onAttach( logic_unit )
    local unit_node = logic_unit._unitNode
    unit_node:setGLProgramState( cc.SHADER_POSITION_TEXTURE_COLOR_BURN )
    local resource = "effects/burn"
    local burn_effect = ue.ArmatureManager:getInstance():createArmature( resource )
    burn_effect:setAnimation( 0, "animation", true )
    local hit_pos = unit_node:_getHitPosition()
    burn_effect:setPosition( cc.p( hit_pos.x, hit_pos.y - 50.0 ) )
    unit_node:addComponet( burn_effect, UnitNodeComponet.LayerType.SelfAbove, "buff_burn" ) 
end

function Burn:onDetach( logic_unit )
    local unit_node = logic_unit._unitNode 
    unit_node:setGLProgramState( cc.SHADER_POSITION_TEXTURE_COLOR )
    unit_node:removeComponent( "buff_burn" )
end

function Burn:update( dt )
    self._accumulator = self._accumulator + dt
end

function Burn:shouldRecycle()
    return self._accumulator >= self._duration
end

return Burn
