local BatSwarm = class( "BatSwarm", function() return cc.Node:create() end )
local math = require( "math" )

function BatSwarm:ctor( ... )
    self._baseDamage = SkillConf.skill["BatSwarm"]["damage"]
    self._range = SkillConf.skill["BatSwarm"]["range"]
    self._count = SkillConf.skill["BatSwarm"]["count"]
    self._duration = SkillConf.skill["BatSwarm"]["duration"]
    self._radius = SkillConf.skill["BatSwarm"]["radius"]
    self._bats = {}
    self._centers = {}
    self._angles = {}
    self._fly_radius = {}
    self._excludeTargets = {}
    self._accumulator = 0
end

function BatSwarm:calculateDamage( atk, armor )
    return ( self._baseDamage + 0.2 * atk ) * DamageCalculate.calculateResistance( armor, 0, 0 )
end

function BatSwarm:activate( ... )
    local function update( delta )
        local units = AI.getOpponentsByCamp( self._src_camp ) 
        local delta_angle = math.pi * 2.0 * delta / self._duration
        for j,v in pairs( self._bats ) do
            --update bat pos 
            self._angles[j] = self._angles[j] + delta_angle
            local bat_pos = cc.p( self._centers[j].x + math.cos( self._angles[j] ) * self._fly_radius[j], self._centers[j].y + math.sin( self._angles[j] ) * self._fly_radius[j] )
            v:setPosition( bat_pos )
            v:setRotation( ( self._angles[j] + math.pi / 2.0 ) * 360.0 )
            --local last_bat_pos = cc.p( v:getPosition() )
            --v:setRotationSkewX( last_bat_pos.x > bat_pos.x and 0 or 180.0 ) 

            if units then
                for i,u in pairs( units ) do
                    if not Helper.tableContainsValue( self._excludeTargets, u:getId() ) then
                        local upos = u:getPos()
                        if cc.pDistanceSQ( bat_pos, upos ) < self._radius * self._radius then
                            local damage = self:calculateDamage( self._atkerData["atk"], u:getData( "def" ) )
                            u:takeDamage( damage, self._srcId )
                            table.insert( self._excludeTargets, u:getId() )
                        end
                    end
                end
            end
        end
        self._accumulator = self._accumulator + delta
        if self._accumulator > self._duration then
            self:unactivate()
        end
    end

    local args = { ... }
    self._battleLayer = args[1]
    local srcUnit = args[2]
    self._src_camp = srcUnit:getCamp()
    self._srcId = srcUnit:getId()
    self._atkerData = srcUnit:getRawData()
    self._srcPos = srcUnit:getPos()

    local armature = srcUnit._unitNode:currentArmature()
    local bonePos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
    local emitPos = armature:convertToWorldSpace( cc.p( bonePos ) )
    emitPos = self._battleLayer._effectLayer:convertToNodeSpace( emitPos )
    local dir = cc.pNormalize( srcUnit:getDirection() )
    local face_angle = math.atan2( dir.y, dir.x )
    local delta_angle = math.pi * 2.0 / self._count

    for i = 1, self._count do
        local resource = G.pathForResource( "bat_swarm", Boids.ResourceType.spineEffect )
        local bat = ue.ArmatureManager:getInstance():createArmature( resource )
        bat:setAnimation( 0, "animation", true )
        self._battleLayer:addEffect( bat, emitPos, 0 )
        table.insert( self._bats, bat )
        local init_angle = face_angle + ( i - 1 ) * delta_angle
        local fly_radius = math.sin( init_angle ) >= 0 and ( self._range / 1.7 ) or ( self._range / 3.0 )
        table.insert( self._centers, cc.p( self._srcPos.x + math.cos( init_angle ) * fly_radius, self._srcPos.y + math.sin( init_angle ) * fly_radius ) )
        table.insert( self._angles, math.pi + init_angle )
        table.insert( self._fly_radius, fly_radius )
    end

    self:scheduleUpdateWithPriorityLua( update, 0 )
    self._battleLayer:addEffect( self, cc.p( 0, 0 ), 0 )
    return true
end

function BatSwarm:unactivate()
    for i,v in pairs( self._bats ) do
        v:clearTracks()
        v:getParent():removeChild( v, true )
    end
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return BatSwarm
