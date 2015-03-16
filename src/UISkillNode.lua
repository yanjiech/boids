local UISkillNode = class( "UISkillNode", function() return cc.Node:create() end )
local math = require( "math" )

function UISkillNode:ctor( ... )
    local function update( delta )	    
        gTrace:accumulate("beforeUISKilllupdate")
        local p = self._unit:getSkillCDById( 1 )
        self._avatar:setPercentage( (1 - p) * 100 )
        if self._hint_effect ~= nil then
            if self._hint_type == "dyn_circle" then
                self._hint_effect:setPosition( cc.pAdd( self._unit:getPos(), self._hint_d_pos ) )
            else
                self._hint_effect:setPosition( self._unit:getPos() )
            end
       end
        gTrace:accumulate("UISKilllupdate")
    end 

    --init
    local arg = { ... }
    self._unit = arg[2]
    self._battleLayer = arg[3]
    local name = self._unit:getName()
    if cc.SpriteFrameCache:getInstance():getSpriteFrame( name .. "_avatar.png" ) == nil then
        name = "saber"
    end
    local sp = cc.Sprite:createWithSpriteFrameName(name.."_avatar.png")
    local mask = cc.Sprite:createWithSpriteFrameName("skill_mask.png")
    self._avatar = cc.ProgressTimer:create( mask )
    self._avatar:setType( cc.PROGRESS_TIMER_TYPE_RADIAL )
    self._avatar:setReverseDirection(true)
    self:addChild( sp )
    self:addChild( self._avatar )
    local centerPos = cc.p( self._avatar:getContentSize().width / 2, self._avatar:getContentSize().height / 2 )
    sp:setPosition( centerPos )
    self._avatar:setPosition( centerPos )
    self:setContentSize( self._avatar:getContentSize() )
    self:setAnchorPoint( cc.p( 0.5, 0.5 ) )
    self:scheduleUpdateWithPriorityLua( update, 0 )
    self._hint_type = self._unit:getSkillHintTypeById( 1 )
    self._range = self._unit:getSkillRangeById( 1 )
    self._hint_d_pos = cc.p( 0, 0 )
    if self._hint_type == "dyn_circle" then
        self._min_range = self._unit:getSkillMinRangeById( 1 )
        self._max_range = self._unit:getSkillMaxRangeById( 1 )
    end
end

function UISkillNode:activate( dir, range_per )
    if self._unit:isSkillReadyById( 1 ) then
        print(self._unit:getId(), "'s skill touched and ready to execute")

        local range = 0
        if self._hint_type == "dyn_circle" then
            range = self._min_range + ( self._max_range - self._min_range ) * range_per
        end

        --网络游戏的话就直接往外发了
        if AI._battleLayer._net_ad then
            AI._battleLayer._net_ad:setSkill(self._unit:getId(), dir.x * 1e6, dir.y * 1e6, range * 1e6)
        else
            --为了和录像放出来的效果一致，统一到下一帧开始的时候再放
            PlayerSkill.data[self._unit:getId()] = { ["dir"] = dir, ["range"] = range }
        end
    else
        print(self._unit:getId(), "'s skill touched but not ready")
    end 
end 

function UISkillNode:getOwnerId()
    return self._unit:getId()
end

function UISkillNode:showHint( dir, range_per )
    if dir.x == 0 and dir.y == 0 then
        dir = cc.pNormalize( self._unit:getDirection() )
    end
    local angle = -math.atan2( dir.y, dir.x ) / math.pi * 180.0

    if self._hint_effect ~= nil then
        if self._hint_type == "arrow" then
            self._hint_effect:setRotation( angle )
        elseif self._hint_type == "dyn_circle" then
            self._range = self._min_range + ( self._max_range - self._min_range ) * range_per
            self._hint_d_pos = MathExtend.deltaFromCenter( self._range, dir )
            --self._hint_d_pos = cc.p( dir.x * self._range, dir.y * self._range )
            self._hint_effect:setPosition( cc.pAdd( self._unit:getPos(), self._hint_d_pos ) ) 
        end
    else
        local hint_pos = self._unit._unitNode:getParent():convertToWorldSpace( self._unit:getPos() )
        if self._hint_type == "arrow" then
            self._hint_effect = cc.Sprite:create( "ui/skillarrow.png" )
            self._hint_effect:setRotation( angle )
            self._hint_effect:setAnchorPoint( cc.p( 0, 0.5 ) )
            self._battleLayer:addEffect( self._hint_effect, hint_pos, 0 )
        elseif self._hint_type == "circle" then
            self._hint_effect = cc.Sprite:create( "ui/skillcircle.png" )
            local range = self._unit:getSkillRangeById( 1 )
            local scale = range / 200.0
            self._hint_effect:setScale( scale )
            self._battleLayer:addGroundEffect( self._hint_effect, hint_pos, 0 ) 
        elseif self._hint_type == "dyn_circle" then
            self._range = self._min_range
            self._hint_effect = cc.Sprite:create( "ui/skillcircle.png" )
            self._hint_effect:setScale( self._range / 200.0 )
            self._hint_d_pos = MathExtend.deltaFromCenter( self._range, dir ) 
            --self._hint_d_pos = cc.p( dir.x * self._range, dir.y * self._range )
            hint_pos = cc.pAdd( hint_pos, self._hint_d_pos )
            self._battleLayer:addGroundEffect( self._hint_effect, hint_pos, 0 )
        end
    end
end

function UISkillNode:hideHint()
    if self._hint_effect then
        self._hint_effect:getParent():removeChild( self._hint_effect, true )
        self._hint_effect = nil
    end
end

return UISkillNode
