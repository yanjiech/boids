local MapToken = class("MapToken", function() return cc.Node:create() end)

function MapToken:ctor(...)
    local arg = {...}
    self._arrow = cc.Sprite:createWithSpriteFrameName("token_arrow.png")
    self._token = cc.Sprite:createWithSpriteFrameName(arg[2])
    local arrowRect = self._arrow:getTextureRect()
    local tokenRect = self._token:getTextureRect()
    self:addChild(self._token)
    self:addChild(self._arrow)
    self:setContentSize(cc.size(tokenRect.width + arrowRect.width * 2, tokenRect.height + arrowRect.width * 2))
end

function MapToken:setArrowPosition(deltaX, deltaY)
    local r = self:getContentSize().width / 2 - self._arrow:getTextureRect().width / 2
    local angle = math.atan2(deltaY, deltaX)
    local x = math.cos(angle) * r
    local y = math.sin(angle) * r
    self._arrow:setRotation(-angle * 180 / math.pi)
    self._arrow:setPosition(cc.p(x, y))
end

return MapToken
