local JumpText = class("JumpText", function() return cc.Node:create() end)
function JumpText:ctor(...)
    local arg = {...}
    local text = arg[2]
    local t = arg[3]
    local crit = arg[4]
    local targetCamp = arg[5]
    local label
    local content
    self._crit = crit
    if t == "heal" then
        label = cc.Label:createWithCharMap("ui/number_heal.png", 33, 40, 43)
        content = string.format("+%s", text)
    elseif t == "damage" then
        content = string.format("-%s", text)
        if targetCamp == Boids.UnitType.Player then
            label = cc.Label:createWithCharMap("ui/number_player_damage.png", 33, 40, 43)
        else
            label = cc.Label:createWithCharMap("ui/number_enemy_damage.png", 33, 40, 43)
        end
    end
    if not crit then
        label:setScale(0.6)
    --else
        --label:setScale(1.2)
    end
    label:setString(content)
    self:addChild(label)
    self._label = label
end

function JumpText:start()
    local function stop()
        self:stop()
    end
    if self._crit then
        local scale = cc.ScaleBy:create(0.10, 1.4)
        local scale2 = cc.ScaleTo:create(0.02, 1.0)
        local delay = cc.DelayTime:create(0.10)
        local fade = cc.FadeTo:create(0.5, 0)
        local moveUp = cc.MoveBy:create(0.5, cc.p(0, 100))
        local spawn = cc.Spawn:create(moveUp, fade)
        local endFunc = cc.CallFunc:create(stop)
        local sequence = cc.Sequence:create(scale, scale2, delay, spawn, endFunc)
        self._label:runAction(sequence)
    else
        local scale = cc.ScaleBy:create(0.15, 1.2)
        local delay = cc.DelayTime:create(0.1)
        local fade = cc.FadeTo:create(0.5, 0)
        local moveUp = cc.MoveBy:create(0.5, cc.p(0, 100))
        local spawn = cc.Spawn:create(moveUp, fade)
        local endFunc = cc.CallFunc:create(stop)
        local sequence = cc.Sequence:create(scale, delay, spawn, endFunc)
        self._label:runAction(sequence)
    end
end

function JumpText:stop()
    --ue.LuaUtils:logf(string.format("JumpText(%s) remove. (%.3f, %.3f)", self._text, self._startPoint.x, self._startPoint.y))
    --self:unscheduleUpdate() --remove的时候似乎自己会unschedule，不用手动调
    self:removeFromParent()
end


return JumpText
