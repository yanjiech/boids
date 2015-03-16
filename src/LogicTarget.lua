local LogicTarget = class("LogicTarget", nil)

function LogicTarget:init(name, camp, radius)
    self:setName(name)
    self:setCamp(camp)
    self:setCollideRadius(radius)
    if not self:attackable() then
        self:setNonAttackable( true )
    else
        self:setNonAttackable( false )
    end
end

function LogicTarget:getName()
    return self._name
end

function LogicTarget:setName(name)
    self._name = name
end

function LogicTarget:getCamp()
    return self._camp
end

function LogicTarget:setCamp(camp)
    if camp ~= self._camp then
        local camp_not_newly_set = self._camp ~= nil
        if camp_not_newly_set then
            if self.camp == Boids.Camp.Enemy or --原先是敌人，现在变化了，那么原来(我方)的target肯定失效了
                    (self.camp == Boids.Camp.Player or self.camp == Boids.Camp.Ally) and 
                    camp ~= Boids.Camp.Player and camp ~= Boids.Camp.Ally then --原先是我方，且现在不是我方，那么原来(敌方)的target肯定失效了
                self:clearTarget()
            end
            AI.unRegisterTarget(self)
        end
        self._camp = camp
        if camp_not_newly_set then
            AI.registerTarget(self)
        end
    end
end

function LogicTarget:getCollideRadius()
    return self._collide_radius
end

function LogicTarget:setCollideRadius(radius)
    self._collide_radius = radius
end

function LogicTarget:getId()
    return self._deployId
end

function LogicTarget:getPos()
    return self._position
end

function LogicTarget:setPos(position)
    self._position = position
end

function LogicTarget:deploy(deployId, position)
    self._deployId = deployId
    ue.LuaUtils:logr(string.format("deploy %s(%d) at (%.3f, %.3f)", self:getName(), deployId, position.x, position.y))
    AI.registerTarget(self)
    self:setPos(position)
end

function LogicTarget:setNonAttackable(nonAttackable)
    if self._nonAttackable ~= nonAttackable then
        self._nonAttackable = nonAttackable
        if nonAttackable then
            self:clearTarget()
        end
    end
end

function LogicTarget:clearTarget()
    local id = self:getId()
    local function clearTargetForPossibleOpponents(opponents)        
        for _, logicUnit in pairs(opponents) do
            if logicUnit._chasing_target and logicUnit._chasing_target:getId() == id then
                logicUnit:reSelectChasingTarget()
            end
            if logicUnit._last_attack_target and logicUnit._last_attack_target:getId() == id then
                logicUnit._last_attack_target = nil
            end
        end
    end
        clearTargetForPossibleOpponents(AI._unitTable)
end

function LogicTarget:_deleteSelf()
    local id = self:getId()
    ue.LuaUtils:logf(string.format("delete self: %d", id))
    self:clearTarget()
    AI.unRegisterTarget(self)
end

return LogicTarget
