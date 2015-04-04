
--------------------------------
-- @module BattleLayer
-- @extend Layer,Updatable
-- @parent_module ue

--------------------------------
-- @function [parent=#BattleLayer] addToEffectLayer 
-- @param self
-- @param #cc.Node node
-- @param #vec2_table vec2
-- @param #int int
        
--------------------------------
-- @function [parent=#BattleLayer] addToOnGroundLayer 
-- @param self
-- @param #cc.Node node
-- @param #vec2_table vec2
-- @param #int int
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveUnitsByTag 
-- @param self
-- @param #string str
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#BattleLayer] resumeBattle 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] isPositionInVision 
-- @param self
-- @param #vec2_table vec2
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveUnits 
-- @param self
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- @function [parent=#BattleLayer] clearChasingTarget 
-- @param self
-- @param #TargetNode targetnode
        
--------------------------------
-- @function [parent=#BattleLayer] getPlayerUnits 
-- @param self
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- @function [parent=#BattleLayer] onUnitDying 
-- @param self
-- @param #UnitNode unitnode
        
--------------------------------
-- @function [parent=#BattleLayer] deployUnit 
-- @param self
-- @param #UnitNode unitnode
-- @param #vec2_table vec2
-- @param #string str
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveUnitsByName 
-- @param self
-- @param #string str
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#BattleLayer] getNextDeployId 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#BattleLayer] setState 
-- @param self
-- @param #int ebattlestate
        
--------------------------------
-- @function [parent=#BattleLayer] deployUnits 
-- @param self
-- @param #array_table array
-- @param #rect_table rect
-- @param #string str
        
--------------------------------
-- @function [parent=#BattleLayer] pauseBattle 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] getDeadUnits 
-- @param self
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- @function [parent=#BattleLayer] adjustCamera 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] getAvailablePosition 
-- @param self
-- @param #float float
-- @param #rect_table rect
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#BattleLayer] init 
-- @param self
-- @param #MapData map
-- @param #bool bool
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#BattleLayer] changeState 
-- @param self
-- @param #int ebattlestate
        
--------------------------------
-- @function [parent=#BattleLayer] quitBattle 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveUnitsByCampAndSightGroup 
-- @param self
-- @param #int eunitcamp
-- @param #string str
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#BattleLayer] startBattle 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] addToObjectLayer 
-- @param self
-- @param #cc.Node node
-- @param #vec2_table vec2
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveUnitsByCamp 
-- @param self
-- @param #int eunitcamp
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#BattleLayer] setGameTime 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#BattleLayer] addToOverObjectLayer 
-- @param self
-- @param #cc.Node node
-- @param #vec2_table vec2
-- @param #int int
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveUnitByDeployId 
-- @param self
-- @param #int int
-- @return UnitNode#UnitNode ret (return value: UnitNode)
        
--------------------------------
-- @function [parent=#BattleLayer] getState 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#BattleLayer] addBullet 
-- @param self
-- @param #int int
-- @param #BulletNode bulletnode
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveOpponentsInRange 
-- @param self
-- @param #int eunitcamp
-- @param #vec2_table vec2
-- @param #float float
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#BattleLayer] centerCameraToPosition 
-- @param self
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#BattleLayer] getAliveOpponents 
-- @param self
-- @param #int eunitcamp
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- @function [parent=#BattleLayer] getControlLayer 
-- @param self
-- @return UIControlLayer#UIControlLayer ret (return value: UIControlLayer)
        
--------------------------------
-- @function [parent=#BattleLayer] addToFloatLayer 
-- @param self
-- @param #cc.Node node
-- @param #vec2_table vec2
-- @param #int int
        
--------------------------------
-- @function [parent=#BattleLayer] setMapLogic 
-- @param self
-- @param #MapLogic map
        
--------------------------------
-- @function [parent=#BattleLayer] getLeaderUnit 
-- @param self
-- @return UnitNode#UnitNode ret (return value: UnitNode)
        
--------------------------------
-- @function [parent=#BattleLayer] getGameTime 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#BattleLayer] reset 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] isPositionOK 
-- @param self
-- @param #vec2_table vec2
-- @param #float float
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#BattleLayer] removeBullet 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#BattleLayer] setup 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] getMapData 
-- @param self
-- @return MapData#MapData ret (return value: MapData)
        
--------------------------------
-- @function [parent=#BattleLayer] addToBelowObjectLayer 
-- @param self
-- @param #cc.Node node
-- @param #vec2_table vec2
-- @param #int int
        
--------------------------------
-- @function [parent=#BattleLayer] restartBattle 
-- @param self
        
--------------------------------
-- @function [parent=#BattleLayer] onUnitDead 
-- @param self
-- @param #UnitNode unitnode
        
--------------------------------
-- @function [parent=#BattleLayer] onUnitAppear 
-- @param self
-- @param #UnitNode unitnode
        
--------------------------------
-- @function [parent=#BattleLayer] updateFrame 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#BattleLayer] create 
-- @param self
-- @param #MapData map
-- @param #bool bool
-- @return BattleLayer#BattleLayer ret (return value: BattleLayer)
        
--------------------------------
-- @function [parent=#BattleLayer] BattleLayer 
-- @param self
        
return nil
