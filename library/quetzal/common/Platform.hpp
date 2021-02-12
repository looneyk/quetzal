#if !defined(KML_QUETZAL_PLATFORM_HPP)
#define KML_QUETZAL_PLATFORM_HPP
//------------------------------------------------------------------------------
// Platform.hpp
//
//------------------------------------------------------------------------------

namespace quetzal
{

    //----------------------------------------------------------------------
    class Platform
    {
    public:

        Platform();
        Platform(const Platform& platform) = delete;
        virtual ~Platform();

        Platform& operator=(const Platform& platform) = delete;
    };

}  //  namespace quetzal

#endif  //  KML_QUETZAL_PLATFORM_HPP
