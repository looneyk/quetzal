#if !defined(QUETZAL_MODEL_EXTENT_HPP)
#define QUETZAL_MODEL_EXTENT_HPP
//------------------------------------------------------------------------------
// model
// Extent.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/Interval.hpp"
#include "SurfaceName.hpp"
#include <string>

namespace quetzal::model
{

    //--------------------------------------------------------------------------
    struct Termination
    {
        enum class Type
        {
            None,
            Flat,
            Center
        };

        Type type = Type::None;
        bool banded = true;
    };

    bool operator==(Termination lhs, Termination rhs)
    {
        return lhs.type == rhs.type && lhs.banded == rhs.banded;
    }

    //--------------------------------------------------------------------------
    template<typename T>
    class Extent
    {
    public:

        using value_type = T;

        Extent(); // Termination::Type::None
        Extent(const math::Interval<T>& interval); // Termination::Type::None
        Extent(const T& tLower, const T& tUpper); // Termination::Type::None
        Extent(const T& tLower, const T& tUpper, const std::string& name); // Termination::Type::Flat
        Extent(const T& tLower, const T& tUpper, const std::string& nameLower, const std::string& nameUpper); // Termination::Type::Center
        Extent(const T& tLower, const T& tUpper, Termination termination, const std::string& nameLower, const std::string& nameUpper);
        Extent(const T& tLower, const T& tUpper, Termination terminationLower, Termination terminationUpper, const std::string& nameLower, const std::string& nameUpper);
        Extent(const Extent&) = default;
        virtual ~Extent() = default;

        Extent& operator=(const Extent&) = default;

        // Interval is parametric, nominally [0, 1] along the given Extent
        math::Interval<T> interval() const;
        void set_interval(const math::Interval<T>& interval);
        // lower, upper ...
        void set_interval(const T& tLower, const T& tUpper);
        bool full() const;

        Termination termination() const;
        void set_termination(Termination termination);

        Termination lower_termination() const;
        Termination upper_termination() const;
        void set_termination(Termination terminationLower, Termination terminationUpper);

        // Flat
        std::string name() const;
        void set_name(const std::string& name);

        // Center
        std::string lower_name() const;
        std::string upper_name() const;
        void set_names(const std::string& nameLower, const std::string & nameUpper);

        bool proper() const;

    private:

        math::Interval<T> m_interval;
        Termination m_terminationLower;
        Termination m_terminationUpper;
        std::string m_nameLower;
        std::string m_nameUpper;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    class ExtentSideFlat : public Extent<T>
    {
    public:

        ExtentSideFlat();
        ExtentSideFlat(const T& tLower, const T& tUpper);
        ExtentSideFlat(const ExtentSideFlat&) = default;
        virtual ~ExtentSideFlat() = default;

        ExtentSideFlat& operator=(const ExtentSideFlat&) = default;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    class ExtentSideCenter : public Extent<T>
    {
    public:

        ExtentSideCenter();
        ExtentSideCenter(const T& tLower, const T& tUpper);
        ExtentSideCenter(const ExtentSideCenter&) = default;
        virtual ~ExtentSideCenter() = default;

        ExtentSideCenter& operator=(const ExtentSideCenter&) = default;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    class ExtentEndsFlat : public Extent<T>
    {
    public:

        ExtentEndsFlat();
        ExtentEndsFlat(const T& tLower, const T& tUpper);
        ExtentEndsFlat(const ExtentEndsFlat&) = default;
        virtual ~ExtentEndsFlat() = default;

        ExtentEndsFlat& operator=(const ExtentEndsFlat&) = default;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    class ExtentEndsCenter : public Extent<T>
    {
    public:

        ExtentEndsCenter();
        ExtentEndsCenter(const T& tLower, const T& tUpper);
        ExtentEndsCenter(const ExtentEndsCenter&) = default;
        virtual ~ExtentEndsCenter() = default;

        ExtentEndsCenter& operator=(const ExtentEndsCenter&) = default;
    };

} // namespace quetzal::model

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent() :
    Extent(T(0), T(1))
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent(const math::Interval<T>& interval) :
    m_interval(interval),
    m_terminationLower({Termination::Type::None}),
    m_terminationUpper({Termination::Type::None}),
    m_nameLower(),
    m_nameUpper()
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent(const T& tLower, const T& tUpper) :
    Extent(tLower, tUpper, {Termination::Type::None}, {Termination::Type::None}, "", "")
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent(const T& tLower, const T& tUpper, const std::string& name) :
    Extent(tLower, tUpper, {Termination::Type::Flat}, {Termination::Type::Flat}, name, name)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent(const T& tLower, const T& tUpper, const std::string& nameLower, const std::string& nameUpper) :
    Extent(tLower, tUpper, {Termination::Type::Center}, {Termination::Type::Center}, nameLower, nameUpper)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent(const T& tLower, const T& tUpper, Termination termination, const std::string& nameLower, const std::string& nameUpper) :
    Extent(tLower, tUpper, termination, termination, nameLower, nameUpper)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Extent<T>::Extent(const T& tLower, const T& tUpper, Termination terminationLower, Termination terminationUpper, const std::string& nameLower, const std::string& nameUpper) :
    m_interval(tLower, tUpper),
    m_terminationLower(terminationLower),
    m_terminationUpper(terminationUpper),
    m_nameLower(nameLower),
    m_nameUpper(nameUpper)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Interval<T> quetzal::model::Extent<T>::interval() const
{
    return m_interval;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::model::Extent<T>::set_interval(const math::Interval<T>& interval)
{
    m_interval = interval;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::model::Extent<T>::set_interval(const T& tLower, const T & tUpper)
{
    m_interval.set_bounds(tLower, tUpper);
    return;
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::model::Extent<T>::full() const
{
    return m_interval.unit_length();
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Termination quetzal::model::Extent<T>::termination() const
{
    assert(m_terminationLower == m_terminationUpper);
    return m_terminationLower;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::model::Extent<T>::set_termination(Termination termination)
{
    m_terminationLower = termination;
    m_terminationUpper = termination;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Termination quetzal::model::Extent<T>::lower_termination() const
{
    return m_terminationLower;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::Termination quetzal::model::Extent<T>::upper_termination() const
{
    return m_terminationUpper;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::model::Extent<T>::set_termination(Termination terminationLower, Termination terminationUpper)
{
    m_terminationLower = terminationLower;
    m_terminationUpper = terminationUpper;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::model::Extent<T>::name() const
{
    assert(m_nameLower == m_nameUpper);
    return m_nameLower;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::model::Extent<T>::set_name(const std::string& name)
{
    m_nameLower = name;
    m_nameUpper = name;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::model::Extent<T>::lower_name() const
{
    return m_nameLower;
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::model::Extent<T>::upper_name() const
{
    return m_nameUpper;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::model::Extent<T>::set_names(const std::string& nameLower, const std::string& nameUpper)
{
    m_nameLower = nameLower;
    m_nameUpper = nameUpper;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::model::Extent<T>::proper() const
{
    return m_interval.proper();
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentSideFlat<T>::ExtentSideFlat() :
    ExtentSideFlat(T(0), T(1))
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentSideFlat<T>::ExtentSideFlat(const T& tLower, const T& tUpper) :
    Extent<T>(tLower, tUpper, SurfaceName::Side)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentSideCenter<T>::ExtentSideCenter() :
    ExtentSideCenter(T(0), T(1))
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentSideCenter<T>::ExtentSideCenter(const T& tLower, const T& tUpper) :
    Extent<T>(tLower, tUpper, SurfaceName::Side0, SurfaceName::Side1)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentEndsFlat<T>::ExtentEndsFlat() :
    ExtentEndsFlat(T(0), T(1))
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentEndsFlat<T>::ExtentEndsFlat(const T& tLower, const T& tUpper) :
    Extent<T>(tLower, tUpper, {Termination::Type::Flat}, SurfaceName::End0, SurfaceName::End1)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentEndsCenter<T>::ExtentEndsCenter() :
    ExtentEndsCenter(T(0), T(1))
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::model::ExtentEndsCenter<T>::ExtentEndsCenter(const T& tLower, const T& tUpper) :
    Extent<T>(tLower, tUpper, {Termination::Type::Center}, SurfaceName::End0, SurfaceName::End1)
{
}

#endif // QUETZAL_MODEL_EXTENT_HPP
