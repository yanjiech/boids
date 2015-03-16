local GiantSword = class( "GiantSword", function() return cc.Node:create() end )

function GiantSword:ctor( ... )
    self._damage = SkillConf.skill["GiantSword"]["damage"]
    self._radius = SkillConf.skill["GiantSword"]["radius"]
    self._range = SkillConf.skill["GiantSword"]["range"]
    self._stun_duration = SkillConf.skill["GiantSword"]["stun"]
    self._min_range = SkillConf.skill["GiantSword"]["min_range"]
    self._max_range = SkillConf.skill["GiantSword"]["max_range"]
end

function GiantSword:calculateDamage( atk, def )
    return ( self._damage + 0.2 * atk ) * DamageCalculate.calculateResistance( def, 0, 0 ) 
end

function GiantSword:activate( ... )
    local function update( delta )
        if self._is_end == true then
            self:unactivate()
        end
    end
    local function onAnimationEnded( event )
        if event.animation == "animation" then
            self._is_end = true
        end
    end

    local function onAnimationEvent( event )
        if event.animation == "animation" and event.eventData.name == "OnDamage" then
            local units = AI.getOpponentsByCamp( self._src_camp )
            if units then
                for i,u in pairs( units ) do
                    local unit_pos = u:getPos()
                    if MathExtend.isPositionInRange( self._sword_pos, self._radius, unit_pos ) then
                    --if cc.pDistanceSQ( unit_pos, self._sword_pos ) < self._radius * self._radius then
                        local damage = self:calculateDamage( self._atkerData["atk"], u:getData( "def" ) )
                        u:takeDamage( damage, self._src_id )
                        if u:getState() ~= Boids.UnitState.Die then
                            u:addBuff("stunned", self._stun_duration)
                        end
                    end
                end
            end
        end
    end
    
    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_camp = src_unit:getCamp()
    self._src_id = src_unit:getId()
    local skill_range = args[4]
    if skill_range > self._max_range then
        skill_range = self._max_range
    elseif skill_range < self._min_range then
        skill_range = self._min_range
    end
    local src_pos = src_unit:getPos()
    local dir = cc.pNormalize( src_unit:getDirection() )
    self._sword_pos = MathExtend.positionFromCenter( src_pos, skill_range, dir ) 
    local emit_pos = self._sword_pos 
    self._atkerData = src_unit:getRawData()
    local up_resource = "effects/saber_skill/up"
    self._up_effect = ue.ArmatureManager:getInstance():createArmature( up_resource )
    self._up_effect:setAnimation( 0, "animation", false )
    self._up_effect:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
    self._up_effect:registerSpineEventHandler( onAnimationEvent, sp.EventType.ANIMATION_EVENT )
    self._battle_layer:addEffect( self._up_effect, emit_pos, 0 )
    local down_resource = "effects/saber_skill/down"
    self._down_effect = ue.ArmatureManager:getInstance():createArmature( down_resource )
    self._down_effect:setAnimation( 0, "animation", true )
    self._battle_layer:addGroundEffect( self._down_effect, emit_pos, 10 )
    
    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    return true
end

function GiantSword:unactivate()
    if self._up_effect then
        self._up_effect:clearTracks()
        self._up_effect:unregisterSpineEventHandler( sp.EventType.ANIMATION_EVENT )
        self._up_effect:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        self._up_effect:getParent():removeChild( self._up_effect, true )
    end
    if self._down_effect then
        self._down_effect:clearTracks()
        self._down_effect:getParent():removeChild( self._down_effect, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return GiantSword
