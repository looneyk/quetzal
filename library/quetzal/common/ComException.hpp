#if !defined(QUETZAL_COMEXCEPTION_HPP)
#define QUETZAL_COMEXCEPTION_HPP
//------------------------------------------------------------------------------
// ComException.hpp
//------------------------------------------------------------------------------

#include "Exception.hpp"
#include <winerror.h>

#define COM_CHECK(x) {HRESULT hr = (x); if (FAILED(hr)) {throw quetzal::ComException(hr, #x, __FILE__, __LINE__);}}
#define HR_CHECK(x) {hr = (x); if (FAILED(hr)) {throw quetzal::ComException(hr, #x, __FILE__, __LINE__);}}

namespace quetzal
{

    //--------------------------------------------------------------------------
    class ComException : public Exception
    {
    public:

        ComException(HRESULT hr, const std::string& message, const std::string& file, int line);
        ComException(const ComException&) = default;
        virtual ~ComException() = default;

        ComException& operator=(const ComException&) = default;
    };

} // namespace quetzal

#endif // QUETZAL_COMEXCEPTION_HPP
