//------------------------------------------------------------------------------
// Keycache.hpp
//
//------------------------------------------------------------------------------

#if !defined(QUETZAL_DEVICE_KEYCACHE_HPP)
#define QUETZAL_DEVICE_KEYCACHE_HPP

#include "Key.hpp"
#include <vector>

namespace quetzal
{

namespace device
{

    //----------------------------------------------------------------------
    class Keycache
    {
    public:

        Keycache();
        ~Keycache();

        void clear();

        bool key_down(int keycode) const;
        bool key_toggled(int keycode) const;

        void set_key_down(int keycode, bool b = true);
        void set_key_toggled(int keycode, bool b = true);

    protected:

        std::vector<unsigned char> m_state;

    private:

        Keycache(const Keycache&); // Not copyable
        Keycache& operator=(const Keycache&); // Not assignable
    };

} // namespace device

} // namespace quetzal

#endif // QUETZAL_DEVICE_KEYCACHE_HPP
