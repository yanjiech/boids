
--------------------------------
-- @module UnitNode
-- @extend TargetNode,Updatable,Collidable
-- @parent_module ue

--------------------------------
-- @function [parent=#UnitNode] getBornPos 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] setCurrentSkeleton 
-- @param self
-- @param #sp.SkeletonAnimation skeletonanimation
        
--------------------------------
-- @function [parent=#UnitNode] removeUnitComponent 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] getSkills 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#UnitNode] findPathToPosition 
-- @param self
-- @param #vec2_table vec2
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] init 
-- @param self
-- @param #BattleLayer battlelayer
-- @param #map_table map
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] walkTo 
-- @param self
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#UnitNode] addBehavior 
-- @param self
-- @param #string str
-- @param #BehaviorBase behaviorbase
        
--------------------------------
-- @function [parent=#UnitNode] addBuff 
-- @param self
-- @param #string str
-- @param #Buff buff
        
--------------------------------
-- @function [parent=#UnitNode] isBoss 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] getSkillMaxRangeById 
-- @param self
-- @param #int int
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#UnitNode] getSkillCDById 
-- @param self
-- @param #int int
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#UnitNode] changeUnitDirection 
-- @param self
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#UnitNode] addUnitComponent 
-- @param self
-- @param #UnitNodeComponent unitnodecomponent
-- @param #string str
-- @param #int ecomponentlayer
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] isCasting 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] setUnitFace 
-- @param self
-- @param #int eunitface
        
--------------------------------
-- @function [parent=#UnitNode] removeBehavior 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] setWanderRadius 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#UnitNode] isHarmless 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] setHesitateFrame 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] getUnitState 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] shouldCatchUp 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] isSkillReadyById 
-- @param self
-- @param #int int
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] getSkillHintTypeById 
-- @param self
-- @param #int int
-- @return string#string ret (return value: string)
        
--------------------------------
-- @function [parent=#UnitNode] canAttack 
-- @param self
-- @param #TargetNode targetnode
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] hasBuff 
-- @param self
-- @param #string str
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] hasUnitTag 
-- @param self
-- @param #string str
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] getCurrentSkeleton 
-- @param self
-- @return SkeletonAnimation#SkeletonAnimation ret (return value: sp.SkeletonAnimation)
        
--------------------------------
-- @function [parent=#UnitNode] isOscillate 
-- @param self
-- @param #vec2_table vec2
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] useSkill 
-- @param self
-- @param #int int
-- @param #vec2_table vec2
-- @param #float float
        
--------------------------------
-- @function [parent=#UnitNode] isFoeOfCamp 
-- @param self
-- @param #int eunitcamp
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] setNextUnitState 
-- @param self
-- @param #int eunitstate
        
--------------------------------
-- @function [parent=#UnitNode] onDying 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getDeployId 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] hideHP 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getHesitateFrame 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] falldown 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] onAttacking 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] onCharging 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] setUnitDirection 
-- @param self
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#UnitNode] removeBuff 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] setUnitState 
-- @param self
-- @param #int eunitstate
        
--------------------------------
-- @function [parent=#UnitNode] getSkillRangeById 
-- @param self
-- @param #int int
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#UnitNode] adjustAllUnitComponents 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getChasingTarget 
-- @param self
-- @return TargetNode#TargetNode ret (return value: TargetNode)
        
--------------------------------
-- @function [parent=#UnitNode] setDeployId 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] isWalking 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] walkAlongPath 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#UnitNode] applyUnitState 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] onSkeletonAnimationEnded 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] endCast 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getEmitPos 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] evaluateCatchUp 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getLocalHeadPos 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] jumpNumber 
-- @param self
-- @param #float float
-- @param #string str
-- @param #bool bool
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] onSkeletonAnimationStart 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] riseup 
-- @param self
-- @param #float float
-- @param #float float
        
--------------------------------
-- @function [parent=#UnitNode] isUnderControl 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] getUnitCamp 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] onSkeletonAnimationEvent 
-- @param self
-- @param #int int
-- @param #spEvent spevent
        
--------------------------------
-- @function [parent=#UnitNode] setPriority 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] setConcentrateOnWalk 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#UnitNode] setUnitTags 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] onCasting 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getUnitDirection 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] isUnitInDirectView 
-- @param self
-- @param #UnitNode unitnode
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] attack 
-- @param self
-- @param #TargetNode targetnode
        
--------------------------------
-- @function [parent=#UnitNode] isAttacking 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] setBornPos 
-- @param self
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#UnitNode] removeAllUnitComponents 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] onAttackBegan 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] setGLProgrameState 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] isConcentrateOnWalk 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @overload self, UnitNode         
-- @overload self, vec2_table, float         
-- @overload self, UnitNode, vec2_table         
-- @function [parent=#UnitNode] willCollide
-- @param self
-- @param #UnitNode unitnode
-- @param #vec2_table vec2
-- @return bool#bool ret (retunr value: bool)

--------------------------------
-- @function [parent=#UnitNode] getLocalHitPos 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] onDisappearEnd 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] applyCustomChange 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] endSkill 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] removeAllBuffs 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] updateFrame 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#UnitNode] getPriority 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] getNextWanderPos 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] isDying 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] takeHeal 
-- @param self
-- @param #float float
-- @param #bool bool
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] showHP 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getUnitData 
-- @param self
-- @return UnitData#UnitData ret (return value: UnitData)
        
--------------------------------
-- @function [parent=#UnitNode] disappear 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] needRelax 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] setMovable 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#UnitNode] changeUnitState 
-- @param self
-- @param #int eunitstate
        
--------------------------------
-- @function [parent=#UnitNode] setBoss 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#UnitNode] getNextUnitState 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] isMovable 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] getHitPos 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#UnitNode] setChasingTarget 
-- @param self
-- @param #TargetNode targetnode
        
--------------------------------
-- @function [parent=#UnitNode] getBattleLayer 
-- @param self
-- @return BattleLayer#BattleLayer ret (return value: BattleLayer)
        
--------------------------------
-- @function [parent=#UnitNode] getUnitFace 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] getAdvisedNewDir 
-- @param self
-- @param #UnitNode unitnode
-- @param #vec2_table vec2
-- @param #vec2_table vec2
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#UnitNode] getAttackTarget 
-- @param self
-- @return TargetNode#TargetNode ret (return value: TargetNode)
        
--------------------------------
-- @function [parent=#UnitNode] setShouldCatchUp 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#UnitNode] getSkillMinRangeById 
-- @param self
-- @param #int int
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#UnitNode] appear 
-- @param self
        
--------------------------------
-- @function [parent=#UnitNode] getWanderRadius 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#UnitNode] setUnitCamp 
-- @param self
-- @param #int eunitcamp
        
--------------------------------
-- @function [parent=#UnitNode] changeFace 
-- @param self
-- @param #int eunitface
        
--------------------------------
-- @function [parent=#UnitNode] takeDamage 
-- @param self
-- @param #float float
-- @param #bool bool
-- @param #bool bool
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] setSightGroup 
-- @param self
-- @param #string str
        
--------------------------------
-- @function [parent=#UnitNode] onSkeletonAnimationCompleted 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#UnitNode] getSightGroup 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- @function [parent=#UnitNode] create 
-- @param self
-- @param #BattleLayer battlelayer
-- @param #map_table map
-- @return UnitNode#UnitNode ret (return value: UnitNode)
        
--------------------------------
-- @function [parent=#UnitNode] getCampByString 
-- @param self
-- @param #string str
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#UnitNode] UnitNode 
-- @param self
        
return nil
