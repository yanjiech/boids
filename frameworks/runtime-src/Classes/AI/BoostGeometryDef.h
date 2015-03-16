#ifndef __Boids_BoostGeometryDef_
#define __Boids_BoostGeometryDef_

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/foreach.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::segment<point> segment;
typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
typedef bg::model::box<point> box;

#endif