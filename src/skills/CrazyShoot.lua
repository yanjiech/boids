local CrazyShoot = class( "CrazyShoot", function() return cc.Node:create() end )
local math = require( "math" )
local Bullet = require( "Bullet" )
function CrazyShoot:ctor( ... )
    self._count = SkillConf.skill["CrazyShoot"]["count"]
    self._speed = SkillConf.skill["CrazyShoot"]["speed"]
    self._baseDamage = SkillConf.skill["CrazyShoot"]["damage"]
    self._interval = SkillConf.skill["CrazyShoot"]["interval"]
    self._range = SkillConf.skill["CrazyShoot"]["range"]
    self._accumulator = self._interval
    self._srcId = nil
    self._targetId = -1 
    self._currentCount = 0
    self._srcPos = nil
end

function CrazyShoot.calculateDamage( baseDamage, atkerData, deferData )
    return ( baseDamage + 0.2 * atkerData["atk"] ) * DamageCalculate.calculateResistance( deferData["def"], 0, 0 )
end

function CrazyShoot:activate( ... )
    local function update( delta )
        ue.LuaUtils:logf(string.format("CrazyShoot update: %.5f", delta))
        self._accumulator = self._accumulator + delta
        local targetUnit = nil
        if self._targetId == -1 then
            self._currentCount = self._currentCount + 1
            targetUnit = self._battleLayer:getUnitInRangeFromTable( AI.getOpponentsByCamp( self._src_camp ), self._srcPos, self._range )
            if targetUnit then
                self._targetId = targetUnit:getId()
            else
                self._targetId = -1
            end
        else
            targetUnit = self._battleLayer:getUnitById( self._targetId )
        end
        local srcUnit = self._battleLayer:getUnitById( self._srcId )
        if srcUnit and targetUnit then
            if self._currentCount < self._count and self._accumulator > self._interval then
                self._accumulator = 0
                local armature = srcUnit._unitNode:currentArmature()
                local bonePos = ue.ArmatureManager:getInstance():getBonePosition( armature, "EmitPoint" )
                local emitPos = armature:convertToWorldSpace( cc.p( bonePos ) )
                local zorder = srcUnit._unitNode:getLocalZOrder()
                local bullet = Bullet:new( ResourceManager.getBulletData( "vanhelsing_bullet"  ), self._baseDamage, CrazyShoot )
                ue.LuaUtils:logf(string.format("will shoot bullet(CrazyShoot's %dth): %s(%d)", self._currentCount, bullet.__cname, bullet._bulletId))
                bullet:shoot( srcUnit, self._battleLayer, targetUnit )
                self._targetId = -1
            else
                ue.LuaUtils:logf(string.format("emit condition not satisfied: %d, %d, %.5f, %.5f", 
                    self._currentCount, self._count, self._accumulator, self._interval))
            end
        elseif targetUnit == nil then
            self._targetId = -1
        end
        if srcUnit == nil or self._currentCount >= self._count then
            self:unactivate()
        end
    end

    local arg = { ... }
    self._battleLayer = arg[1]
    local srcUnit = arg[2]
    self._src_camp = srcUnit:getCamp()
    self._atkerData = srcUnit:getRawData()
    self._srcId = srcUnit:getId()
    self._srcPos = srcUnit:getPos()
    self._targetId = -1
    self._battleLayer:addEffect( self, cc.p( 0, 0 ), 0 )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    return true
end

function CrazyShoot:unactivate( ... )
    self:unscheduleUpdate()
    self:getParent():removeChild( self, true )
end

return CrazyShoot
