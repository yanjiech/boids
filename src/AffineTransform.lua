AffineTransform = {}
AffineTransform.identity = {a=1,b=0,c=0,d=1,tx=0,ty=0}
AffineTransform.translate = function(t, tx, ty)
    return {a=t.a,b=t.b,c=t.c,d=t.d,tx=t.tx+t.a*tx+t.c*ty,ty=t.ty+t.b*tx+t.d*ty}
end
--TODO other affinetransform functions

return AffineTransform
