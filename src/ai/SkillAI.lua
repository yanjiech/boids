local SkillAI = class( "SkillAI", nil ) 
local math = require( "math" )
local cAI = ue.AI:getInstance()

function SkillAI:ctor( ... )

end

function SkillAI:init( ... )
    local args = { ... }
    self._logic_unit = args[1]
    self._accumulator = 0
    self._min_interval = 1.0
    self._max_interval = 5.0
    self._interval = 0
    self:refreshInterval()
end

function SkillAI:refreshInterval()
    ue.LuaUtils:logr(string.format("skillAI refresh_interval(%s)", self._logic_unit:getName()), true)
    self._interval = self._min_interval + ue.LuaUtils:randomFloat() * ( self._max_interval - self._min_interval )
end

function SkillAI:isTargetInRange( skill_name )
    local range = SkillConf.skill[skill_name]["range"]
    local target = self._logic_unit:getTarget() 
    if target then
        if range == 0 then
            return true
        end
        local pos = self._logic_unit:getPos()
        local tpos = target:getPos()
        if cc.pDistanceSQ( pos, tpos ) < range * range then
            return true
        end
    end
    return false
end
    

function SkillAI:behave( delta )
    --print( "skill ai update:", self._logic_unit:getData( "name" ) )
    self._accumulator = self._accumulator + delta
    local camp = self._logic_unit:getCamp()
    local target = self._logic_unit:getTarget()
    if camp == Boids.UnitType.Enemy and self._logic_unit:canUseSkill() then
        local skills = self._logic_unit:getSkills()
        for i,v in pairs( skills ) do
            if self._logic_unit:isSkillReadyByName( v ) and self:isTargetInRange( v ) and self._accumulator > self._interval then
                self._logic_unit:useSkillById( i, cc.p( 0, 0 ) )
                self._accumulator = 0
                self:refreshInterval()
                return true
            end
        end
    end
    return false
end

return SkillAI

