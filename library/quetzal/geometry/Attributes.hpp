#if !defined(QUETZAL_GEOMETRY_ATTRIBUTES_HPP)
#define QUETZAL_GEOMETRY_ATTRIBUTES_HPP
//------------------------------------------------------------------------------
// geometry
// Attributes.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/Matrix.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include "quetzal/math/math_util.hpp"
#include "quetzal/math/transformation_matrix.hpp"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

namespace quetzal
{

namespace geometry
{

    namespace Flags
    {
        using type = unsigned int;

        enum : type
        {
            Position = 0x01,
            Normal = 0x02,
            Texcoord0 = 0x04,
            Texcoord1 = 0x08,
            Tangent = 0x10,
            Bitangent = 0x20
        };
    };

    namespace Attribute
    {
        using Types = unsigned int;

        enum : Types
        {
            Position = 0x01,
            Normal = 0x02,
            Texcoord0 = 0x04,
            Texcoord1 = 0x08,
            Tangent = 0x10,
            Bitangent = 0x20
        };

        // or ...
        //    static const type Position = 0x01;
        // in any case, not Types or Flags, but Attributes::contents or similar ...
    };


    //--------------------------------------------------------------------------
    template<typename Traits>
    class Attributes
    {
    public:

        using vector_traits = Traits;
        using texcoord_traits = typename vector_traits::reduced_traits;

        using value_type = typename Traits::value_type;
        using point_type = math::Vector<vector_traits>;
        using vector_type = math::Vector<vector_traits>;
        using texcoord_type = math::Vector<texcoord_traits>;
        using matrix_type = math::Matrix<value_type>; // N ...

        Attributes() = default;
        Attributes(const Attributes&) = default;
        virtual ~Attributes() = default;

        Attributes& operator=(const Attributes&) = default;

        virtual void transform(const matrix_type& matrixPosition) = 0;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) = 0;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord) = 0;
        virtual bool validate() const = 0;
        virtual bool validate(std::string& error) const = 0;

        // virtual instead of in addition?
        // free functions instead?

        static bool validate_normal(const vector_type& normal);
        static bool validate_normal(const vector_type& normal, std::string& error);

        static bool validate_texcoord(const texcoord_type& texcoord);
        static bool validate_texcoord(const texcoord_type& texcoord, std::string& error);

        static bool validate_tangent(const vector_type& tangent, std::string& error);
        static bool validate_tangent(const vector_type& tangent);

        static bool validate_bitangent(const vector_type& bitangent);
        static bool validate_bitangent(const vector_type& bitangent, std::string& error);
    };

    template<typename A, typename B>
    bool coincident(const A& a, const B& b);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Position : public Attributes<Traits>
    {
    public:

        using value_type = Attributes<Traits>::value_type;
        using point_type = Attributes<Traits>::point_type;
        using vector_type = Attributes<Traits>::vector_type;
        using texcoord_type = Attributes<Traits>::texcoord_type;
        using matrix_type = Attributes<Traits>::matrix_type;

        static constexpr Flags::type flags();

        Position() = default;
        Position(const point_type& position);
        Position(const Position&) = default;
        ~Position() = default;

        Position& operator=(const Position&) = default;

        point_type position() const;
        point_type& position();
        void set_position(const point_type& position);

        void set(const point_type& position);

        virtual void transform(const matrix_type& matrixPosition) override;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) override;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord = [](const texcoord_type& texcoord) -> texcoord_type {return texcoord;}) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& error) const override;

    private:

        static constexpr Flags::type m_flags = Flags::Position;

        point_type m_position;
    };

    template<typename Traits>
    Position<Traits> lerp(const Position<Traits>& a, const Position<Traits>& b, typename Position<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Position<Traits>& a);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class PositionNormal : public Attributes<Traits>
    {
    public:

        using value_type = Attributes<Traits>::value_type;
        using point_type = Attributes<Traits>::point_type;
        using vector_type = Attributes<Traits>::vector_type;
        using texcoord_type = Attributes<Traits>::texcoord_type;
        using matrix_type = Attributes<Traits>::matrix_type;

        static constexpr Flags::type flags();

        PositionNormal() = default;
        PositionNormal(const point_type& position, const vector_type& normal);
        PositionNormal(const PositionNormal&) = default;
        ~PositionNormal() = default;

        PositionNormal& operator=(const PositionNormal&) = default;

        point_type position() const;
        point_type& position();
        void set_position(const point_type& position);

        vector_type normal() const;
        vector_type& normal();
        void set_normal(const vector_type& normal);

        void set(const point_type& position, const vector_type& normal);

        virtual void transform(const matrix_type& matrixPosition) override;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) override;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord = [](const texcoord_type& texcoord) -> texcoord_type {return texcoord;}) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& error) const override;

    private:

        static constexpr Flags::type m_flags = Flags::Position | Flags::Normal;

        point_type m_position;
        vector_type m_normal;
    };

    template<typename Traits>
    PositionNormal<Traits> lerp(const PositionNormal<Traits>& a, const PositionNormal<Traits>& b, typename PositionNormal<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const PositionNormal<Traits>& a);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class PositionNormalTexture : public Attributes<Traits>
    {
    public:

        using value_type = Attributes<Traits>::value_type;
        using point_type = Attributes<Traits>::point_type;
        using vector_type = Attributes<Traits>::vector_type;
        using texcoord_type = Attributes<Traits>::texcoord_type;
        using matrix_type = Attributes<Traits>::matrix_type;

        static constexpr Flags::type flags();

        PositionNormalTexture() = default;
        PositionNormalTexture(const point_type& position, const vector_type& normal, const texcoord_type& texcoord);
        PositionNormalTexture(const PositionNormalTexture&) = default;
        ~PositionNormalTexture() = default;

        PositionNormalTexture& operator=(const PositionNormalTexture&) = default;

        point_type position() const;
        point_type& position();
        void set_position(const point_type& position);

        vector_type normal() const;
        vector_type& normal();
        void set_normal(const vector_type& normal);

        texcoord_type texcoord() const;
        texcoord_type& texcoord();
        void set_texcoord(const texcoord_type& texcoord);

        void set(const point_type& position, const vector_type& normal, const texcoord_type& texcoord);

        virtual void transform(const matrix_type& matrixPosition) override;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) override;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord = [](const texcoord_type& texcoord) -> texcoord_type {return texcoord;}) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& error) const override;

    private:

        static constexpr Flags::type m_flags = Flags::Position | Flags::Normal | Flags::Texcoord0;

        point_type m_position;
        vector_type m_normal;
        texcoord_type m_texcoord;
    };

    template<typename Traits>
    PositionNormalTexture<Traits> lerp(const PositionNormalTexture<Traits>& a, const PositionNormalTexture<Traits>& b, typename PositionNormalTexture<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const PositionNormalTexture<Traits>& a);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class PositionNormalTextureTangent : public Attributes<Traits>
    {
    public:

        using value_type = Attributes<Traits>::value_type;
        using point_type = Attributes<Traits>::point_type;
        using vector_type = Attributes<Traits>::vector_type;
        using texcoord_type = Attributes<Traits>::texcoord_type;
        using matrix_type = Attributes<Traits>::matrix_type;

        static constexpr Flags::type flags();

        PositionNormalTextureTangent() = default;
        PositionNormalTextureTangent(const point_type& position, const vector_type& normal, const texcoord_type& texcoord, const vector_type& tangent);
        PositionNormalTextureTangent(const PositionNormalTextureTangent&) = default;
        ~PositionNormalTextureTangent() = default;

        PositionNormalTextureTangent& operator=(const PositionNormalTextureTangent&) = default;

        point_type position() const;
        point_type& position();
        void set_position(const point_type& position);

        vector_type normal() const;
        vector_type& normal();
        void set_normal(const vector_type& normal);

        texcoord_type texcoord() const;
        texcoord_type& texcoord();
        void set_texcoord(const texcoord_type& texcoord);

        vector_type tangent() const;
        vector_type& tangent();
        void set_tangent(const vector_type& tangent);

        void set(const point_type& position, const vector_type& normal, const texcoord_type& texcoord, const vector_type& tangent);

        virtual void transform(const matrix_type& matrixPosition) override;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) override;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord = [](const texcoord_type& texcoord) -> texcoord_type {return texcoord;}) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& error) const override;

    private:

        static constexpr Flags::type m_flags = Flags::Position | Flags::Normal | Flags::Texcoord0 | Flags::Tangent;

        point_type m_position;
        vector_type m_normal;
        texcoord_type m_texcoord;
        vector_type m_tangent;
    };

    template<typename Traits>
    PositionNormalTextureTangent<Traits> lerp(const PositionNormalTextureTangent<Traits>& a, const PositionNormalTextureTangent<Traits>& b, typename PositionNormalTextureTangent<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const PositionNormalTextureTangent<Traits>& a);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Normal : public Attributes<Traits>
    {
    public:

        using value_type = Attributes<Traits>::value_type;
        using point_type = Attributes<Traits>::point_type;
        using vector_type = Attributes<Traits>::vector_type;
        using texcoord_type = Attributes<Traits>::texcoord_type;
        using matrix_type = Attributes<Traits>::matrix_type;

        static constexpr Flags::type flags();

        Normal() = default;
        Normal(const vector_type& normal);
        Normal(const Normal&) = default;
        ~Normal() = default;

        Normal& operator=(const Normal&) = default;

        vector_type normal() const;
        vector_type& normal();
        void set_normal(const vector_type& normal);

        void set(const vector_type& normal);

        virtual void transform(const matrix_type& matrixPosition) override;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) override;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord = [](const texcoord_type& texcoord) -> texcoord_type {return texcoord;}) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& error) const override;

    private:

        static constexpr Flags::type m_flags = Flags::Normal;

        vector_type m_normal;
    };

    template<typename Traits>
    Normal<Traits> lerp(const Normal<Traits>& a, const Normal<Traits>& b, typename Normal<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Normal<Traits>& a);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class NormalTangent : public Attributes<Traits>
    {
    public:

        using value_type = Attributes<Traits>::value_type;
        using point_type = Attributes<Traits>::point_type;
        using vector_type = Attributes<Traits>::vector_type;
        using texcoord_type = Attributes<Traits>::texcoord_type;
        using matrix_type = Attributes<Traits>::matrix_type;

        static constexpr Flags::type flags();

        NormalTangent() = default;
        NormalTangent(const vector_type& normal, const vector_type& tangent);
        NormalTangent(const NormalTangent&) = default;
        ~NormalTangent() = default;

        NormalTangent& operator=(const NormalTangent&) = default;

        vector_type normal() const;
        vector_type& normal();
        void set_normal(const vector_type& normal);

        vector_type tangent() const;
        vector_type& tangent();
        void set_tangent(const vector_type& tangent);

        void set(const vector_type& normal, const vector_type& tangent);

        virtual void transform(const matrix_type& matrixPosition) override;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) override;
        virtual void invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord = [](const texcoord_type& texcoord) -> texcoord_type {return texcoord;}) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& error) const override;

    private:

        static constexpr Flags::type m_flags = Flags::Normal | Flags::Tangent;

        vector_type m_normal;
        vector_type m_tangent;
    };

    template<typename Traits>
    NormalTangent<Traits> lerp(const NormalTangent<Traits>& a, const NormalTangent<Traits>& b, typename NormalTangent<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const NormalTangent<Traits>& a);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_normal(const vector_type& normal)
{
    return math::float_eq(normal.norm_squared(), value_type(1));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_normal(const vector_type& normal, std::string& error)
{
    if (validate_normal(normal))
    {
        return true;
    }

    std::ostringstream oss;
    oss << "|normal| != 1: " << normal;
    error = oss.str();
    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_texcoord(const texcoord_type& texcoord)
{
    return texcoord.x() >= value_type(0) && texcoord.x() <= value_type(1) && texcoord.y() >= value_type(0) && texcoord.y() <= value_type(1);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_texcoord(const texcoord_type& texcoord, std::string& error)
{
    if (validate_texcoord(texcoord))
    {
        return true;
    }

    std::ostringstream oss;
    oss << "texcoord out of range: " << texcoord;
    error = oss.str();
    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_tangent(const vector_type& tangent)
{
    return math::float_eq(tangent.norm_squared(), value_type(1));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_tangent(const vector_type& tangent, std::string& error)
{
    if (validate_tangent(tangent))
    {
        return true;
    }

    std::ostringstream oss;
    oss << "|tangent| != 1: " << tangent;
    error = oss.str();
    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_bitangent(const vector_type& bitangent)
{
    return math::float_eq(bitangent.norm_squared(), value_type(1));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Attributes<Traits>::validate_bitangent(const vector_type& bitangent, std::string& error)
{
    if (validate_bitangent(bitangent))
    {
        return true;
    }

    std::ostringstream oss;
    oss << "|bitangent| != 1: " << bitangent;
    error = oss.str();
    return false;
}

//------------------------------------------------------------------------------
template<typename A, typename B>
bool quetzal::geometry::coincident(const A& a, const B& b)
{
    static_assert(a.flags() & Flags::Position);
    static_assert(b.flags() & Flags::Position);

    return(vector_eq(a.position(), b.position()));
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr quetzal::geometry::Flags::type quetzal::geometry::Position<Traits>::flags()
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Position<Traits>::Position(const point_type& position) :
    m_position(position)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Position<Traits>::point_type quetzal::geometry::Position<Traits>::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Position<Traits>::point_type& quetzal::geometry::Position<Traits>::position()
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Position<Traits>::set_position(const point_type& position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Position<Traits>::set(const point_type& position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Position<Traits>::transform(const matrix_type& matrixPosition)
{
    m_position *= matrixPosition;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Position<Traits>::transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal)
{
    m_position *= matrixPosition;
    matrixNormal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Position<Traits>::invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord)
{
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Position<Traits>::validate() const
{
    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Position<Traits>::validate(std::string& error) const
{
    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Position<Traits> quetzal::geometry::lerp(const Position<Traits>& a, const Position<Traits>& b, typename Position<Traits>::value_type t)
{
    return {lerp(a.position(), b.position(), t)};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Position<Traits>& a)
{
    os << "[" << a.position() << "]";
    return os;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr quetzal::geometry::Flags::type quetzal::geometry::PositionNormal<Traits>::flags()
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::PositionNormal<Traits>::PositionNormal(const point_type& position, const vector_type& normal) :
    m_position(position),
    m_normal(normal)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormal<Traits>::point_type quetzal::geometry::PositionNormal<Traits>::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormal<Traits>::point_type& quetzal::geometry::PositionNormal<Traits>::position()
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormal<Traits>::set_position(const point_type& position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormal<Traits>::vector_type quetzal::geometry::PositionNormal<Traits>::normal() const
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormal<Traits>::vector_type& quetzal::geometry::PositionNormal<Traits>::normal()
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormal<Traits>::set_normal(const vector_type& normal)
{
    assert(math::float_eq(normal.norm_squared(), value_type(1)));

    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormal<Traits>::set(const point_type& position, const vector_type& normal)
{
    m_position = position;
    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormal<Traits>::transform(const matrix_type& matrixPosition)
{
    m_position *= matrixPosition;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormal<Traits>::transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal)
{
    m_position *= matrixPosition;
    m_normal = normalize(m_normal * matrixNormal);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormal<Traits>::invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord)
{
    m_normal = -m_normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::PositionNormal<Traits>::validate() const
{
    return Attributes<Traits>::validate_normal(m_normal);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::PositionNormal<Traits>::validate(std::string& error) const
{
    return Attributes<Traits>::validate_normal(m_normal, error);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::PositionNormal<Traits> quetzal::geometry::lerp(const PositionNormal<Traits>& a, const PositionNormal<Traits>& b, typename PositionNormal<Traits>::value_type t)
{
    return {lerp(a.position(), b.position(), t), normalize(a.normal() * rotation_lerp_unit(a.normal(), b.normal(), t))};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const PositionNormal<Traits>& a)
{
    os << "[" << a.position() << "]";
    os << " [" << a.normal() << "]";
    return os;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr quetzal::geometry::Flags::type quetzal::geometry::PositionNormalTexture<Traits>::flags()
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::PositionNormalTexture<Traits>::PositionNormalTexture(const point_type& position, const vector_type& normal, const texcoord_type& texcoord) :
    m_position(position),
    m_normal(normal),
    m_texcoord(texcoord)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTexture<Traits>::point_type quetzal::geometry::PositionNormalTexture<Traits>::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTexture<Traits>::point_type& quetzal::geometry::PositionNormalTexture<Traits>::position()
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::set_position(const point_type& position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTexture<Traits>::vector_type quetzal::geometry::PositionNormalTexture<Traits>::normal() const
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTexture<Traits>::vector_type& quetzal::geometry::PositionNormalTexture<Traits>::normal()
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::set_normal(const vector_type& normal)
{
    assert(math::float_eq(normal.norm_squared(), value_type(1)));

    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTexture<Traits>::texcoord_type quetzal::geometry::PositionNormalTexture<Traits>::texcoord() const
{
    return m_texcoord;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTexture<Traits>::texcoord_type& quetzal::geometry::PositionNormalTexture<Traits>::texcoord()
{
    return m_texcoord;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::set_texcoord(const texcoord_type& texcoord)
{
    m_texcoord = texcoord;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::set(const point_type& position, const vector_type& normal, const texcoord_type& texcoord)
{
    m_position = position;
    m_normal = normal;
    m_texcoord = texcoord;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::transform(const matrix_type& matrixPosition)
{
    m_position *= matrixPosition;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal)
{
    m_position *= matrixPosition;
    m_normal = normalize(m_normal * matrixNormal);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTexture<Traits>::invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord)
{
    m_normal = -m_normal;
    m_texcoord = transform_texcoord(m_texcoord);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::PositionNormalTexture<Traits>::validate() const
{
    return Attributes<Traits>::validate_normal(m_normal) && Attributes<Traits>::validate_texcoord(m_texcoord);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::PositionNormalTexture<Traits>::validate(std::string& error) const
{
    std::string e;
    bool bOK = true;

    if (!Attributes<Traits>::validate_normal(m_normal, e))
    {
        error = e;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_texcoord(m_texcoord, e))
    {
        if (!e.empty())
        {
            error += "; ";
        }

        error += e;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::PositionNormalTexture<Traits> quetzal::geometry::lerp(const PositionNormalTexture<Traits>& a, const PositionNormalTexture<Traits>& b, typename PositionNormalTexture<Traits>::value_type t)
{
    return {lerp(a.position(), b.position(), t), normalize(a.normal() * rotation_lerp_unit(a.normal(), b.normal(), t)), lerp(a.texcoord(), b.texcoord(), t)};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const PositionNormalTexture<Traits>& a)
{
    os << "[" << a.position() << "]";
    os << " [" << a.normal() << "]";
    os << " [" << a.texcoord() << "]";
    return os;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr quetzal::geometry::Flags::type quetzal::geometry::PositionNormalTextureTangent<Traits>::flags()
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::PositionNormalTextureTangent<Traits>::PositionNormalTextureTangent(const point_type& position, const vector_type& normal, const texcoord_type& texcoord, const vector_type& tangent) :
    m_position(position),
    m_normal(normal),
    m_texcoord(texcoord),
    m_tangent(tangent)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::point_type quetzal::geometry::PositionNormalTextureTangent<Traits>::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::point_type& quetzal::geometry::PositionNormalTextureTangent<Traits>::position()
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::set_position(const point_type& position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::vector_type quetzal::geometry::PositionNormalTextureTangent<Traits>::normal() const
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::vector_type& quetzal::geometry::PositionNormalTextureTangent<Traits>::normal()
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::set_normal(const vector_type& normal)
{
    assert(math::float_eq(normal.norm_squared(), value_type(1)));

    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::texcoord_type quetzal::geometry::PositionNormalTextureTangent<Traits>::texcoord() const
{
    return m_texcoord;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::texcoord_type& quetzal::geometry::PositionNormalTextureTangent<Traits>::texcoord()
{
    return m_texcoord;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::set_texcoord(const texcoord_type& texcoord)
{
    m_texcoord = texcoord;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::vector_type quetzal::geometry::PositionNormalTextureTangent<Traits>::tangent() const
{
    return m_tangent;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PositionNormalTextureTangent<Traits>::vector_type& quetzal::geometry::PositionNormalTextureTangent<Traits>::tangent()
{
    return m_tangent;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::set_tangent(const vector_type& tangent)
{
    m_tangent = tangent;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::set(const point_type& position, const vector_type& normal, const texcoord_type& texcoord, const vector_type& tangent)
{
    m_position = position;
    m_normal = normal;
    m_texcoord = texcoord;
    m_tangent = tangent;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::transform(const matrix_type& matrixPosition)
{
    m_position *= matrixPosition;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal)
{
    m_position *= matrixPosition;
    m_normal = normalize(m_normal * matrixNormal);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PositionNormalTextureTangent<Traits>::invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord)
{
    m_normal = -m_normal;
    m_texcoord = transform_texcoord(m_texcoord);
    m_tangent = -m_tangent;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::PositionNormalTextureTangent<Traits>::validate() const
{
    return Attributes<Traits>::validate_normal(m_normal) && Attributes<Traits>::validate_texcoord(m_texcoord) && Attributes<Traits>::validate_tangent(m_tangent);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::PositionNormalTextureTangent<Traits>::validate(std::string& error) const
{
    std::string e;
    bool bOK = true;

    if (!Attributes<Traits>::validate_normal(m_normal, e))
    {
        error = e;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_texcoord(m_texcoord, e))
    {
        if (!e.empty())
        {
            error += "; ";
        }

        error += e;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_tangent(m_tangent, e))
    {
        if (!e.empty())
        {
            error += "; ";
        }

        error += e;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::PositionNormalTextureTangent<Traits> quetzal::geometry::lerp(const PositionNormalTextureTangent<Traits>& a, const PositionNormalTextureTangent<Traits>& b, typename PositionNormalTextureTangent<Traits>::value_type t)
{
    return {lerp(a.position(), b.position(), t), normalize(a.normal() * rotation_lerp_unit(a.normal(), b.normal(), t)), lerp(a.texcoord(), b.texcoord(), t), tangentize(a.tangent() * rotation_lerp_unit(a.tangent(), b.tangent(), t))};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const PositionNormalTextureTangent<Traits>& a)
{
    os << "[" << a.position() << "]";
    os << " [" << a.normal() << "]";
    os << " [" << a.texcoord() << "]";
    os << " [" << a.tangent() << "]";
    return os;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr quetzal::geometry::Flags::type quetzal::geometry::Normal<Traits>::flags()
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Normal<Traits>::Normal(const vector_type& normal) :
    m_normal(normal)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Normal<Traits>::vector_type quetzal::geometry::Normal<Traits>::normal() const
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Normal<Traits>::vector_type& quetzal::geometry::Normal<Traits>::normal()
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Normal<Traits>::set_normal(const vector_type& normal)
{
    assert(math::float_eq(normal.norm_squared(), value_type(1)));

    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Normal<Traits>::set(const vector_type& normal)
{
    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Normal<Traits>::transform(const matrix_type& matrixPosition)
{
    matrixPosition;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Normal<Traits>::transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal)
{
    matrixPosition;
    m_normal = normalize(m_normal * matrixNormal);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Normal<Traits>::invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord)
{
    m_normal = -m_normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Normal<Traits>::validate() const
{
    return Attributes<Traits>::validate_normal(m_normal);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Normal<Traits>::validate(std::string& error) const
{
    return Attributes<Traits>::validate_normal(m_normal, error);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Normal<Traits> quetzal::geometry::lerp(const Normal<Traits>& a, const Normal<Traits>& b, typename Normal<Traits>::value_type t)
{
    return {normalize(a.normal() * rotation_lerp_unit(a.normal(), b.normal(), t))};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Normal<Traits>& a)
{
    os << "[" << a.normal() << "]";
    return os;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr quetzal::geometry::Flags::type quetzal::geometry::NormalTangent<Traits>::flags()
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::NormalTangent<Traits>::NormalTangent(const vector_type& normal, const vector_type& tangent) :
    m_normal(normal),
    m_tangent(tangent)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::NormalTangent<Traits>::vector_type quetzal::geometry::NormalTangent<Traits>::normal() const
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::NormalTangent<Traits>::vector_type& quetzal::geometry::NormalTangent<Traits>::normal()
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::NormalTangent<Traits>::set_normal(const vector_type& normal)
{
    assert(math::float_eq(normal.norm_squared(), value_type(1)));

    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::NormalTangent<Traits>::vector_type quetzal::geometry::NormalTangent<Traits>::tangent() const
{
    return m_tangent;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::NormalTangent<Traits>::vector_type& quetzal::geometry::NormalTangent<Traits>::tangent()
{
    return m_tangent;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::NormalTangent<Traits>::set_tangent(const vector_type& tangent)
{
    m_tangent = tangent;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::NormalTangent<Traits>::set(const vector_type& normal, const vector_type& tangent)
{
    m_normal = normal;
    m_tangent = tangent;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::NormalTangent<Traits>::transform(const matrix_type& matrixPosition)
{
    matrixPosition;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::NormalTangent<Traits>::transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal)
{
    matrixPosition;
    m_normal = normalize(m_normal * matrixNormal);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::NormalTangent<Traits>::invert(std::function<texcoord_type(const texcoord_type&)> transform_texcoord)
{
    m_normal = -m_normal;
    m_tangent = -m_tangent;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::NormalTangent<Traits>::validate() const
{
    return Attributes<Traits>::validate_normal(m_normal) && Attributes<Traits>::validate_tangent(m_tangent);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::NormalTangent<Traits>::validate(std::string& error) const
{
    std::string e;
    bool bOK = true;

    if (!Attributes<Traits>::validate_normal(m_normal, e))
    {
        error = e;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_tangent(m_tangent, e))
    {
        if (!e.empty())
        {
            error += "; ";
        }

        error += e;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::NormalTangent<Traits> quetzal::geometry::lerp(const NormalTangent<Traits>& a, const NormalTangent<Traits>& b, typename NormalTangent<Traits>::value_type t)
{
    return {normalize(a.normal() * rotation_lerp_unit(a.normal(), b.normal(), t))};
    return {normalize(a.tangent() * rotation_lerp_unit(a.tangent(), b.tangent(), t))};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const NormalTangent<Traits>& a)
{
    os << "[" << a.normal() << "]";
    os << " [" << a.tangent() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_ATTRIBUTES_HPP
