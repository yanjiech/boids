local LightShield = class( "LightShield", function() return cc.Node:create() end )

function LightShield:ctor( ... )
    self._duration = SkillConf.skill["LightShield"]["duration"]
    self._hp = SkillConf.skill["LightShield"]["hp"][1]
    self._accumulator = 0
end

function LightShield:absorbDamage( value )
    self._hp = self._hp - value
    local ret = self._hp
    if ret <= 0 then
        self:unactivate()
        return -ret
    end
    return 0
end

function LightShield:activate( ... )
    local function update( delta )
        self._accumulator = self._accumulator + delta
        if self._accumulator > self._duration then
            self:unactivate()
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_id = src_unit:getId()
    local resource = "effects/light_shield"
    self._effect = ue.ArmatureManager:getInstance():createArmature( resource )
    self._effect:setScale(0.6)
    self._effect:setAnimation( 0, "animation", true )
    self:addChild( self._effect, 1, 1 )
    src_unit:addShield( self )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    return true
end

function LightShield:unactivate()
    self._effect:clearTracks()
    self:unscheduleUpdate()
    local src_unit = self._battle_layer:getUnitById( self._src_id )
    if src_unit then
        src_unit:removeShield()
    end
end

return LightShield
