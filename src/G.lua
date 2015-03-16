G = G or {}

local SceneBase = require("SceneBase")
local BattleLayer = require("BattleLayer")
local LevelChooseLayer = require "LevelChooseLayer"

function G.init( ... )
    G.winSize = cc.Director:getInstance():getWinSize()
    G.visibleSize = cc.Director:getInstance():getVisibleSize()
    G.targetPlatform = cc.Application:getInstance():getTargetPlatform()
end

function G.pathForResource(name, type)
    if (type == Boids.ResourceType.characterFront) then
        return string.format("characters/%s/%s_f", name, name)
    elseif (type == Boids.ResourceType.characterBack) then
        return string.format("characters/%s/%s_b", name, name)
    elseif (type == Boids.ResourceType.mapVision) then
        return string.format("maps/vision/%s.png", name)
    elseif (type == Boids.ResourceType.ui) then
        return string.format("ui/%s.png", name)
    elseif (type == Boids.ResourceType.characterDoubleFace) then
        return string.format("characters/%s", name)
    elseif (type == Boids.ResourceType.spineEffect) then
        return string.format("effects/%s", name )
    end
end

function G.getPosition(node)
    local x, y = node:getPosition()
    return cc.p(x, y)
end

function G.center(rect)
    return cc.p(cc.rectGetMidX(rect), cc.rectGetMidY(rect))
end

function G.runLevelChooseScene()
    local levelLayer = LevelChooseLayer:new()
    G.runLayerInScene(levelLayer)
end

function G.runeBattleScene(path, mapData, logicJson, pvp)
    local logicJson, tmxMap = ResourceManager.loadMapData(mapData, logicJson, pvp)
    local battleLayer = BattleLayer:new()
    G.runLayerInScene(battleLayer)
    battleLayer:initWithData(logicJson, tmxMap, path, pvp)
end

function G.runLayerInScene(layer)
    cc.Director:getInstance():resume()
    local scene = SceneBase:new()
    scene:addChild(layer)
    if cc.Director:getInstance():getRunningScene() then
        cc.Director:getInstance():replaceScene(scene)
    else
        cc.Director:getInstance():runWithScene(scene)
    end
end

function G.shallowcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in pairs(orig) do
            copy[orig_key] = orig_value
        end
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

function G.runBattleSceneWithMapPath(mapPath)
    local battleLayer = BattleLayer:new()
    battleLayer:initWithMapPath(mapPath)
    G.runLayerInScene(battleLayer)
end

function G.isDesktopPlatform()
    return ue.LuaPlatform:isPlatformMac() or ue.LuaPlatform:isPlatformWin32()
end

function G.concatTable(t1, t2)
    for i=1,#t2 do
        t1[#t1+1] = t2[i]
    end
    return t1
end

function G.textureWithColor(color4F, size)
    local rt = cc.RenderTexture:create(size.width, size.height)
    rt:beginWithClear(color4F.r, color4F.g, color4F.b, color4F.a)
    rt:endToLua()
    return rt:getSprite():getTexture()
end

function G.spriteWithColor(color4F, size)
    return cc.Sprite:createWithTexture(G.textureWithColor(color4F, size))
end

function G.bezierTo(t, p1, cp1, cp2, p2)
    local x = p1.x * math.pow((1 - t), 3) + 3 * t * cp1.x * math.pow((1 - t), 2) + 3 * cp2.x * math.pow(t, 2) * (1 - t) + p2.x * math.pow(t, 3);
    local y = p1.y * math.pow((1 - t), 3) + 3 * t * cp1.y * math.pow((1 - t), 2) + 3 * cp2.y * math.pow(t, 2) * (1 - t) + p2.y * math.pow(t, 3);
    return cc.p(x, y)
end

function G.setFrame(rate)
    if rate then
        Boids.frameRate = rate
    end
    if not Boids.frameRate then
        Boids.frameRate = 30
    end    
    print("FrameRate:", Boids.frameRate)
    Boids.dt = 1.0 / Boids.frameRate
    local director = cc.Director:getInstance()
    director:setAnimationInterval(Boids.dt)
end

return G
