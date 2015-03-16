MathExtend = MathExtend or {}
local math = require 'math'
local scale_factor_constant = 1.74

function MathExtend.deltaFromCenter( radius, dir )
    return cc.p( dir.x * radius, dir.y * radius / scale_factor_constant )
end

function MathExtend.randomPositionFromCenter( center, radius )
    local dir = MathExtend.randomDir()
    local r = ue.LuaUtils:randomFloat() * radius
    return cc.p( dir.x * r + center.x, dir.y * r / scale_factor_constant + center.y )
end

function MathExtend.positionFromCenter( center, radius, dir )
    return cc.pAdd( center, MathExtend.deltaFromCenter( radius, dir ) ) 
end

function MathExtend.isPositionInRange( center, range, pos )
    local dx = pos.x - center.x
    local dy = ( pos.y - center.y ) * scale_factor_constant
    return dx * dx + dy * dy < range * range
end

function MathExtend.deviateMoreThan( vec, base_dir, angle)
    return math.deg( math.acos( ( vec.x * base_dir.x + vec.y * base_dir.y ) / math.sqrt( cc.pLengthSQ( base_dir ) * cc.pLengthSQ( vec ) ) ) ) > angle
end

function MathExtend.isPointInSector( pt, origin, dir, radius, angle )
    if cc.pDistanceSQ( pt, origin ) > radius * radius then
        return false
    end

    local vec = cc.pSub( pt, origin )
    if MathExtend.deviateMoreThan( vec, dir, angle / 2) then
        return false
    end

    return true
end 

function MathExtend.pedal( lineSrc, lineDst, pt )
    local a = lineDst.y - lineSrc.y
    local b = lineSrc.x - lineDst.x
    local c = lineDst.x * lineSrc.y - lineSrc.x * lineDst.y
    local asq = a * a
    local bsq = b * b
    local d = a * pt.y - b * pt.x
    local x = ( bsq * pt.x - a * b * pt.y - a * c )  / ( asq + bsq )
    local y = ( -a * b * pt.x + asq * pt.y - b * c ) / ( asq + bsq )
    return x, y 
end 

function MathExtend.anticlockwisePerpendicularVecToLine(line)
    return cc.p( -line.y, line.x )
end

function MathExtend.interp(a, b, ratio)
    return a + (b - a) * ratio
end

function MathExtend.fuzzyEqual(a, b, fuzzy_threshold)
    return math.abs(a - b) < fuzzy_threshold
end

function MathExtend.randomDir()
    local angle = ue.LuaUtils:randomFloat() * math.pi * 2.0
    return cc.p( math.cos( angle ), math.sin( angle ) )
end

function MathExtend.randomUnitDir()
    local x_sqr = ue.LuaUtils:randomFloat()
    local y_sqr = 1 - x_sqr
    return math.sqrt(x_sqr), math.sqrt(y_sqr)
end
