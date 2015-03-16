local SkeletonKingSkillAI = class( "SkeletonKingSkillAI", require( "ai/SkillAI" ) )

function SkeletonKingSkillAI:ctor( ... )
    SkeletonKingSkillAI.super.ctor( self, ... )
end

function SkeletonKingSkillAI:init( ... )
    --print( "skeleton king ai init" )
    local args = { ... }
    self._logic_unit = args[1]
    self._accumulator = 0
    self._min_interval = 2.0
    self._max_interval = 5.0
    self._interval = 0
    self:refreshInterval()
end

function SkeletonKingSkillAI:behave( delta )
    local camp = self._logic_unit:getCamp()
    local target = self._logic_unit:getTarget()
    if camp == Boids.UnitType.Enemy and self._logic_unit:canUseSkill() then
        self._accumulator = self._accumulator + delta
        local skills = self._logic_unit:getSkills()
        for i,v in pairs( skills ) do
            if self._logic_unit:isSkillReadyByName( v ) and self:isTargetInRange( v ) then
                if ( v == "BonesRain" and self._accumulator > self._interval ) or ( v == "BonesSacrifice" and self._logic_unit._currentHP / self._logic_unit._fullHP < 0.5 ) then
                    self._logic_unit:useSkillById( i, cc.p( 0, 0 ) )
                    self._accumulator = 0
                    self:refreshInterval()
                    return true
                end
            end
        end
    end
    return false

end

return SkeletonKingSkillAI
