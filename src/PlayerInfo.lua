PlayerInfo = PlayerInfo or {}
local json = require "json"

PlayerInfo.team = { --PvP这边(暂时)需求team里全都列着，以确保资源都load了；PvE不上的英雄可以把count置0
{name = "saber", count = 0},
--{name = "gandalf", count = 1},
{name = "panda", count = 1},
{name = "vanhelsing", count = 0},
{name = "dracula", count = 0},
{name = "enchantress", count = 0},
{name = "zeus", count = 0},
--{name = "rattletrap", count = 1 },
--{name = "women_wizard", count = 1}
}

function PlayerInfo.getSpecificPlayerUnits()
    local ret = {}

    local all_heroes_count = #PlayerInfo.team
    local hero_pick_count = 4
    print("all heroes:", all_heroes_count, "pick:", hero_pick_count)
    local r = {}
    for i = 0, hero_pick_count - 1 do
        table.insert(r, ue.LuaUtils:randomNumber2(all_heroes_count - hero_pick_count + 1))
    end
    table.sort(r)
    for i = 0, hero_pick_count - 1 do
        print("pick index:", i + r[i + 1])
        table.insert(ret, PlayerInfo.team[i + r[i + 1] ].name)
    end

    --[[ret = {}
    for _, v in ipairs(PlayerInfo.team) do
        if v.count ~= 0 then
            ret[#ret + 1] = v.name
        end
    end]]

    return ret
end

function PlayerInfo.getPlayerUnits()
	if not PlayerInfo.ret then
        PlayerInfo.ret = {}
        local all_heroes_count = #PlayerInfo.team
        local hero_pick_count = 4
        print("all heroes:", all_heroes_count, "pick:", hero_pick_count)
        local r = {}
        for i = 0, hero_pick_count - 1 do
            table.insert(r, ue.LuaUtils:randomNumber2(all_heroes_count - hero_pick_count + 1))
        end
        table.sort(r)
        for i = 0, hero_pick_count - 1 do
            print("pick index:", i + r[i + 1])
            table.insert(PlayerInfo.ret, PlayerInfo.team[i + r[i + 1] ].name)
        end
    end
    return PlayerInfo.ret
end

function PlayerInfo.getPlayerUnitsNoMatterCount()
    local ret = {}
    for _, v in ipairs(PlayerInfo.team) do
        ret[#ret + 1] = v.name
    end
    return ret
end

return PlayerInfo
