local LightningChain = class( "LightningChain", function() return cc.Node:create() end )
local math = require( "math" )

function LightningChain:ctor( ... )
    self._damage = SkillConf.skill["LightningChain"]["damage"][1]
    self._count = SkillConf.skill["LightningChain"]["count"]
    self._length = SkillConf.skill["LightningChain"]["length"]
    self._interval = SkillConf.skill["LightningChain"]["interval"]
    self._range = SkillConf.skill["LightningChain"]["range"]
    self._duration = SkillConf.skill["LightningChain"]["duration"]
    self._scale = 0
    self._current_count = 0
    self._end_count = 0
    self._chains = {}
    self._hits = {}
    self._accumulator = 0
    self._exclude_targets = {}
    self._timers = {}
end

function LightningChain:calculateDamage( atk, def )
    return ( self._damage + 0.2 * atk ) * DamageCalculate.calculateResistance( def, 0, 0 ) 
end

function LightningChain:activate( ... )
    local function onAnimationEnded( event )
        if event.animation == "animation" then
            self._end_count = self._end_count + 1
            self._hits[self._end_count]:setVisible( false )
        end
    end

    local function update( delta )
        for i,v in pairs( self._timers ) do
            if i > self._end_count then
                self._timers[i] = self._timers[i] + delta
                if self._timers[i] > self._duration then
                    self._hits[i]:clearTracks()
                    self._hits[i]:setVisible( false )
                    self._chains[i]:clearTracks()
                    self._chains[i]:setVisible( false )
                    self._end_count = self._end_count + 1
                end
            end
        end
        if self._current_count < self._count then
            self._accumulator = self._accumulator + delta
            if self._accumulator > self._interval then
                self._accumulator = 0
                local next_target_found = false
                local units = AI.getOpponentsByCamp( self._src_camp ) 
                local length = self._current_count == 0 and self._range or self._length
                if units then
                    for i,u in pairs( units ) do
                        local unit_id = u:getId()
                        if not Helper.tableContainsValue( self._exclude_targets, unit_id ) then
                            local unit_pos = u:getPos()
                            local distance = math.sqrt( cc.pDistanceSQ( unit_pos, self._last_hit_pos ) ) 
                            if distance < length then
                                --hit unit
                                local hit_pos = u._unitNode:getHitPos()
                                self._current_count = self._current_count + 1
                                local resource = "effects/lightning_chain/chain"
                                local chain = ue.ArmatureManager:getInstance():createArmature( resource )
                                chain:setScaleX( distance / self._length )
                                chain:setAnimation( 0, "animation", true )
                                --chain:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                                self._battle_layer:addEffect( chain, self._emit_pos, 0 )
                                table.insert( self._chains, chain )
                                local chain_pos = cc.p( chain:getPosition() )
                                local rotation = math.atan2( hit_pos.y - chain_pos.y, hit_pos.x - chain_pos.x )
                                chain:setRotation( -rotation * 180.0 / math.pi )

                                local hit_resource = "effects/lightning_chain/hit"
                                local hit = ue.ArmatureManager:getInstance():createArmature( hit_resource )
                                --hit:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                                hit:setAnimation( 0, "animation", true )
                                --hit:setScale( 0.5 )
                                local hit_effect_pos = hit_pos
                                self._battle_layer:addEffectToObjectLayer( hit, hit_effect_pos, 1 )
                                table.insert( self._hits, hit )

                                table.insert( self._timers, 0 )
                                --damage
                                local damage = self:calculateDamage( self._atker_data["atk"], u:getData( "def" ) )
                                u:takeDamage( damage, self._src_id )
                                table.insert( self._exclude_targets, unit_id )
                                self._emit_pos = hit_pos
                                self._last_hit_pos = unit_pos
                                next_target_found = true
                                break
                            end
                        end
                    end
                end
                if next_target_found == false then
                    print( "target not found" )
                    self._count = self._current_count
                end
            end
        end
        if self._end_count >= self._count then
            self:unactivate()
        end
    end

    local args = { ... }
    self._battle_layer = args[1]
    local src_unit = args[2]
    self._src_camp = src_unit:getCamp()
    self._atker_data = src_unit:getRawData()
    self._src_id = src_unit:getId()
    self._atker_data = src_unit:getRawData()
    local armature = src_unit._unitNode:currentArmature()
    local bone_pos = cc.p( ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" ) )
    self._last_hit_pos = src_unit:getPos()
    self._emit_pos = armature:convertToWorldSpace( bone_pos ) 
    self._emit_pos = src_unit._unitNode:getParent():convertToNodeSpace( self._emit_pos )

    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )

    return true
end

function LightningChain:unactivate()
    for i,v in pairs( self._chains ) do
        v:clearTracks()
        --v:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        v:getParent():removeChild( v, true )
    end
    for i,v in pairs( self._hits ) do
        v:clearTracks()
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return LightningChain
