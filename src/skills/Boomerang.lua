local Boomerang = class( "Boomerang", function() return cc.Node:create() end )

function Boomerang:ctor( ... )
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo( "effects/desert_thief_leader_skill/feidao.ExportJson" )
    self._effect = ccs.Armature:create( "feidao" ) 
    self:addChild( self._effect )
    self._unitId = -1 
    self._targetId = -1
    self._targetPos = nil
    self._speed = SkillConf.skill["Boomerang"]["speed"] 
    self._maxStep = SkillConf.skill["Boomerang"]["maxstep"]
    self._currentStep = 0
    self._excludeTargets = {}
    self._battleLayer = nil
    self._maxGapSQ = SkillConf.skill["Boomerang"]["maxgapsq"]
    self._atkerData = nil
    self._baseDamage = SkillConf.skill["Boomerang"]["damage"]
end 

function Boomerang:calculateDamage( atk, armor )
    return ( self._baseDamage + 0.1 * atk ) * DamageCalculate.calculateResistance( armor, 0, 0 )
end

function Boomerang:findNextTarget()
    local newTarget = nil
    local lastTarget = self._battleLayer:getUnitById( self._targetId ) 
    if lastTarget then
        local minGapSQ = self._maxGapSQ
        local srcPos = lastTarget:getPos()
        local units = AI.getUnitsByCamp( self._src_camp ) 
        for _,v in pairs( units ) do 
            if not Helper.tableContainsValue( self._excludeTargets, v:getId() ) then
                local dstPos = v:getPos()
                local d = cc.pDistanceSQ( dstPos, srcPos )
                if d < minGapSQ then
                    newTarget = v
                    minGapSQ = d
                end 
            end 
        end 
    end

    return newTarget 
end 

function Boomerang:activate( ... )
    local function update( delta )
        local target = self._battleLayer:getUnitById( self._targetId )
        if self._targetId and self._targetPos then
            if target then
                self._targetPos = target:getPos()
            end 
            local lastPos =cc.p( self:getPosition() )
            local dstPos = self._targetPos
            local dir = cc.pSub( dstPos, lastPos )
            dir = cc.pNormalize( dir )
            local newPos = cc.pAdd( lastPos, cc.pMul( dir, self._speed * delta ) )
            self:setPosition( newPos )
            self._targetPos = newPos

            if cc.pDistanceSQ( newPos, dstPos ) < 4000 then 
                --calculate damage
                if target then 
                    local damage = self:calculateDamage( self._atkerData["atk"], target:getData( "def" ) )
                    target:takeDamage( damage, self._srcId )
                    self._excludeTargets[#self._excludeTargets+1] = self._targetId 
                    self._currentStep = self._currentStep + 1
                    if self._currentStep < self._maxStep then 
                        local nextTarget = self:findNextTarget()
                        if nextTarget then
                            self._targetId = nextTarget:getId()
                        else
                            self._targetId = nil 
                        end 
                    else
                        self._targetId = nil 
                    end
                end
            end 
        else
            self:unactivate()
        end 
    end 

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    self._srcId = srcUnit:getId()
    self._atkerData = srcUnit:getRawData()
    local target = srcUnit:getTarget()
    if target then
        self._targetId = target:getId()
        self._targetPos = target:getPos()
        local armature = srcUnit._unitNode:currentArmature()
        local bonePos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
        local emitPos = armature:convertToWorldSpace( cc.p( bonePos ) )
        emitPos = self._battleLayer._effectLayer:convertToNodeSpace( emitPos )
        self._effect:getAnimation():play( "Fly" )
        self._battleLayer:addEffect( self, emitPos, srcUnit._unitNode:getLocalZOrder() )
        self:scheduleUpdateWithPriorityLua( update, 0 )
        return true
    end 
    return false
end 

function Boomerang:unactivate()
    self._effect:getAnimation():stop()
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end 

return Boomerang
