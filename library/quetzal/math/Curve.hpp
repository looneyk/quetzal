#if !defined(QUETZAL_MATH_CURVE_HPP)
#define QUETZAL_MATH_CURVE_HPP
//------------------------------------------------------------------------------
// math
// Curve.hpp
// Abstract base class, common interface for curves of various types
//------------------------------------------------------------------------------

#include <vector>

namespace quetzal::math
{

    // ParametricCurve
    // multiple sections? here of in a higher level class?
    // dimension part of T ...
    // bezier, interpolated points, multi/connected, ...

    //--------------------------------------------------------------------------
    template<typename T, typename U = T::component_type>
    class Curve
    {
    public:

        typedef T point_type; // curve point type
        typedef U parameter_type; // curve parameter type
        typedef point_type::component_type component_type; // curve point component type

        Curve() = default;
        Curve(const Curve&) = default;
        Curve(const Curve&&) = default;
        virtual ~Curve() = default;

        Curve operator=(const Curve&) = default;
        Curve operator=(const Curve&&) = default;

        virtual point_type operator()(parameter_type t) const = 0;
    };

} // namespace quetzal::math

#endif // QUETZAL_MATH_CURVE_HPP
