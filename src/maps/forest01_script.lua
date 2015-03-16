local MapScript = class("MapScript", nil)

function MapScript:ctor(...)
    local args = {...}
    self._battle_layer = args[2]
    local custom_data = self._battle_layer._logic._logic.custom_data
    if custom_data.patrol_graph_edges then
        self._patrol_graph = {}
        for _, edge in ipairs(custom_data.patrol_graph_edges) do
            local edge_src
            local edge_dst
            for _k, _v in pairs(edge) do
                edge_src = _k
                edge_dst = _v
                break
            end
            self:addEdge(edge_src, edge_dst)
            self:addEdge(edge_dst, edge_src)                
        end
    else
        print("UNEXPECTED! patrol_graph_edges not found.")
    end
    
    local patrol_pos_area = self._battle_layer._logic:getAreasByNamePrefix("patrol_pos_")
    self._patrol_points = {}
    for _, area in ipairs(patrol_pos_area) do
        --去掉"patrol_pos_"这11个字符；取中心点
        --print("patrol_pos", string.sub(area.name, 12))
        local area_center = G.center(area.rect)
        self._patrol_points[string.sub(area.name, 12)] = area_center
    end
    
    self._light_enemy = {}
    self._units_in_light_monster_view = {}
end

function MapScript:getClosestPatrolPos(current_pt)
    local closest_dist_sq = 1000000000
    local closest_patrol
    for name, pt in pairs(self._patrol_points) do
        local dist_sq = cc.pDistanceSQ(current_pt, pt)
        if dist_sq < closest_dist_sq then
            closest_dist_sq = dist_sq
            closest_patrol = name
        end
    end
    return closest_patrol
end

function MapScript:getPatrolPt(patrol_pos)
    local pt = self._patrol_points[patrol_pos]    
    if not pt then
        print("unexpected. maybe missing patrol point")
    end
    return pt
end

function MapScript:getNextPatrolPos(last_patrol, current_patrol)    
    local adjacent_patrol_pos_count = #self._patrol_graph[current_patrol] 
    local next_pos_list
    local next_prob_list
    if last_patrol then        
        next_pos_list = {last_patrol}
        local rest_prob_per_pos = 0
        if adjacent_patrol_pos_count == 1 then
            next_prob_list = {100}
        elseif adjacent_patrol_pos_count <= 3 then
            next_prob_list = {10}
            rest_prob_per_pos = 90 / (adjacent_patrol_pos_count - 1)
        else
            next_prob_list = {4}
            rest_prob_per_pos = 96 / (adjacent_patrol_pos_count - 1)
        end
        
        for _, next_pos in ipairs(self._patrol_graph[current_patrol]) do
            if next_pos ~= last_patrol then
                next_pos_list[#next_pos_list + 1] = next_pos
                next_prob_list[#next_prob_list + 1] = rest_prob_per_pos
            end
        end
    else --估计是刚开始的时候，没有last_patrol，那么各个相邻的patrol_pos平均分
        next_pos_list = self._patrol_graph[current_patrol]
        next_prob_list = {}
        --print("patrol:", current_patrol, adjacent_patrol_pos_count)
        for i = 1, adjacent_patrol_pos_count do
            next_prob_list[#next_prob_list + 1] = 100 / adjacent_patrol_pos_count
        end
    end    
    --print("next_prob_list", current_patrol, #next_prob_list)
    for i = 2, #next_prob_list do
        next_prob_list[i] = next_prob_list[i] + next_prob_list[i - 1]
    end
    --print("_next_prob_list", current_patrol, #next_prob_list)
    
    local r = ue.LuaUtils:randomNumber(100)
    for i, next_prob in ipairs(next_prob_list) do
        if r <= next_prob then
            --print("next_pos.", r, next_prob, i)
            return next_pos_list[i]
        end
    end
    --print("return max next_pos.", r, next_prob_list[#next_prob_list])
    return next_pos_list[#next_prob_list]
end

function MapScript:beforeBehavior()
    --测试addCollidablePolygon
    --[[if cAI:get_current_frame() == 1 then
        local poly_x = {450, 1200, 1300, 300,}
        local poly_y = {1550, 1000, 3000, 2000,}
        cAI:addCollidablePolygon("nice_poly", poly_x, poly_y)
    elseif cAI:get_current_frame() == 200 then
        cAI:removeCollidablePolygon("nice_poly")
    end]]
    
    for id, _ in pairs(self._units_in_light_monster_view) do --每一帧先清空_units_in_light_monster_view：可能有单位不再被看到，或者死了
        self._units_in_light_monster_view[id] = nil
    end
    for _, player_unit in pairs(AI._player_and_ally) do
        for light_unit, _ in pairs(self._light_enemy) do
            if not light_unit._is_stun and light_unit:inDirectView(player_unit) then --晕着的灯怪不起作用                
                self._units_in_light_monster_view[player_unit:getId()] = player_unit
                break
            end            
        end
    end
end

function MapScript:getExtraOpponents(unit)
    if unit:hasTag("night_enemy") then
        return self._units_in_light_monster_view
    else
        return nil
    end
end

function MapScript:postDeploy(unit)
    if unit:hasTag("night_enemy") then
        unit._guard_radius = unit:getCollideRadius() * 3
        if Boids.debugGuard then
            unit._unitNode:_attachGuardNode()
        end
    end
    if unit:hasTag("light_enemy") then
        self._light_enemy[unit] = true
    end
end

function MapScript:clearTargetForNightEnemy()
    for _, unit2 in pairs(AI._unitTable) do
        if unit2:hasTag("night_enemy") then
            --ue.LuaUtils:logf(string.format("clear chasing target: %d", unit2:get_unit_id()))
            unit2:reSelectChasingTarget() --有灯怪死/晕就清空所有夜间怪的目标; 直接赋chasing_target不管用，主要是要清path
        end
    end
end

--private

function MapScript:addEdge(src, dst)
    if not self._patrol_graph[src] then
        self._patrol_graph[src] = {}
    end
    self._patrol_graph[src][#self._patrol_graph[src] + 1] = dst
end

return MapScript
