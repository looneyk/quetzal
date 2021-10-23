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

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    class AttributesFlags
    {
    public:

        static constexpr unsigned int Position = 0x01;
        static constexpr unsigned int Normal = 0x02;
        static constexpr unsigned int Texcoord0 = 0x04;
        static constexpr unsigned int Texcoord1 = 0x08;
        static constexpr unsigned int Tangent = 0x10;
        static constexpr unsigned int Bitangent = 0x20;

        AttributesFlags() = default;
        constexpr AttributesFlags(unsigned int flags);
        AttributesFlags(const AttributesFlags&) = default;
        ~AttributesFlags() = default;

        AttributesFlags& operator=(const AttributesFlags&) = default;

        constexpr operator unsigned int() const;

    private:

        const unsigned int m_flags;
    };

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Attributes
    {
    public:

        using vector_traits = Traits;
        using texcoord_traits = vector_traits::reduced_traits;

        using value_type = Traits::value_type;
        using point_type = math::Vector<vector_traits>;
        using vector_type = math::Vector<vector_traits>;
        using texcoord_type = math::Vector<texcoord_traits>;
        using matrix_type = math::Matrix<value_type>; // N ...
        using transform_texcoord_type = std::function<texcoord_type(const texcoord_type&)>;

        Attributes() = default;
        Attributes(const Attributes&) = default;
        virtual ~Attributes() = default;

        Attributes& operator=(const Attributes&) = default;

        virtual void transform(const matrix_type& matrixPosition) = 0;
        virtual void transform(const matrix_type& matrixPosition, const matrix_type& matrixNormal) = 0;
        virtual void invert(transform_texcoord_type transform_texcoord) = 0;
        virtual bool validate() const = 0;
        virtual bool validate(std::string& errors) const = 0;

        // virtual instead of in addition?
        // free functions instead?

        static texcoord_type transform_texcoord_null(const texcoord_type& texcoord);
        static texcoord_type transform_texcoord_reflect_u(const texcoord_type& texcoord);
        static texcoord_type transform_texcoord_reflect_v(const texcoord_type& texcoord);

        static bool validate_normal(const vector_type& normal);
        static bool validate_normal(const vector_type& normal, std::string& error);

        static bool validate_texcoord(const texcoord_type& texcoord);
        static bool validate_texcoord(const texcoord_type& texcoord, std::string& error);

        static bool validate_tangent(const vector_type& tangent, std::string& error);
        static bool validate_tangent(const vector_type& tangent);

        static bool validate_bitangent(const vector_type& bitangent);
        static bool validate_bitangent(const vector_type& bitangent, std::string& error);
    };

    // requires A and B are Attributes with position ...
    template<typename A, typename B>
    bool coincident(const A& a, const B& b);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Position : public Attributes<Traits>
    {
    public:

        using typename Attributes<Traits>::value_type;
        using typename Attributes<Traits>::point_type;
        using typename Attributes<Traits>::vector_type;
        using typename Attributes<Traits>::texcoord_type;
        using typename Attributes<Traits>::matrix_type;
        using typename Attributes<Traits>::transform_texcoord_type;
        using Attributes<Traits>::transform_texcoord_null;

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
        virtual void invert(transform_texcoord_type transform_texcoord = transform_texcoord_null) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& errors) const override;

        static constexpr bool contains(AttributesFlags components);

    private:

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

        using typename Attributes<Traits>::value_type;
        using typename Attributes<Traits>::point_type;
        using typename Attributes<Traits>::vector_type;
        using typename Attributes<Traits>::texcoord_type;
        using typename Attributes<Traits>::matrix_type;
        using typename Attributes<Traits>::transform_texcoord_type;
        using Attributes<Traits>::transform_texcoord_null;

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
        virtual void invert(transform_texcoord_type transform_texcoord = transform_texcoord_null) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& errors) const override;

        static constexpr bool contains(AttributesFlags components);

    private:

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

        using typename Attributes<Traits>::value_type;
        using typename Attributes<Traits>::point_type;
        using typename Attributes<Traits>::vector_type;
        using typename Attributes<Traits>::texcoord_type;
        using typename Attributes<Traits>::matrix_type;
        using typename Attributes<Traits>::transform_texcoord_type;
        using Attributes<Traits>::transform_texcoord_null;

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
        virtual void invert(transform_texcoord_type transform_texcoord = transform_texcoord_null) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& errors) const override;

        static constexpr bool contains(AttributesFlags components);

    private:

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

        using typename Attributes<Traits>::value_type;
        using typename Attributes<Traits>::point_type;
        using typename Attributes<Traits>::vector_type;
        using typename Attributes<Traits>::texcoord_type;
        using typename Attributes<Traits>::matrix_type;
        using typename Attributes<Traits>::transform_texcoord_type;
        using Attributes<Traits>::transform_texcoord_null;

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
        virtual void invert(transform_texcoord_type transform_texcoord = transform_texcoord_null) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& errors) const override;

        static constexpr bool contains(AttributesFlags components);

    private:

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

        using typename Attributes<Traits>::value_type;
        using typename Attributes<Traits>::point_type;
        using typename Attributes<Traits>::vector_type;
        using typename Attributes<Traits>::texcoord_type;
        using typename Attributes<Traits>::matrix_type;
        using typename Attributes<Traits>::transform_texcoord_type;
        using Attributes<Traits>::transform_texcoord_null;

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
        virtual void invert(transform_texcoord_type transform_texcoord = transform_texcoord_null) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& errors) const override;

        static constexpr bool contains(AttributesFlags components);

    private:

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

        using typename Attributes<Traits>::value_type;
        using typename Attributes<Traits>::point_type;
        using typename Attributes<Traits>::vector_type;
        using typename Attributes<Traits>::texcoord_type;
        using typename Attributes<Traits>::matrix_type;
        using typename Attributes<Traits>::transform_texcoord_type;
        using Attributes<Traits>::transform_texcoord_null;

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
        virtual void invert(transform_texcoord_type transform_texcoord = transform_texcoord_null) override;
        virtual bool validate() const override;
        virtual bool validate(std::string& errors) const override;

        static constexpr bool contains(AttributesFlags components);

    private:

        vector_type m_normal;
        vector_type m_tangent;
    };

    template<typename Traits>
    NormalTangent<Traits> lerp(const NormalTangent<Traits>& a, const NormalTangent<Traits>& b, typename NormalTangent<Traits>::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const NormalTangent<Traits>& a);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
constexpr quetzal::geometry::AttributesFlags::AttributesFlags(unsigned int flags) :
    m_flags(flags)
{
}

//------------------------------------------------------------------------------
constexpr quetzal::geometry::AttributesFlags::operator unsigned int() const
{
    return m_flags;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Attributes<Traits>::texcoord_type quetzal::geometry::Attributes<Traits>::transform_texcoord_null(const texcoord_type& texcoord)
{
    return texcoord;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Attributes<Traits>::texcoord_type quetzal::geometry::Attributes<Traits>::transform_texcoord_reflect_u(const texcoord_type& texcoord)
{
    return {Traits::val(1) - texcoord.x(), texcoord.y()};
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Attributes<Traits>::texcoord_type quetzal::geometry::Attributes<Traits>::transform_texcoord_reflect_v(const texcoord_type& texcoord)
{
    return {texcoord.x(), Traits::val(1) - texcoord.y()};
}

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
    static_assert(A::contains(AttributesFlags::Position));
    static_assert(B::contains(AttributesFlags::Position));

    return(vector_eq(a.position(), b.position()));
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
void quetzal::geometry::Position<Traits>::invert(transform_texcoord_type transform_texcoord)
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
bool quetzal::geometry::Position<Traits>::validate(std::string& errors) const
{
    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr bool quetzal::geometry::Position<Traits>::contains(AttributesFlags components)
{
    constexpr AttributesFlags contained = AttributesFlags::Position;
    return (contained & components) == components;
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
void quetzal::geometry::PositionNormal<Traits>::invert(transform_texcoord_type transform_texcoord)
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
bool quetzal::geometry::PositionNormal<Traits>::validate(std::string& errors) const
{
    return Attributes<Traits>::validate_normal(m_normal, errors);
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr bool quetzal::geometry::PositionNormal<Traits>::contains(AttributesFlags components)
{
    constexpr AttributesFlags contained = AttributesFlags::Position | AttributesFlags::Normal;
    return (contained & components) == components;
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
void quetzal::geometry::PositionNormalTexture<Traits>::invert(transform_texcoord_type transform_texcoord)
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
bool quetzal::geometry::PositionNormalTexture<Traits>::validate(std::string& errors) const
{
    std::string error;
    bool bOK = true;

    if (!Attributes<Traits>::validate_normal(m_normal, error))
    {
        errors = error;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_texcoord(m_texcoord, error))
    {
        if (!errors.empty())
        {
            errors += "; ";
        }

        errors += error;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr bool quetzal::geometry::PositionNormalTexture<Traits>::contains(AttributesFlags components)
{
    constexpr AttributesFlags contained = AttributesFlags::Position | AttributesFlags::Normal | AttributesFlags::Texcoord0;
    return (contained & components) == components;
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
void quetzal::geometry::PositionNormalTextureTangent<Traits>::invert(transform_texcoord_type transform_texcoord)
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
bool quetzal::geometry::PositionNormalTextureTangent<Traits>::validate(std::string& errors) const
{
    std::string error;
    bool bOK = true;

    if (!Attributes<Traits>::validate_normal(m_normal, error))
    {
        errors = error;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_texcoord(m_texcoord, error))
    {
        if (!errors.empty())
        {
            errors += "; ";
        }

        errors += error;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_tangent(m_tangent, error))
    {
        if (!errors.empty())
        {
            errors += "; ";
        }

        errors += error;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr bool quetzal::geometry::PositionNormalTextureTangent<Traits>::contains(AttributesFlags components)
{
    constexpr AttributesFlags contained = AttributesFlags::Position | AttributesFlags::Normal | AttributesFlags::Texcoord0 | AttributesFlags::Tangent;
    return (contained & components) == components;
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
void quetzal::geometry::Normal<Traits>::invert(transform_texcoord_type transform_texcoord)
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
bool quetzal::geometry::Normal<Traits>::validate(std::string& errors) const
{
    return Attributes<Traits>::validate_normal(m_normal, errors);
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr bool quetzal::geometry::Normal<Traits>::contains(AttributesFlags components)
{
    constexpr AttributesFlags contained = AttributesFlags::Normal;
    return (contained & components) == components;
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
void quetzal::geometry::NormalTangent<Traits>::invert(transform_texcoord_type transform_texcoord)
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
bool quetzal::geometry::NormalTangent<Traits>::validate(std::string& errors) const
{
    std::string error;
    bool bOK = true;

    if (!Attributes<Traits>::validate_normal(m_normal, error))
    {
        errors = error;
        bOK = false;
    }

    if (!Attributes<Traits>::validate_tangent(m_tangent, error))
    {
        if (!errors.empty())
        {
            errors += "; ";
        }

        errors += error;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename Traits>
constexpr bool quetzal::geometry::NormalTangent<Traits>::contains(AttributesFlags components)
{
    constexpr AttributesFlags contained = AttributesFlags::Normal | AttributesFlags::Tangent;
    return (contained & components) == components;
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
