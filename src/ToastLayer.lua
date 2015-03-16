local ToastLayer = class("ToastLayer", function() return cc.Layer:create() end)

function ToastLayer:ctor(...)
    self._label = cc.Label:createWithSystemFont("", "Helvetica", 100)
    self._label:setNormalizedPosition(cc.p(0.5, 0.5))
    self:addChild(self._label)
    self._label:setVisible(false)
end

function ToastLayer:showToast(toast)
    self._label:setVisible(true)
    self._label:setOpacity(100)
    self._label:setString(toast)
    local fade = cc.FadeTo:create(2, 0)
    self._label:runAction(fade)
end

return ToastLayer
