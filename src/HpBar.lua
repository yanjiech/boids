local HpBar = class("HpBar", function() return cc.Node:create() end)

local hmargin = 1
local vmargin = 1

local backgroudColor = cc.c4f(0, 0, 0, 1)
local greenColor = cc.c4f(0, 1, 47/255.0, 1)
local yellowColor = cc.c4f(1, 1, 0, 1)
local redColor = cc.c4f(1, 0, 0, 1)

local backgroudTextureName = "HpBar_backgroundTextueName"
local greenTextureName = "HpBar_greenTextureName"
local yellowTextureName = "HpBar_yellowTextureName"
local redTextureName = "HpBar_redTextureName"

local yellowPercentage = 50
local redPercentage = 20

function HpBar:ctor(...)
    local arg = {...}
    local size = arg[2]
    assert(#arg == 2, "invalid arg count: " .. #arg .. ", require 2")
    local innerSize = cc.size(size.width - 2 * hmargin, size.height - 2 * vmargin)
    self:setContentSize(size)
    self:setAnchorPoint(cc.p(0.5, 0.5))

    self._bgProgressTimer = self:_addProgressTimer(backgroudColor, size, backgroudTextureName)
    self._redProgressTimer = self:_addProgressTimer(redColor, innerSize, redTextureName)
    self._yellowProgressTimer = self:_addProgressTimer(yellowColor, innerSize, yellowTextureName)
    self._greenProgressTimer = self:_addProgressTimer(greenColor, innerSize, greenTextureName)
    self:setPercentage(1)
end

function HpBar:setPercentage(percentage)
    local p = self:_normalizePercentage(percentage)
    local pt = self:_currentProgressTimer(p)
    pt:setPercentage(p)
end

function HpBar:_currentProgressTimer(percentage)
    if (percentage <= redPercentage) then
        self._greenProgressTimer:setVisible(false)
        self._yellowProgressTimer:setVisible(false)
        self._redProgressTimer:setVisible(true)
        return self._redProgressTimer
    elseif (percentage <= yellowPercentage) then
        self._greenProgressTimer:setVisible(false)
        self._yellowProgressTimer:setVisible(true)
        self._redProgressTimer:setVisible(false)
        return self._yellowProgressTimer
    else
        self._greenProgressTimer:setVisible(true)
        self._yellowProgressTimer:setVisible(false)
        self._redProgressTimer:setVisible(false)
        return self._greenProgressTimer
    end
end

-- private

function HpBar:_normalizePercentage(percentage)
    percentage = percentage * 100
    return percentage > 100 and 100 or (percentage > 0 and percentage or 0)
end

function HpBar:_addProgressTimer(color, size, textureName)
    local textureCache = cc.Director:getInstance():getTextureCache()
    local tex = textureCache:getTextureForKey(textureName)
    if (tex == nil) then
        tex = G.textureWithColor(color, cc.size(1, 1))
        textureCache:addTexture(tex, textureName)
    end
    local sprite = cc.Sprite:createWithTexture(tex)
    local pt = cc.ProgressTimer:create(sprite)
    pt:setType(cc.PROGRESS_TIMER_TYPE_BAR)
    pt:setMidpoint(cc.p(0, 0.5))
    pt:setBarChangeRate(cc.p(1, 0))
    pt:setPercentage(100)
    pt:setNormalizedPosition(cc.p(0.5, 0.5))
    pt:setScale(size.width, size.height)
    self:addChild(pt)
    return pt
end

return HpBar
