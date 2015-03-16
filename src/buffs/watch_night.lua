local WatchNight = class("WatchNight", nil)
local UnitNodeComponet = require "UnitNodeComponet"

function WatchNight:ctor(...)
end

function WatchNight:onAttach(logicUnit)
    local unitNode = logicUnit._unitNode
    local stencil = cc.Sprite:createWithSpriteFrameName("light_mask.png")
    unitNode:addComponet(stencil, UnitNodeComponet.LayerType.Stencil, "buff_watch_night")
end

function WatchNight:onDetach(logicUnit)
    local unitNode = logicUnit._unitNode
    unitNode:removeComponent("buff_watch_night")
end

function WatchNight:update(dt)
end

function WatchNight:shouldRecycle()
    return false
end

return WatchNight
