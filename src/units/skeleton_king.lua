local SkeletonKing = class( "SkeletonKing", require( "LogicUnit" ) )
local SkeletonKingSkillAI = require( "ai/SkeletonKingSkillAI" )

function SkeletonKing:ctor( ... )
    SkeletonKing.super.ctor( self, ... )
end

function SkeletonKing:init( ... )
    self.super.init( self, ... )
    self._skill_ai = SkeletonKingSkillAI:new()
    self._skill_ai:init( self )
end

return SkeletonKing
