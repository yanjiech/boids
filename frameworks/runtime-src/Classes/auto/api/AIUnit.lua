
--------------------------------
-- @module AIUnit
-- @extend Collidable
-- @parent_module ue

--------------------------------
-- 
-- @function [parent=#AIUnit] resetPath 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#AIUnit] buildingInDirectView 
-- @param self
-- @param #float building_x
-- @param #float building_y
-- @param #float building_radius
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] catchup 
-- @param self
-- @param #float dest_x
-- @param #float dest_y
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_move_type 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] canAttack 
-- @param self
-- @param #AIUnit other
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] findPathWithDestRadius 
-- @param self
-- @param #float dest_x
-- @param #float dest_y
-- @param #float dest_radius
-- @param #int path_validity_frame
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_move_type 
-- @param self
-- @param #int var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_priority 
-- @param self
-- @param #int var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_position 
-- @param self
-- @param #float x
-- @param #float y
        
--------------------------------
-- 
-- @function [parent=#AIUnit] hasPath 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] is_during_catchup 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_max_walk_length 
-- @param self
-- @param #float var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_priority2 
-- @param self
-- @param #int var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_priority2 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] findPathForTarget 
-- @param self
-- @param #AIUnit target
-- @param #int path_validity_frame
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_collide_radius 
-- @param self
-- @param #float var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] deleteUnit 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_guard_radius 
-- @param self
-- @param #float var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_position 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_hesitate_frame 
-- @param self
-- @param #int hesitate_frame
        
--------------------------------
-- 
-- @function [parent=#AIUnit] findPathForTargetWithRadius 
-- @param self
-- @param #AIUnit target
-- @param #float dest_radius
-- @param #int path_validity_frame
        
--------------------------------
-- 
-- @function [parent=#AIUnit] findPath 
-- @param self
-- @param #float dest_x
-- @param #float dest_y
-- @param #int path_validity_frame
        
--------------------------------
-- 
-- @function [parent=#AIUnit] isPathEnd 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_unit_camp 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] initialize 
-- @param self
-- @param #int unit_id
        
--------------------------------
-- 
-- @function [parent=#AIUnit] walkFrameOnPath 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] canAttackBuilding 
-- @param self
-- @param #float building_x
-- @param #float building_y
-- @param #float building_radius
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] inDirectView 
-- @param self
-- @param #AIUnit other
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_unit_camp 
-- @param self
-- @param #string var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_priority 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] isPathOutdated 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_unit_id 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_collide_radius 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_max_walk_length 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] walkStep 
-- @param self
-- @param #float step_x
-- @param #float step_y
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] get_dir_want_to_go 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- 
-- @function [parent=#AIUnit] resetLastWalk 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#AIUnit] set_attack_radius 
-- @param self
-- @param #float var
        
--------------------------------
-- 
-- @function [parent=#AIUnit] AIUnit 
-- @param self
        
return nil
