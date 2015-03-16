local BuildingData = { camp = Boids.UnitType.Enemy }

function BuildingData.new(data, centerPos, source)
    local mdata = {}
    if data.data ~= nil then
        mdata = ResourceManager.buildingData[data.data]
    end
    local metaTable = setmetatable(mdata, { __index = BuildingData })
    local ret = setmetatable(data, { __index = metaTable } )
    ret.centerPos = centerPos
    ret.res = source
    return ret
end

function BuildingData:getCenterPos()
    return cc.p(self.centerPos.x + self.x, self.centerPos.y + self.y)
end

function BuildingData:getPos()
    return cc.p(self.x, self.y)
end

return BuildingData
