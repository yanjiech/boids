local MissileShowers = class( "MissileShowers", function() return cc.Node:create() end )
local Bullet = require( "Bullet" )

function MissileShowers:ctor( ... )
    self._damage = SkillConf.skill["MissileShowers"]["damage"][1]
    self._interval = SkillConf.skill["MissileShowers"]["interval"]
    self._count = SkillConf.skill["MissileShowers"]["count"]
    self._range = SkillConf.skill["MissileShowers"]["range"]
    self._accumulator = 0
    self._current_count = 0
end

function MissileShowers.calculateDamage( base_damage, atker_data, defer_data )
    return ( base_damage + 0.2 * atker_data["atk"] ) * DamageCalculate.calculateResistance( defer_data["def"], 0, 0 )
end

function MissileShowers:activate( ... )
    local function update( delta )
        local src_unit = self._battle_layer:getUnitById( self._src_id )
        if src_unit == nil then
            self:unactivate() 
        else
            if self._current_count < self._count then
                self._accumulator = self._accumulator + delta
                if self._accumulator >= self._interval then
                    self._current_count = self._current_count + 1
                    self._accumulator = 0
                    
                    local target_unit = self._battle_layer:getUnitInRangeFromTable( AI.getOpponentsByCamp( self._src_camp ), self._src_pos, self._range )

                    local bullet = Bullet:new( ResourceManager.getBulletData( "rattletrap_bullet" ), self._damage, MissileShowers )
                    if target_unit then
                        bullet:shoot( src_unit, self._battle_layer, target_unit )
                    else
                        local target_pos = MathExtend.randomPositionFromCenter( self._src_pos, self._range )
                        bullet:shoot( src_unit, self._battle_layer, target_pos, true )
                    end
                end
            end
            if self._current_count >= self._count then
                self:unactivate()
            end
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_camp = src_unit:getCamp()
    self._src_id = src_unit:getId()
    self._atker_data = src_unit:getRawData()
    self._skill_id = args[5]
    self._src_pos = src_unit:getPos()
    local armature = src_unit._unitNode:currentArmature()
    local bone_pos = cc.p( ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" ) )
    self._emit_pos = self._battle_layer._effectLayer:convertToNodeSpace( armature:convertToWorldSpace( bone_pos ) )
    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    src_unit:setState( Boids.UnitState.Cast, self._skill_id, 1 ) 
    src_unit._ignore_next_idle_state = true
    self:scheduleUpdateWithPriorityLua( update, 0 )
    return true
end

function MissileShowers:unactivate()
    local src_unit = self._battle_layer:getUnitById( self._src_id )
    if src_unit then
        src_unit:setState( Boids.UnitState.Cast, self._skill_id, 2 )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return MissileShowers
