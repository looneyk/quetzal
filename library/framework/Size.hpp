#if !defined(QUETZAL_FRAMEWORK_SIZE_HPP)
#define QUETZAL_FRAMEWORK_SIZE_HPP
//------------------------------------------------------------------------------
// quetzal framework
// Size.hpp
//------------------------------------------------------------------------------

#include <iosfwd>

namespace quetzal::framework
{

    //--------------------------------------------------------------------------
    struct Size
    {
        Size() = default;
        Size(int x_, int y_);
        Size(const Size&) = default;
        ~Size() = default;

        Size& operator=(const Size&) = default;
        bool operator==(const Size& other);
        bool operator!=(const Size& other);

        double aspect_ratio() const;

        int x;
        int y;
    };

    std::ostream& operator<<(std::ostream& os, const Size& size);

} // namespace quetzal::framework

#endif // QUETZAL_FRAMEWORK_SIZE_HPP
