local BarrageOfArrow = class( "BarrageOfArrow", function() return cc.Node:create() end )
local math = require( "math" )

function BarrageOfArrow:ctor( ... )
    self._damage = SkillConf.skill["BarrageOfArrow"]["damage"][1]
    self._angle = SkillConf.skill["BarrageOfArrow"]["angle"]
    self._count_per_wave = SkillConf.skill["BarrageOfArrow"]["count_per_wave"]
    self._waves = SkillConf.skill["BarrageOfArrow"]["waves"] 
    self._interval = SkillConf.skill["BarrageOfArrow"]["interval"]
    self._speed = SkillConf.skill["BarrageOfArrow"]["speed"]
    self._range = SkillConf.skill["BarrageOfArrow"]["range"]
    self._radius = SkillConf.skill["BarrageOfArrow"]["radius"]
    self._hit_effects = {}
    self._effects = {}
    self._exclude_targets = {}
    self._arrows = {}
    self._arrow_angles = {}
    self._arrow_dirs = {}
    self._is_arrow_end = {}
    for i = 1, self._waves do
        table.insert( self._arrows, {} )
        local arrow_exclude_targets = {}
        local arrow_end = {}
        for j = 1,self._count_per_wave do
            table.insert( arrow_exclude_targets, {} )
            table.insert( arrow_end, false )
        end
        table.insert( self._exclude_targets, arrow_exclude_targets )
        table.insert( self._is_arrow_end, arrow_end )
    end
    self._accumulator = self._interval
    self._current_wave = 0
    self._end_wave = 1
    self._end_count = 0
    self._cast_end_accumulator = 0
end

function BarrageOfArrow.calculateDamage( base_damage, atker_data, defer_data )
    return ( base_damage + 0.2 * atker_data["atk"] ) * DamageCalculate.calculateResistance( defer_data["def"], 0, 0 )
end

function BarrageOfArrow:activate( ... )
    local function onAnimationEnded( event )
        if event.animation == "disappear" then
            self._end_count = self._end_count + 1
            local arrow = self._arrows[self._end_wave][self._end_count]
            arrow:setVisible( false )
            if self._end_count >= self._count_per_wave then
                self._end_wave = self._end_wave + 1
                self._end_count = 0
            end
            if self._end_wave > self._waves then
                self._is_end = true
            end
        end
    end

    local function update( delta )
        --update old arrows
        local units = AI.getOpponentsByCamp( self._src_camp )
        for i,v in pairs( self._arrows ) do
            for j,u in pairs( v ) do
                if u:isVisible() then
                    local old_pos = cc.p( u:getPosition() )
                    local new_pos = cc.pAdd( old_pos, cc.pMul( self._arrow_dirs[j], self._speed * delta ) )
                    u:setPosition( new_pos )
                    local new_world_pos = new_pos
                    if cc.pDistanceSQ( self._emit_pos, new_world_pos ) > self._range * self._range then
                        u:clearTracks()
                        if self._is_arrow_end[i][j] == false then
                            self._is_arrow_end[i][j] = true
                            u:setAnimation( 0, "disappear", false )
                            u:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                        end
                    end
                    --calculate damage
                    if units then
                        for k,unit in pairs( units ) do
                            local unit_id = unit:getId()
                            if not Helper.tableContainsValue( self._exclude_targets[i][j], unit_id ) then
                                local unit_pos = unit._unitNode:getHitPos()
                                if cc.pDistanceSQ( unit_pos, new_pos ) < self._radius * self._radius then
                                    local resource = "effects/bullets/blue_ball_hit"
                                    local hit_effect = ue.ArmatureManager:getInstance():createArmature( resource )
                                    local emit_pos = unit._unitNode:getHitPos()
                                    self._battle_layer:addEffect( hit_effect, emit_pos, 0 )
                                    hit_effect:setAnimation( 0, "animation", false )
                                    table.insert( self._hit_effects, hit_effect )
                                    local damage = BarrageOfArrow.calculateDamage( self._damage, self._atker_data, unit:getRawData() )
                                    unit:takeDamage( damage, self._src_id )
                                    table.insert( self._exclude_targets[i][j], unit_id )
                                end
                            end
                        end
                    end
                end
            end
        end

        --emit new arrows
        if self._current_wave < self._waves then
            self._accumulator = self._accumulator + delta
            if self._accumulator > self._interval then
                self._current_wave = self._current_wave + 1
                self._accumulator = 0
                --emit arrows
                for i = 1, self._count_per_wave do
                    local resource = "effects/bullets/vanhelsing_bullet_body"
                    local arrow = ue.ArmatureManager:getInstance():createArmature( resource )
                    arrow:setAnimation( 0, "animation", true )
                    arrow:setRotation( -self._arrow_angles[i] / math.pi * 180.0 )
                    self._battle_layer:addEffect( arrow, self._emit_pos, 0 )
                    table.insert( self._arrows[self._current_wave], arrow )
                end
                local resource = "effects/vanhelsing_skill_1/shoot"
                local effect = ue.ArmatureManager:getInstance():createArmature( resource )
                effect:setRotation(  -self._dir_angle * 180.0 / math.pi )
                effect:setAnimation( 0, "animation", false )
                self._battle_layer:addEffect( effect, self._emit_pos, 1 )
                table.insert( self._effects, effect )
                if self._current_wave >= self._waves then
                    self._is_cast_end = true
                end
            end
        end

        if self._is_cast_end == true then
            self._cast_end_accumulator = self._cast_end_accumulator + delta
            if self._cast_end_accumulator >= self._interval then
                self._is_cast_end = false
                self:finishCast()
            end
        end
        
        if self._is_end == true then
            self:unactivate()  
        end
    end
    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._skill_id = args[5]
    self._atker_data = src_unit:getRawData()
    self._src_camp = src_unit:getCamp()
    self._src_id = src_unit:getId()
    local dir = cc.pNormalize( src_unit:getDirection() )
    self._dir_angle = math.atan2( dir.y, dir.x )
    local section_angle = self._count_per_wave > 1 and self._angle * math.pi / 180.0 / ( self._count_per_wave - 1 ) or 0
    for i = 1,self._count_per_wave do
        local arrow_angle = self._dir_angle + section_angle * ( i - 1.0 - ( self._count_per_wave - 1 ) / 2.0 )
        table.insert( self._arrow_angles, arrow_angle )
        local arrow_dir = cc.p( math.cos( arrow_angle ), math.sin( arrow_angle ) ) 
        table.insert( self._arrow_dirs, arrow_dir ) 
    end
    local armature = src_unit._unitNode:currentArmature()
    local bone_pos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
    self._emit_pos = armature:convertToWorldSpace( bone_pos )
    self._emit_pos = self._battle_layer._effectLayer:convertToNodeSpace( self._emit_pos )
    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    src_unit:setState( Boids.UnitState.Cast, self._skill_id, 1 ) 
    src_unit._ignore_next_idle_state = true

    self:scheduleUpdateWithPriorityLua( update, 0 )
    return true
end

function BarrageOfArrow:finishCast()
    local src_unit = self._battle_layer:getUnitById( self._src_id )
    if src_unit then
        src_unit:setState( Boids.UnitState.Cast, self._skill_id, 2 )
    end
end

function BarrageOfArrow:unactivate()
    for i,v in pairs( self._hit_effects ) do
        v:getParent():removeChild( v, true )
    end
    for i,v in pairs( self._arrows ) do
        for j,u in pairs( v ) do
           u:clearTracks()
           u:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
           u:getParent():removeChild( u, true )
        end
    end
    for i,v in pairs( self._effects ) do
        v:clearTracks()
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return BarrageOfArrow
