local PatrolUnit = class("PatrolUnit", require( "LogicUnit" ) )

function PatrolUnit:init(...)    
    PatrolUnit.super.init( self, ... )
    self._map_script = self._battleLayer._logic._map_script    
end

function PatrolUnit:beforeBehavior()
    if not self.c_unit:hasPath() then --当前没有要走的路，那么就开始巡逻逻辑
        self._speed_scale = Boids.wanderSpeedScale
        self._patrol_dest = self._map_script:getClosestPatrolPos(self:getPos())
        self:_findPathWithDestRadius(self._map_script:getPatrolPt(self._patrol_dest), 60, 3000)
    end
end

function PatrolUnit:pathEnd()
    self.c_unit:resetPath()
    local new_patrol_dest = self._map_script:getNextPatrolPos(self._last_patrol, self._patrol_dest)
    self._last_patrol = self._patrol_dest
    self._patrol_dest = new_patrol_dest
    self:_findPathWithDestRadius(self._map_script:getPatrolPt(new_patrol_dest), 60, 3000)
end

return PatrolUnit