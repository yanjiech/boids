local DarkPierce = class( "DarkPierce", function() return cc.Node:create() end )

function DarkPierce:ctor( ... )
    self._damage = SkillConf.skill["DarkPierce"]["damage"]
    self._range = SkillConf.skill["DarkPierce"]["range"]
    self._count = SkillConf.skill["DarkPierce"]["count"]
    self._stun_duration = SkillConf.skill["DarkPierce"]["stun"]
    self._end_count = 0
    self._current_count = 0
    self._up_thorns = {}
    self._down_thorns = {}
    self._target_ids = {}
    self._accumulator = 0
end

function DarkPierce:calculateDamage( atk, def )
    return ( self._damage + 0.2 * atk ) * DamageCalculate.calculateResistance( def, 0, 0 ) 
end

function DarkPierce:activate( ... )
    local function update( delta )
        --[[for i,v in pairs( self._target_ids ) do
            local unit = self._battle_layer:getUnitById( v )
            if unit then
                local hit_pos = unit._unitNode:getHitPos()
                hit_pos = cc.pAdd( hit_pos, cc.p( 0, 30.0 ) )
                --local hit_pos = unit:getPos()
                self._up_thorns[i]:setPosition( hit_pos )
                self._down_thorns[i]:setPosition( hit_pos ) 
            end
        end
        if self._end_count >= self._count then
            self:unactivate()
        end]]
        self._accumulator = self._accumulator + delta
        if self._accumulator >= self._stun_duration then
            self:unactivate()
        end
    end

    local function onAnimationEnded( event )
        if event.animation == "animation" then
            self._end_count = self._end_count + 1
        end
    end

    local function onAnimationEvent( event )
        if event.animation == "animation" and event.eventData.name == "OnDamage" then
            self._current_count = self._current_count + 1
            local unit = self._battle_layer:getUnitById( self._target_ids[self._current_count] )
            if unit then
                local damage = self:calculateDamage( self._atker_data["atk"], unit:getData( "def" ) )
                unit:takeDamage( damage, self._src_id )
            end
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_camp = src_unit:getCamp()
    self._src_id = src_unit:getId()
    self._atker_data = src_unit:getRawData()

    local src_pos = src_unit:getPos()
    local units = AI.getOpponentsByCamp( self._src_camp ) 
    local count = 0
    if units then
        for i,u in pairs( units ) do
            local unit_pos = u:getPos()
            if MathExtend.isPositionInRange( src_pos, self._range, unit_pos ) then
                local hit_pos = u._unitNode:getHitPos()
                local emit_pos = hit_pos
                emit_pos = cc.pAdd( emit_pos, cc.p( 0, 30.0 ) )

                local up_resource = "effects/dracula_skill_1/dark_pierce/up"
                local up_thorn = ue.ArmatureManager:getInstance():createArmature( up_resource )
                up_thorn:setAnimation( 0, "animation", false )
                up_thorn:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                self._battle_layer:addGroundEffect( up_thorn, emit_pos, 0 )
                table.insert( self._up_thorns, up_thorn )
                    
                local down_resource = "effects/dracula_skill_1/dark_pierce/down"
                local down_thorn = ue.ArmatureManager:getInstance():createArmature( down_resource )
                down_thorn:setAnimation( 0, "animation", false )
                down_thorn:registerSpineEventHandler( onAnimationEvent, sp.EventType.ANIMATION_EVENT )
                self._battle_layer:addEffect( down_thorn, emit_pos, 0 )
                table.insert( self._down_thorns, down_thorn )

                table.insert( self._target_ids, u:getId() )
                u:addBuff( "pierce", self._stun_duration )
                count = count + 1
            end
            if count >= self._count then
                break
            end
        end
    end
    self._count = count

    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    
    return true
end

function DarkPierce:unactivate( ... )
    for i,v in pairs( self._up_thorns ) do
        v:clearTracks()
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        v:getParent():removeChild( v, true )
    end
    for i,v in pairs( self._down_thorns ) do
        v:clearTracks()
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_EVENT )
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return DarkPierce
