local DeathField = class( "DeathField", function() return cc.Node:create() end )
local math = require( 'math' )

function DeathField:ctor( ... )
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo( "effects/anubies_skill/siwanglingyu.ExportJson" )
    self._radius = SkillConf.skill["DeathField"]["radius"]
    self._effect = ccs.Armature:create( "siwanglingyu" )
    self:addChild( self._effect )
    self._duration = SkillConf.skill["DeathField"]["duration"]
    self._accumulator = 0
    self._damageAccumulator = 0
    self._battleLayer = nil
    self._baseDamage = SkillConf.skill["DeathField"]["damage"]
end 

function DeathField:calculateDamage( atk, armor )
    return ( self._baseDamage + 0.4 * atk ) * DamageCalculate.calculateResistance( armor, 0, 0 )
end

function DeathField:activate( ... )
    local function update( delta )
        self._accumulator = self._accumulator + delta
        self._damageAccumulator = self._damageAccumulator + delta
        if self._accumulator > self._duration then
            self:unactivate()            
        else
            if self._damageAccumulator > 0.5 then 
                local units = AI.getOpponentsByCamp( self._src_camp ) 
                if units then 
                    local center = cc.p( self:getPosition() )
                    local radiusSQ = self._radius * self._radius
                    for _,v in pairs( units ) do
                        local uPos = cc.p( v:getPos() )
                        if cc.pDistanceSQ( uPos, center ) < radiusSQ then
                            local damage = self:calculateDamage( self._atkerData["atk"], v:getData( "def" ) ) * self._damageAccumulator
                            v:takeDamage( damage, self._srcId )
                        end 
                    end
                end
                self._damageAccumulator = 0
           end 
        end 
    end

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    self._srcId = srcUnit:getId()
    self._atkerData = srcUnit:getRawData( )
    local target = srcUnit:getTarget()
    if target then
        local emitPos = target:getPos()
        self._battleLayer:addEffect( self, emitPos, 0 )
        self._accumulator = 0
        self._damageAccumulator = 0
        self:scheduleUpdateWithPriorityLua( update, 0 )
        self._effect:getAnimation():play( "Animation1" )
        return true
    end 
    return true
end 

function DeathField:unactivate()
    self._effect:getAnimation():stop()
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return DeathField 

