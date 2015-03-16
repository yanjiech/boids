local MagicPulse = class( "MagicPulse", function() return cc.Node:create() end )
local math = require( "math" )

function MagicPulse:ctor( ... )
    self._damage = SkillConf.skill["MagicPulse"]["damage"][1]
    self._range = SkillConf.skill["MagicPulse"]["range"]
    self._duration = SkillConf.skill["MagicPulse"]["duration"]
    self._interval = SkillConf.skill["MagicPulse"]["interval"]
    self._length = SkillConf.skill["MagicPulse"]["length"]
    self._accumulator = 0
    self._damage_accumulator = 0
end

function MagicPulse.calculateDamage( base_damage, atker_data, defer_data )
    return ( base_damage + 0.2 * atker_data["atk"] ) * DamageCalculate.calculateResistance( defer_data["def"], 0, 0 )
end

function MagicPulse:adjustPulse( src_pos, hit_pos )
    local rotation = math.atan2( hit_pos.y - self._emit_pos.y, hit_pos.x - self._emit_pos.x )
    self._pulse:setRotation( -rotation * 180.0 / math.pi ) 
    local distance = math.sqrt( cc.pDistanceSQ( src_pos, hit_pos ) )
    self._pulse:setScaleX( distance / self._length )
end

function MagicPulse:activate( ... )
    local function update( delta )
        self._accumulator = self._accumulator + delta
        self._damage_accumulator = self._damage_accumulator + delta
        if self._accumulator >= self._duration then
            self:unactivate()
        else
            local src_unit = self._battle_layer:getUnitById( self._src_id )
            if src_unit == nil then
                self:unactivate()
            else
                local target = self._battle_layer:getUnitById( self._target_id )
                if target then
                    local src_pos = src_unit:getPos()
                    local target_pos = target:getPos()
                    if MathExtend.isPositionInRange( src_pos, self._range, target_pos ) then
                        local hit_pos = target._unitNode:getHitPos()
                        self:adjustPulse( self._emit_pos, hit_pos )
                        self._hit_effect:setPosition( hit_pos )
                        if self._damage_accumulator >= self._interval then
                            self._damage_accumulator = 0
                            local damage = MagicPulse.calculateDamage( self._damage, self._atker_data, target:getRawData() )
                            target:takeDamage( damage, self._src_id )
                        end
                    else
                        self:unactivate()
                    end
                else
                    self:unactivate()
                end
            end
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_id = src_unit:getId()
    self._atker_data = src_unit:getRawData()
    self._skill_id = args[5]

    local src_pos = src_unit:getPos()
    local target = src_unit:getTarget()
    if target == nil then
        local units = AI.getOpponentsByCamp( src_unit:getCamp() )
        if units then
            for i,v in pairs( units ) do
                local unit_pos = v:getPos()
                if MathExtend.isPositionInRange( src_pos, self._range, unit_pos ) then
                    target = v
                    break
                end
            end
        end
    end
    if target == nil then
        return false
    else
        local target_pos = target:getPos()
        if MathExtend.isPositionInRange( src_pos, self._range, target_pos ) then
            self._target_id = target:getId()
            local armature = src_unit._unitNode:currentArmature()
            local bone_pos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
            bone_pos = armature:convertToWorldSpace( bone_pos )
            self._emit_pos = self._battle_layer._effectLayer:convertToNodeSpace( bone_pos )
            local resource = "effects/women_wizard_skill_1/pulse"
            self._pulse = ue.ArmatureManager:getInstance():createArmature( resource )
            self._battle_layer:addEffect( self._pulse, self._emit_pos, 0 )
            self._pulse:setAnimation( 0, "animation", true )
            local hit_pos = target._unitNode:getHitPos()
            self:adjustPulse( self._emit_pos, hit_pos )
            resource = "effects/women_wizard_skill_1/hit"
            self._hit_effect = ue.ArmatureManager:getInstance():createArmature( resource )
            self._hit_effect:setAnimation( 0, "animation", true )
            self._battle_layer:addEffect( self._hit_effect, hit_pos, 1 )
        else
            return false
        end
    end
    
    src_unit:setState( Boids.UnitState.Cast, self._skill_id, 1 )
    src_unit._ignore_next_idle_state = true
    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    return true
end

function MagicPulse:unactivate()
    if self._pulse then
        self._pulse:clearTracks()
        self._pulse:getParent():removeChild( self._pulse, true )
    end
    if self._hit_effect then
        self._hit_effect:clearTracks()
        self._hit_effect:getParent():removeChild( self._hit_effect, true )
    end
    local src_unit = self._battle_layer:getUnitById( self._src_id )
    if src_unit then
        src_unit:setState( Boids.UnitState.Cast, self._skill_id, 2 )
    end

    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return MagicPulse
