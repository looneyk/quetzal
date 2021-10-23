#if !defined(QUETZAL_GEOMETRY_RELATIONSHIPS_HPP)
#define QUETZAL_GEOMETRY_RELATIONSHIPS_HPP
//------------------------------------------------------------------------------
// geometry
// relationships.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include <concepts>

namespace quetzal::geometry
{

    template<typename T>
    concept Directional = requires (T t)
    {
        typename T::traits_type;
        { t.direction() } -> std::same_as<math::Vector<typename T::traits_type>>;
    };

    template<typename T>
    concept Planar = requires (T t)
    {
        typename T::traits_type;
        T::traits_type::dimension >= 3;
        { t.normal() } -> std::same_as<math::Vector<typename T::traits_type>>;
    };

    template<typename T, typename U>
    concept MatchingTraits = requires (T, U)
    {
        typename T::traits_type;
        typename U::traits_type;
        { std::same_as<typename T::traits_type, typename U::traits_type> };
    };

    // concept Spherical requires radius allows for generalized tangent ...

    // why does implementation work here but not outside namespaces? ...

    template<Directional T, Directional U> requires MatchingTraits<T, U>
    bool parallel(const T& t, const U& u)
    {
        if constexpr (T::traits_type::dimension == 3)
        {
            return vector_eq0(cross(t.direction(), u.direction()));
        }
        else if constexpr (T::traits_type::dimension == 2)
        {
            return math::float_eq0(dot_perp(t.direction(), u.direction()));
        }
    }

    template<Directional T, Planar U> requires MatchingTraits<T, U>
    bool parallel(const T& t, const U& u)
    {
        return math::float_eq0(dot(t.direction(), u.normal()));
    }

    template<Planar T, Directional U> requires MatchingTraits<T, U>
    bool parallel(const T& t, const U& u)
    {
        return math::float_eq0(dot(t.normal(), u.direction()));
    }

    template<Planar T, Planar U> requires MatchingTraits<T, U>
    bool parallel(const T& t, const U& u)
    {
        return vector_eq0(cross(t.normal(), u.normal()));
    }

    template<Directional T, Directional U> requires MatchingTraits<T, U>
    bool perpendicular(const T& t, const U& u)
    {
        return math::float_eq0(dot(t.direction(), u.direction())) && intersects(t, u);
    }

    template<Directional T, Planar U> requires MatchingTraits<T, U>
    bool perpendicular(const T& t, const U& u)
    {
        return vector_eq0(cross(t.direction(), u.normal()));
    }

    template<Planar T, Directional U> requires MatchingTraits<T, U>
    bool perpendicular(const T& t, const U& u)
    {
        return vector_eq0(cross(t.normal(), u.direction()));
    }

    template<Planar T, Planar U> requires MatchingTraits<T, U>
    bool perpendicular(const T& t, const U& u)
    {
        return math::float_eq0(dot(t.normal(), u.normal()));
    }

} // namespace quetzal::geometry
/*
template<quetzal::geometry::Directional T, quetzal::geometry::Directional U> requires quetzal::geometry::MatchingTraits<T, U>
bool quetzal::geometry::parallel(const T& t, const U& u)
{
    if constexpr (T::traits_type::dimension == 3)
        return vector_eq0(cross(t.direction(), u.direction()));
    else if constexpr (T::traits_type::dimension == 2)
        return math::float_eq0(dot_perp(t.direction(), u.direction()));
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Directional T, quetzal::geometry::Planar U>
bool quetzal::geometry::parallel<T, U>(const T& t, const U& u)
{
    return float_eq0(dot(t.direction(), u.normal()));
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Planar T, quetzal::geometry::Directional U>
bool quetzal::geometry::parallel<T, U>(const T& t, const U& u)
{
    return float_eq0(dot(t.normal(), u.direction()));
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Planar T, quetzal::geometry::Planar U>
bool quetzal::geometry::parallel<T, U>(const T& t, const U& u)
{
    return vector_eq0(cross(t.normal(), u.normal()));
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Directional T, quetzal::geometry::Directional U>
bool quetzal::geometry::perpendicular<T, U>(const T& t, const U& u)
{
    return float_eq0(dot(t.direction(), u.direction())) && intersects(t, u);
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Directional T, quetzal::geometry::Planar U>
bool quetzal::geometry::perpendicular<T, U>(const T& t, const U& u)
{
    return vector_eq0(cross(t.direction(), u.normal()));
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Planar T, quetzal::geometry::Directional U>
bool quetzal::geometry::perpendicular<T, U>(const T& t, const U& u)
{
    return vector_eq0(cross(t.normal(), u.direction()));
}

//------------------------------------------------------------------------------
template<quetzal::geometry::Planar T, quetzal::geometry::Planar U>
bool quetzal::geometry::perpendicular<T, U>(const T& t, const U& u)
{
    return float_eq0(dot(t.normal(), u.normal()));
}
*/
#endif // QUETZAL_GEOMETRY_RELATIONSHIPS_HPP
