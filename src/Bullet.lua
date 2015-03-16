local Bullet = class( "Bullet", function() return cc.Node:create() end )

Bullet.id = 0

function Bullet:ctor( ... )
    local arg = { ... }
    self._data = arg[2]
    self._bulletName = self._data.name
    self._streak = nil
    self._targetId = -1
    if self._data.speed then
        self._speed = self._data.speed
    else
        self._speed = 500.0
    end
    self._duration = 1.0
    self._battleLayer = nil
    self._targetPos = nil
    self._atkerData = nil
    self._baseDamage = #arg >= 3 and arg[3] or 0
    self._damageCalculator = #arg >= 4 and arg[4] or nil
    self._buff = #arg >= 5 and arg[5] or nil
    Bullet.id = Bullet.id + 1
    self._bulletId = Bullet.id
end 

function Bullet:shoot( ... )
    self._elapse = 0
    local arg = { ... }
    local srcUnit = arg[1]
    self._src_camp = srcUnit:getCamp()
    self._srcId = srcUnit:getId()
    self._battleLayer = arg[2]
    
    local dstUnit = nil
    if #arg >= 3 then
        if arg[4] then
            self._targetPos = arg[3]
        else
            dstUnit = arg[3]
        end
    end

    if dstUnit then
        self._targetId = dstUnit:getId()
        if dstUnit._category == "building" then
            self._targetPos = dstUnit:getPos()
        else
            if self._data.track_target == true then
                self._targetPos = dstUnit._unitNode:getHitPos()
            else
                self._targetPos = dstUnit:getPos()
            end
        end

    end
    
    if self._targetPos then
        if self._data.body_type == "png" then
            self._bullet = cc.Sprite:createWithSpriteFrameName( self._bulletName .. ".png")
        elseif self._data.body_type == "cocos" then
            self._bullet = ccs.Armature:create( self._bulletName )
            self._bullet:getAnimation():play( "Fly" )
        elseif self._data.body_type == "spine" then
            local res = string.format( "effects/bullets/%s_body", self._bulletName )
            self._bullet = ue.ArmatureManager:getInstance():createArmature( res ) 
            self._bullet:setAnimation( 0, "animation", true )
        end
        if self._data.scale then
            self:setScale( self._data.scale )
        end
        self:addChild( self._bullet )
        self._atkerData = srcUnit:getRawData()
        local armature = srcUnit._unitNode:currentArmature()
        local bonePos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
        local emitpos = armature:convertToWorldSpace( cc.p( bonePos ) )
        emitpos = self._battleLayer._effectLayer:convertToNodeSpace( emitpos )
        local zorder = srcUnit._unitNode:getLocalZOrder()
        --add streak
        local color = self._data.streak_color
        if color then
            self._streak = cc.MotionStreak:create( 0.5, 3, self._data.streak_width, cc.c3b( color.r, color.g, color.b ), "effects/tuowei.png" )
            self._battleLayer:addEffect( self._streak, emitpos, zorder  )
        end
        self._battleLayer:addEffect( self, emitpos, zorder )
        self._initPos = cc.pAdd(srcUnit._unitNode:getEmitPos(), srcUnit:getPos())
        local full_dir = cc.pNormalize(cc.pSub(self._targetPos, self._initPos))
        local full_dis = cc.pGetDistance(self._targetPos, self._initPos)
        local isParacurve = self._data.move_type == "paracurve" and math.abs(full_dir.x / full_dir.y) > 0.2

        --moved to end by weiyuemin: schedule should be called after addChild, so that `paused` will be false at next frame
        ue.LuaUtils:logr(string.format("bullet(%d) schedule; srcId: %d, targetId: %d", self._bulletId, self._srcId, self._targetId))
        local function update( delta )	
            gTrace:accumulate("beforeBulletupdate")
            self._elapse = delta + self._elapse
            local function hitTest(startP, endP)
                if not isParacurve then
                    return cc.pGetDistance( startP, endP ) < self._speed * delta
                else
                    return cc.pGetDistance( startP, endP ) < 50
                end
            end
            --ue.LuaUtils:logf(string.format("bullet(%d) update: %.5f", self._bulletId, delta))
            local lastPos = cc.p( self:getPosition() )
            local target = AI._targetTable[ self._targetId ]
            if target ~= nil and target._category == "character" and self._data.track_target then
                self._targetPos = target._unitNode:getHitPos()
            end
            if hitTest( self._targetPos, lastPos ) then
                local victims = {}
                if self._data.damage_radius == nil then
                    if target ~= nil then
                        victims[#victims + 1] = target
                    end
                else
                    local radius = self._data.damage_radius
                    if self._src_camp ~= nil then
                        local units = AI.getOpponentsByCamp( self._src_camp ) 
                        if units then
                            for i,u in pairs( units ) do
                                local unit_pos = u:getPos()
                                --if MathExtend.isPositionInRange( self._targetPos, radius, unit_pos ) then
                                if cc.pDistanceSQ( self._targetPos, unit_pos ) <= radius * radius then
                                    victims[#victims + 1] = u
                                end
                            end
                        end
                    end
                end
                if #victims > 0 then
                    for _, t in pairs(victims) do
                        if self._buff and t._category == "charactor" then
                            t:addBuff( self._buff.name, self._buff.duration ) 
                        end
                        if self._damageCalculator then
                            local damage = self._damageCalculator.calculateDamage( self._baseDamage, self._atkerData, t:getRawData() )
                            ue.LuaUtils:logf(string.format("bullet(%d) damage: %.5f, srcId: %d, targetId: %d", self._bulletId, damage, self._srcId, t:getId()))
                            t:takeDamage( damage, self._srcId )
                        else 
                            t:takeNormalAttack(self._atkerData, self._srcId)
                        end
                    end
                end

                if self._data.hit_type ~= nil then
                    local res = string.format("effects/bullets/%s_hit", self._data.hit_name)
                    local bulletHit = ue.ArmatureManager:getInstance():createArmature(res)
                    if self._data.hit_scale then
                        bulletHit:setScale( self._data.hit_scale )
                    end
                    local function onAnimationEnded(event)
                        bulletHit.shouldRecycle = true
                    end
                    bulletHit:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
                    bulletHit:clearTracks()
                    bulletHit:setAnimation(0, "animation", false)
                    self._battleLayer:addToOverObjectLayer(bulletHit, self._targetPos)
                end
                if self._data.body_type == "cocos" then
                    self._bullet:getAnimation():stop()
                elseif self._data.body_type == "spine" then
                    self._bullet:clearTracks()
                end
                if self._streak then
                    self._streak:getParent():removeChild( self._streak, true )
                end
                --ue.LuaUtils:logf(string.format("bullet(%d) unschedule", self._bulletId))
                self:unscheduleUpdate()
                self:getParent():removeChild( self, true )
                gTrace:accumulate("Bulletupdate")
                return
            end
            local dir = cc.pNormalize( cc.pSub( self._targetPos, lastPos ) )
            local distance = cc.pGetDistance(self._targetPos, lastPos)
            local angle = cc.pToAngleSelf(dir)
            local newPos
            local newAngle
            if isParacurve then
                local sp = self._initPos
                local ep = self._targetPos
                local cos = math.cos(angle)
                local sin = math.sin(angle)
                local h = 200 * cos
                local cp1 = cc.p(sp.x, sp.y)
                local cp2 = cc.p(sp.x + 0.5 *(ep.x - sp.x) - h * sin , sp.y + 0.5 * (ep.y -sp.y) + h * cos)
                local t = self._elapse / self._duration
                newPos = G.bezierTo(t, sp, cp1, cp2, ep)
                local oldx, oldy = self:getPosition()
                newAngle = cc.pToAngleSelf(cc.p(newPos.x - oldx, newPos.y - oldy))
            else
                newPos = cc.pAdd( lastPos, cc.pMul( dir, self._speed * delta ) )
                newAngle = angle
            end
            self:setPosition( newPos )
            if self._streak then
                self._streak:setPosition( newPos )
            end
            if self._data.need_rotate then
                self._bullet:setRotation(-newAngle * 180 / math.pi)
            end
        end
        self:scheduleUpdateWithPriorityLua( update, 0 )
    end 
end

return Bullet
