local WrathOfThunder = class( "WrathOfThunder", function() return cc.Node:create() end )
local math = require( "math" )

function WrathOfThunder:ctor( ... )
    self._base_damage = SkillConf.skill["WrathOfThunder"]["damage"]
    self._range = SkillConf.skill["WrathOfThunder"]["range"]
    self._count = SkillConf.skill["WrathOfThunder"]["count"]
    self._hit_count = 0
    self._end_count = 0
    self._lightnings = {}
    self._target_ids = {}
end

function WrathOfThunder:calculateDamage( atk, armor )
    return ( self._base_damage + 0.2 * atk ) * DamageCalculate.calculateResistance( armor, 0, 0 )
end

function WrathOfThunder:activate( ... )
    local function update( delta )
        if self._end_count >= self._count then
            self:unactivate()
        end
    end
    
    local function onAnimationEnded( event )
        if event.animation == "animation" then
            self._end_count = self._end_count + 1
        end
    end

    local function onAnimationEvent( event )
        if event.animation == "animation" and event.eventData.name == "OnHit" then
            if #self._target_ids > 0 then
                self._hit_count = self._hit_count + 1
                local target_unit = self._battle_layer:getUnitById( self._target_ids[self._hit_count] )
                if target_unit then
                    --damage
                    local damage = self:calculateDamage( self._atker_data["atk"], target_unit:getData( "def" ) )
                    target_unit:takeDamage( damage, self._src_id )
                end
            end
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_id = src_unit:getId()
    self._atker_data = src_unit:getRawData()
    local src_pos = src_unit:getPos() 
    --local units = self._battle_layer:getUnitsByCamp( self._target_camp )
    local units = src_unit:getOpponents()
    if units then
        local count = 0
        for i,v in pairs( units ) do
            local unit_pos = v:getPos()
            if MathExtend.isPositionInRange( src_pos, self._range, unit_pos ) then
            --if cc.pDistanceSQ( src_pos, unit_pos ) < self._range * self._range then
                --add effect
                local emit_pos = unit_pos
                local resource = G.pathForResource( "lightning", Boids.ResourceType.spineEffect )
                local lightning = ue.ArmatureManager:getInstance():createArmature( resource )
                lightning:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                lightning:registerSpineEventHandler( onAnimationEvent, sp.EventType.ANIMATION_EVENT ) 
                lightning:setAnimation( 0, "animation", false )
                self._battle_layer:addEffect( lightning, emit_pos, 0 )
                table.insert( self._target_ids, v:getId() )
                table.insert( self._lightnings, lightning )
                count = count + 1
                if count >= self._count then
                    break
                end
            end
        end
        if count == 0 then
            local angle = 0
            for i = 1,self._count do
                local emit_pos = cc.p( src_pos.x + math.cos( angle ) * self._range / 2.0, src_pos.y + math.sin( angle ) * self._range / 2.0 )
                local resource = "effects/lightning"
                local lightning = ue.ArmatureManager:getInstance():createArmature( resource )
                lightning:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                lightning:registerSpineEventHandler( onAnimationEvent, sp.EventType.ANIMATION_EVENT )
                lightning:setAnimation( 0, "animation", false )
                self._battle_layer:addEffect( lightning, emit_pos, 0 )
                table.insert( self._lightnings, lightning )
                angle = angle + math.pi * 2.0 / self._count
            end
        else
            self._count = count
        end
        self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
        self:scheduleUpdateWithPriorityLua( update, 0 )
    end

    return true
end

function WrathOfThunder:unactivate()
    for i,v in pairs( self._lightnings ) do
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_EVENT )
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        v:clearTracks()
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return WrathOfThunder
