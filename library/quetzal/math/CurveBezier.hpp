#if !defined(QUETZAL_MATH_CURVEBezier_HPP)
#define QUETZAL_MATH_CURVEBezier_HPP
//------------------------------------------------------------------------------
// math
// CurveBezier.hpp
//------------------------------------------------------------------------------

#include "Curve.hpp"

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename T, typename U = T::component_type>
    class CurveBezier : public Curve
    {
    public:

        typedef std::vector<point_type> control_points_type;

        CurveBezier() = default;
        CurveBezier(const Curve&) = default;
        CurveBezier(const Curve&&) = default;
        virtual ~CurveBezier() = default;

        CurveBezier operator=(const CurveBezier&) = default;
        CurveBezier operator=(const CurveBezier&&) = default;

        virtual point_type operator()(parameter_type t) const;

        // Expose directly so that the container's interface can be used to its full extent
        control_points_type& control_points();
        const control_points_type& control_points() const;

    private:

        control_points_type m_control_points;
    };

} // namespace quetzal::math

//------------------------------------------------------------------------------
quetzal::math::CurveBezier::point_type quetzal::math::CurveBezier::operator()(parameter_type t) const
{
    assert(t >= static_cast<parameter_type>(0));
    assert(t <= static_cast<parameter_type>(1));

    ...

    return ...
}

//------------------------------------------------------------------------------
quetzal::math::CurveBezier::control_points_type& quetzal::math::CurveBezier::control_points()
{
    return m_control_points;
}

//------------------------------------------------------------------------------
const quetzal::math::CurveBezier::control_points_type& quetzal::math::CurveBezier::control_points() const
{
    return m_control_points;
}

#endif // QUETZAL_MATH_CURVEBezier_HPP
