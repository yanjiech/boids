local MapLogic = require( "MapLogic" )
local Forest02_2 = class( "Forest02_2", MapLogic )
local LogicUnit = require( "LogicUnit" )
local ResourceManager = require( "ResourceManager" )
require( "Helper" )
local wave_deploy_interval = 0.1

function Forest02_2:postInit()
    self._units_to_clear = {}
end

function Forest02_2:onCustomEvent( action )
    --区域1开启
    if action.action_name == "enter_area1" then
        if self._area1_activated ~= true then
            self:startWave( 1, "area1", action.waves, action.position_tag )
        end
    --区域2开启
    elseif action.action_name == "enter_area2" then
        if self._area2_activated ~= true then
            self:startWave( 2, "area2", action.waves, action.position_tag )
        end
    --区域3开启
    elseif action.action_name == "enter_area3" then
        if self._area3_activated ~= true then
            self:startWave( 3, "area3", action.waves, action.position_tag )
        end
    else
        MapLogic.onCustomEvent( self, action )
    end
end

function Forest02_2:startWave( area_id, area_name, waves, position_tag )
    --set buildings in area 1 to be attackable
    for i,v in pairs( self._battleLayer._buildings ) do
        if v._data.tag == area_name then
            v:setNonAttackable( false )
        end
    end

    self._wave_start = true
    self._waves = waves
    self._current_wave = 1
    self._area1_activated = true
    self._current_area = area_id
    self._total_waves = #self._waves
    self._accumulator = 0
    self._next_collapse = self._waves[1].next_collapse
    self._deploy_poses = {}
    for i,v in pairs( self._logic.positions ) do
        if v.tag == position_tag then
            table.insert( self._deploy_poses, v )
        end
    end
end

function Forest02_2:areaCleared( area_id )
    for i,v in pairs( self._battleLayer._buildings ) do
        print( v._data.name )
        if v._data.name == ( "door" .. area_id ) then
            if v._state == "normal" then
                v:takeDamage( v._hp, 0 )
            end
            break
        end
    end
end

function Forest02_2:update( delta )
    if self._wave_start == true then
        self._accumulator = self._accumulator + delta
        if self._during_deploy ~= true and self._accumulator > self._next_collapse then 
            self._accumulator = 0
            self._during_deploy = true
        elseif self._during_deploy == true and self._accumulator > wave_deploy_interval then
            for _,p in pairs( self._deploy_poses ) do
                local units = {}
                for i,v in pairs( self._waves[self._current_wave].units ) do
                    local count = v.count
                    local name = v.name
                    local unit_data = ResourceManager.getUnitData( name )
                    local is_boss = v.is_boss
                    for j = 1,count do
                        local unit = LogicUnit.createUnitByName( name )
                        unit:init( name, Boids.Camp.Enemy, unit_data.guard_radius, ( is_boss or false ), "", self._battleLayer ) 
                        table.insert( units, unit )
                    end
                end
                self._battleLayer:deployUnits( units, p, "" ) 
                for i,v in pairs( units ) do
                    table.insert( self._units_to_clear, v:getId() )
                end
            end
            self._current_wave = self._current_wave + 1
            if self._current_wave > self._total_waves then
                self._wave_start = false
            else
                self._next_collapse = self._waves[self._current_wave].next_collapse
            end

            self._accumulator = 0
            self._during_deploy = false
        end
    end
end

function Forest02_2:onUnitDisappear( unit, isDie )
    local unit_id = unit:getId()
    if Helper.tableRemoveObject( self._units_to_clear, unit_id ) then
        if #self._units_to_clear == 0 and self._wave_start == false then
            self:areaCleared( self._current_area )
        end
    end
    MapLogic.onUnitDisappear( self, unit, isDie )
end

return Forest02_2
