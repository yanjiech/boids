local InCage = class("InCage", nil)
local UnitNodeComponet = require "UnitNodeComponet"

function InCage:ctor(...)
end

function InCage:onAttach(logicUnit)
    local unitNode = logicUnit._unitNode
    local cageBottom = cc.Sprite:createWithSpriteFrameName("cage_bottom.png")
    unitNode:addComponet(cageBottom, UnitNodeComponet.LayerType.SelfBelow, "buff_cage_bottom")
    local cageTop = cc.Sprite:createWithSpriteFrameName("cage_top.png")
    unitNode:addComponet(cageTop, UnitNodeComponet.LayerType.SelfAbove, "buff_cage_top")
end

function InCage:onDetach(logicUnit)
    local unitNode = logicUnit._unitNode
    unitNode:removeComponent("buff_cage_bottom")
    unitNode:removeComponent("buff_cage_top")
end

function InCage:update(dt)
end

function InCage:shouldRecycle()
    return false
end

return InCage
