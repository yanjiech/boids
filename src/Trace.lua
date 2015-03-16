local Trace = class("Trace", nil)
gTrace = ue.Trace:new()

function Trace:ctor(...)    
    self:beginTrace()
    self:beginAccumulate()
    
    self.accumulated_time = {}
end

function Trace:beginTrace()
    self._time = os.clock()
end

function Trace:trace(title)
    local time_new = os.clock()
    local t = time_new - self._time
    self._time = time_new
        ue.LuaUtils:logf( string.format( "[%s]: %f s", title, t ) )
    --print('[', title, ']: ', t, 's')
end

function Trace:beginAccumulate()
    self._accumulate_time = os.clock()
end

function Trace:accumulate(title)
    local time_new = os.clock()
    local t = time_new - self._accumulate_time
    self._accumulate_time = time_new
    if (self.accumulated_time[title] == nil) then
        self.accumulated_time[title] = t
    else
        self.accumulated_time[title] = self.accumulated_time[title] + t
    end
end

function Trace:printStatistics()
    for k,v in pairs(self.accumulated_time) do
            ue.LuaUtils:logf( string.format( "{%s}:%f s", k, v ) )
        --print('{', k, '}: ', v, 's')
    end
end

return Trace
