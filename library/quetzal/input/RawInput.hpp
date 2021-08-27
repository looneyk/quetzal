#if !defined(QUETZAL_INPUT_RAWINPUT_HPP)
#define QUETZAL_INPUT_RAWINPUT_HPP
//------------------------------------------------------------------------------
// RawInput.hpp
//------------------------------------------------------------------------------

#include <windows.h>
#include <vector>
#include <iosfwd>

namespace quetzal::input
{

    //--------------------------------------------------------------------------
    class RawInput
    {
    public:

        RawInput();
        RawInput(const RawInput&) = delete;
        ~RawInput() = default;

        RawInput& operator=(const RawInput&) = delete;

        const RAWINPUTHEADER& header(LPARAM lParam);
        const RAWMOUSE& mouse() const;

    private:

        std::vector<BYTE> m_buffer;
    };

    std::ostream& operator<<(std::ostream& os, const RawInput& ri);

} // namespace quetzal::input

#endif // QUETZAL_INPUT_RAWINPUT_HPP
