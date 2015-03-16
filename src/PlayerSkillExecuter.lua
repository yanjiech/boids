PlayerSkill = PlayerSkill or { data = {} }

function PlayerSkill.execute()
    for unit_id, params in pairs(PlayerSkill.data) do        
        ue.LuaUtils:logr(string.format("exeute skill: %d", unit_id))
        --假设外面赋值的时候可行条件如CD啊等等都判过了，这里就直接执行就行了
        local unit = AI._battleLayer:getUnitById(unit_id)
        if unit then
            unit:useSkillById( 1, params["dir"], params["range"] )
        else
            print("to execute skill but unit not found:", unit_id)
        end
    end
    PlayerSkill.data = {}
end
