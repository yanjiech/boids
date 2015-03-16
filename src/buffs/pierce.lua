local Pierce = class( "Pierce", nil )
local UnitNodeComponet = require( "UnitNodeComponet" )

function Pierce:ctor( ... )
    local args = { ... }
    self._duration = args[2]
    self._accumulator = 0
end

function Pierce:onAttach( logic_unit )
    local resource = "effects/blood"
    local blood_effect = ue.ArmatureManager:getInstance():createArmature( resource )
    blood_effect:setAnimation( 0, "animation", true )
    local hit_pos = logic_unit._unitNode:_getHitPosition()
    local blood_pos = cc.p( hit_pos.x, hit_pos.y - 50 ) 
    blood_effect:setPosition( blood_pos )
    logic_unit._unitNode:addComponet( blood_effect, UnitNodeComponet.LayerType.SelfAbove, "blood" )

    resource = "effects/dracula_skill_1/bat"
    local bat_effect = ue.ArmatureManager:getInstance():createArmature( resource )
    bat_effect:setAnimation( 0, "animation", true )
    bat_effect:setPosition( cc.p( -30.0, logic_unit._unitNode:_getMaxY() - 50.0 ) )
    bat_effect:setRotationSkewY( 180.0 )
    logic_unit._unitNode:addComponet( bat_effect, UnitNodeComponet.LayerType.SelfAbove, "bat1" )

    local bat_effect_2 = ue.ArmatureManager:getInstance():createArmature( resource )
    bat_effect_2:setAnimation( 0, "animation", true )
    bat_effect_2:setPosition( cc.p( 50.0, logic_unit._unitNode:_getMaxY() - 80.0 ) )
    logic_unit._unitNode:addComponet( bat_effect_2, UnitNodeComponet.LayerType.SelfAbove, "bat2" )

    logic_unit:setState( Boids.UnitState.Pierce, 0.1, 30.0 )
end

function Pierce:onDetach( logic_unit )
    local unit_node = logic_unit._unitNode
    unit_node:removeComponent( "blood" )
    unit_node:removeComponent( "bat1" )
    unit_node:removeComponent( "bat2" )
    logic_unit:setState( Boids.UnitState.Idle )
end

function Pierce:update( dt )
    self._accumulator = self._accumulator + dt
end

function Pierce:shouldRecycle()
    return self._accumulator >= self._duration
end

return Pierce
