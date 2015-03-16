local EvilWizard = class( "EvilWizard", require( "LogicUnit" ) )
local UnitNodeComponet = require( "UnitNodeComponet" )

function EvilWizard:ctor( ... )
    local args = { ... }
    EvilWizard.super.ctor( args[1], ... )
end

function EvilWizard:useSkillById( i, dir, range )
    if self.super.useSkillById( self, i, dir, range ) == false then
        return false
    end
    if i == 1 then
        self:startInvoke() 
    end
    return true
end

function EvilWizard:startInvoke()
    print( "start invoke" )
    self._totemup = ue.ArmatureManager:getInstance():createArmature( "effects/evil_invoke/up" )
    self._totemup:setPosition( cc.p( 0, 0 ) )
    self._unitNode:addComponet( self._totemup, UnitNodeComponet.LayerType.SelfAbove, "totemup" )
    self._totemup:setAnimation( 0, "animation", true )
    self._totemdown = ue.ArmatureManager:getInstance():createArmature( "effects/evil_invoke/down" )
    self._totemdown:setPosition( cc.p( 0, 0 ) )
    self._unitNode:addComponet( self._totemdown, UnitNodeComponet.Layer.SelfBelow, "totemdown" )
    self._totemdown:setAnimation( 0, "animation", true )
end

function EvilWizard:endInvoke()
    self._unitNode:removeComponent( "totemup" )
    self._unitNode:removeComponent( "totemdown" )
end

return EvilWizard
