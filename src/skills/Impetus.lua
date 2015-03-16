local Impetus = class( "Impetus", function() return cc.Node:create() end )
local Bullet = require( "Bullet" )

function Impetus:ctor( ... )
    self._damage = SkillConf.skill["Impetus"]["damage"][1]
    self._range = SkillConf.skill["Impetus"]["range"]
    self._buff_duration = SkillConf.skill["Impetus"]["buff_duration"]
end

function Impetus.calculateDamage( base_damage, atker_data, defer_data )
    return ( base_damage + 0.2 * atker_data.atk ) * DamageCalculate.calculateResistance( defer_data.def, 0, 0 )
end

function Impetus:activate( ... )
    local args = { ... }
    local battle_layer = args[1]
    local src_unit = args[2]
    local target = src_unit:getTarget()
    local src_pos = src_unit:getPos()
    if target then
        local target_pos = target:getPos()
        if cc.pDistanceSQ( src_pos, target_pos ) >= self._range * self._range then
            target = nil
        end
    end
    if target == nil then
        local units = AI.getOpponentsByCamp( src_unit:getCamp() ) 
        for i,v in pairs( units ) do
            local target_pos = v:getPos()
            if cc.pDistanceSQ( src_pos, target_pos ) < self._range * self._range then
                target = v
                break
            end
        end
    end
    if target then 
        local armature = src_unit._unitNode:currentArmature()
        local bone_pos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
        local emit_pos = armature:convertToWorldSpace( bone_pos )
        local bullet = Bullet:new( ResourceManager.getBulletData( "enchantress_skill_javelin" ), self._damage, Impetus, { ["name"] = "poison", ["duration"] = self._buff_duration } )
        bullet:shoot( src_unit, battle_layer, target )
        return true
    end
    return false
end

function Impetus:unactivate()
end

return Impetus
