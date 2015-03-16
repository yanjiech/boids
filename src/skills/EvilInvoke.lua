local EvilInvoke = class( "EvilInvoke", function() return cc.Node:create() end )
local LogicUnit = require( "LogicUnit" )
function EvilInvoke:ctor( ... )
    self._battleLayer = nil
    self._radius = SkillConf.skill["EvilInvoke"]["radius"]
    self._count = SkillConf.skill["EvilInvoke"]["count"]
    self._duration = SkillConf.skill["EvilInvoke"]["duration"]
    self._accumulator = 0
end

function EvilInvoke:activate( ... )
    local function update( delta )
        self._accumulator = self._accumulator + delta
        if self._accumulator >= self._duration then
            self:unactivate()
        end
    end

    local arg = { ... }
    self._battle_layer = arg[1]
    local src_unit = arg[2]
    self._src_id = src_unit:getId()
    local pos = src_unit:getPos()
    local area = cc.rect( pos.x - self._radius, pos.y - self._radius, self._radius * 2, self._radius * 2 )
    local units = {}
    for i = 1, self._count do
         local unit = LogicUnit:new()
         unit:init( "white_ghost", src_unit:getCamp(), src_unit:getData( "guard_radius" ), false, "", self._battle_layer )
         table.insert( units, unit )
    end
    self._battle_layer:deployUnits( units, { ["name"] = "evil_invoke_pos", ["rect"] = area }, src_unit._sight_group )
    self._battle_layer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )

    return true
end

function EvilInvoke:unactivate()
    local src_unit = self._battle_layer:getUnitById( self._src_id )
    if src_unit then
        src_unit:endInvoke()
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return EvilInvoke
