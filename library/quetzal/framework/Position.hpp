#if !defined(QUETZAL_FRAMEWORK_POSITION_HPP)
#define QUETZAL_FRAMEWORK_POSITION_HPP
//------------------------------------------------------------------------------
// quetzal framework
// Position.hpp
//------------------------------------------------------------------------------

#include <iosfwd>

namespace quetzal::framework
{

    //--------------------------------------------------------------------------
    struct Position
    {
        Position() = default;
        Position(int x_, int y_);
        Position(const Position&) = default;
        ~Position() = default;

        Position& operator=(const Position&) = default;
        bool operator==(const Position& other);
        bool operator!=(const Position& other);

        int x;
        int y;
    };

    std::ostream& operator<<(std::ostream& os, const Position& position);

} // namespace quetzal::framework

#endif // QUETZAL_FRAMEWORK_POSITION_HPP
