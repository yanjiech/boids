local Building = class("Building", function() return cc.Node:create() end)

function Building:init(data)
    self._data = data
    self._normal = self:spriteOfState("normal")
    self._change = self:spriteOfState("unnormal")
    self:setContentSize(cc.size(self._normal:getTextureRect().width, self._normal:getTextureRect().height))
    self._normal:setNormalizedPosition(cc.p(0.5, 0.5))
    self._change:setNormalizedPosition(cc.p(0.5, 0.5))
    self:addChild(self._normal)
    self:addChild(self._change)
end

function Building:setState(state)
    if state == "normal" or state == "disable" then
        self._normal:setVisible(true)
        self._change:setVisible(false)
        if self._data.collide ~= nil then
            cAI:addCollidablePolygon(self._data.collide)
        end
    elseif state == "unnormal" then
        self._normal:setVisible(false)
        self._change:setVisible(true)
        if  self._data.collide ~= nil then
            cAI:removeCollidablePolygon(self._data.collide)
        end
    end
end

function Building:spriteOfState(state)
    local name = string.format("%s_%s.png", self._data.res, state)
    local spriteFrame = cc.SpriteFrameCache:getInstance():getSpriteFrame(name)
    if spriteFrame ~= nil then
        return cc.Sprite:createWithSpriteFrame(spriteFrame)
    else
        return cc.Sprite:createWithSpriteFrameName(string.format("%s.png", self._data.res))
    end
end

return Building
