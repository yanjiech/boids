require("Trace")
ResourceManager = ResourceManager or {}
local json = cjson
local Building = require "buildings/building"
ResourceManager.loadedAnimations = {}

function ResourceManager.init()
    ResourceManager.loadUnitData()
    ResourceManager.loadBulletData()
    ResourceManager.loadItemData()
    ResourceManager.loadCenterData()
    ResourceManager.loadBuildingData()
    cc.SpriteFrameCache:getInstance():addSpriteFrames("ui/map_common.plist", "ui/map_common.png")
    cc.SpriteFrameCache:getInstance():addSpriteFrames("ui/hero_avatars.plist", "ui/hero_avatars.png")
end

function ResourceManager.loadUnitData()
    local str = cc.FileUtils:getInstance():getStringFromFile("unit.json")
    ResourceManager.udata = json.decode(str)
    ResourceManager.loadUnitEffects()
end

function ResourceManager.loadBulletData()
    local str = cc.FileUtils:getInstance():getStringFromFile("bullet.json")
    ResourceManager.bdata = json.decode(str)
    cc.SpriteFrameCache:getInstance():addSpriteFrames("effects/bullets/bullets.plist", "effects/bullets/bullets.png")
end

function ResourceManager.loadCenterData()
    local str = cc.FileUtils:getInstance():getStringFromFile("vision_center.json")
    ResourceManager.centerData = json.decode(str)
end

function ResourceManager.loadBuildingData()
    local str = cc.FileUtils:getInstance():getStringFromFile("building.json")
    ResourceManager.buildingData = json.decode(str)
end

function ResourceManager.loadItemData()
    ResourceManager.itemData = json.decode(cc.FileUtils:getInstance():getStringFromFile("item.json"))
end

function ResourceManager.loadUnitEffects()
    cc.SpriteFrameCache:getInstance():addSpriteFrames("effects/fx_unit_common.plist", "effects/fx_unit_common.png")
    local animation = ResourceManager.createAnimation("unit_bleed", 5, 0.04, 1)
    cc.AnimationCache:getInstance():addAnimation(animation, Boids.CommonAnimation.UnitBleed)
end

function ResourceManager.loadBulletArmature(name, armaturetype)
    if armaturetype == "spine" then
        local path = string.format("effects/bullets/%s", name)
        ue.ArmatureManager:getInstance():loadArmatureData(path)
    elseif armaturetype == "cocos" then
        local path = string.format("effects/bullets/%s/skeleton.ExportJson", name)
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(path)
    end
end

function ResourceManager.purgeBulletArmature(name, armaturetype)
    if armaturetype == "spine" then
        local path = string.format("effects/bullets/%s", name)
        ue.ArmatureManager:getInstance():unloadArmatureData(path)
    elseif armaturetype == "cocos" then
        local path = string.format("effects/bullets/%s/skeleton.ExportJson", name)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(path)
    end
end

function ResourceManager.getUnitData(unitName)
    return ResourceManager.udata[unitName]
end

function ResourceManager.getBulletData(bulletName)
    return ResourceManager.bdata[bulletName]
end

function ResourceManager.armatureDoubleFace(armature)
    local data = ResourceManager.udata[armature]
    return data.double_face
end

function ResourceManager.loadUnitArmatures(armatures)
    for k, v in ipairs(armatures) do
        ResourceManager.loadUnitArmature(v)
    end
end

function ResourceManager.loadUnitArmature(armature)
    print("load armature " .. armature)
    if (ResourceManager.armatureDoubleFace(armature)) then
        ue.ArmatureManager:getInstance():loadArmatureData(G.pathForResource(armature, Boids.ResourceType.characterDoubleFace))
    else
        ue.ArmatureManager:getInstance():loadArmatureData(G.pathForResource(armature, Boids.ResourceType.characterFront))
        ue.ArmatureManager:getInstance():loadArmatureData(G.pathForResource(armature, Boids.ResourceType.characterBack))
    end
    local udata = ResourceManager.getUnitData(armature)
    if not udata.is_melee and udata.bullet_name ~= nil and udata.bullet_name ~= "" then
        local bdata = ResourceManager.getBulletData(udata.bullet_name)
        if bdata.body_type ~= nil then
            local name = string.format("%s_body", bdata.name)
            ResourceManager.loadBulletArmature(name, bdata.body_type)
        end
        if bdata.start_type ~= nil then
            local name = string.format("%s_start", bdata.name)
            ResourceManager.loadBulletArmature(name, bdata.start_type)
        end
        if bdata.hit_type ~= nil then
            local name = string.format("%s_hit", bdata.hit_name)
            ResourceManager.loadBulletArmature(name, bdata.hit_type)
        end
    end
end

function ResourceManager.purgeUnitArmatures(armatures)
    if (armatures ~= nil) then
        for k, v in ipairs(armatures) do
            ResourceManager.purgeUnitArmature(v)
        end
    end
end

function ResourceManager.purgeUnitArmature(armature)
    if (ResourceManager.armatureDoubleFace(armature)) then
        ue.ArmatureManager:getInstance():unloadArmatureData(G.pathForResource(armature, Boids.ResourceType.characterDoubleFace))
    else
        ue.ArmatureManager:getInstance():unloadArmatureData(G.pathForResource(armature, Boids.ResourceType.characterFront))
        ue.ArmatureManager:getInstance():unloadArmatureData(G.pathForResource(armature, Boids.ResourceType.characterBack))
    end
    local udata = ResourceManager.getUnitData(armature)
    if not udata.is_melee then
        local bdata = ResourceManager.getBulletData(udata.bullet_name)
        if bdata.body_type ~= nil then
            local name = string.format("%s_body", bdata.name)
            ResourceManager.purgeBulletArmature(name, bdata.body_type)
        end
        if bdata.start_type ~= nil then
            local name = string.format("%s_start", bdata.name)
            ResourceManager.purgeBulletArmature(name, bdata.start_type)
        end
        if bdata.hit_type ~= nil then
            local name = string.format("%s_hit", bdata.name)
            ResourceManager.purgeBulletArmature(name, bdata.hit_type)
        end
    end
end

function ResourceManager.loadBuildingArmature(name)
    local class = Building.type2Class(name)
    if class.Resources ~= nil then
        for _, res in pairs(class.Resources) do
            ue.ArmatureManager:getInstance():loadArmatureData(res)
        end
    end
end

function ResourceManager.purgeBuildingArmature(name)
    local class = Building.type2Class(name)
    if class.Resources ~= nil then
        for _, res in pairs(class.Resources) do
            ue.ArmatureManager:getInstance():unloadArmatureData(res)
        end
    end
end

function ResourceManager.loadMap(mapPath, pvp)
    local mapData = ue.MapData:new(mapPath)
    local metaStr = mapData:getMetaData()
    local logicJson = json.decode(metaStr)
    ResourceManager.loadMapData(mapData, logicJson, pvp)
end

function ResourceManager.loadMapData(mapData, logicJson, pvp)
    gTrace:beginTrace()
    mapData:loadImagesToCache()
    gTrace:trace("load_map_meta")
    local mapStr = mapData:getMapData()
    local units = logicJson.units
    ResourceManager.currentMapData = mapData
    if units == nil then units = {} end
    if pvp then
        print("pvp load units resource")
        G.concatTable(units, PlayerInfo.getPlayerUnitsNoMatterCount())
        printtable(units)
    else
        print("pve load resource")
        G.concatTable(units, PlayerInfo.getPlayerUnits())
    end
    local collides = {30, 40}
    ResourceManager.loadUnitArmatures(units)
    mapData.units = units
    gTrace:trace("load_Armatures")
    local ret = cAI:parseMap(mapStr,collides)
    if (not ret) then
        ue.LuaUtils:logf("parse map error!")
    else
        ue.LuaUtils:logf("parse map finish!")
    end
    local tmxMap = mapData:generateTiledMapWithFlags(3)
    mapData.buildings = {}
    local visionObjects = tmxMap:getObjectGroup("vision"):getObjects()
    for _, object in ipairs(visionObjects) do
        local t = object["type"]
        if t ~= nil and mapData.buildings[t] ~= nil then
            ResourceManager.loadBuildingArmature(object["type"])
            mapData.buildings[t] = true
        end
    end
    return logicJson, tmxMap
end

function ResourceManager.purgeMap(mapPath)
    print("call purge")
    if ResourceManager.currentMapData then
        local data = ResourceManager.currentMapData
        ResourceManager.purgeUnitArmatures(data.units)
        for k, _ in pairs(data.buildings) do
            ResourceManager.purgeBulletArmature(k)
        end
        data:removeImagesFromCache()
        ResourceManager.currentMapData = nil
        cAI:releaseCurrentMap()
        PlayerInfo.ret = nil
    end
end

function ResourceManager.createAnimation(name, count, delay, loops)
    local frames = {}
    local frameCache = cc.SpriteFrameCache:getInstance()
    local firstFrameName = nil
    for i = 1, count do
        local frameName = string.format("%s_%02d.png", name, i)
        if (i == 1) then firstFrameName = frameName end
        frames[i] = frameCache:getSpriteFrame(frameName)
    end
    ResourceManager.loadedAnimations[name] = {firstFrame = firstFrameName}
    return cc.Animation:createWithSpriteFrames(frames, delay, loops)
end

function ResourceManager.getAnimationSprite(name)
    local firstFrameName = ResourceManager.loadedAnimations[name].firstFrame
    return cc.Sprite:createWithSpriteFrameName(firstFrameName)
end

return ResourceManager
