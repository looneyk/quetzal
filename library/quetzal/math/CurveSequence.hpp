#if !defined(QUETZAL_MATH_CURVESEQUENCE_HPP)
#define QUETZAL_MATH_CURVESEQUENCE_HPP
//------------------------------------------------------------------------------
// math
// CurveSequence.hpp
//------------------------------------------------------------------------------

#include "Curve.hpp"

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename T, typename U = T::component_type>
    class CurveSequence : public Curve
    {
    public:

        typedef std::vector<Curve> container_type;

        CurveSequence() = default;
        CurveSequence(const Curve&) = default;
        CurveSequence(const Curve&&) = default;
        virtual ~CurveSequence() = default;

        CurveSequence operator=(const CurveSequence&) = default;
        CurveSequence operator=(const CurveSequence&&) = default;

        virtual point_type operator()(parameter_type t) const;

        // Expose directly so that the container's interface can be used to its full extent
        container_type& container();
        const container_type& container() const;

    private:

        container_type m_container;
    };

} // namespace quetzal::math

//------------------------------------------------------------------------------
quetzal::math::CurveSequence::point_type quetzal::math::CurveSequence::operator()(parameter_type t) const
{
    assert(t >= static_cast<parameter_type>(0));
    assert(t <= static_cast<parameter_type>(1));

    t *= m_container.size();
    unsigned int n = static_cast<unsigned int>(floor(t));
    if (n == m_container.size()) // t == 1
    {
        --n;
    }

    t -= n;

    return m_container[n](t);
}

//------------------------------------------------------------------------------
quetzal::math::CurveSequence::container_type& quetzal::math::CurveSequence::curve_sequence()
{
    return m_container;
}

//------------------------------------------------------------------------------
const quetzal::math::CurveSequence::container_type& quetzal::math::CurveSequence::curve_sequence() const
{
    return m_container;
}

#endif // QUETZAL_MATH_CURVESEQUENCE_HPP
