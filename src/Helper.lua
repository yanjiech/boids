Helper = Helper or {}

function Helper.tableContainsValue( table, value )
    for _,v in ipairs( table ) do
        if v == value then
            return true
        end
    end
    return false
end

function Helper.tableRemoveObject( t, obj )
    local i = 0
    for j,v in pairs( t ) do
        if v == obj then
            i = j
            break
        end
    end
    if i == 0 then
        return false
    else
        print( "tableid:", i )
        table.remove( t, i )
        return true
    end
end
