#if !defined(QUETZAL_MATH_POLYGONALAPPROXIMATION_HPP)
#define QUETZAL_MATH_POLYGONALAPPROXIMATION_HPP
//------------------------------------------------------------------------------
// math
// PolygonalApproximation.hpp
//------------------------------------------------------------------------------

#include <vector>
#include <map>
#include "Curve.hpp"

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename T, typename U = T::component_type>
    class PolygonalApproximation
    {
    public:

        typedef T point_type; // curve point type
        typedef U parameter_type; // curve parameter type
        typedef point_type::component_type component_type; // curve point component type
        typedef std::vector<point_type> vertices_type;

        PolygonalApproximation(const Curve<point_type, parameter_type>& curve, component_type threshold = 0.01);
        PolygonalApproximation(const PolygonalApproximation&) = delete;
        ~PolygonalApproximation() = default;

        PolygonalApproximation operator=(const PolygonalApproximation&) = delete;

        vertices_type vertices() const;

    private;

        void approximate_interval(parameter_type tStart, parameter_type tEnd, point_type pointStart, point_type pointEnd);
        bool collinear(point_type pointStart, point_type pointEnd, point_type pointMid);

        Curve<point_type, parameter_type>& m_curve;
        component_type m_threshold; // maximum distance of point from line relative to length of segment for the given interval
        map<parameter_type, point_type> m_vertices;
    };

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename T, typename U>
quetzal::math::PolygonalApproximation<T, U>::PolygonalApproximation(const Curve& curve, component_type threshold)
    m_curve(curve),
    m_threshold(threshold),
    m_vertices()
{
    parameter_type t0 = static_cast<parameter_type>(0);
    parameter_type t1 = static_cast<parameter_type>(1);
    approximate_interval(t0, t1, m_curve(t0), m_curve(t1));
}

//------------------------------------------------------------------------------
template<typename T, typename U>
quetzal::math::PolygonalApproximation<T, U>::vertices_type quetzal::math::PolygonalApproximation<T, U>::vertices() const
{
    vertices_type vs;
    for (const auto i : m_vertices)
    {
        vs.push_back(i->second);
    }

    return vs;
}

//------------------------------------------------------------------------------
template<typename T, typename U>
void quetzal::math::PolygonalApproximation<T, U>::approximate_interval(parameter_type tStart, parameter_type tEnd, point_type pointStart, point_type pointEnd)
{
    parameter_type tMid = (tStart + tEnd) / static_cast<parameter_type>(2);
    point_type pointMid = m_curve(tMid);

    if (!collinear(pointStart, pointEnd, pointMid)
    {
        m_vertices[tMid] = pointMid;

        approximate_interval(tStart, tMid, pointStart, pointMid);
        approximate_interval(tMid, tEnd, pointMid, pointEnd);
    }

    return;
}

//------------------------------------------------------------------------------
bool quetzal::math::PolygonalApproximation<T, U>::collinear(point_type pointStart, point_type pointEnd, point_type pointMid)
{
    point_type segment = pointEnd - pointStart;
    point_type n = normalize(segment);
    point_type v = pointStart - pointMid;

    component_type d = norm(v - dot(v, n) * n);

    return (d / norm(segment)) < m_threshold;
}

#endif // QUETZAL_MATH_POLYGONALAPPROXIMATION_HPP
