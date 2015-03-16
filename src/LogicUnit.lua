local LogicTarget = require "LogicTarget"
local LogicUnit = class("LogicUnit", LogicTarget)
local UnitNode = require "UnitNode"
local Bullet = require "Bullet"
local MapToken = require "MapToken"
local UnitNodeComponet = require "UnitNodeComponet"
local SkillAI = require( "ai/SkillAI" )
local ViceSkillAI = require( "ai/ViceSkillAI" )

function LogicUnit:setCollideRadius(radius)
    self.c_unit:set_collide_radius(radius)
    LogicTarget.setCollideRadius(self, radius)
end

function LogicUnit:getPos() --在doDefaultBehavior那个循环执行期间调的，getPos都得sync才能和c里的保证一致，因为可能有别人推
    if self._battleLayer._frameStage == Boids.FrameStage.Behavior then
        self:syncPos()
    end
    return LogicTarget.getPos(self)
end

function LogicUnit:setPos(position)
    self.c_unit:set_position(position.x, position.y)
    LogicTarget.setPos(self, position)
end

function LogicUnit.createUnitByName( name )
    if name == "thin_vampire" or name == "fat_vampire" then
        return require( "units/vampire" ):new()
    elseif name == "skeleton_king" then
        return require( "units/skeleton_king" ):new()
    else
        return LogicUnit:new()
    end
end

function LogicUnit:isPlayerOrOpponent()
    return self:getCamp() == Boids.Camp.Player or self:getCamp() == Boids.Camp.Opponent
end

function LogicUnit:init(...)
    self._current_mid_point = 1
    self._mid_points = nil
    self._category = "charactor"
    self._ignore_next_idle_state = false 
    local arg = {...}

    self.c_unit = ue.AIUnit:new()
    local name = arg[1]
    self._data = G.shallowcopy(ResourceManager.getUnitData(name))
    local camp = arg[2]
    LogicTarget.init(self, name, camp, self._data.collide)

    if self:isPlayerOrOpponent() then
        self.c_unit:set_hesitate_frame(Boids.frameRate * (Boids.hesitateSeconds or 0.4) * 0.25)
    else
        self.c_unit:set_hesitate_frame(Boids.frameRate * (Boids.hesitateSeconds or 0.4))
    end

    self._isBoss = false
    local guardRadius = arg[3]
    if guardRadius ~= 0 then
        self._guard_radius = guardRadius
    else
        self._guard_radius = self._data.guard_radius
    end
    self.c_unit:set_guard_radius(self._guard_radius)
    self.c_unit:set_attack_radius(self._data.range)

    self._showHp = arg[4]
    self:setTag(arg[5])
    self._fullHP = self._data.hp
    self._currentHP = self._fullHP
    self._state = Boids.UnitState.unknown
    self._skillStatus = {}
    self._buffs = {}
    self._battleLayer = arg[6]
    if not self._data.is_melee and self._data.bullet_name ~= nil and self._data.bullet_name ~= "" then
        self._bulletData = ResourceManager.getBulletData(self._data.bullet_name)
    end

    --init skill
    local skills = self._data["skills"]
    if skills then
        for k,v in pairs( skills ) do
            self._skillStatus[v] = {}
            self._skillStatus[v]["cd"] = SkillConf.skill[v]["cd"]
            self._skillStatus[v]["state"] = "ready" --loading,ready,casting
            self._skillStatus[v]["load"] = self._skillStatus[v]["cd"]
        end 
    end 

    self._skill_point = 0
    --init skill ai
    if #skills > 0 then
        if self:isPlayerOrOpponent() then
            self._skill_ai = ViceSkillAI:new( self )
        else 
            self._skill_ai = SkillAI:new()
            self._skill_ai:init( self )
        end
    end

    self._shield = nil

    --自己的成员变量：
    self._speed_scale = 1.0 --掉队会加速，巡逻或别的特定逻辑也有可能导致减速
    self._concentrate_on_walk = false

    --这种LogicUnit的特性：
    --self._always_chasing_last_target = true --如果是false，那么就不会舍尽求远去追那个曾经打到过一下的人了。以及有巢穴的怪也会设为false（或者另设完全独立的参数）
    self._stop_chasing_when_lose_sight = true --如果是false，那么一旦发现你就追你到死，相当于对你点了个灯，你怎么藏都能找到你。。这个变量现在没起作用
    self._territory_radius = self._guard_radius * 3
    
    --追人的寻路约(1/4秒)会失效，到时候会重新定位。如果遇到紧急情况需要立刻反应，那可以直接将path置空
    self._path_validity = (Boids.frameRate + ue.LuaUtils:randomNumber(8)) --平均1秒多重新寻一次路
    --print("Boids.frameRate:", Boids.frameRate, "path validity:", self._path_validity)

    --[[other attribute list:

    自己的成员变量：
    _attacking_this_frame
    _chasing_target
    _last_attack_target
    _longterm_destination
    _stay_unmoved_this_frame
    _stay_unmoved
    ]]
end

function LogicUnit:deploy(deployId, position)
    LogicTarget.deploy(self, deployId, position)
    self.c_unit:initialize(deployId)
    AI.registerUnit(self)
    self._unitNode = UnitNode:new(self, self._battleLayer)
    self._unitNode:setShowHP(self._showHp)
    self._battleLayer:addToObjectLayerByPos(self._unitNode, position)
    if self._isBoss then
        self._unitNode:addComponet(MapToken:new("boss.png"), UnitNodeComponet.LayerType.Float, "token")
    end
    self._unitNode:appear()
    self._unitNode:setToPosition(position, false)
    if Boids.showId then
        self._unitNode:_attachIdNode() --这时候才有id可以显示，UnitNode构造的时候Id都没有
    end
    self._born_position = position
    self._wander_radius = 200 --self:getCollideRadius() * 10
end

function LogicUnit:postDeploy()
    if self._mid_points then
        self:walkToDestination( self._mid_points[1], nil, false, self._mid_points )
    end
end

function LogicUnit:beAttackedBy(attacker) --通常和takeDamage一起调用
    if attacker and attacker._currentHP > 0 and not self._chasing_target and 
        not attacker._nonAttackable and not self._concentrate_on_walk and not self:isHarmless() then  --攻击者无敌，或者被击者专注走路，或者被击者无害，都不会设目标
        --之所以还要判血>0，是因为可能刚被人砍死，这个地方还没来得及把AI的unitTable里的引用置空。如果这里进去的话会导致砍一会儿死人
        --ue.LuaUtils:logf(string.format("%d was attacked by %d", self:getId(), attacker:getId()))
        self:reSelectChasingTarget(attacker)
    else
        --ue.LuaUtils:logf(string.format("%d was attacked, but: ", self:getId()))
        if attacker then
            --ue.LuaUtils:logf(string.format("..attacker: %d, attacker_current_hp: %d, attacker_nonAtt: %d", 
            --    attacker:getId(), attacker._currentHP, attacker._nonAttackable or 0))
        else
            --ue.LuaUtils:logf("..attacker is nil")
        end
        if self._chasing_target then
            --ue.LuaUtils:logf(string.format("..have _chasing_target: %d", self._chasing_target:getId()))
        end
    end
end

function LogicUnit:addShield( shield )
    if shield ~= nil then
        self:removeShield()
        self._shield = shield
        self._unitNode:addChild( self._shield, 100, 100 )
    end
end

function LogicUnit:setPopup(popup, duration)
    self._unitNode:setPopup(popup, duration)
end

function LogicUnit:removeShield()
    if self._shield ~= nil then
        self._unitNode:removeChild( self._shield, true )
        self._shield = nil
    end
end

function LogicUnit:takeDamage(value, srcId, isCrit)
    local crit = isCrit ~= nil and isCrit
    if self._currentHP <= 0 then return end
    self:beAttackedBy(AI._unitTable[srcId])
    if self._shield then
        value = self._shield:absorbDamage( value )
    end
    self._currentHP = self._currentHP - value
    self._unitNode:takeDamage(value, crit)
    if (self._currentHP <= 0) then
        self:unitDie()
    end
end

function LogicUnit:attackable()
    return true
end

function LogicUnit:say(content, duration)
    self._unitNode:say(content, duration)
end

function LogicUnit:unitDie()
    if self._shield then
        self._shield:unactivate()
    end
    self:clearAllBuffs()
    self:setState(Boids.UnitState.Die)
end

function LogicUnit:takeNormalAttack(attackerData, srcId)
    local ret = DamageCalculate.settleNormalAttack(attackerData, self, srcId)
end

function LogicUnit:takeHealingByPercentage(p)
    local value = self._data.hp * p
    self:takeHealing(value)
end

function LogicUnit:takeHealing(value)
    self._currentHP = self._currentHP + value
    if (self._currentHP > self._data.hp) then
        self._currentHP = self._data.hp
    end
    self._unitNode:takeHealing(value)
end

function LogicUnit:setState(state, arg2, arg3)
    if self:getId() == 26 then
        ue.LuaUtils:logf(string.format("archer set_state:%s", state))
    end
    repeat
        if self._state == Boids.UnitState.Pierce and state ~= self._state then
            self._unitNode:falldown()
        end
        if (state == Boids.UnitState.Appear) then
            self._unitNode:setMovement(Boids.UnitMovement.idle)
        elseif (state == Boids.UnitState.MoveTo) then
            self:setDestination(arg2.rect, true)
        elseif (state == Boids.UnitState.PatrolTo) then
            self:setDestination(arg2.rect, false)
        elseif (state == Boids.UnitState.Disappear) then
            self._unitNode:fadeOut()
            self:_deleteSelf()
            self._battleLayer:onUnitDisappear(self, false)
        elseif (state == Boids.UnitState.Idle) then
            if self._ignore_next_idle_state == true then
                self._ignore_next_idle_state = false
                break
            else
                self._unitNode:setMovement(Boids.UnitMovement.idle)
            end
        elseif state == Boids.UnitState.Dead then
            self._unitNode:setMovement( Boids.UnitMovement.dead ) 
        elseif (state == Boids.UnitState.Die) then
            self._unitNode:setMovement(Boids.UnitMovement.die)
            self:_deleteSelf()
            self._battleLayer:onUnitDisappear(self, true)
        elseif state == Boids.UnitState.Stunned then
            self._unitNode:setMovement(Boids.UnitMovement.stunned)
        elseif state == Boids.UnitState.Attack then
            self._unitNode:setMovement( Boids.UnitMovement.attacking )    
        elseif state == Boids.UnitState.Cast then
            if arg2 == 1 then
                if arg3 == 1 then
                    self._unitNode:setMovement( Boids.UnitMovement.casting_cont_1 )
                elseif arg3 == 2 then
                    self._unitNode:setMovement( Boids.UnitMovement.casting_cont_2 )
                else
                    self._unitNode:setMovement( Boids.UnitMovement.casting )
                end
            elseif arg2 == 2 then
                if arg3 == 1 then
                    self._unitNode:setMovement( Boids.UnitMovement.casting2_cont_1 )
                elseif arg3 == 2 then
                    self._unitNode:setMovement( Boids.UnitMovement.casting2_cont_2 )
                else
                    self._unitNode:setMovement( Boids.UnitMovement.casting2 )
                end
            end
        elseif state == Boids.UnitState.Pierce then
            self._unitNode:setMovement( Boids.UnitMovement.stunned )
            self._unitNode:riseup( arg2, arg3 )
        end
        self._state = state
    until true
end

function LogicUnit:setCustomChange(customChange)
    local t = customChange:split(",")
    for _, v in ipairs(t) do
        local kvp = v:split(":")
        local key = kvp[1]
        local value = kvp[2]
        if key == "guard_radius" then
            self._guard_radius = tonumber(value)
        end
        self._data[key] = tonumber(value)
    end
    if self._currentHP == self._fullHP then
        self._currentHP = self._data.hp
    end
    self._fullHP = self._data.hp
end

function LogicUnit:setDestination(region, isForce)
    local point = G.center(region)
    --self:walkToDestination(point, nil, true)
    --现在行为是走到region的内接中心圆
    local radius = math.min(region.width, region.height) * 0.5
    self:walkToDestination(point, radius, isForce)
end

function LogicUnit:setSightGroup(group)
    self._sight_group = group
end

function LogicUnit:setCamp(camp)
    if camp ~= self._camp then
        LogicTarget.setCamp(self, camp)
        self.c_unit:set_unit_camp(camp)
        if self._battleLayer then
            self._battleLayer:changeUnitCamp(self, camp)
        end
        if camp == Boids.Camp.Player or camp == Boids.Camp.Opponent then --如果设置为player的，那么要使用player_extra。暂时假定不会有player_unit变成普通unit的情况，有了再改
            self._player_extra = require("PlayerUnitExtraLogic"):new(self)
            --print("extra_logic")
        end
    end
end

function LogicUnit:setShowHP(showHp)
    self._showHp = showHp
    if self._unitNode ~= nil then
        self._unitNode:setShowHP(showHp)
    end
end

function LogicUnit:setGuardRadius(guardRadius)
    if (guardRadius == 0) then
        self._guard_radius = self._data.guard_radius
    else
        self._guard_radius = guardRadius
    end
end

function LogicUnit:setTag(tag)
    self._tags = tag:splitIntoSet(",")
end

function LogicUnit:getTags()
    return self._tags
end

function LogicUnit:addTag(tag)
    self._tags[tag] = true
end

function LogicUnit:hasTag(tag)
    if self._tags[tag] then
        return true
    end
    return false
end

function LogicUnit:setPatrol(areas)
end

function LogicUnit:setChasingTarget(target)
    local originalTarget = self._chasing_target
    self._chasing_target = target
    if self._chasing_target ~= nil and self._chasing_target:getCamp() == Boids.UnitType.Player and originalTarget == nil then
        self:foundPlayer()
    end
end

function LogicUnit:setBoss(isBoss)
    if not self._showHp then
        self:setShowHP(isBoss)
    end
    self._isBoss = isBoss
end

function LogicUnit:doAttack(unit)
    local direction = cc.pSub(unit:getPos(), self:getPos())
    self._unitNode:turnface(direction)
    self:setState( Boids.UnitState.Attack )
end

function LogicUnit:updateFrame(dt)
    self:updateSkill(dt)
    for _, buff in pairs(self._buffs) do
        buff:update(dt)
    end
    for name, buff in pairs(self._buffs) do
        if buff:shouldRecycle() then
            self:removeBuff(name)
        end
    end
end

function LogicUnit:updateSkill(dt)
    local camp = self:getCamp()
    for k,v in pairs( self._skillStatus ) do
        if v["state"] == "loading" then
            v["load"] = v["load"] + dt
            if v["load"] >= v["cd"] then
                v["state"] = "ready"
                v["load"] = v["cd"]
            end
        end
    end
    if self._skill_ai ~= nil then
        self._skill_ai:behave( dt )
    end
end

function LogicUnit:setSkillAI( ai )
    self._skill_ai = ai
end

function LogicUnit:getSkills()
    return self._data["skills"]
end

function LogicUnit:getSkillHintTypeById( i )
    local skill_name = self._data["skills"][i]
    return SkillConf.skill[skill_name]["hint"]
end

function LogicUnit:getSkillRangeById( i )
    local skill_name = self._data["skills"][i]
    return SkillConf.skill[skill_name]["range"]
end

function LogicUnit:getSkillMinRangeById( i )
    local skill_name = self._data["skills"][i]
    return SkillConf.skill[skill_name]["min_range"]
end

function LogicUnit:getSkillMaxRangeById( i )
    local skill_name = self._data["skills"][i]
    return SkillConf.skill[skill_name]["max_range"]
end

function LogicUnit:isSkillReadyById( i )
    if i == 0 then
        return self._skill_point >= 100.0 
    else
        local name = self._data["skills"][i]
        return self:isSkillReadyByName( name )
    end
end

function LogicUnit:isSkillReadyByName( name )
    local sk = self._skillStatus[name]
    if sk then
        return sk["state"] == "ready"
    end
    return false
end

function LogicUnit:getSkillCDById( i )
    if i == 0 then
        return self._skill_point >= 100.0 and 1.0 or ( self._skill_point / 100.0 )
    else
        local name = self._data["skills"][i]
        return self:getSkillCDByName( name )
    end
end

function LogicUnit:getSkillCDByName( name )
    local sk = self._skillStatus[name]
    if sk then
        local cd = sk["cd"]
        local ld = sk["load"]
        return ld / cd
    end
    return 0
end

function LogicUnit:useSkillById( i, dir, range )
    if not self:canUseSkill() then
        return false
    end
    --现在录像假设了player_unit只有一个技能，即i始终为1。不是这样的时候再改
    if i == 1 then
        self._skill_point = 0
    end
    local battle_layer = self._battleLayer
    if i == 1 and battle_layer._replay_to_save then --只有手动放的技能(第一个技能)才进录像
        battle_layer._replay_to_save:setSkill(self:getId(), dir.x, dir.y, range or 0)
    end
    
    local sk = self._data["skills"][i] 
    self._using_skill_id = i
    self:useSkill( sk, dir, range )
    if i == 1 then
        self:setState( Boids.UnitState.Cast, 1, 0 )
    elseif i == 2 then
        self:setState( Boids.UnitState.Cast, 2, 0 )
    end

    return true
end

--do not invoke directly
function LogicUnit:useSkill( skill_name, dir, range ) 
    ue.LuaUtils:logr(string.format("use skill: %s, %.2f, %.2f", skill_name, dir.x, dir.y ))
    self._skill_dir = dir
    self._skill_range = range
    self._relax_frame_count = nil
    self.c_unit:resetLastWalk()
    if dir and not( dir.x == 0 and dir.y == 0 ) then
        self._unitNode:turnface( dir )
    end
    self._castingSkill = skill_name
    self._skillStatus[skill_name]["load"] = 0
    self._skillStatus[skill_name]["state"] = "loading"
end

function LogicUnit:isMelee()
    return self._data["is_melee"]
end

function LogicUnit:getRawData()
    return self._data
end

function LogicUnit:getData(key)
    return self._data[key]
end

function LogicUnit:getHp()
    return self._currentHP
end

function LogicUnit:getHpPercent()
    return self._currentHP / self._fullHP
end

function LogicUnit:getDirection()
    return self._unitNode._direction
end

function LogicUnit:getTarget()
    return self._chasing_target
end

function LogicUnit:addBuff(buffName, ...)
    if not self:hasBuff(buffName) then
        local class = require(string.format("buffs/%s", buffName))
        local buff = class:new(...)
        self._buffs[buffName] = buff
        buff:onAttach(self)
    end
end

function LogicUnit:hasBuff(buffName)
    return self._buffs[buffName] ~= nil
end

function LogicUnit:removeBuff(buffName)
    local buff = self._buffs[buffName]
    if buff ~= nil then
        self._buffs[buffName] = nil
        buff:onDetach(self)
    end
end

function LogicUnit:clearAllBuffs()
    for i,v in pairs( self._buffs ) do
        v:onDetach( self ) 
    end
    self._buffs = {}
end

function LogicUnit:onCasting()
    if self:isPlayerOrOpponent() then
        ue.LuaUtils:logf(string.format("unit %d onCasting", self:getId()))
    end
    if self._castingSkill then
        local SkillClass = require( "skills/" .. self._castingSkill )
        local skill = SkillClass:new()
        if skill:activate( self._battleLayer, self, self._skill_dir, self._skill_range, self._using_skill_id ) then
            ue.LuaUtils:logf(string.format("activate skill: %s", skill.__cname))
            --activate skill success
        else
            --activate skill failed
            ue.LuaUtils:logf( string.format("unit %d use skill failed!", self:getId()))
            self._skillStatus[self._castingSkill]["state"] = "ready"
            self._skillStatus[self._castingSkill]["load"] = self._skillStatus[self._castingSkill]["cd"]
            self:setState( Boids.UnitState.Idle )
        end
        self._castingSkill = nil
    end

end

function LogicUnit:cancelCast()
   self._skillStatus[self._castingSkill]["state"] = "ready"
   self._skillStatus[self._castingSkill]["load"] = self._skillStatus[self._castingSkill]["cd"]
   self._castingSkill = nil
end

function LogicUnit:addHitEffect( name )
    local resource = "effects/bullets/" .. name
    local hit_effect = ue.ArmatureManager:getInstance():createArmature( resource )
    hit_effect:setPosition( self._unitNode:_getHitPosition() )
    hit_effect:setAnimation( 0, "animation", false )
    self._unitNode:addComponet( hit_effect, UnitNodeComponet.LayerType.SelfAbove, "default_hit", true ) 
end

function LogicUnit:onAttacking()
    --[[if self:getId() <= 4 then
        ue.LuaUtils:logf(string.format("unit %d onAttacking", self:getId()))
    end]]
    if self:isMelee() then
        local target = self._chasing_target
        if target ~= nil then
            self:addHitEffect( "default_hit" )
            target:takeNormalAttack(self:getRawData(), self:getId())
        end 
    else
        local bullet = Bullet:new(self._bulletData)
        bullet:shoot( self, self._battleLayer, self:getTarget() )
    end
    --for test purpose
    if self._skill_point < 100.0 then
        self._skill_point = self._skill_point + 10.0
    end
end

function LogicUnit:onAttackEnd()
    self:setState( Boids.UnitState.Idle )
end

function LogicUnit:isHarmless()
    return self._guard_radius < 0
end

function LogicUnit:calculatePriority()
    local p = 0
    if self:isUnpushable() then
        p = Boids.AI.UNPUSHABLE_PRIORITY
    else
        local camp = self:getCamp()
        if camp == Boids.Camp.Enemy or camp == Boids.Camp.Ally then
            if self._priviledged_enemy then
                p = 20
            elseif self._low_enemy then
                p = 0
            else
                p = 10
            end
        elseif camp == Boids.UnitType.Neutral then
            p = 30
        elseif camp == Boids.UnitType.Player or camp == Boids.UnitType.Opponent then
            p = 50
        end
    end
    self.c_unit:set_priority(p)

    --优先级相同，且同阵营的，用priority2来比较
    if self:isDuringAttackOrCast() then --优先级：走路的 > 站着或卡着不动的 > 正在打的        
        self.c_unit:set_priority2(0)
    elseif self._stay_unmoved then
        self.c_unit:set_priority2(1)
    else
        self.c_unit:set_priority2(2)
    end

    --debug
    --ue.LuaUtils:logf(string.format("unit %d priority: %d, %d", self:getId(), self.c_unit:get_priority(), self.c_unit:get_priority2()))
end

function LogicUnit:syncPos()
    self._position = self.c_unit:get_position()
end

function LogicUnit:canUseSkill()
    return not ( self._state == Boids.UnitState.Cast or self._state == Boids.UnitState.Stunned or self._state == Boids.UnitState.Pierce )
end

function LogicUnit:isDuringAttackOrCast()
    --[[local mm = self._unitNode:getMovement()
    return mm == Boids.UnitMovement.attacking or 
                mm == Boids.UnitMovement.casting or 
                mm == Boids.UnitMovement.casting2 or
                mm == Boids.UnitMovement.casting_cont_1 or
                mm == Boids.UnitMovement.casting_cont_2 or
                mm == Boids.UnitMovement.casting2_cont_1 or
                mm == Boids.UnitMovement.casting2_cont_2 or
                mm == Boids.UnitMovement.stunned]]
    return self._state == Boids.UnitState.Cast or self._state == Boids.UnitState.Attack or self._state == Boids.UnitState.Stunned or self._state == Boids.UnitState.Pierce
end

function LogicUnit:foundPlayer()
    self._unitNode:setPopup("red_surprise", 1)
end

function LogicUnit:isWalking()
    return self._unitNode:getMovement() == Boids.UnitMovement.walking
end

function LogicUnit:findOpponent()
    if self._always_chasing_last_target and self._last_attack_target then --always_chasing_last_target优先级次高
        return self._last_attack_target
    else
        local target = self:findOpponentInView() --视野(警戒范围)内的敌人的远近优先级第三高。每一帧都找视野里最近的，如果之前在追另一个，现在有更近的，那么立马打近的。

        if target == nil and self._chasing_target then --如果没有在视野内找到敌人，但因为别的原因被赋了chasing_target，那么去找这个chasing_target打。通常是因为被在警戒范围外的人攻击了。。
            return self._chasing_target
        end
        return target
    end
end

function LogicUnit:getNextWanderPos()
    ue.LuaUtils:logr(string.format("%d getNextWanderPos.", self:getId()), true)
    for t = 1, 3 do
        local r = ue.LuaUtils:randomNumber(self._wander_radius)
        local new_pos = cc.pAdd(self._born_position, cc.pMul(cc.pForAngle(ue.LuaUtils:randomFloat() * math.pi), r))
        if cAI:isPositionOK(new_pos, self:getCollideRadius()) then
            return new_pos
        end
    end
    return nil
end

function LogicUnit:isHoldPosition()
    return self._hold_position or self:hasBuff(Boids.UnitBuff.InCage)
end

function LogicUnit:beforeBehavior()	
    gTrace:accumulate("beforeBeforeBehavior")
    if not self.c_unit:hasPath() and not self:isHoldPosition() then --当前没有要走的路，那么就开始徘徊逻辑            
        if self._final_destination ~= nil then
            self:walkToDestination( self._final_destination, nil, false, self._mid_points )
        elseif self._relax_frame_count then
            if self._relax_frame_count > 0 then --如果正在休息的话继续休息到0
                self._relax_frame_count = self._relax_frame_count - 1
            else
                local new_pos = self:getNextWanderPos()
                if new_pos then        
                    --ue.LuaUtils:logf(string.format("wander again: %d", self:getId()))
                    self._relax_frame_count = nil
                    self._speed_scale = Boids.wanderSpeedScale
                    self:_findPath(new_pos, 1000)
                else --居然没找到合适的徘徊位置，只能继续休息了
                    self._relax_frame_count = Boids.frameRate * 2 + ue.LuaUtils:randomNumber(4) / 2
                    --ue.LuaUtils:logf(string.format("%d continue rest for %d frames", self:getId(), self._relax_frame_count))
                end
            end
        else --还没有休息过的话先休息会儿
            self._relax_frame_count = Boids.frameRate * 2 + ue.LuaUtils:randomNumber(4) / 2
            self.c_unit:resetLastWalk()
            --ue.LuaUtils:logf(string.format("%d start rest for %d frames", self:getId(), self._relax_frame_count))
        end
    else --如果现在有路要走，或者平时hold_position的怪，要判断一下有没有出巢穴；出了巢穴就强制会出生点
        if not self._longterm_destination then --如果有longterm_destination那么是不管的
            if cc.pDistanceSQ(self:getPos(), self._born_position) > self._territory_radius * self._territory_radius then
                ue.LuaUtils:logf(string.format("%d return to born position", self:getId()))
                self:walkToDestination(self._born_position, nil, true)
            end
        end
    end
    gTrace:accumulate("beforeBehavior")
end

function LogicUnit:tryAttack()
    --只要harmless，且不是义无反顾地一心走路的，那么就要看看能不能打人，以及是否要为了打人或别的原因而改变路线
    if not self:isHarmless() and not self._concentrate_on_walk then
        local target = self:findOpponent()
        if target then
            --debug
            --if self:isPlayerOrOpponent() then
                --ue.LuaUtils:logr(string.format("unit %d try attack %d", self:getId(), target:getId()), true)
            --end

            if self:canAttack(target) then --有目标且目标在攻击范围内
                self:attack(target)
                gTrace:accumulate("tryAttack")
                return true
            end
            self:reSelectChasingTarget(target) --当然，内部会判，如果target其实没变，是不会重新寻路的
        end
    end
    gTrace:accumulate("tryAttack")
    return false
end

function LogicUnit:doDefaultBehavior()
    if not self:tryAttack() then
        --到这儿的除了要发呆的都是要走的。或者是本来就想走，或者是还不能直接打，需要走。

        if Boids.debugPush then
            if self.c_unit:get_move_type() == Boids.AIMoveType.be_pushed then --但是被推了的单位是不能走的
                self._unitNode.baseNode:setVisible(true)
            else
                self._unitNode.baseNode:setVisible(false)
            end
        end
        if self.c_unit:get_move_type() == Boids.AIMoveType.be_pushed then --但是被推了的单位是不能走的
            --ue.LuaUtils:logf(string.format("unit %d be pushed", self:getId()))
            return
        end

        if self.c_unit:hasPath() then
            self:walkAlongPath()
        end
    end
    gTrace:accumulate("behavior")
end

function LogicUnit:performAction()
    local move_type = self.c_unit:get_move_type()
    if self._attacking_this_frame then
        --ue.LuaUtils:logf(string.format("%d attacks %d", self:getId(), self._chasing_target:getId()))
        self:doAttack(self._chasing_target)
    elseif move_type ~= Boids.AIMoveType.stay then
        if move_type == Boids.AIMoveType.stuck then
            self._unitNode:turnface(self.c_unit:get_dir_want_to_go())
        else
            local pos = self:getPos()
            self._unitNode:setToPosition(pos, self.c_unit:get_move_type() ~= Boids.AIMoveType.be_pushed)
            self._battleLayer:onUnitMove(self, pos)
        end
    elseif (self._unitNode:getMovement() == Boids.UnitMovement.walking) then
        --ue.LuaUtils:logf(string.format("%d turned from walking to idle", self:getId()))
        --self._unitNode:setMovement(Boids.UnitMovement.idle)
        self:setState( Boids.UnitState.Idle )
    end
end

function LogicUnit:walkToDestination(destination, radius, is_force, mid_points)
    --指定了一个什么目的地的话，要清空正追着打的目标。上一个攻击的目标也清掉，免得他又跑去打
    self._last_attack_target = nil
    self:reSelectChasingTarget()

    if radius then
        self:_findPathWithDestRadius(destination, radius, self._path_validity)
        self._longterm_destination_radius = radius
    else
        self:_findPath(destination, self._path_validity) --还是不能寻一次一直用，因为中途可能被推走什么的
        self._longterm_destination_radius = nil
    end

    self._concentrate_on_walk = is_force

    self._longterm_destination = destination

    self._final_destination = destination
    self._mid_points = mid_points
end

--private

function LogicUnit:inDirectView(other) --即不考虑共享视野能否看到
    if other.c_unit then
        return self.c_unit:inDirectView(other.c_unit)
    else
        local pos = other:getPos()
        return self.c_unit:buildingInDirectView(pos.x, pos.y, other:getCollideRadius())
    end
end

function LogicUnit:canAttack(other)
    if other.c_unit then
        return self.c_unit:canAttack(other.c_unit)
    else
        local pos = other:getPos()
        return self.c_unit:canAttackBuilding(pos.x, pos.y, other:getCollideRadius())
    end
end

function LogicUnit:resetPath()
    self.c_unit:resetPath()
end

function LogicUnit:reSelectChasingTarget(new_target)
    if new_target then
        if new_target ~= self._chasing_target then
            self:setChasingTarget(new_target)
            if new_target:getCamp() == Boids.UnitType.Player then
                self:foundPlayer()
            end
            self:generatePathForTarget()
        elseif not self.c_unit:hasPath() then --目标没变，但没path了; 通常是因为打了一下后目标走开了
            --print("target not change but no path", self:getId())
            self:generatePathForTarget()
        end
    else
        self:setChasingTarget(nil)
        self:resetPath()
    end
end

function LogicUnit:findOpponentInView()
    local opponent = nil
    local opponent_distance_sqr = 1000000000

    local function compare_distance(candidate_opponent)
        local len_sqr = cc.pDistanceSQ(self:getPos(), candidate_opponent:getPos())
        if len_sqr < opponent_distance_sqr then
            opponent = candidate_opponent
            opponent_distance_sqr = len_sqr
        end
    end

    local function if_in_view_then_compare_distance(candidate_opponent)
        if not candidate_opponent._nonAttackable and 
            --如果本来就在追这个单位，那么就算作能看到
            (self._chasing_target == candidate_opponent or self:inDirectView(candidate_opponent)) then
            compare_distance(candidate_opponent)
        end
    end

    local function compare_distances(candidates)
        for _, candidate_opponent in pairs(candidates) do
            compare_distance(candidate_opponent)
        end
    end

    local function if_in_view_then_compare_distances(candidates)
        for _, other in pairs(candidates) do
            if_in_view_then_compare_distance(other)
        end
    end

    if self._sight_group == '' then
        if_in_view_then_compare_distances(self:getOpponents())

        if self:getCamp() == Boids.Camp.Wild then
            for _, other in pairs(AI._targetTable) do
                if other:getCamp() ~= Boids.Camp.Wild then
                    if_in_view_then_compare_distance(other)
                end
            end
        else
            if_in_view_then_compare_distances(AI._wild)
        end
    else
        compare_distances(AI._can_see[self._sight_group])
    end

    if AI._map_script and AI._map_script.getExtraOpponents then
        local extras = AI._map_script:getExtraOpponents(self)
        if extras then
            compare_distances(extras)
        end
    end

    return opponent
end

function LogicUnit:getOpponents()
    return AI.getOpponents(self)
end

function LogicUnit:attack(target)
    --debug
    if self:isPlayerOrOpponent() then
        ue.LuaUtils:logr(string.format("unit %d attacks %d", self:getId(), target:getId()), true)
    end
    self:resetPath()
    self._relax_frame_count = nil
    self.c_unit:resetLastWalk()
    self._last_attack_target = target
    self:setChasingTarget(target)
    self._attacking_this_frame = true
end

function LogicUnit:generatePathForTarget()
    self._speed_scale = 1.0
    if self._data.movespeed > 2e-4 then --所有的寻路都得有移动速度才能动
        --print(self:getId(), "findPathForTarget:", self._chasing_target:getId(), self._chasing_target, self._chasing_target.c_unit)
        if self._chasing_target.c_unit then --有c_unit的估计是单位
            self.c_unit:findPathForTarget(self._chasing_target.c_unit, self._path_validity)
        else
            local dest = self._chasing_target:getPos()
            self.c_unit:findPath(dest.x, dest.y, self._path_validity)
        end
    end
end

function LogicUnit:calculateMaxWalkLength()
    self.max_walk_length = self._data.movespeed * Boids.dt * self._speed_scale
    self.c_unit:set_max_walk_length(self.max_walk_length)
end

function LogicUnit:refreshPath() --这段有些冗余：c++有相关信息，且逻辑应该不怎么变，其实可以写到c++里的。以后有必要的时候再重构
    if self._concentrate_on_walk then --concentrate_on_walk的话，一定有长期目标
        if self._longterm_destination_radius then
            self:_findPathWithDestRadius(self._longterm_destination, self._longterm_destination_radius, self._path_validity)
        else
            self:_findPath(self._longterm_destination, self._path_validity)
        end
    elseif self._chasing_target then
        self:generatePathForTarget()
    elseif self._longterm_destination then
        if self._longterm_destination_radius then
            self:_findPathWithDestRadius(self._longterm_destination, self._longterm_destination_radius, self._path_validity)
        else
            self:_findPath(self._longterm_destination, self._path_validity)
        end
    else --这种情况可能是chasing_target死了且这帧还没找到新的目标
        ue.LuaUtils:logf(string.format("%d refresh outdated path but donot know what to do", self:getId()))
        self.c_unit:resetPath()
        return false
    end
    return true
end

function LogicUnit:walkAlongPath()
    if self.c_unit:isPathOutdated() then --路径过期则重新生成
        --ue.LuaUtils:logf(string.format("%d path outdated", self:getId()))
        if not self:refreshPath() then    
            return
        end
    end

    --前面都还有可能改speed_scale，所以到这里再算max_walk_length
    self:calculateMaxWalkLength()
    gTrace:accumulate("beforeWalkFrame")
    if self.c_unit:walkFrameOnPath() then --要保证先确认路径没有过期再walk。否则直接调c++的walk可能导致这一帧发呆
        self._stay_unmoved_this_frame = false
    end
    gTrace:accumulate("walkFrame")
    
    if self.c_unit:isPathEnd() then
        self:pathEnd()
    end
end

function LogicUnit:pathEnd()
    --print("pathend:", self:getId(), self:getName())
    self.c_unit:resetPath()
    self._concentrate_on_walk = false
    if self._longterm_destination then
        self._born_position = self._longterm_destination
        self._longterm_destination = nil
    end
    if self._mid_points and self._current_mid_point < #self._mid_points then
        self._current_mid_point = self._current_mid_point + 1
        self:walkToDestination( self._mid_points[self._current_mid_point], nil, false, self._mid_points )
    else 
        self._final_destination = nil
        self._mid_points = nil
        self._current_mid_point = 1
    end
end    

function LogicUnit:_findPath(destination, path_validity_frame)
    if self._data.movespeed > 2e-4 then --所有的寻路都得有移动速度才能动
        gTrace:accumulate("beforeFindPath")
        self.c_unit:findPath(destination.x, destination.y, path_validity_frame)
        gTrace:accumulate("findPath")
    end
end

function LogicUnit:_findPathWithDestRadius(destination, radius, path_validity_frame)
    if self._data.movespeed > 2e-4 then --所有的寻路都得有移动速度才能动	
        gTrace:accumulate("beforeFindPath")
        --print("findPathWithDestRadius")
        self.c_unit:findPathWithDestRadius(destination.x, destination.y, radius, path_validity_frame)
        gTrace:accumulate("findPath")
    end
end

function LogicUnit:_deleteSelf()
    self._skill_ai = nil
    LogicTarget._deleteSelf(self)
    AI.unRegisterUnit(self)
    self.c_unit:deleteUnit()
end

function LogicUnit:getUnitNode()
    return self._unitNode
end

function LogicUnit:getState()
    return self._state
end

function LogicUnit:setForceUnpushable( b )
    self._forceUnpushable = b
end

function LogicUnit:isUnpushable()
    return self._isBoss or self:hasBuff(Boids.UnitBuff.InCage) or self._forceUnpushable
end

return LogicUnit
