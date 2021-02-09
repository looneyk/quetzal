#if !defined(QUETZAL_GEOMETRY_ORIENTATION_HPP)
#define QUETZAL_GEOMETRY_ORIENTATION_HPP
//------------------------------------------------------------------------------
// geometry
// Orientation.hpp
//------------------------------------------------------------------------------

#include "math/Matrix.hpp"
#include "math/Vector.hpp"

namespace quetzal
{

namespace geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Orientation
    {
    public:

		static_assert(Traits::dimension >= 3);

        using value_type = typename Traits::value_type;
        using vector_type = math::Vector<Traits>;

        Orientation();
        Orientation(const vector_type& up, const vector_type& right);
        Orientation(const Orientation&) = default;
        ~Orientation() = default;

        Orientation& operator=(const Orientation&) = default;

        vector_type up() const;
        vector_type& up();
        void set_up(const vector_type& v);

        vector_type right() const;
        vector_type& right();
        void set_right(const vector_type& v);

        // Reorients right so that it is perpendicular to up and normalizes the resulting vectors
        void normalize();

        // Sets up and right to align with z-axis and x-axis respectively
        void clear();

    private:

        vector_type m_up;
        vector_type m_right;
    };

    // only valid for square matrices, consider removal ...
    template<typename Traits>
    Orientation<Traits>& operator*=(Orientation<Traits>& lhs, const math::Matrix<typename Traits::value_type>& rhs);

    // only valid for square matrices, consider removal ...
    template<typename Traits>
    Orientation<typename Traits::reduced_traits>& operator*=(Orientation<typename Traits::reduced_traits>& lhs, const math::Matrix<typename Traits::value_type>& rhs);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Orientation<Traits>::Orientation() :
    Orientation({Traits::val(0), Traits::val(0), Traits::val(1)}, {Traits::val(1), Traits::val(0), Traits::val(0)})
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Orientation<Traits>::Orientation(const vector_type& up, const vector_type& right) :
    m_up(up),
    m_right(right)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Orientation<Traits>::vector_type quetzal::geometry::Orientation<Traits>::up() const
{
    return m_up;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Orientation<Traits>::vector_type& quetzal::geometry::Orientation<Traits>::up()
{
    return m_up;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Orientation<Traits>::set_up(const vector_type& v)
{
    m_up = v;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Orientation<Traits>::vector_type quetzal::geometry::Orientation<Traits>::right() const
{
    return m_right;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Orientation<Traits>::vector_type& quetzal::geometry::Orientation<Traits>::right()
{
    return m_right;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Orientation<Traits>::set_right(const vector_type& v)
{
    m_right = v;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Orientation<Traits>::normalize()
{
    auto front = cross(m_up, m_right);
    m_right = math::normalize(cross(front, m_up));
    math::normalize(m_up);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Orientation<Traits>::clear()
{
    m_up = {Traits::val(0), Traits::val(0), Traits::val(1)};
    m_right = {Traits::val(1), Traits::val(0), Traits::val(0)};
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Orientation<Traits>& quetzal::geometry::operator*=(Orientation<Traits>& lhs, const math::Matrix<typename Traits::value_type>& rhs)
{
    lhs.up() *= rhs;
    lhs.right() *= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Orientation<typename Traits::reduced_traits>& quetzal::geometry::operator*=(Orientation<typename Traits::reduced_traits>& lhs, const math::Matrix<typename Traits::value_type>& rhs)
{
    lhs.up() *= rhs;
    lhs.right() *= rhs;
    return lhs;
}

#endif // QUETZAL_GEOMETRY_ORIENTATION_HPP
