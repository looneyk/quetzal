#if !defined(QUETZAL_EXCEPTION_HPP)
#define QUETZAL_EXCEPTION_HPP
//------------------------------------------------------------------------------
// Exception.hpp
//------------------------------------------------------------------------------

#include <stdexcept>
#include <string>

namespace quetzal
{

    //--------------------------------------------------------------------------
    class Exception : public std::runtime_error
    {
    public:

        Exception(const std::string& message, const std::string& file, int line);
        Exception(const Exception&) = default;
        virtual ~Exception() = default;

        Exception& operator=(const Exception&) = default;
    };

} // namespace quetzal

#endif // QUETZAL_EXCEPTION_HPP
