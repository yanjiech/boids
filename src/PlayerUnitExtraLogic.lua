local PlayerUnitExtraLogic = class("PlayerUnitExtraLogic", nil)

--本来可以继承自LogicUnit的，结果为了支持熊猫的换阵营，只能写成这种可插拔的附加类了

function PlayerUnitExtraLogic:beforeBehavior()
end

function PlayerUnitExtraLogic:walkByJoystick(step)
    if self.c_unit:is_during_catchup() then
        ue.LuaUtils:logr(string.format("%d during catchup", self:getId()), true)
        self:walkAlongPath()
    else
        ue.LuaUtils:logr(string.format("%d walkStep: %.4f, %.4f", self:getId(), step.x, step.y), true)
        if self.c_unit:walkStep(step.x, step.y) then
            self._stay_unmoved_this_frame = false
        end
    end
end

function PlayerUnitExtraLogic:tryAttack()  
    --ue.LuaUtils:logf(string.format("player %d try attack", self:getId()))
    if self.c_unit:is_during_catchup() then
        return false
    else
        return self:old_tryAttack()
    end
end

function PlayerUnitExtraLogic:resetPath()
    if not self.c_unit:is_during_catchup() then --如果正因为卡住而绕障碍，是不会重置path的，否则我一松摇杆，下次再动摇杆可能那个掉队的家伙又走到歧路里了
        self.c_unit:resetPath()
    end
end

function PlayerUnitExtraLogic:ctor(...)
    local args = {...}
    local unit = args[2]
    --暂时假设没有player_unit转成普通unit的情况
    unit.beforeBehavior = self.beforeBehavior
    unit.walkByJoystick = self.walkByJoystick
    unit.old_tryAttack = unit.tryAttack
    unit.tryAttack = self.tryAttack
end

return PlayerUnitExtraLogic
