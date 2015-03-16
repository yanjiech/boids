local EagleAttack = class( "EagleAttack", function() return cc.Node:create() end )
local math = require( 'math' )

function EagleAttack:ctor( ... )
    local visibleOrigin = cc.Director:getInstance():getVisibleOrigin()
    local visibleSize = cc.Director:getInstance():getVisibleSize()
    local resource = G.pathForResource( "gandalf_skill", Boids.ResourceType.spineEffect )
    self._flyIn = false
    self._toFly = false
    self._shadows = {}
    self._shadowCount = 3 
    self._track = {}
    self._currentTrack = 1
    self._maxTrack = 64
    self._delay = 0.08
    self._accumulator = 0
    self._targetId = nil
    self._targetPos = nil
    self._atkerData = nil
    self._battleLayer = nil
    --self._targetCamp = nil 
    self._speed = SkillConf.skill["EagleAttack"]["speed"]
    self._baseDamage = SkillConf.skill["EagleAttack"]["damage"]
    self._nelgp = SkillConf.skill["EagleAttack"]["neglp"]
    self._radius = SkillConf.skill["EagleAttack"]["radius"]
    self._range = SkillConf.skill["EagleAttack"]["range"]
    self._effect = ue.ArmatureManager:getInstance():createArmature( resource )
    self:addChild( self._effect )
    self._appearPoint = cc.p( visibleOrigin.x + visibleSize.width * 0.75, visibleOrigin.y + visibleSize.height )
    self._disappearPoint = cc.p( visibleOrigin.x + visibleSize.width * 0.25, visibleOrigin.y + visibleSize.height )
    self._lastDistanceSQ = 88888888.0
end

function EagleAttack:calculateDamage( atk, armor )
    return ( self._baseDamage + 0.3 * atk ) * DamageCalculate.calculateResistance( armor, 0, self._nelgp )
end

function EagleAttack:getTrackPosition( startTrack, timestamp )
    local count = #self._track 
    local lastTrack = self._track[startTrack]
    local currentTrack = nil
    local i = startTrack - 1
    while i ~= startTrack  do
        currentTrack = self._track[i]
        if timestamp > currentTrack[3] then
            return { cc.pAdd( currentTrack[1], cc.pMul( cc.pSub( lastTrack[1], currentTrack[1] ), ( timestamp - currentTrack[3] ) / ( lastTrack[3] - currentTrack[3] ) ) ), currentTrack[2] }
        end
        lastTrack = currentTrack
        i = i == 1 and count or ( i - 1 )
    end
    return {}
end

function EagleAttack:activate( ... )
    local function update( delta )
        if self._toFly and not self._flyIn then
            self._effect:setAnimation( 0, "Out", true )
            self._toFly = false
        end 
        self._currentTrack = ( self._currentTrack % self._maxTrack ) + 1
        self._accumulator = self._accumulator + delta

        local lastPos = cc.p( self:getPosition() ) 
        local target = self._battleLayer:getUnitById( self._targetId )
        if target then 
            self._targetPos = target:getPos()
        end 
        local dstPos = self._flyIn and self._targetPos or self._disappearPoint
        local dir = cc.pNormalize( cc.pSub( dstPos, lastPos ) )
        local newPos = cc.pAdd( lastPos, cc.pMul( dir, self._speed * delta ) )
        local rotation = dir.x > 0 and 180 or 0
        if #self._track < 1 then
            self._track[1] = { lastPos, 0, 0 }
        end 
        self._track[self._currentTrack] = { newPos, rotation, self._accumulator } 
        self:setRotationSkewY( rotation )
        self:setPosition( newPos )

        --update shadows
        for i,v in ipairs( self._shadows ) do 
            local param = self:getTrackPosition( self._currentTrack, self._accumulator - self._delay * i )
            if #param == 0 then
                break
            end
            v:setPosition( param[1] )
            v:setRotationSkewY( param[2] )
        end 
        local distanceSQ = cc.pDistanceSQ( dstPos, newPos )
        if distanceSQ < self._radius * self._radius or distanceSQ > self._lastDistanceSQ then
            if self._flyIn then 
                --calculate damage
                local units = AI.getOpponentsByCamp( self._src_camp ) 
                if units then
                    for _,v in pairs( units ) do 
                        local upos = v:getPos()
                        if cc.pDistanceSQ( newPos, upos ) < self._radius * self._radius then
                            local damage = self:calculateDamage( self._atkerData["atk"], v:getData( "def" ) )
                            v:takeDamage( damage, self._srcId )
                        end
                    end
                end 
                self._effect:clearTracks()
                for i,v in ipairs( self._shadows ) do    
                    v:setVisible( false )
                end
                self._effect:clearTracks()
                self._effect:setAnimation( 0, "Attack", false )
                self:unscheduleUpdate()
                self._lastDistanceSQ = 88888888.0
            else    
                self:unactivate()
            end
        else
            self._lastDistanceSQ = distanceSQ
            local wpos = self:getParent():convertToWorldSpace( newPos )
            local visibleOrigin = cc.Director:getInstance():getVisibleOrigin()
            local visibleSize = cc.Director:getInstance():getVisibleSize()
            local rect = cc.rect( visibleOrigin.x, visibleOrigin.y, visibleSize.width, visibleSize.height )
            if not cc.rectContainsPoint( rect, wpos ) then
                for i,v in ipairs( self._shadows ) do
                    v:getParent():removeChild( v, true )
                end 
                self._effect:clearTracks()
                self:unscheduleUpdate()
                self:getParent():removeChild( self, true )
            end
        end 
    end 

    local function onAnimationEnded( event )
        if event.animation == "Attack" then
            for i, v in ipairs( self._shadows ) do
                v:setVisible( true )
                v:setPosition( cc.p( self:getPosition() ) )
            end 
            self._currentTrack = 1
            self._track = {}
            self._accumulator = 0
            self._flyIn = false 
            self:scheduleUpdateWithPriorityLua( update, 0 )
            self._toFly = true
        end
    end

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    local armature = srcUnit._unitNode:currentArmature()
    local bonePos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
    local emitPos = armature:convertToWorldSpace( cc.p( bonePos ) ) 
    local target = srcUnit:getTarget()
    self._atkerData = srcUnit:getRawData( )
    self._srcId = srcUnit:getId()
    --if target then 
        self._targetId = target:getId()
        self._targetPos = target:getPos()
    --else
        local src_pos = srcUnit:getPos()
        --local units = self._battleLayer:getUnitsByCamp( self._targetCamp )
        local units = self._opponents
        if units then
            for i,v in pairs( units ) do
                local upos = v:getPos()
                if cc.pDistanceSQ( upos, src_pos ) < self._range * self._range then
                    target = v
                    break
                end
            end
        end
        if target then
            self._targetId = target:getId()
            self._targetPos = target:getPos()
        else
            self._targetId = -1
            local src_pos = srcUnit:getPos()
            local dir = srcUnit:getDirection()
            dir = cc.pNormalize( dir )
            self._targetPos = cc.pAdd( src_pos, cc.pMul( dir, self._range ) )
        end
    --end
    self._flyIn = true 
    self._appearPoint = self._battleLayer._effectLayer:convertToNodeSpace( self._appearPoint )
    self._disappearPoint = self._battleLayer._effectLayer:convertToNodeSpace( self._disappearPoint ) 
    self._battleLayer:addEffect( self, self._appearPoint, 99999 )
    for i = 1, self._shadowCount do
        self._shadows[i] = cc.Sprite:create( "effects/gandalf_skill/eagle.png" )
        self._shadows[i]:setColor( cc.c3b( 255,255, 255 ) )
        self._shadows[i]:setAnchorPoint( cc.p( 0.14, 0 ) )
        self._shadows[i]:setOpacity( 255 - 255 * 0.25 * i )
        self._battleLayer:addEffect( self._shadows[i], self._appearPoint, 99999 - i )
    end 
    self._disappearPoint = self:getParent():convertToNodeSpace( self._disappearPoint )
    self._effect:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )
    self._effect:setAnimation( 0, "In", true )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    --AudioManager.playEffect( "cast", "eagle", false )

    return true
end 

function EagleAttack:unactivate()
    for i,v in ipairs( self._shadows ) do 
        v:getParent():removeChild( v, true )
    end 
    self._effect:clearTracks()
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return EagleAttack 
