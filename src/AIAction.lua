local AIAction = class("AIAction", nil)

function AIAction:ctor(...)
    local arg = {...}
    self._data = arg[2]
end

function AIAction:getPos()
    return cc.p(self._data.x, self._data.y)
end

function AIAction:isMove()
    return self._data.move_type ~= 0
end

function AIAction:isForceMove()
    return self._data.move_type < 0
end

function AIAction:isAttack()
    return self._data.attacking_this_frame
end

function AIAction:targetId()
    return self._data.attack_target
end

function AIAction:unitId()
    return self._data.unit_id
end

return AIAction
