local ViceSkillAI = class( "ViceSkillAI", nil )
local math = require( "math" )
local cAI = ue.AI:getInstance()

function ViceSkillAI:ctor( ... )
    local args = { ... }
    self._logic_unit = args[2]
    self._chance = 0.1
    self._accumulator = 0
    self._interval = 0.5
end

function ViceSkillAI:behave( delta )
    self._accumulator = self._accumulator + delta
    if self._accumulator > self._interval then
        self._accumulator = 0
        local target = self._logic_unit:getTarget()
        if target ~= nil and target._category == "charactor" and self._logic_unit:canUseSkill() then
            local skills = self._logic_unit:getSkills()
            for i,v in pairs( skills ) do
                if i ~= 1 then
                    if self._logic_unit:isSkillReadyByName( v ) and ue.LuaUtils:randomFloat() < self._chance then
                        self._logic_unit:useSkillById( i, cc.p( 0, 0 ) )
                        break
                    end
                end
            end
        end
    end
end

return ViceSkillAI
