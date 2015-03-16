local Frozen = class( "Frozen", nil )

function Frozen:ctor( ... )
    local args = { ... }
    self._duration = args[2]
    self._accumulator = 0
end

function Frozen:onAttach( logic_unit )
    local unit_node = logic_unit._unitNode
    unit_node:setGLProgramState( cc.SHADER_POSITION_TEXTURE_COLOR_ICE )
end

function Frozen:onDetach( logic_unit )
    local unit_node = logic_unit._unitNode
    unit_node:setGLProgramState( cc.SHADER_POSITION_TEXTURE_COLOR )
end

function Frozen:update( dt )
    self._accumulator = self._accumulator + dt
end

function Frozen:shouldRecycle()
    return self._accumulator >= self._duration
end

return Frozen
