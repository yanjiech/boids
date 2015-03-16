local UnitNodeComponet = class("UnitNodeComponet", nil)

UnitNodeComponet.LayerType = {
    SelfBelow = "self_below",
    SelfAbove = "self_above",
    Object = "object",
    OverObject = "over_object",
    Stencil = "stencil",
    Float = "float"
}

function UnitNodeComponet:ctor(...)
    local function onAnimationEnded( event )
        self._should_recycle = true
    end

    local arg = {...}
    self._node = arg[2]
    self._layerType = arg[3]
    self._name = arg[4]
    self._auto_remove = arg[5]
    if self._auto_remove then
        self._node:registerSpineEventHandler( onAnimationEnded, sp.EventType.ANIMATION_END )    
    end
    self._should_recycle = false
end

function UnitNodeComponet:getName()
    return self._name
end

function UnitNodeComponet:getLayerType()
    return self._layerType
end

function UnitNodeComponet:getNode()
    return self._node
end

return UnitNodeComponet
