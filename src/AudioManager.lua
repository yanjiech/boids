AudioManager = AudioManager or {}

--[[
params:
1 name
2 dir
3 loop
]]--
function AudioManager.playEffect( ... )
    local args = { ... }
    local argc = #args
    local path = args[1]
    return true
--[[
    if argc > 1 then
        path = args[2] .. "/" .. path
    end
    path = "audio/" .. path
    local loop = argc > 2 and args[3] or false
    local fullpath = path .. ".wav"
    if not cc.FileUtils:getInstance():isFileExist( fullpath ) then
       fullpath = path .. ".mp3" 
    end
    return cc.SimpleAudioEngine:getInstance():playEffect( fullpath, loop )]]
end

function AudioManager.playMusic( ... )
    local args = { ... }
    local argc = #args
    local path = args[1]
    if argc > 1 then
        path = args[2] .. "/" .. path
    end
    path = "audio/" .. path
    local fullpath = path .. ".mp3"
    local loop = argc > 2 and args[3] or false
    if not cc.FileUtils:getInstance():isFileExist( fullpath ) then
        fullpath = path .. ".wav"
    end
    cc.SimpleAudioEngine:getInstance():playMusic( fullpath, loop )
end

function AudioManager.stopMusic()
    cc.SimpleAudioEngine:getInstance():stopMusic()
end 
