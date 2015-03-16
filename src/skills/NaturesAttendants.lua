local NaturesAttendants = class( "NaturesAttendants", function() return cc.Node:create() end )
local math = require( "math" )

function NaturesAttendants:ctor( ... )
    self._heal = SkillConf.skill["NaturesAttendants"]["heal"][1]
    self._range = SkillConf.skill["NaturesAttendants"]["range"]
    self._count = SkillConf.skill["NaturesAttendants"]["count"][1]
    self._duration = SkillConf.skill["NaturesAttendants"]["duration"]
    self._interval = SkillConf.skill["NaturesAttendants"]["interval"]
    self._init_speed = SkillConf.skill["NaturesAttendants"]["init_speed"]
    self._accel = SkillConf.skill["NaturesAttendants"]["accel"]
    self._spirits = {}
    self._pos_bias = {}
    self._appeared_count = 1
    self._will_appear_count = 0
    self._disappear_count = 0
    self._accumulator = 0
    self._accel_accumulator = 0
    self._spirits_speed = {}    
    self._spirits_accel_points = {}
end

function NaturesAttendants:updateRandomAccelPoints()
    for i = 1,self._count do
        self._spirits_accel_points[i] = MathExtend.randomPositionFromCenter( self._src_pos, self._range / 4.0 )
    end
end

function NaturesAttendants:updateSpiritPositions( delta )
    for i,v in pairs( self._spirits ) do
        local unit_pos = cc.p( v:getPosition() )
        if not MathExtend.isPositionInRange( self._src_pos, self._range, unit_pos ) then
            self._spirits_speed[i] = cc.p( 0, 0 )
        end

        local dist = math.sqrt( cc.pDistanceSQ( self._src_pos, unit_pos ) )
        local ratio = 0.5 + dist * 1.5 / self._range
        local accel_dir = cc.pNormalize( cc.pSub( self._spirits_accel_points[i], unit_pos ) )
        local accel = cc.pMul( accel_dir, self._accel * ratio )
        self._spirits_speed[i] = cc.pAdd( self._spirits_speed[i], cc.pMul( accel, delta ) )
        local new_pos = cc.pAdd( unit_pos, cc.pMul( self._spirits_speed[i], delta ) )
        v:setPosition( new_pos )
    end
end

function NaturesAttendants.calculateDamage( base_damage, atker_data, defer_data )
    return base_damage + 0.2 * atker_data["atk"]
end

function NaturesAttendants:activate( ... )
    local function update( delta )
        if self._is_end == true then
            self:unactivate()
        elseif self._start_end == true then
            for i,v in pairs( self._spirits ) do
                v:setAnimation( 0, "Disappear", false )
            end
        else 
            local src_unit = self._battle_layer:getUnitById( self._src_id )
            local last_pos = self._src_pos
            if src_unit then
                self._src_pos = src_unit:getPos()
            end
            local delta_pos = cc.pSub( self._src_pos, last_pos )
            if delta_pos.x ~= 0 or delta_pos.y ~= 0 then
                for i,v in pairs( self._spirits ) do
                    local sp_pos = cc.p( v:getPosition() )
                    sp_pos = cc.pAdd( sp_pos, delta_pos )
                    v:setPosition( sp_pos )
                end
            end
            if self._appeared_count <= self._will_appear_count then
                for i = self._appeared_count, self._will_appear_count do
                    self._spirits[i]:setAnimation( 0, "animation", true )
                end
                self._appeared_count = self._will_appear_count + 1
            end
            self._effect:setPosition( self._src_pos )
            --update spirit acceleration
            self._accel_accumulator = self._accel_accumulator + delta
            if self._accel_accumulator > self._interval / 2.0 then
                self._accel_accumulator = 0
                self:updateRandomAccelPoints()
            end
            --update spirit position
            self:updateSpiritPositions( delta )
            --heal
            self._accumulator = self._accumulator + delta
            if self._accumulator >= self._interval then
                self._accumulator = 0
                local units = AI.getAlliesByCamp( self._src_camp )
                if units then
                    local heal = NaturesAttendants.calculateDamage( self._heal, self._atker_data, nil )
                    for i,u in pairs( units ) do
                        local unit_pos = u:getPos()
                        if MathExtend.isPositionInRange( self._src_pos, self._range, unit_pos ) then
                            u:takeHealing( heal )
                        end
                    end
                end
            end
            
            self._duration = self._duration - delta
            if self._duration <= 0 then
                self._start_end = true
            end
        end
    end

    local function onAnimationEnded( event )
        if event.animation == "Appear" then
            self._will_appear_count = self._will_appear_count + 1
        elseif event.animation == "Disappear" then
            self._disappear_count = self._disappear_count + 1
            if self._disappear_count >= self._count then
                self._is_end = true
            end
        end
    end

    local function onEffectAnimationEnded( event )
        if event.animation == "animation" then
            self._effect:setVisible( false )
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_camp = src_unit:getCamp()
    self._atker_data = src_unit:getRawData()
    self._src_id = src_unit:getId()
    self._src_pos = src_unit:getPos()
    local emit_pos = self._src_pos

    local resource = "effects/enchantress_skill_1/cast"
    self._effect = ue.ArmatureManager:getInstance():createArmature( resource )
    self._effect:setAnimation( 0, "animation", false )
    self._effect:registerSpineEventHandler( onEffectAnimationEnded, sp.EventType.ANIMATION_END )
    self._battle_layer:addEffect( self._effect, emit_pos, 0 )
    for i = 1, self._count do
        resource = "effects/enchantress_skill_1/spirit"
        local spirit = ue.ArmatureManager:getInstance():createArmature( resource )
        spirit:setAnimation( 0, "Appear", false )
        spirit:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
        self._battle_layer:addEffect( spirit, emit_pos, 0 )
        --spirit:setScale( 0.5 )
        table.insert( self._spirits, spirit )
        local dir = MathExtend.randomDir()
        local speed = cc.pMul( dir, self._init_speed )
        table.insert( self._spirits_speed, speed )
    end
    self:updateRandomAccelPoints()

    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )

    return true
end

function NaturesAttendants:unactivate()
    for i,v in pairs( self._spirits ) do
        v:clearTracks()
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        v:getParent():removeChild( v, true )
    end
    if self._effect then
        self._effect:clearTracks()
        self._effect:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        self._effect:getParent():removeChild( self._effect, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return NaturesAttendants
