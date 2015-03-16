local BonesSacrifice = class( "BonesSacrifice", function() return cc.Node:create() end )

function BonesSacrifice:ctor( ... )
    self._skeletons = {}
    self._battleLayer = nil
    self._srcId = -1
    self._radius = SkillConf.skill["BonesSacrifice"]["radius"]
    self._distance = SkillConf.skill["BonesSacrifice"]["distance"]
    self._pos = nil
end

function BonesSacrifice:activate( ... )
    local function update( delta )
        local newTbl = {}
        local srcUnit = self._battleLayer:getUnitById( self._srcId )
        if srcUnit then
            for i,v in pairs( self._skeletons ) do
                local sk = self._battleLayer:getUnitById( v )
                if sk then
                    local pos = sk:getPos()
                    if cc.pDistanceSQ( pos, self._pos ) < self._radius * self._radius then
                        srcUnit:takeHealing( sk:getData( "hp" ) )
                        sk:setState( Boids.UnitState.Disappear )
                    else
                        table.insert( newTbl, v )
                    end
                end
            end
        end
        if #newTbl == 0 then
            self:unactivate()
        else
            self._skeletons = newTbl
        end
    end

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    self._pos = srcUnit:getPos()
    self._srcId = srcUnit:getId()

    --addEffects
    local unode = srcUnit:getUnitNode()
    self._deffect = ue.ArmatureManager:getInstance():createArmature( "effects/sacrifice/down" )
    unode:addChild( self._deffect, 0 )
    self._deffect:setAnimation( 0, "animation", true )
    self._ueffect = ue.ArmatureManager:getInstance():createArmature( "effects/sacrifice/up" )
    unode:addChild( self._ueffect, 10 )
    self._ueffect:setAnimation( 0, "animation", true )

    local LogicUnit = require( "LogicUnit" )
    local factors = { 1, 0, -1.0, 0 }
    for i = 1, 2 do
        local rect = cc.rect( self._pos.x + factors[i*2-1] * self._distance, self._pos.y + factors[i*2] * self._distance, 1.0, 1.0 )
        local area = { ["name"] = "skeleton", ["rect"] = rect }
        local classname = require "units/skeleton"
        local unit = classname:new()
        unit:init( "skeleton", srcUnit:getCamp(), 0, true, "", self._battleLayer )
        local units = { unit }
        self._battleLayer:deployUnits( units, area, "" ) 
        self._radius = srcUnit:getData( "collide" ) * 2
        table.insert( self._skeletons, unit:getId() )
        unit:setForceUnpushable( true )
        unit:walkToDestination( self._pos, true )
    end
    self:scheduleUpdateWithPriorityLua( update, 0 )
    self._battleLayer:addEffect( self, srcUnit:getPos(), 0 )
    srcUnit:setNonAttackable( true )
    return true
end

function BonesSacrifice:unactivate()
    for i,v in pairs( self._skeletons ) do
        local sk = self._battleLayer:getUnitById( v )
        if sk then
            sk:setState( Boids.UnitState.Disappear )
        end
    end
    local srcUnit = self._battleLayer:getUnitById( self._srcId )
    if srcUnit then
        srcUnit:setNonAttackable( false )
        local unode = srcUnit:getUnitNode()
        unode:setMovement( Boids.UnitMovement.casting3 )
        unode:removeChild( self._deffect, true )
        unode:removeChild( self._ueffect, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self )
end

return BonesSacrifice
