local math = require 'math'

DamageCalculate = DamageCalculate or {} 

function DamageCalculate.doesHit( hit, dodge, atklv, deflv )
    local chance = 1.0 - dodge / ( hit + dodge ) * 2 * deflv / ( deflv + atklv )
    ue.LuaUtils:logr("doesHit", true)
    local rand = ue.LuaUtils:randomFloat()
    return rand <= chance
end 

function DamageCalculate.doesCritical( cri, ten, atklv, deflv )
    local chance = 0
    if cri > ten then
        chance = ( cri - ten ) / ( ten * 2 + 1 ) * 2 * atklv / ( 80 + atklv )
    else
        chance = 1.0 / ( ten * 2 * ( 80 - deflv ) )
    end
    ue.LuaUtils:logr("doesCritical", true)
    local rand = ue.LuaUtils:randomFloat()
    return rand <= chance
end 

--[[
args:
1 atk
2 def
3 doescri
4 negl
5 neglp
]]--
function DamageCalculate.calculateDamage( ... )
    local args = { ... }
    local argc = #args
    local atk = args[1]
    local def = args[2]
    local negl = 0
    local neglp = 0
    if argc >= 3 and args[3] then
        atk = atk * 2.0
    end
    if argc >= 4 then
        negl = args[4]
    end
    if argc >= 5 then
        neglp = args[4]
    end 
    local armor = DamageCalculate.calculateArmor( def, negl, neglp )
    return ( atk - 0.1 * armor ) * DamageCalculate.calculateResistance( armor )
end

function DamageCalculate.calculateArmor( def, negl, neglp )
    return ( def - negl ) * ( 1 - neglp )
end 

function DamageCalculate.calculateResistance( armor )
    return 1.0 - math.pow( armor, 0.4 ) / 100.0  
end

--[[
return
0 miss
1 hit
2 critical
]]--
function DamageCalculate.settleNormalAttack( atkerData, target, srcId )
    local ret = 0
    local damage = 0
    local atklv = atkerData.lv
    local deflv = target:getData( "lv" )
    if DamageCalculate.doesHit( atkerData.hit, target:getData( "dodge" ), atklv, deflv ) then
        local atk = atkerData.atk
        local def = target:getData( "def" )
        if DamageCalculate.doesCritical( atkerData.cri, target:getData( "ten" ), atklv, deflv ) then 
            ret = 2
            atk = atk * 2
        else
            ret = 1
        end
        damage = DamageCalculate.calculateDamage( atk, def, doescri )
        target:takeDamage( damage, srcId, ret == 2)
    end

    return ret
end
