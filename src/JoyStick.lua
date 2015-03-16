local JoyStick = class("JoyStick", function() return cc.Node:create() end)

function JoyStick:ctor( ... )
    self._isFollow = true
    self._bgSprite = cc.Sprite:createWithSpriteFrameName("joystick_back.png")
    self._fgSprite = cc.Sprite:createWithSpriteFrameName("joystick_front.png")
    self._radius = self._bgSprite:getContentSize().width / 2
    self:addChild(self._bgSprite)
    self:addChild(self._fgSprite)
    self:setVisible(false)
end

function JoyStick:onTouchBegan(touch, event)
    local pos = self:getParent():convertTouchToNodeSpace(touch)
    self:setPosition(pos)
    self._fgSprite:setPosition(0, 0)
end

function JoyStick:getDirection()
    local pos = G.getPosition(self._fgSprite)
    if (cc.pFuzzyEqual(pos, cc.p(0, 0), 1)) then
        return cc.p(0, 0)
    else
        return cc.pNormalize(pos)
    end
end

function JoyStick:onTouchMoved(touch, event)
    self:setVisible(true)
    local pos = self:convertTouchToNodeSpace(touch)
    if (cc.pGetLength(pos) > self._radius) then
        local direction = cc.pNormalize(pos)
        self._fgSprite:setPosition(cc.pMul(direction, self._radius))
    else
        self._fgSprite:setPosition(pos)
    end
end

function JoyStick:onTouchEnded(touch, event)
    self:setVisible(false)
end

return JoyStick
