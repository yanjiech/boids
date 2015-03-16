local BonesRain = class( "BonesRain", function() return cc.Node:create() end )
local math = require( "math" )

function BonesRain:ctor( ... )
    --self._targetCamp = nil
    self._srcUnit = nil
    self._battleLayer = nil
    self._bones = {}
    self._crosses = {}
    self._radius = SkillConf.skill["BonesRain"]["radius"]
    self._count = SkillConf.skill["BonesRain"]["count"]
    self._baseDamage = SkillConf.skill["BonesRain"]["damage"]
    self._interval = SkillConf.skill["BonesRain"]["interval"]
    self._atkerData = nil
    self._accumulator = 0
    self._loadedCount = 0
    self._damageCount = 0
    self._endCount = 0
    self._emitPos = nil
    self._dpos = nil
    self._excludeTargets = {}
end

function BonesRain:calculateDamage( atk, armor )
    return ( self._baseDamage + 0.2 * atk ) * DamageCalculate.calculateResistance( armor, 0, 0 )
end

function BonesRain:activate( ... )
    local function onDamage( event )
        if event.animation == "animation" and event.eventData.name == "OnDamage" then
            self._damageCount = self._damageCount + 1
            local bonePos = cc.p( self._bones[self._damageCount]:getPosition() )
            local units = AI.getOpponentsByCamp( self._src_camp ) 
            for _,v in pairs( units ) do
                if not Helper.tableContainsValue( self._excludeTargets, v:getId() ) then
                    local uPos = v:getPos()
                    if cc.pDistanceSQ( bonePos, uPos ) < self._radius * self._radius then
                        local damage = self:calculateDamage( self._atkerData["atk"], v:getData( "def" ) )
                        v:takeDamage( damage, self._src_id )
                        table.insert( self._excludeTargets, v:getId() )
                    end
                end
            end
        end
    end
    local function onAnimationEnded( event )
        if event.animation == "animation" then
            self._endCount = self._endCount + 1
        end
    end

    local function update( delta )
        if self._loadedCount < self._count then
            self._accumulator = self._accumulator + delta
            if self._accumulator > self._interval then
                self._accumulator = 0
                self._loadedCount = self._loadedCount + 1
                local emitPos = cc.p( self._emitPos.x + self._dpos.x * self._loadedCount, self._emitPos.y + self._dpos.y * self._loadedCount )
                local resource = ue.LuaUtils:randomFloat() < 0.5 and "effects/bones_rain/gu01" or "effects/bones_rain/gu02"
                local bone = ue.ArmatureManager:getInstance():createArmature( resource )
                bone:setAnimation( 0, "animation", false )
                bone:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
                bone:registerSpineEventHandler( onDamage, sp.EventType.ANIMATION_EVENT )
                self._battleLayer:addEffect( bone, emitPos, 0 )
                table.insert( self._bones, bone )
                --insert aim cross
                resource = "effects/bones_rain/cross"
                local cross = ue.ArmatureManager:getInstance():createArmature( resource )
                cross:setAnimation( 0, "animation", false )
                self._battleLayer:addGroundEffect( cross, emitPos, 0 )
                table.insert( self._crosses, cross )
            end
        end
        if self._endCount >= self._count then
            self:unactivate()
        end
    end

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    self._src_id = srcUnit:getId()
    self._atkerData = srcUnit:getRawData()
    
    local dir = srcUnit:getDirection()
    dir = cc.pNormalize( dir )
    self._dpos = cc.pMul( dir, 2 * self._radius )
    self._emitPos = srcUnit:getPos()
    self._battleLayer:addEffect( self, self._emitPos, 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    
    return true
end

function BonesRain:unactivate()
    for i,v in pairs( self._bones ) do
        v:clearTracks()
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        v:unregisterSpineEventHandler( sp.EventType.ANIMATION_EVENT )
        v:getParent():removeChild( v, true )
    end
    for i,v in pairs( self._crosses ) do
        v:clearTracks();
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return BonesRain
