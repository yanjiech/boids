local PandaFire = class( "PandaFire", function() return cc.Node:create()  end )
local math = require( 'math' )

function PandaFire:ctor( ... )
    self._radius = SkillConf.skill["PandaFire"]["radius"] 
    self._damage = SkillConf.skill["PandaFire"]["damage"]
    self._count = SkillConf.skill["PandaFire"]["count"]
    self._interval = SkillConf.skill["PandaFire"]["interval"]
    self._buff_duration = SkillConf.skill["PandaFire"]["buff_duration"]
    self._effects = {}
    self._current_count = 0
    self._end_count = 0
    self._accumulator = 0
    self._exclude_targets = {}
end 

function PandaFire:calculateDamage( atk, def )
    return ( self._damage + 0.2 * atk ) * DamageCalculate.calculateResistance( def, 0, 0 ) 
end

function PandaFire:activate( ... )
    local function onAnimationEnded( event )
        if event.animation == "animation" then
            self._end_count = self._end_count + 1
        end
    end

    local function update( delta )
        if self._current_count < self._count then
            self._accumulator = self._accumulator + delta
            if self._accumulator > self._interval then
                self._current_count = self._current_count + 1
                local scale = 0.2 + 0.8 / self._count * self._current_count 
                local delta_pos = cc.p( self._dir.x * self._radius * scale, self._dir.y * self._radius * scale )
                self._last_pos = cc.pAdd( self._last_pos, delta_pos )
                local resource = "effects/panda_skill"
                local fire = ue.ArmatureManager:getInstance():createArmature( resource ) 
                fire:setAnimation( 0, "animation", false )
                fire:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                fire:setScale( scale )
                local zorder = self._dir.y >= 0 and self._count - self._current_count or self._current_count
                --self._battle_layer:addEffect( fire, self._last_pos, zorder ) 
                self._battle_layer:addEffectToObjectLayer( fire, self._last_pos, zorder )
                self._accumulator = 0
                table.insert( self._effects, fire )
                
                local fire_pos_in_view = cc.p( fire:getPosition() ) 
                --damage
                local units = AI.getOpponentsByCamp( self._src_camp ) 
                if units then
                    for i,u in pairs( units ) do
                        local uid = u:getId()
                        if not Helper.tableContainsValue( self._exclude_targets, uid ) then
                            local unit_pos = u:getPos()
                            if cc.pDistanceSQ( unit_pos, fire_pos_in_view ) < self._radius * self._radius * 2.0 then
                                u:addBuff( "burn", self._buff_duration )
                                local damage = self:calculateDamage( self._atker_data["atk"], u:getData( "def" ) )
                                u:takeDamage( damage, self._src_id )
                                table.insert( self._exclude_targets, uid )
                            end
                        end
                    end
                end
            end
        end
        if self._end_count >= self._count then
            self:unactivate()
        end
    end 

    local arg = { ... }
    self._battle_layer = arg[1]
    local src_unit = arg[2]
    self._src_camp = src_unit:getCamp()
    self._src_id = src_unit:getId()
    self._atker_data = src_unit:getRawData()
    local armature = src_unit._unitNode:currentArmature()
    self._bone_pos = cc.p( ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" ) )
    --self._last_pos = armature:convertToWorldSpace( self._bone_pos ) 
    self._last_pos = src_unit:getPos()
    self._dir = cc.pNormalize( src_unit:getDirection() )
    self._last_pos = cc.pAdd( self._last_pos, cc.pMul( self._dir, 50.0 ) )
    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )

    return true
end

function PandaFire:unactivate()
    for i,v in pairs( self._effects ) do
        v:clearTracks()
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return PandaFire 
