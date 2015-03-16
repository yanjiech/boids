local Kobold = class("Kobold", require("units/patrol_unit"))
local EventAction = require("EventAction")

function Kobold:init(...)
    Kobold.super.init(self, ...)
    self._priviledged_enemy = true
end

function Kobold:postDeploy()
    self:setSightGroup('')
end

function Kobold:doDefaultBehavior()
    if not self._is_stun then
        Kobold.super.doDefaultBehavior(self)
    end
end

function Kobold:unitDie()
    ue.LuaUtils:logf("kobold stun")
    self._is_stun = true
    if self._showHp then
        self._unitNode:setShowHP(false)
    end
    self._unitNode:setMovement(Boids.UnitMovement.unknown)
    self._unitNode:setAnimation("Stun", true)
    self:removeBuff(Boids.UnitBuff.WatchNight)
    
    self:setNonAttackable(true)
    
    self._map_script:clearTargetForNightEnemy()
    
    self:reSelectChasingTarget()
    self._last_attack_target = nil
    
    self._awake_event = EventAction:new(0, 15, 0) --只触发一次的时候interval无效
    
    local function awake(finish)
        ue.LuaUtils:logf("kobold awake")
        self._currentHP = self._fullHP
        self:setNonAttackable(false)
        if self._showHp then
            self._unitNode:setShowHP(true)
        end
        self:addBuff(Boids.UnitBuff.WatchNight)
        self._is_stun = false
        --这里要不要把awake_event置空？
    end
    self._awake_event:schedule(awake, true)
end

return Kobold
