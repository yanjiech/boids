local EventTrigger = class("EventTrigger", nil)

function EventTrigger:ctor(...)
    local arg = {...}
    self._event = arg[2]
    self._triggers = self._event.triggers
    self._triggerIndex = 1
    self._isEnable = true
    self._repeat = self._event.trigger_meta.is_repeated
    self._relation = self._event.trigger_meta.trigger_relation
    if not self._relation then
        self._relation = "then"
    end
    for _, trigger in ipairs(self._triggers) do
        trigger.fired = false
    end
end

function EventTrigger:triggersForTest()
    if self._relation == "then" then
        local ret = {}
        ret[1] = self._triggers[self._triggerIndex];
        return ret;
    else
        return self._triggers;
    end
end

function EventTrigger:onSingleTriggerPass(trigger, logic, unit)
    trigger.fired = true
    if self:canFire() then
        logic:excuteEvent(self:event(), unit)
        if self._repeat then
            self._triggerIndex = 1
            for _, trigger in ipairs(self._triggers) do
                trigger.fired = false
            end
        else
            self._isEnable = false
        end
    end
end

function EventTrigger:canFire()
    if self._relation == "then" then
        return self:moveOn()
    elseif self._relation == "or" then
        return true
    else
        for _, trigger in ipairs(self._triggers) do
            if trigger.fired == false then
                return false
            end
        end
        return true
    end
end

function EventTrigger:event()
    return self._event
end

function EventTrigger:moveOn()
    self._triggerIndex = self._triggerIndex + 1
    return self._triggerIndex > #self._triggers
end

function EventTrigger:valid()
    return self._triggerIndex <= #self._triggers and self._event.isEnable and self._isEnable
end

function EventTrigger:setEnable(isEnable)
    self._event.isEnable = isEnable
    self._triggerIndex = 1
end

return EventTrigger
