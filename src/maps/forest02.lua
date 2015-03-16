local MapLogic = require "MapLogic"
local Forest02 = class("Forest02", MapLogic)

function Forest02:postInit()
    self._vampireKillCount = 0
    self._vampireEscapeCount = 0
    self._second = 0
end

function Forest02:onUnitDisappear(unit, isDie)
    MapLogic.onUnitDisappear(self, unit, isDie)
    local uname = unit:getName()
    if uname == "thin_vampire" or uname == "fat_vampire" then
        if isDie then
            self._vampireKillCount = self._vampireKillCount + 1
        else
            print("vampire escape")
            self._vampireEscapeCount = self._vampireEscapeCount + 1
        end
    end
    if self._vampireEscapeCount > self._logic.custom_data.max_escape then
        self:onGameStateChanged(Boids.GameState.Lose)
    end
end

function Forest02:update(dt)
    self._second = self._second + dt
    if self._second >= self._logic.custom_data.time_limit then
        if self._vampireKillCount >= self._logic.custom_data.min_kill then
            self:onGameStateChanged(Boids.GameState.Win)
        else
            self:onGameStateChanged(Boids.GameState.Lose)
        end
    end
end

return Forest02
