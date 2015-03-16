local UIShadeNode = class( "UIShadeNode", function() return cc.ClippingNode:create() end )

--[[
param1 self
param2 shade image if exist, should be a Node, such as LayerColor
]]--
function UIShadeNode:ctor( ... )
    local args = { ... }
    local argc = #args 
    self._stencil = cc.Node:create()
    self:setStencil( self._stencil )
    self:setInverted( true )
    self:setAlphaThreshold( 0.4 )
    if argc >= 2 then
        self:addChild( args[2] )
        self._bgLayer = args[2]
    end
end

function UIShadeNode:addStencilComponent( component, zorder, tag )
    self._stencil:addChild( component, zorder, tag )
end

function UIShadeNode:removeStencilComponent( component )
    self._stencil:removeChild( component, false )
end

function UIShadeNode:removeStencilComponentByTag( tag )
    self._stencil:removeChildByTag( tag );
end

function UIShadeNode:getStencilComponentByTag( tag )
    return self.stencil:getChildByTag( tag )
end

function UIShadeNode:getBackgroundLayer()
    return self._bgLayer
end

return UIShadeNode
