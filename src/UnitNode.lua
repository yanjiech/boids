local UnitNode = class("UnitNode", function() return cc.Node:create() end)
local UnitNodeComponet = require "UnitNodeComponet"
local HpBar = require "HpBar"
local math = require('math')
require("AffineTransform")

local armatureZOrder = 1
local collideNodeZOrder = -2
local BelowShadowZOder = -1
local AboveArmatureZOrder = 2
local shadowZOrder = 0
local bleedZOrder = 3
local scale = 0.8
local defaultCollideRadius = 30

local defaultHpSize = { height = 6, width = 50 }
local hpBarZOrder = 1

function UnitNode:ctor(...)
    local arg = {...}
    self._logicUnit = arg[2]
    self._battleLayer = arg[3]
    self._data = self._logicUnit:getRawData()
    self._face = Boids.UnitFace.back
    self._direction = cc.p( 0, 0 )
    self._componets = {}
    self._saying = false
    --ccs.ArmatureDataManager:getInstance():addArmatureFileInfo( "effects/heal_skill/Fx_HolyLight.ExportJson")
    --self._healEffect = ccs.Armature:create( "Fx_HolyLight" )
    local heal_resource = G.pathForResource( "heal_skill", Boids.ResourceType.spineEffect )
    self._healEffect = ue.ArmatureManager:getInstance():createArmature( heal_resource )
    self._healEffect:setScale(0.5)
    self:addChild(self._healEffect, 10)

    local function onAnimationStart(event)
        if (event.animation == "Die") then
            if self:isInScreen() then
                --AudioManager.playEffect( "die", self._logicUnit:getData( "name" ) )
            end
            self:_showDeadBlood()
        elseif event.animation == "Attack" then
        elseif event.animation == "Cast" then
        end
    end
    
    local function onAnimationEnded(event)
        if (event.animation == "Attack" or event.animation == "Cast_2" or event.animation == "Cast2_2" ) then 
            self._logicUnit:setState( Boids.UnitState.Idle )
        elseif event.animation == "Cast" or event.animation == "Cast2" then
            self._logicUnit:setState( Boids.UnitState.Idle )
        elseif (event.animation == "Die") then
            self._logicUnit:setState( Boids.UnitState.Dead )
        end
    end 

    local function stateEventListener( event )
        if ( event.animation == "Cast" or event.animation == "Cast2" ) and event.eventData.name == "OnCasting" then
            if self:isInScreen() then
                AudioManager.playEffect( "cast", self._logicUnit:getData( "name" ) )
            end
            self._logicUnit:onCasting()
        elseif event.animation == "Attack" and event.eventData.name == "OnAttacking" then
            if self:isInScreen() then
                AudioManager.playEffect( "attack", self._logicUnit:getData( "name" ) )
            end
            self._logicUnit:onAttacking()
        elseif event.animation == "Attack" and event.eventData.name == "OnAttackBegan" then
            self:_onAttackBegan()
        elseif event.animation == "Cast" and event.eventData.name == "OnJuneng" then 
            self:addChargingEffect( 1 )
        elseif event.animation == "Cast2" and event.eventData.name == "OnJuneng" then
            self:addChargingEffect( 2 )
        end
    end

    -- load armature    
    local name = self._data["name"]
    if self._logicUnit:getData("double_face") then
        local resource = G.pathForResource(name, Boids.ResourceType.characterDoubleFace)
        self._front = ue.ArmatureManager:getInstance():createArmature(resource)
        self._front:registerSpineEventHandler(onAnimationStart, sp.EventType.ANIMATION_START)
        self._front:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
        self._front:registerSpineEventHandler( stateEventListener, sp.EventType.ANIMATION_EVENT)
        self._front:setScale(scale * self._data.scale)
        self:addChild(self._front, armatureZOrder)
        self._front:setVisible(false)
    else
        local frontResource = G.pathForResource(name, Boids.ResourceType.characterFront)
        local backResource = G.pathForResource(name, Boids.ResourceType.characterBack)
        self._front = ue.ArmatureManager:getInstance():createArmature(frontResource)
        self._front:setScale(scale * self._data.scale)
        self._back = ue.ArmatureManager:getInstance():createArmature(backResource)
        self._back:setScale(scale * self._data.scale)
        self._front:registerSpineEventHandler(onAnimationStart, sp.EventType.ANIMATION_START)
        self._front:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
        self._front:registerSpineEventHandler( stateEventListener, sp.EventType.ANIMATION_EVENT )
        self._back:registerSpineEventHandler(onAnimationStart, sp.EventType.ANIMATION_START)
        self._back:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
        self._back:registerSpineEventHandler( stateEventListener, sp.EventType.ANIMATION_EVENT )
        self:addChild(self._front, armatureZOrder)
        self:addChild(self._back, armatureZOrder)
        self._front:setVisible(false)
        self._back:setVisible(false)
    end

    --[[self._front:setGLProgramState( cc.GLProgramState:getOrCreateWithGLProgramName( cc.SHADER_POSITION_TEXTURE_COLOR_BURN ) )
    if self._back then
        self._back:setGLProgramState( cc.GLProgramState:getOrCreateWithGLProgramName( cc.SHADER_POSITION_TEXTURE_COLOR_POISON ) )
    end]]

    local function onAppearing(event)
        if event.animation == "animation" and event.eventData.name == "Appear" then
            self:turnface(cc.p(-1, 1))
            self._logicUnit:setState( Boids.UnitState.Idle )
            --self:setMovement(Boids.UnitMovement.idle)
        end
    end

    self._appear_effect = ue.ArmatureManager:getInstance():createArmature("effects/unit_appear")
    self._appear_effect:registerSpineEventHandler( onAppearing, sp.EventType.ANIMATION_EVENT )
    self:addComponet(self._appear_effect, UnitNodeComponet.LayerType.SelfAbove, "appear_effect")

    -- unit shadow
    local collide = self._data["collide"]
    local shadowSprite = cc.Sprite:createWithSpriteFrameName("unit_shadow.png")
    shadowSprite:setScale(collide / defaultCollideRadius)
    self:addChild(shadowSprite, shadowZOrder)
    self._shadow = shadowSprite

    -- debug collide
    if Boids.debugCollide or Boids.debugPush then
        self:_attachBaseNode()
    end
    if Boids.debugGuard then
        self:_attachGuardNode()
    end
    if Boids.debugAttackRange then
        self:_attachAttackRangeNode()
    end
    if (Boids.hideUnit) then
        self._front:setVisible(false)
        if (self._back ~= nil) then
            self._back:setVisible(false)
        end
        self._shadow:setVisible(false)
    end
end

function UnitNode:addChargingEffect( i )
    local function onChargingAnimationEnded( event )
        if event.animation == "animation" then
            self._is_charging_end = true
        end
    end

    local resource = "effects/" .. self._logicUnit:getData( "name" ) .. "_skill_" .. i .. "/charge"
    self._charging_effect = ue.ArmatureManager:getInstance():createArmature( resource )
    if self._charging_effect ~= nil then
        local armature = self:currentArmature()
        local bone_pos = ue.ArmatureManager:getInstance():getBonePosition( armature, "Juneng" )
        bone_pos = armature:convertToWorldSpace( bone_pos ) 
        bone_pos = self:convertToNodeSpace( bone_pos )
        self._charging_effect:setPosition( bone_pos )
        self:addChild( self._charging_effect, 11 )
        self._charging_effect:setAnimation( 0, "animation", false )
        self._charging_effect:registerSpineEventHandler( onChargingAnimationEnded, sp.EventType.ANIMATION_END ) 
    end
end

function UnitNode:removeChargingEffect()
    self._is_charging_end = false
    if self._charging_effect ~= nil then
        self._charging_effect:clearTracks()
        self._charging_effect:unregisterSpineEventHandler( sp.EventType.ANIMATION_END )
        self:removeChild( self._charging_effect, true )
        self._charging_effect = nil
    end
end

function UnitNode:takeDamage( value, isCrit )
    local hp = self._logicUnit:getHp()
    local bleedSprite = ResourceManager.getAnimationSprite(Boids.CommonAnimation.UnitBleed)
    bleedSprite:setAnchorPoint(cc.p(0, 0))
    bleedSprite:setScale(0.5)
    local animation = cc.AnimationCache:getInstance():getAnimation(Boids.CommonAnimation.UnitBleed)
    local animate = cc.Animate:create(animation)
    local function remove(node)
        node:removeFromParent()
    end
    local removeFunc = cc.CallFunc:create(remove)
    local sequence = cc.Sequence:create(animate,removeFunc)
    bleedSprite:runAction(sequence)
    bleedSprite:setPosition(self:_getHitPosition())
    self:addChild(bleedSprite, bleedZOrder)
    if self._showHP then
        self:_getHpBar():setPercentage(hp / self._data.hp)
    end
    self:_jumpNumber(value, "damage", isCrit)
    
    --play sound effect
    if self:isInScreen() then
        AudioManager.playEffect( "attacked", self._logicUnit:getData( "name" ) )
    end
end

function UnitNode:setPopup(popup, duration)
    if popup == nil or popup == "normal" then
        self:removeComponent("popup")
    elseif not self:hasComponent("popup") then
        local popupImageName = string.format("popup_%s.png", popup)
        local popupSprite = cc.Sprite:createWithSpriteFrameName(popupImageName)
        popupSprite:setAnchorPoint(cc.p(0.5, 0))
        local y = self:_getMaxY() + 10
        popupSprite:setPosition(cc.p(0, y))
        self:addComponet(popupSprite, "self_above", "popup")
        if duration ~= nil and duration > 0 then
            local function removePopup()
                self:setPopup("normal")
            end
            performWithDelay(self, removePopup, duration)
        end
    end
end

function UnitNode:takeHealing(value)
    --self._healEffect:getAnimation():play("Animation1")
    self._healEffect:setAnimation( 0, "animation", false )
    self:_jumpNumber(value, "heal", false)
    local hp = self._logicUnit:getHp()
    if self._showHP then
        self:_getHpBar():setPercentage(hp / self._data.hp)
    end
end

function UnitNode:setGLProgramState( name )
    local program_state = cc.GLProgramState:getOrCreateWithGLProgramName( name )
    if program_state then
        self._front:setGLProgramState( program_state )
        if self._back then
            self._back:setGLProgramState( program_state )
        end
    end
end

function UnitNode:riseup( time, height )
    local riseup = cc.MoveBy:create( time, cc.p( 0, height ) )
    self._front:runAction( riseup )
    if self._back then
        self._back:runAction( riseup )
    end
end

function UnitNode:falldown()
    self._front:stopAllActions()
    self._front:setPosition( cc.p( 0, 0 ) )
    if self._back then
        self._back:stopAllActions()
        self._back:setPosition( cc.p( 0, 0 ) )
    end
end

function UnitNode:addEffect(node)
    node:setPosition(self:_getHitPosition())
    self:addChild(node, bleedZOrder)
end

function UnitNode:hasComponent(name)
    return self._componets[name] ~= nil
end

function UnitNode:addComponet(node, layerType, name, auto_remove)
    local component = UnitNodeComponet:new(node, layerType, name, auto_remove)
    local pos = self:getPos()
    if layerType == UnitNodeComponet.LayerType.SelfBelow then
        self:addChild(node, BelowShadowZOder)
    elseif layerType == UnitNodeComponet.LayerType.SelfAbove then
        self:addChild(node, AboveArmatureZOrder)
    elseif layerType == UnitNodeComponet.LayerType.Object then
        self._battleLayer:addToObjectLayerByPos(node, pos)
    elseif layerType == UnitNodeComponet.LayerType.OverObject then
        self._battleLayer:addToOverObjectLayer(node, pos)
    elseif layerType == UnitNodeComponet.LayerType.Stencil then
        self._battleLayer:addToStencil(node, pos)
    elseif layerType == UnitNodeComponet.LayerType.Float then
        self._battleLayer:addToFloatLayer(node, pos)
    end
    self._componets[name] = component
end

function UnitNode:adjustComponent(component, pos)
    local t = component:getLayerType()
    local node = component:getNode()
    if t == UnitNodeComponet.LayerType.Object or t == UnitNodeComponet.LayerType.OverObject
        or t == UnitNodeComponet.LayerType.Stencil then
        node:setPosition(pos)
        node:setLocalZOrder(self:getLocalZOrder())
    elseif t == UnitNodeComponet.LayerType.Float then
        self._battleLayer:adjustFloatNode(node, pos)
    end
end

function UnitNode:adjustComponents()
    local pos = self:getPos()
    for k, v in pairs(self._componets) do
        self:adjustComponent(v, pos)
    end
end

function UnitNode:removeComponent(name)
    if self._componets[name] then
        self._componets[name]:getNode():removeFromParent()
        self._componets[name] = nil
    end
end

function UnitNode:clearComponents()
    for k, v in pairs(self._componets) do
        v:getNode():removeFromParent()
    end
    self._componets = {}
end

function UnitNode:fadeOut()
    local function removeCallback()
        self:removeFromBattle()
    end
    local fade = cc.FadeTo:create(1.2, 0)
    local callback = cc.CallFunc:create( removeCallback )
    self:currentArmature():runAction( cc.Sequence:create( fade, callback ))
    if self._saying then
        self._battleLayer._logic:endCurrentConversation()
        self._battleLayer:setCameraMode(Boids.CameraMode.Follow)
    end
end

function UnitNode:setMovement(movement)
    if (self._movement == Boids.UnitMovement.die or self._movement == Boids.UnitMovement.dead) and movement ~= Boids.UnitMovement.dead then
        return
    end
    self._movement = movement
end

function UnitNode:applyMovement()
    local movement = self._movement
    if (movement == Boids.UnitMovement.idle) then
        self:setAnimation("Idle", true)
    elseif (movement == Boids.UnitMovement.walking) then
        self:setAnimation("Walk", true)
    elseif (movement == Boids.UnitMovement.attacking) then
        self:setAnimation("Attack", false)
    elseif movement == Boids.UnitMovement.die then
        self:setAnimation("Die", false )
        self:clearComponents()
    elseif movement == Boids.UnitMovement.dead then
        self:fadeOut()
    elseif (movement == Boids.UnitMovement.casting) then
        self:setAnimation( "Cast", false )
    elseif (movement == Boids.UnitMovement.casting2) then
        local ret = self:setAnimation( "Cast2", false )
        if ret ~= 1 then
            self:setAnimation( "Cast", false )
        end
    elseif movement == Boids.UnitMovement.casting_cont_1 then
        self:setAnimation( "Cast_1", true )
    elseif movement == Boids.UnitMovement.casting_cont_2 then
        self:setAnimation( "Cast_2", false )
    elseif movement == Boids.UnitMovement.casting2_cont_1 then
        self:setAnimation( "Cast2_1", true )
    elseif movement == Boids.UnitMovement.casting2_cont_2 then
        self:setAnimation( "Cast2_2", false )
    elseif (movement == Boids.UnitMovement.stunned) then
        self:setAnimation("Stun", true)
    end
end

function UnitNode:getMovement()
    return self._movement
end

function UnitNode:setShowHP(showHP)
    self._showHP = showHP
    if self._showHP then
        self:_getHpBar()
    end
end

function UnitNode:getPos()
    return G.getPosition(self)
end

function UnitNode:getHitPos()
    local hitPos = self:_getHitPosition()
    local pos = self:getPos()
    return cc.p(pos.x + hitPos.x, pos.y + hitPos.y)
end

function UnitNode:updateFrame(dt)
    self:applyMovement()
    if self._componets["token"] ~= nil then
        self._battleLayer:adjustFloatNode(self._componets["token"]:getNode(), self:getPos())
    end
    if self._bulletStart ~= nil then
        self._bulletStart:setPosition(self:getEmitPos())
    end
    if self._is_charging_end == true then
        self:removeChargingEffect()
    end
    for i,v in pairs( self._componets ) do
        if v._should_recycle == true then
            self:removeComponent( i )
        end
    end
end

function UnitNode:appear()
    if self._movement ~= Boids.UnitMovement.appearing then
        self._appear_effect:setAnimation(0, "animation", false)
        self._logicUnit:setState( Boids.UnitState.Appear )
    end
end

function UnitNode:setToPosition(position, turnface)
    if (turnface) then
        direction = cc.pSub(position, G.getPosition(self))
        self:turnface(direction)
        self:setMovement(Boids.UnitMovement.walking)
    end
    self:setPosition(position)
    self:setLocalZOrder(self._battleLayer:zOrderForPosition(position))
    self:adjustComponents()
end

function UnitNode:turnface(direction)
    self:_setFace(direction.y > 0 and Boids.UnitFace.back or Boids.UnitFace.front)
    local rotation = 0
    if direction.y > 0 then 
        if direction.x > 0 then
            rotation = ( self._data.faceleft == 2 or self._data.faceleft == 3 )  and 180 or 0
        else
            rotation = ( self._data.faceleft == 0 or self._data.faceleft == 1  ) and 180 or 0
        end
    else
        if direction.x > 0 then
            rotation = ( self._data.faceleft == 1 or self._data.faceleft == 3 ) and 180 or 0
        else
            rotation = ( self._data.faceleft == 0 or self._data.faceleft == 2 ) and 180 or 0
        end 
    end 
    self:currentArmature():setRotationSkewY(rotation)
    self._direction = direction
end

function UnitNode:setAnimation(animationName, loop)
    local ret = 1
    local armature = self:currentArmature()
    local currentAnimationName = armature:currentAnimation()
    if (currentAnimationName ~= animationName) then
        ret = armature:setAnimation( 0, animationName, loop )
    end
    return ret
end

function UnitNode:currentArmature()
    if (self._data.double_face or self._face == Boids.UnitFace.front) then
        return self._front
    else
        return self._back
    end
end

function UnitNode:getEmitPos()
    local armature = self:currentArmature()
    return  ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
end

function UnitNode:removeFromBattle()
    self:clearComponents()
    self:removeFromParent()
end

function UnitNode:isInScreen()
    local world_pos = self:getParent():convertToWorldSpace( cc.p( self:getPosition() ) )
    local screen_size = cc.Director:getInstance():getWinSize()
    return world_pos.x >= 0 and world_pos.x < screen_size.width and world_pos.y >= 0 and world_pos.y < screen_size.height
end

function UnitNode:say(content, duration)
    local insetRect = cc.rect(100, 40, 60, 60)
    local sprite = ccui.Scale9Sprite:createWithSpriteFrameName("chat_popup.png", insetRect)
    local label = cc.Label:createWithSystemFont(content, "Helvetica", 40)
    label:setTextColor(cc.c4b(0, 0, 0, 1))
    label:setLineBreakWithoutSpace(true)
    label:setDimensions(400, 0)
    label:setHorizontalAlignment(0)
    label:setString(content)
    local contentSize = label:getContentSize()
    local realContentSize = cc.size(contentSize.width + 60, contentSize.height + 70)
    sprite:setPreferredSize(realContentSize)
    label:setAnchorPoint(cc.p(0.5, 0.5))
    label:setPosition(cc.p(realContentSize.width / 2, realContentSize.height / 2 + 15))
    sprite:addChild(label)
    local offsetY = self:_getMaxY() + realContentSize.height / 2
    local offsetX = realContentSize.width / 2 - 60
    sprite:setAdditionalTransform(AffineTransform.translate(AffineTransform.identity, offsetX, offsetY))
    self:addComponet(sprite, UnitNodeComponet.LayerType.OverObject, "chat_popup")
    local function chatTimeOutFunc()
        self._saying = false
        self:removeComponent("chat_popup")
        self._battleLayer._logic:onChatTimeOut()
    end
    local delay = cc.DelayTime:create(duration)
    local callback = cc.CallFunc:create(chatTimeOutFunc)
    sprite:runAction(cc.Sequence:create(delay, callback))
    self._saying = true
end
-- private

function UnitNode:_jumpNumber(damage, t, isCrit)
    local text = string.format("%d", damage)
    local unitPos = self:getPos()
    local maxY = self:_getMaxY()
    local startPoint = cc.p(unitPos.x, unitPos.y + maxY)
    local JumpText = require "JumpText"
    local jt = JumpText:new(text, t, isCrit, self._logicUnit:getCamp())
    self._battleLayer:addToOverObjectLayer(jt, startPoint)
    jt:start()
end

function UnitNode:_getHpBar()
    if self._componets["hpbar"] == nil then
        local bar = HpBar:new(defaultHpSize)
        local offsetY = bar:getContentSize().height * bar:getAnchorPoint().y
        offsetY = offsetY + self:_getMaxY()
        bar:setAdditionalTransform(AffineTransform.translate(AffineTransform.identity, 0, offsetY))
        self:addComponet(bar, UnitNodeComponet.LayerType.OverObject, "hpbar")
    end
    return self._componets["hpbar"]:getNode()
end

function UnitNode:_getMaxY()
    local rect = self:currentArmature():getBoundingBox()
    return rect.height * (1 - self:getAnchorPoint().y)
end

function UnitNode:_getMidY()
    local rect = self:currentArmature():getBoundingBox()
    return rect.height * (0.5 - self:getAnchorPoint().y)
end

function UnitNode:_attachGuardNode()
    local guardNode = cc.DrawNode:create()
    guardNode:drawDot(cc.p(0, 0), self._logicUnit._guard_radius, cc.c4f(1, 0, 0, 0.15))
    self:addChild(guardNode, collideNodeZOrder)
end

function UnitNode:_attachAttackRangeNode()    
    local attackNode = cc.DrawNode:create()
    attackNode:drawDot(cc.p(0, 0), self._data.range, cc.c4f(0, 1, 0, 0.3))
    self:addChild(attackNode, collideNodeZOrder)
end

function UnitNode:_attachBaseNode()
    self.baseNode = cc.DrawNode:create()
    self.baseNode:drawDot(cc.p(0, 0), self._data.collide, cc.c4f(0, 0, 1, 0.6))
    self:addChild(self.baseNode, collideNodeZOrder)
end

function UnitNode:_attachIdNode()
    --ue.LuaUtils:logf("attachIdNode")
    local label = cc.Label:createWithSystemFont(string.format("%d", self._logicUnit:getId()), "HelveticaNeue-Bold", 20)
    label:setTextColor(cc.c4b(0, 0, 255, 255))
    self:addChild(label, 100)
end

function UnitNode:_setFace(face)
    self._face = face
    if Boids.hideUnit then
        return
    end
    if self._data.double_face then
        self._front:setVisible(true)
    else
        local isFront = face == Boids.UnitFace.front
        self._front:setVisible(isFront)
        self._back:setVisible(not isFront)
    end
end


function UnitNode:_showDeadBlood()
    if (self._shadow ~= nil) then
        self._shadow:removeFromParent()
        self._shadow = nil
    end
    local bloodSprite = cc.Sprite:createWithSpriteFrameName("unit_deadblood.png")
    self._battleLayer:addToBelowObjectLayer(bloodSprite, self:getPos())
    local fade = cc.FadeTo:create(3, 0)
    local function remove(node)
        node:removeFromParent()
    end
    local removeFunc = cc.CallFunc:create(remove)
    local sequence = cc.Sequence:create(fade,removeFunc)
    bloodSprite:runAction(sequence)
end

function UnitNode:_getHitPosition()
    return ue.ArmatureManager:getInstance():getBonePosition(self:currentArmature(), "shen")
end

function UnitNode:_onAttackBegan()
    local bulletData = self._logicUnit._bulletData
    if not self._data.is_melee and bulletData.start_type ~= nil then
        if bulletData.start_type == "spine" then
            if self._bulletStart == nil then
                local res = string.format("effects/bullets/%s_start", bulletData.name)
                self._bulletStart = ue.ArmatureManager:getInstance():createArmature(res)
                local function onAnimationEnded(event)
                    self._bulletStart:setVisible(false)
                end
                self._bulletStart:registerSpineEventHandler(onAnimationEnded, sp.EventType.ANIMATION_COMPLETE)
                self:addChild(self._bulletStart, AboveArmatureZOrder)
            end
            self._bulletStart:setVisible(true)
            self._bulletStart:clearTracks()
            self._bulletStart:setAnimation(0, "animation", false)
        end
    end
end

return UnitNode
