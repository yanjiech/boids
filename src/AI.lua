AI = AI or {}

cAI = ue.AI:getInstance()

require("MathExtend")
local LogicUnit = require("LogicUnit")
local LogicTarget = require("LogicTarget")
require("BoidsConstants")

function AI.init()
    Boids.AI.CATCHUP_LEVEL2_DIST_MORE = Boids.AI.CATCHUP_LEVEL2_DIST - Boids.AI.CATCHUP_LEVEL1_DIST
    Boids.AI.CATCHUP_LEVEL3_DIST_MORE = Boids.AI.CATCHUP_LEVEL3_DIST - Boids.AI.CATCHUP_LEVEL2_DIST
    cAI:initialize(Boids.AI) --将各个常数传进去
    
    --一些性能测试--
    --[[local testcase = 300000
    local ai = ue.AI:getInstance();
    
    gTrace:beginTrace()
    local pt = cc.p(ue.LuaUtils:randomNumber(1000), ue.LuaUtils:randomNumber(1000))
    local sum = 0
    for i = 1, testcase do
        sum = sum + cc.pDistanceSQ(pt, ai:test1_pos(i))
    end
    gTrace:trace("test1_mixed")

    sum = 0
    for i = 1, testcase do
        sum = sum + cc.pDistanceSQ(pt, cc.p(i, i))
    end
    gTrace:trace("test1_lua_whole")
    
    local sumi = 0
    for i = 1, testcase do
        sum = sum + ai:test2_int(i)
    end
    gTrace:trace("test2_mixed")

    sumi = 0
    for i = 1, testcase do
        sumi = sumi + i
    end
    gTrace:trace("test2_lua_whole")
    
    ai:test_c_whole(1)
    gTrace:trace("test1_c_whole")
    
    ai:test_c_whole(2)
    gTrace:trace("test2_c_whole")]]
end

function AI.registerTarget(target) --把target存到targetTable，也存到根据camp分类的几个table: players opponents wild player_and_ally(我方英雄和我方小兵，即敌方单位的敌人) enemies(敌方小兵和敌方英雄，即我方单位的敌人)
    local id = target:getId()
    local camp = target:getCamp()
    
    AI._targetTable[id] = target
    
    if camp == Boids.Camp.Enemy or camp == Boids.Camp.Opponent then
        AI._enemies[id] = target --这里的_enemies是敌方小兵加上敌方玩家的英雄
        if camp == Boids.Camp.Opponent then
            AI._opponents[id] = target
        end
    elseif camp == Boids.Camp.Player or camp == Boids.Camp.Ally then
        AI._player_and_ally[id] = target
        if camp == Boids.Camp.Player then
            AI._players[id] = target
        end
    elseif camp == Boids.Camp.Wild then
        AI._wild[id] = target
    end
end

function AI.unRegisterTarget(target) --unRegister，这里就不写一堆if了，全清一遍(赋nil应该等价于删除吧)
    local id = target:getId()    
    AI._targetTable[id] = nil
    AI._enemies[id] = nil
    AI._players[id] = nil
    AI._player_and_ally[id] = nil
    AI._wild[id] = nil
    AI._opponents[id] = nil
end

function AI.registerUnit(unit) --unitTable和targetTable的区别：targetTable包含了建筑
    AI._unitTable[unit:getId()] = unit
end

function AI.unRegisterUnit(unit)
    AI._unitTable[unit:getId()] = nil
end

function AI.getAllOpponentsByCamp( camp )
    if camp == Boids.Camp.Enemy or camp == Boids.Camp.Opponent then
        return AI._player_and_ally
    elseif camp == Boids.Camp.Player or camp == Boids.Camp.Ally then
        return AI._enemies
    else
        return {}
    end
end

function AI.getAlliesByCamp( camp )
    if camp == Boids.Camp.Enemy or camp == Boids.Camp.Opponent then
        return AI.getOpponentsByCamp( Boids.Camp.Player )
    elseif camp == Boids.Camp.Player or camp == Boids.Camp.Ally then
        return AI.getOpponentsByCamp( Boids.Camp.Enemy )
    else
        return {}
    end
end

function AI.getOpponentsByCamp( camp )
    local opponents_all = AI.getAllOpponentsByCamp( camp )
    local only_units = {}
    for id, unit in pairs(opponents_all) do
        if unit._category == "charactor" then
            only_units[id] = unit
        end
    end
    return only_units
end

function AI.getOpponents(unit)
    local camp = unit:getCamp()
    if camp == Boids.Camp.Enemy or camp == Boids.Camp.Opponent then
        return AI._player_and_ally
    elseif camp == Boids.Camp.Player or camp == Boids.Camp.Ally then
        return AI._enemies
    else
        return {}
    end
end

function AI.reset() --开始游戏及再玩一次的时候得调这个方法
    cAI:reset()
    AI._targetTable = {}
    AI._unitTable = {}
    AI._players = {}
    AI._player_and_ally = {}
    AI._enemies = {}
    AI._wild = {}
    AI._opponents = {}
    AI._has_joystick_operation = {}
end

function AI.playerExtraLogic(player_camp, player_units, player_direction)
    --先把玩家单位的掉队相关的这个变量赋上初值，接下来会处理掉队
    for _, logic_unit in pairs(player_units) do
        logic_unit._speed_scale = 1.0
    end
    
    --计算摇杆及掉队
    local just_finished_joystick_operation = false
    if player_direction then
        AI._has_joystick_operation[player_camp] = true
        --AI.maintainFormation(player_direction)
        AI.processFallBehind(player_direction, player_camp) --有摇杆操作才处理掉队逻辑
    else --没有摇杆操作
        if AI._has_joystick_operation[player_camp] then --如果上一帧有摇杆操作，但现在没了，那么这帧就是刚结束了次摇杆操作
            just_finished_joystick_operation = true
        end
        AI._has_joystick_operation[player_camp] = false
    end
  
    if just_finished_joystick_operation then --如果刚结束摇杆操作，那么我方单位重新选目标。我们也可以有角色设计成每次都重新选目标的，需要的时候再改
        for _, player_unit in pairs(player_units) do
            player_unit._last_attack_target = nil
            player_unit:reSelectChasingTarget() --不带参数的调这个方法就是清空目标
            --ue.LuaUtils:logf("reselect chasing target and _last_attack_target because of just_finished_joystick_operation")
        end
    end
    
    --gTrace:accumulate("catchup")
end

function AI.updateFrame(direction, dt)
    
    --ue.LuaUtils:logf("AI update")
    
    AI._battleLayer._frameStage = Boids.FrameStage.BeginAI --这个stage那套翻成c之后估计不需要了，之前搞stage主要是为了在Behavior stage的时候getPos要从c里取，其它stage的时候用lua的值就行。
    
    --print("AI:", direction, dt)    
    
    if AI._battleLayer._my_force_id == 1 then
        AI.playerExtraLogic(Boids.Camp.Player, AI._players, direction[Boids.Camp.Player])
        AI.playerExtraLogic(Boids.Camp.Opponent, AI._opponents, direction[Boids.Camp.Opponent])
    else --要保证双方的顺序一致，后来想想应该是没有关系.. 里面目前只是算了下调队，没有做出什么实质性的操作
        AI.playerExtraLogic(Boids.Camp.Opponent, AI._opponents, direction[Boids.Camp.Opponent])
        AI.playerExtraLogic(Boids.Camp.Player, AI._players, direction[Boids.Camp.Player])
    end

    local units_order_by_priority = {}
    local max_unit_id = table.maxn(AI._unitTable)
    for _, unit in pairs(AI._unitTable) do
        --初始化各项（每帧都要初始化的）属性
        unit:calculatePriority()
        unit.c_unit:set_move_type(Boids.AIMoveType.stay)
        unit._attacking_this_frame = false
        unit._stay_unmoved_this_frame = true --这个变量现在没用。草地相关逻辑里用的。但草地代码上次从c翻lua的时候就没翻。
        units_order_by_priority[unit.c_unit:get_priority() * max_unit_id + unit:getId()] = unit --这里其实是要根据优先级排了个序。理论上这里排序的key得再改改，第一关键字是priority, 第二关键字是priority2，然后才是unit_id。
    end
    
    if AI._map_script and AI._map_script.beforeBehavior then
        AI._map_script:beforeBehavior() --每一帧比较开始的时候调一下，现在是灯怪那关用到了
    end
    
    gTrace:accumulate("before_behavior")
    
    AI._battleLayer._frameStage = Boids.FrameStage.Behavior

    --计算共享视野
    AI.calculateSight()
    gTrace:accumulate("sight")
    
    --目前是怪先被人推走之后，自己不能走，但是能打
    for _, unit in pairs(units_order_by_priority) do        
        if not unit:isDuringAttackOrCast() and unit._state ~= Boids.UnitState.Stunned then --如果正处于一次攻击或施法(或者是被晕了)，那么AI是不需要操心的。施法和攻击间隔的时候AI才需要操心                +
            local behaved = false
            if unit:isPlayerOrOpponent() then --
                if AI._has_joystick_operation[unit:getCamp()] then
                    unit:calculateMaxWalkLength() --先算一下根据dt和speed_scale得出来目前该帧可以走多远
                    ue.LuaUtils:logr(string.format("unit %d max_walk_length: %.4f", unit:getId(), unit.max_walk_length), true)
                    unit:walkByJoystick(cc.pMul(direction[unit:getCamp()], unit.max_walk_length))
                    behaved = true
                end
            end
            if not behaved then --如果前面没有要摇杆操作过，那么需要进下面的两个方法
                unit:beforeBehavior() --主要是徘徊逻辑，以及走太远会巢穴
                unit:doDefaultBehavior() --默认行为逻辑，即看看找个最近的看看打不打得到，打不到就走过去
            end
        end
        unit:syncPos() --虽然自己没行动，但可能被推着走了。这个方法是把c里该单位的位置信息同步到lua里
    end
    
    --一些移动后的处理
    for _, unit in pairs(units_order_by_priority) do
        --todo: grass
        unit._stay_unmoved = unit._stay_unmoved_this_frame --这个现在没用
    end
    
    gTrace:accumulate("behave")
    
    AI._battleLayer._frameStage = Boids.FrameStage.PostBehavior
    
    for _, unit in pairs(units_order_by_priority) do
        unit:performAction() --具体执行前面的结果，即调unitNode的移位置啊，turnface啊，还有onUnitMove(触发那些矩形位置的trigger)
    end
    
    --一些执行后的处理
    for _, unit in pairs(units_order_by_priority) do
        unit:updateFrame(dt) --包含updateSkill和buff的每帧回复    
    end

    gTrace:accumulate("updateUnit")
end

--private

function AI.calculateSight()
    gTrace:accumulate("before get groups")
    local sight_groups = {} --将有sight_group取出来
    for _, logic_unit in pairs(AI._unitTable) do
        if logic_unit.sight_group ~= '' then
            if sight_groups[logic_unit._sight_group] == nil then
                sight_groups[logic_unit._sight_group] = {}
            end            
            local that_sight_group = sight_groups[logic_unit._sight_group]
            table.insert(that_sight_group, logic_unit) --每个sight_group具体有哪些单位都存好
        end
    end
    
    gTrace:accumulate("get_groups")
    
    AI._can_see = {} --can_see每帧都重新计算，AI._can_see[group_name]为group_name这个组能看到哪些人

    for group_name, group in pairs(sight_groups) do        
        --ue.LuaUtils:logf(string.format("sight_group:%s", group_name))
        AI._can_see[group_name] = {}
        local can_see_targets_list = AI._can_see[group_name]
        
        local function checkIfCandidateCanBeSeenByGroup(candidate)
            if not candidate._nonAttackable then
                for _, unit_in_group in pairs(group) do
                    --如果本来就在追这个单位，那么就算作能看到
                    if unit_in_group._chasing_target == candidate or unit_in_group:inDirectView(candidate) then
                        --[[print(string.format("%s can see %s", group_name, candidate:getName()))
                        if unit_in_group._chasing_target then
                            print(string.format("unit %s has been chaising %s", unit_in_group:getName(), unit_in_group._chasing_target:getName()))
                        end]]
                        table.insert(can_see_targets_list, candidate)
                        break
                    end
                end
            end
        end    
    
        local function checkIfCandidatesCanBeSeenByGroup(candidates)
            for _, be_seen_candidate in pairs(candidates) do
                checkIfCandidateCanBeSeenByGroup(be_seen_candidate)
            end
        end
        
        --取该组的一个人，取得该人的对手列表然后看我们组能看到列表中的哪些人
        checkIfCandidatesCanBeSeenByGroup(group[1]:getOpponents()) --getOpponents对野怪是返回空的
        
        --下面这段是关于野怪的额外逻辑
        if group[1]:getCamp() == Boids.Camp.Wild then --如果这组是野怪，那么所有的非wild的target都有可能成为目标；如果不是野怪，那么野怪可能成为目标
            for _, be_seen_candidate in pairs(AI._targetTable) do
                if be_seen_candidate:getCamp() ~= Boids.Camp.Wild then
                    checkIfCandidateCanBeSeenByGroup(be_seen_candidate)
                end
            end
        else
            checkIfCandidatesCanBeSeenByGroup(AI._wild)
        end
    end
end

function AI.getUnitsFromIDs(group)
    local ret = {}
    for _, uid in ipairs(group) do
        if AI._unitTable[uid] then
            table.insert(ret, AI._unitTable[uid])
        else
            print("unit ", uid, " not found")
        end
    end
    return ret
end

function AI.processFallBehind(direction, player_camp)
    if not cAI:splitIntoTwoGroups(player_camp, direction.x, direction.y) then
        return --分组失败，或者是因为只剩1个人了，所以分不出组了；或者是因为距离都比较近
    end

    local in_narrow = cAI:get_in_narrow()

    local lead_group = AI.getUnitsFromIDs(cAI:get_lead_group())
    local fell_behind_group = AI.getUnitsFromIDs(cAI:get_fell_behind_group())
    local catch_up_position = cAI:get_catch_up_position()

    for _, u in ipairs(fell_behind_group) do
        local fell_behind_distance = cc.pGetDistance(u:getPos(), catch_up_position)
    
        if in_narrow then
            if fell_behind_distance > Boids.AI.WILL_CATCHUP_BY_PATH_WHEN_IN_NARROW then
                u.c_unit:catchup(catch_up_position.x, catch_up_position.y)
                --ue.LuaUtils:logf(string.format("let's catchup (in narrow): %.3f, %.3f; mine pos: %.3f, %.3f; id: %d", catch_up_position.x, catch_up_position.y, u:getPos().x, u:getPos().y, u:getId()))
            end
        else
            if fell_behind_distance > Boids.AI.WILL_CATCHUP_BY_PATH then
                u.c_unit:catchup(catch_up_position.x, catch_up_position.y)
                --ue.LuaUtils:logf(string.format("let's catchup: %.3f, %.3f; mine pos: %.3f, %.3f; id: %d", catch_up_position.x, catch_up_position.y, u:getPos().x, u:getPos().y, u:getId()))
            end
            --不在窄路的时候才有落后者加速；在窄路里面再加速容易撞一起甩头
            if fell_behind_distance > Boids.AI.CATCHUP_LEVEL1_DIST then
                if fell_behind_distance > Boids.AI.CATCHUP_LEVEL2_DIST then
                    if fell_behind_distance > Boids.AI.CATCHUP_LEVEL3_DIST then
                        u._speed_scale = Boids.AI.CATCHUP_LEVEL3_SPEED_SCALE
                    else --在level2和level3之间
                        u._speed_scale = MathExtend.interp(Boids.AI.CATCHUP_LEVEL2_SPEED_SCALE, Boids.AI.CATCHUP_LEVEL3_SPEED_SCALE, (fell_behind_distance - Boids.AI.CATCHUP_LEVEL2_DIST) / Boids.AI.CATCHUP_LEVEL3_DIST_MORE)
                    end
                else --在level1和level2之间
                    u._speed_scale = MathExtend.interp(Boids.AI.CATCHUP_LEVEL1_SPEED_SCALE, Boids.AI.CATCHUP_LEVEL2_SPEED_SCALE, (fell_behind_distance - Boids.AI.CATCHUP_LEVEL1_DIST) / Boids.AI.CATCHUP_LEVEL2_DIST_MORE)
                end
                --ue.LuaUtils:logr(string.format("unit %d speed scale: %.4f", u:getId(), u._speed_scale), true)
            end
        end        
    end

    for _, u in ipairs(lead_group) do
        if u.c_unit:is_during_catchup() then --不用继续catchup了。估计是摇杆方向转了，掉队和领先的组对调了
            --ue.LuaUtils:logf(string.format("lead group member %d reset path", u:getId()))
            u.c_unit:resetPath()
        end
    end
end

function AI.maintainFormation(direction)

end

return AI
