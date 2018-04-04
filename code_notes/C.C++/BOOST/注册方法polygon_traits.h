#pragma once
#include "boost/polygon/polygon.hpp"
#include "boost/geometry/geometries/register/point.hpp"
#include "boost/geometry/geometries/register/ring.hpp"
#include "base_define.h"

namespace boost
{
    namespace polygon
    {
        template<>
        struct geometry_concept<TGS::Algebra::Point3D> { typedef point_concept type; };

        template<>
        struct point_traits<TGS::Algebra::Point3D>
        {
            typedef double coordinate_type;

            static coordinate_type get(const TGS::Algebra::Point3D& point, orientation_2d orient)
            {
                return (orient == HORIZONTAL) ? point[TGS::Algebra::X] : point[TGS::Algebra::Y]; 
            }
        };

        template <>
        struct geometry_concept<ContourLine>{ typedef polygon_concept type; };

        template<>
        struct polygon_traits<ContourLine>
        {
            typedef double coordinate_type;
            typedef ContourLine::const_iterator iterator_type;
            typedef TGS::Algebra::Point3D point_type;
            // Get the begin iterator
            static inline iterator_type begin_points(const ContourLine& t)
            {
                return t.begin();
            }

            // Get the end iterator
            static inline iterator_type end_points(const ContourLine& t)
            {
                return t.end();
            }
            // Get the number of sides of the polygon
            static inline std::size_t size(const ContourLine& t)
            {
                return t.size();
            }
        };
    }
}

/// <summary>
/// 注册二维点类型
/// </summary>
BOOST_GEOMETRY_REGISTER_POINT_2D(TGS::Algebra::Point2D, double, boost::geometry::cs::cartesian, 
                                 operator[](TGS::Algebra::X), operator[](TGS::Algebra::Y));
BOOST_GEOMETRY_REGISTER_POINT_2D(TGS::Algebra::Point3D, double, boost::geometry::cs::cartesian, 
                                 operator[](TGS::Algebra::X), operator[](TGS::Algebra::Y));
/// <summary>
/// 注册环类型
/// </summary>
BOOST_GEOMETRY_REGISTER_RING(ContourLine);
