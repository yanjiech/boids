local UISkillLayer = class( "UISkillLayer", function() return cc.Layer:create() end )
local UISkillNode = require "UISkillNode"

local min_swipe_distance_sq = 400.0
local max_swipe_distance_sq = 40000.0

function UISkillLayer:ctor( ... )
    --init
    local arg = { ... }
    self._battleLayer = arg[2]
    self._skills = {}
    self._selectedSkill = nil
    self._isTouchBegan = false

    if self._battleLayer._replay_to_play then --播录像的时候不出技能UI
        return
    end
    
    local visibleSize = cc.Director:getInstance():getVisibleSize()

    self:alignSkillNodesWithPadding( 10.0 )

    local function onTouchBegan( touch, event )
        if self._isTouchBegan == false then
            self._touch_down_pos = self:convertTouchToNodeSpace( touch )
            self._selectedSkill = self:skillNodeForTouch( touch )
            if( self._selectedSkill ~= nil ) then
                self._isTouchBegan = true
                self._touch = touch
                self._selectedSkill:showHint( cc.p( 0, 0 ), 0 )
                return true
            end
        end

        return false
    end

    local function onTouchMoved( touch, event )
        if self._isTouchBegan and self._touch == touch then
            local touch_pos = self:convertTouchToNodeSpace( touch )
            local dir = nil
            local range_per = 0
            local distance = cc.pDistanceSQ( self._touch_down_pos, touch_pos )
            if distance >= min_swipe_distance_sq  then
                dir = cc.pNormalize( cc.pSub( touch_pos, self._touch_down_pos ) ) 
                if distance > max_swipe_distance_sq then
                    distance = max_swipe_distance_sq
                end
                range_per = ( distance - min_swipe_distance_sq ) / ( max_swipe_distance_sq - min_swipe_distance_sq )
            else
                dir = cc.p( 0, 0 )
            end
            self._selectedSkill:showHint( dir, range_per )
        end
    end

    local function onTouchCancelled( touch, event )
        if self._isTouchBegan and self._touch == touch then
            self._selectedSkill:hideHint()
            self._selectedSkill = nil
            self._isTouchBegan = false
        end
    end

    local function onTouchEnded( touch, event )
        local touch_pos = self:convertTouchToNodeSpace( touch )
        if self._isTouchBegan and self._touch == touch then
            ue.LuaUtils:logf(string.format("%s skill touched", self._selectedSkill._unit:getId()))
            local dir = nil
            local range_per = 0
            local distance = cc.pDistanceSQ( self._touch_down_pos, touch_pos )
            if distance >= min_swipe_distance_sq  then
                dir = cc.pNormalize( cc.pSub( touch_pos, self._touch_down_pos ) ) 
                if distance > max_swipe_distance_sq then
                    distance = max_swipe_distance_sq
                end
                range_per = ( distance - min_swipe_distance_sq ) / ( max_swipe_distance_sq - min_swipe_distance_sq )
            else
                dir = cc.p( 0, 0 )
            end
            self._selectedSkill:hideHint()
            self._selectedSkill:activate( dir, range_per )

            self._isTouchBegan = false
            self._selectedSkill = nil
            self._touch = nil
        end
    end  

    local listener = cc.EventListenerTouchOneByOne:create()
    listener:setSwallowTouches( true )
    listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
    listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
    listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_CANCELLED)
    self:getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, self)

end

function UISkillLayer:clearSkillNodes()
    for i,v in pairs( self._skills ) do
        v:hideHint()
    end
    self:removeAllChildren()
    self._skills = {}
end

function UISkillLayer:addSkillNode( unit )
    if self._battleLayer._replay_to_play then
        return
    end
    local newId = #self._skills + 1
    self._skills[newId] = UISkillNode:new( unit, self._battleLayer )
    self:addChild( self._skills[newId] )
    self:alignSkillNodesWithPadding( 10.0 )
end

function UISkillLayer:removeSkillNode( unit )
    if self._battleLayer._replay_to_play then
        return
    end
    local skills = {}
    for _, skill in pairs(self._skills) do
        if (skill:getOwnerId() ~= unit:getId()) then
            skills[#skills + 1] = skill
        else
            skill:hideHint()
            skill:removeFromParent()
        end
    end
    self._skills = skills
    self:alignSkillNodesWithPadding( 50.0 )
end

function UISkillLayer:skillNodeForTouch( touch )
    for i,v in pairs( self._skills ) do
        local rect = cc.rect( 0, 0, v:getContentSize().width, v:getContentSize().height )
        if cc.rectContainsPoint( rect, v:convertTouchToNodeSpace( touch ) ) then
            return v
        end
    end
    return nil
end 

function UISkillLayer:alignSkillNodesWithPadding( padding )
    local count = #self._skills
    if count > 0 then
        local x = cc.Director:getInstance():getVisibleSize().width - ( padding + self._skills[1]:getContentSize().width ) * count + 0.382 * self._skills[1]:getContentSize().width
        for i,v in pairs(self._skills) do
            v:setPosition( cc.p( x, 0.618 * self._skills[1]:getContentSize().height ) ) -- original_y: v:getContentSize().height  modified by weiyuemin
            x = x + padding + v:getContentSize().width
        end
    end
end 

return UISkillLayer
