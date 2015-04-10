#ifndef __Boids_Math__
#define __Boids_Math__

#include "cocos2d.h"
#include <math.h>
#include "BoostGeometryDef.h"

class Fuzzy
{
public:
//#define FUZZY_THRESHOLD (2e-4f)
#define FUZZY_THRESHOLD (1e-2f)

	inline static int side(float v) 
	{
		if (_equal(v, 0.0f))
		{
			return 0;
		}
		if (_greater(v, 0.0f))
		{
			return 1;
		}
		return -1;
	}

	inline static bool _equal(float x, float y)
	{
		return fabs(x - y) <= FUZZY_THRESHOLD; 
	}

	inline static bool _greater(float x, float y)
	{
		return x - y > FUZZY_THRESHOLD;
	}

	inline static bool _less(float x, float y)
	{
		return y - x > FUZZY_THRESHOLD;
	}

	inline static bool _equalGreater(float x, float y)
	{
		return x >= y - FUZZY_THRESHOLD;
	}

	inline static bool _equalLess(float x, float y)
	{
		return y >= x - FUZZY_THRESHOLD;
	}
};

class FuzzyLoose
{
public:
#define FUZZY_LOOSE_THRESHOLD (1e-2f)

	inline static bool _equal(float x, float y)
	{
		return fabs(x - y) <= FUZZY_LOOSE_THRESHOLD;
	}

	inline static bool _greater(float x, float y)
	{
		return x - y > FUZZY_LOOSE_THRESHOLD;
	}

	inline static bool _less(float x, float y)
	{
		return y - x > FUZZY_LOOSE_THRESHOLD;
	}

	inline static bool _equalGreater(float x, float y)
	{
		return x >= y - FUZZY_LOOSE_THRESHOLD;
	}

	inline static bool _equalLess(float x, float y)
	{
		return y >= x - FUZZY_LOOSE_THRESHOLD;
	}
};

#define PT_INVALID (cocos2d::Point(FLT_MAX, FLT_MAX))

class Line
{
public:
	cocos2d::Point src, dst;

	Line(cocos2d::Point s, cocos2d::Point t) : src(s), dst(t)
	{
	}
};

#define DISTANCE_Y_FACTOR 1.74f
class Math
{
public:
	static float interp(float a, float b, float ratio)
	{
		return a + (b - a) * ratio;
	}
    
    static cocos2d::Point bezierTo( float t, const cocos2d::Point& p1, const cocos2d::Point& cp1, const cocos2d::Point& cp2, const cocos2d::Point& p2 ) {
        float x = p1.x * powf( ( 1.0f - t ), 3.0f ) + 3 * t * cp1.x * powf( ( 1.0f - t ), 2.0f ) + 3 * cp2.x * powf( t, 2.0f ) * ( 1.0f - t ) + p2.x * powf( t, 3.0f );
        float y = p1.y * powf( ( 1.0f - t ), 3.0f ) + 3 * t * cp1.y * powf( ( 1.0f - t ), 2.0f ) + 3 * cp2.y * powf( t, 2.0f ) * ( 1.0f - t ) + p2.y * powf( t, 3.0f );
        return cocos2d::Point( x, y );
    }
    
    static bool deviateMoreThan( const cocos2d::Point& vec, const cocos2d::Point& dir, float angle ) {
        return CC_RADIANS_TO_DEGREES( acosf( ( vec.x * dir.x + vec.y * dir.y ) / dir.length() / vec.length() ) ) > angle;
    }
    
    static bool isPointInSector( const cocos2d::Point& pos, const cocos2d::Point& center, const cocos2d::Point& dir, float radius, float angle ) {
        if( pos.distance( center ) > radius ) {
            return false;
        }
        cocos2d::Point bias = pos - center;
        
        return !Math::deviateMoreThan( bias, dir, angle / 2 );
    }
    
    static bool isPositionInRange( const cocos2d::Point& pos, const cocos2d::Point& center, float range ) {
        float dx = pos.x - center.x;
        float dy = ( pos.y - center.y ) * DISTANCE_Y_FACTOR;
        return dx * dx + dy * dy <= range * range;
    }
};

class Geometry
{
public:
	static float distanceToLine(const cocos2d::Point& pt, const cocos2d::Point& src, const cocos2d::Point& dst)
	{
		assert(!(src.x == dst.x && src.y == dst.y));

		float a = dst.y - src.y;
		float b = src.x - dst.x;
		float c = dst.x * src.y - src.x * dst.y;
		return abs(a * pt.x + b * pt.y + c) / sqrt(a * a + b * b);
	}

	static float distanceSqrToLine(const cocos2d::Point& pt, const cocos2d::Point& src, const cocos2d::Point& dst)
	{
		assert(!(src.x == dst.x && src.y == dst.y));

		float a = dst.y - src.y;
		float b = src.x - dst.x;
		float c = dst.x * src.y - src.x * dst.y;
		float to_sqr = a * pt.x + b * pt.y + c;
		return to_sqr / (a * a + b * b) * to_sqr;
	}

	static cocos2d::Vec2 distanceVecToLine(const cocos2d::Point& pt, const cocos2d::Point& src, const cocos2d::Point& dst)
	{
		cocos2d::Vec2 perpendicular_vec = Geometry::clockwisePerpendicularVecToLine(cocos2d::Vec2(src, dst));
		float s, t;
		if (cocos2d::Vec2::isLineIntersect(src, dst, pt, pt + perpendicular_vec, &s, &t))
		{
			if (Fuzzy::_equalGreater(t, 0.0f))
			{
				if (Fuzzy::_equalGreater(s, 0.0f) && Fuzzy::_equalLess(s, 1.0f))
				{
					return perpendicular_vec * t;
				}
				else
				{
					if (Fuzzy::_less(s, 0.0f))
					{
						return cocos2d::Vec2(pt, src);
					}
					else
					{
						return cocos2d::Vec2(pt, dst);
					}
				}
			}
			else
			{
				return -cocos2d::Vec2::ONE;
			}
		}
		else
		{
			assert(false && "perpendicular_vec parallel with vec");
			return -cocos2d::Vec2::ONE;
		}
	}

	static box getBoundingBox(cocos2d::Point a, cocos2d::Point b)
	{		
		return box(point(std::min(a.x, b.x), std::min(a.y, b.y)), point(std::max(a.x, b.x), std::max(a.y, b.y)));
	}

	static cocos2d::Vec2 anticlockwisePerpendicularVecToLine(const cocos2d::Vec2 v)
	{
		return cocos2d::Vec2(-v.y, v.x);
	}

	static cocos2d::Vec2 clockwisePerpendicularVecToLine(const cocos2d::Vec2 v)
	{
		return cocos2d::Vec2(v.y, -v.x);
	}

	static cocos2d::Vec2 anticlockwiseRotate(const cocos2d::Vec2 v, float cos_alpha, float sin_alpha)
	{
		return cocos2d::Vec2(v.x * cos_alpha - v.y * sin_alpha, v.x * sin_alpha + v.y * cos_alpha);
	}

	static cocos2d::Vec2 anticlockwiseRotate(const cocos2d::Vec2 v, float radian)
	{
		return anticlockwiseRotate(v, cos(radian), sin(radian));
	}

	static cocos2d::Vec2 anticlockwiseRotateDegree(const cocos2d::Vec2 v, float degree)
	{
		return anticlockwiseRotate(v, degree / 180.0f * M_PI);
	}

	static cocos2d::Vec2 anticlockwiseRotate60(const cocos2d::Vec2 v)
	{
		return anticlockwiseRotate(v, 0.5f, 0.8660254f);
	}

	static cocos2d::Vec2 clockwiseRotate60(const cocos2d::Vec2 v)
	{
		return anticlockwiseRotate(v, 0.5f, -0.8660254f);
	}
	
	static cocos2d::Vec2 anticlockwiseRotate45(const cocos2d::Vec2 v)
	{
		return anticlockwiseRotate(v, 0.7071068f, 0.7071068f);
	}

	static cocos2d::Vec2 clockwiseRotate45(const cocos2d::Vec2 v)
	{
		return anticlockwiseRotate(v, 0.7071068f, -0.7071068f);
	}

	static cocos2d::Vec2 anticlockwiseRotate1(const cocos2d::Vec2 v)
	{
		return anticlockwiseRotate(v, 0.9998477f, 0.0174524f);
	}

	static cocos2d::Vec2 clockwiseRotate1(const cocos2d::Vec2 v)
	{
		return anticlockwiseRotate(v, 0.9998477f, -0.0174524f);
	}

	static bool deviateLessThan90(const cocos2d::Vec2 original_dir, const cocos2d::Vec2 new_dir)
	{
		if (Fuzzy::_greater(original_dir.cross(new_dir), 0.0f))
		{
			if (Fuzzy::_equalGreater(Geometry::anticlockwisePerpendicularVecToLine(original_dir).cross(new_dir), 0.0f))
			{
				return false;
			}
		}
		else
		{
			if (Fuzzy::_equalLess(Geometry::clockwisePerpendicularVecToLine(original_dir).cross(new_dir), 0.0f))
			{
				return false;
			}
		}
		return true;
	}

	static bool deviateLessThan60(const cocos2d::Vec2 original_dir, const cocos2d::Vec2 new_dir)
	{
		if (Fuzzy::_greater(original_dir.cross(new_dir), 0.0f))
		{
			if (Fuzzy::_equalGreater(Geometry::anticlockwiseRotate60(original_dir).cross(new_dir), 0.0f))
			{
				return false;
			}
		}
		else
		{
			if (Fuzzy::_equalLess(Geometry::clockwiseRotate60(original_dir).cross(new_dir), 0.0f))
			{
				return false;
			}
		}
		return true;
	}

	static bool deviateLessThanAngle(const cocos2d::Vec2 original_dir, const cocos2d::Vec2 new_dir, float angle)
	{
		if (Fuzzy::_greater(original_dir.cross(new_dir), 0.0f))
		{
			if (Fuzzy::_equalGreater(Geometry::anticlockwiseRotateDegree(original_dir, angle).cross(new_dir), 0.0f))
			{
				return false;
			}
		}
		else
		{
			if (Fuzzy::_equalLess(Geometry::anticlockwiseRotateDegree(original_dir, -angle).cross(new_dir), 0.0f))
			{
				return false;
			}
		}
		return true;
	}

	static bool clockwiseCloser(cocos2d::Point origin, cocos2d::Point base, cocos2d::Point x, cocos2d::Point y)
	{
		cocos2d::Vec2 base_vec(origin, base);
		cocos2d::Vec2 vec_x(origin, x);
		cocos2d::Vec2 vec_y(origin, y);

		int side_x = Fuzzy::side(base_vec.cross(vec_x));
		int side_y = Fuzzy::side(base_vec.cross(vec_y));

		if (side_y == 0 && base_vec.dot(vec_y) > 0)
		{
			return false;
		}

		if (side_x == 0 && base_vec.dot(vec_x) > 0)
		{
			return true;
		}

		if (side_x != side_y)
		{
			return side_x < side_y;
		}
		else
		{
			if (side_x == 0) return false;

			return Fuzzy::side(vec_x.cross(vec_y)) == -1;
		}		
	}

	static void runTest();
};

#endif