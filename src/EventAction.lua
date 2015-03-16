local EventAction = class("EventAction", nil)

g_eventActionPool = __eventActionPool or {}

function EventAction:ctor(...)
    local arg = {...}
    if #arg > 2 then
        self._repeat = arg[2]
        self._delay = arg[3] == 0 and 0.01 or arg[3]
        self._interval = arg[4]
    else
        self._meta = arg[2]
        self:_parseMeta()
    end
    self._count = 0
end

function EventAction:schedule(func, autoUnschedule)
    print("schedule action")
    local scheduler = cc.Director:getInstance():getScheduler()
    local function onTrigger(elapsed)
        local finish = false
        if not self:_isInfinite() then
            self._count = self._count + 1
            finish = self._count > self._repeat
        end
        func(finish)
        if (finish and autoUnschedule) then
            self:unschedule()
        end
    end
    self._eid = scheduler:scheduleScriptFunc(onTrigger, self._interval, self._repeat, self._delay, false)
    ue.LuaUtils:logf(string.format("%d = scheduleScriptFunc(, %.2f, %d, %.2f)", self._eid, self._interval, self._repeat, self._delay))
    g_eventActionPool[self._eid] = self
    return self._eid
end

function EventAction.unscheduleAll()
    for _, ea in pairs(g_eventActionPool) do
        ea:unschedule()
    end
end

function EventAction:unschedule()
    local scheduler = cc.Director:getInstance():getScheduler()
    g_eventActionPool[self._eid] = nil
    scheduler:unscheduleScriptEntry(self._eid)
end

function EventAction:_parseMeta()
    local metaResult = self._meta:split(",")
    self._repeat = tonumber(metaResult[1])
    local delay = tonumber(metaResult[2])
    self._delay = delay == 0 and 0.01 or delay
    self._interval = tonumber(metaResult[3])
end

function EventAction:_isInfinite()
    return self._repeat < 0
end

return EventAction
