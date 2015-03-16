-- CC_USE_DEPRECATED_API = true
cc.FileUtils:getInstance():addSearchPath("bin")
cc.FileUtils:getInstance():addSearchPath("src")
cc.FileUtils:getInstance():addSearchPath("res")
cc.FileUtils:getInstance():addSearchPath("res/BoidsEditor/res")
cc.FileUtils:getInstance():addSearchPath("src/units")
cc.FileUtils:getInstance():addSearchPath("res/BoidsUI/res")
cc.FileUtils:getInstance():addSearchPath("res/maps/map_images/buildings")
cc.FileUtils:getInstance():addSearchPath("res/maps/map_images/decorations")
cc.FileUtils:getInstance():addSearchPath("res/maps/map_images/obstacles")
cc.FileUtils:getInstance():addSearchPath("res/maps/map_images/terrains")

--local lib_path = cc.FileUtils:getInstance():fullPathForFilename("lua_libs")
--local extra_path = string.format(";%s/?.so", lib_path)
--package.cpath = package.cpath .. extra_path

require "cocos.init"
local BattleLayer = require "BattleLayer"
local LevelChooseLayer = require "LevelChooseLayer"

-- cclog
cclog = function(...)
    print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
    return msg
end

local function initScene( ... )
    ue.Net:initialize(Boids.serverIP, Boids.serverPort)
    G.runLevelChooseScene()
end

local function main()
    collectgarbage("collect")
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    -- initialize director
    local director = cc.Director:getInstance()
    local glview = director:getOpenGLView()
    if nil == glview then
        glview = cc.GLViewImpl:createWithRect( "Boids", cc.rect( 0, 0, 1728, 972 ) )   --in order to show task bar, multiply factor 0.9
        director:setOpenGLView(glview)
    end

    glview:setDesignResolutionSize(1920, 1080, cc.ResolutionPolicy.NO_BORDER)

    require("BoidsConstants")
    if (Boids.config == Boids.BuildConfig.debug) then
        require("BoidsDebug")
    end
    require("PlayerInfo")
    require("extensions")
    require("EnemyInfo")
    require("G").init()
    require("ResourceManager").init()
    require("MathExtend")
    require("DamageCalculate")
    require("Helper")
    require("skills/SkillConf")
    require( "AudioManager" )

    --turn on display FPS
    if (Boids.config == Boids.BuildConfig.debug) then director:setDisplayStats(true) end

    --set FPS. the default value is 1.0/60 if you don't call this
    G.setFrame()
    
    ---------------
    if G.isDesktopPlatform() and Boids.inEditMode then
        local function onExit()
            print("onExit")
            Boids.inEditMode = false
            initScene()
        end
        Boids.inEditMode = true
        ue.EditMode:getInstance():enterMain(onExit)
    else
        initScene()
    end
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end
