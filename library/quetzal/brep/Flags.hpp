#if !defined(QUETZAL_BREP_FLAGS_HPP)
#define QUETZAL_BREP_FLAGS_HPP
//------------------------------------------------------------------------------
// brep
// Flags.hpp
//
// This is intended to be used in cases where its state has no impact on the constness of the class it is a part of.
// Thus, all methods are const and the data is mutable so that it can be used even in the const context.
//
//------------------------------------------------------------------------------

#include <iosfwd>
#include <utility>
#include <cstdint>

namespace quetzal::brep
{

    //--------------------------------------------------------------------------
    class Flags
    {
    public:

        bool deleted() const;
        void set_deleted(bool b = true) const;

        bool checked() const;
        void set_checked(bool b = true) const;

        bool marked() const;
        void set_marked(bool b = true) const;

        bool unset() const; // If deleted, remaining bits are ignored
        virtual void reset() const; // Preserves deleted state

        bool empty() const;
        virtual void clear() const;

    protected:

        Flags() = default;
        Flags(const Flags&) = default;
        Flags(Flags&&) = default;
        virtual ~Flags() = default;

        Flags& operator=(const Flags&) = default;
        Flags& operator=(Flags&&) = default;

        friend constexpr void swap(Flags& lhs, Flags& rhs) noexcept
        {
            using std::swap;
            swap(lhs.m_bits, rhs.m_bits);
        }

    private:

        mutable uint32_t m_bits = {};
    };

    std::ostream& operator<<(std::ostream& os, const Flags& flags);

} // namespace quetzal::brep

#endif // QUETZAL_BREP_FLAGS_HPP
