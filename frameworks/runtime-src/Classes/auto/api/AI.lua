
--------------------------------
-- @module AI
-- @parent_module ue

--------------------------------
-- 
-- @function [parent=#AI] reset 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#AI] isPositionOK 
-- @param self
-- @param #vec2_table pos
-- @param #float radius
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AI] releaseCurrentMap 
-- @param self
        
--------------------------------
-- @overload self, string         
-- @overload self, string, array_table, array_table         
-- @function [parent=#AI] addCollidablePolygon
-- @param self
-- @param #string name
-- @param #array_table points_x
-- @param #array_table points_y

--------------------------------
-- 
-- @function [parent=#AI] test2_int 
-- @param self
-- @param #int t
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#AI] get_fell_behind_group 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#AI] get_catch_up_position 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- 
-- @function [parent=#AI] splitIntoTwoGroups 
-- @param self
-- @param #string player_camp
-- @param #float dir_x
-- @param #float dir_y
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AI] test1_pos 
-- @param self
-- @param #int t
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- 
-- @function [parent=#AI] updateFrame 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#AI] getAvailablePosition 
-- @param self
-- @param #array_table radius
-- @param #rect_table region
-- @param #int group_count
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#AI] get_current_frame 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#AI] paintDebugLayer 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#AI] get_in_narrow 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AI] removeCollidablePolygon 
-- @param self
-- @param #string name
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AI] initialize 
-- @param self
-- @param #map_table constants
        
--------------------------------
-- 
-- @function [parent=#AI] get_lead_group 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#AI] parseMap 
-- @param self
-- @param #string tmx_file_content
-- @param #array_table all_collide_radius
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#AI] test_c_whole 
-- @param self
-- @param #int test_id
        
--------------------------------
-- 
-- @function [parent=#AI] getInstance 
-- @param self
-- @return AI#AI ret (return value: AI)
        
--------------------------------
-- 
-- @function [parent=#AI] AI 
-- @param self
        
return nil
