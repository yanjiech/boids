local DeathTornado = class( "DeathTornado", function() return cc.Node:create() end )
local math = require( 'math' )

function DeathTornado:ctor( ... )
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo( "effects/anubies_skill/zisexuanfeng.ExportJson" )
    self._effect = ccs.Armature:create( "zisexuanfeng" )
    self:addChild( self._effect )
    self._duration = SkillConf.skill["DeathTornado"]["duration"]
    self._radius = SkillConf.skill["DeathTornado"]["radius"]
    self._accumulator = 0
    self._dir = nil 
    self._speed = SkillConf.skill["DeathTornado"]["speed"]
    self._delay = SkillConf.skill["DeathTornado"]["delay"]
    self._battleLayer = nil
    self._atkerData = nil
    self._excludeTargets = {}
    self._baseDamage = SkillConf.skill["DeathTornado"]["damage"]
end 

function DeathTornado:calculateDamage( atk, armor )
    return ( self._baseDamage + 0.4 * atk ) * DamageCalculate.calculateResistance( armor, 0, 0 ) 
end 

function DeathTornado:activate( ... )
    local function update( delta )
        self._accumulator = self._accumulator + delta
        if self._accumulator > self._duration then
            self:unactivate()
        else 
            local lastPos = cc.p( self:getPosition() )
            local units = AI.getOpponentsByCamp( self._src_camp )
            if units then
                for _,v in pairs( units ) do
                    local pos = v:getPos()
                    if not Helper.tableContainsValue( self._excludeTargets, v:getId() ) and cc.pDistanceSQ( pos, lastPos ) < self._radius * self._radius then
                        local damage = self:calculateDamage( self._atkerData["atk"], v:getData( "def" ) )
                        v:takeDamage( damage, self._srcId )
                        self._excludeTargets[#self._excludeTargets+1] = v:getId()
                    end 
                end 
            end 
            if self._accumulator > self._delay then 
                local newPos = cc.pAdd( lastPos, cc.pMul( self._dir, self._speed * delta ) )
                self:setPosition( newPos )
                self._effect:setScale( math.min( 1.5, 0.2 + 1.5 * self._accumulator / self._duration ) )
            end 
        end 
    end

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    self._srcId = srcUnit:getId()
    local emitPos = srcUnit:getPos()

    self._atkerData = srcUnit:getRawData()
    self._dir = cc.pNormalize( srcUnit:getDirection() )
    self._effect:getAnimation():play( "Animation1" )
    self._accumulator = 0
    self._effect:setScale( 0.2 )
    self._battleLayer:addEffect( self, emitPos, srcUnit._unitNode:getLocalZOrder() )
    self:scheduleUpdateWithPriorityLua( update, 0 )

    return true 
end 

function DeathTornado:unactivate()
    self._effect:getAnimation():stop()
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end 

return DeathTornado 
