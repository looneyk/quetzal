#if !defined(QUETZAL_FACTORYREGISTERING_HPP)
#define QUETZAL_FACTORYREGISTERING_HPP
//------------------------------------------------------------------------------
// FactoryRegistering.hpp
// Policy class for factory self-registration
//------------------------------------------------------------------------------

#include <memory>
#include "Factory.hpp"
#include "Singleton.hpp"

namespace quetzal
{

    //--------------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename ConcreteType, typename... Args>
    class FactoryRegistering
    {
    public:

        typedef IdType id_type;
        typedef ConcreteType concrete_type;
        typedef BaseType base_type;

        static std::shared_ptr<base_type> create(Args... args);

    protected:

        FactoryRegistering();
        FactoryRegistering(const FactoryRegistering& other) = delete;
        virtual ~FactoryRegistering();

        FactoryRegistering& operator=(const FactoryRegistering& other) = delete;

    private:

        static const volatile bool s_registered;

        typedef SingletonHolder<Factory<IdType, BaseType, Args...>> factory_singleton_type;
    };
    
    //--------------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename ConcreteType, typename... Args>
    FactoryRegistering<IdType, BaseType, ConcreteType, Args...>::FactoryRegistering()
    {
    }
    
    //--------------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename ConcreteType, typename... Args>
    FactoryRegistering<IdType, BaseType, ConcreteType, Args...>::~FactoryRegistering()
    {
        &s_registered;
    }

    //--------------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename ConcreteType, typename... Args>
    std::shared_ptr<BaseType> FactoryRegistering<IdType, BaseType, ConcreteType, Args...>::create(Args... args)
    {
        static_assert(is_base_of<base_type, concrete_type>::value && !is_same<base_type, concrete_type>::value, "not base and derived classes");

        return std::shared_ptr<BaseType>(new concrete_type(args...));
    }

    template<typename IdType, typename BaseType, typename ConcreteType, typename... Args>
    const volatile bool FactoryRegistering<IdType, BaseType, ConcreteType, Args...>::s_registered =
        factory_singleton_type::instance().register_creator(ConcreteType::factory_id(), FactoryRegistering<IdType, BaseType, ConcreteType, Args...>::create);

} // namespace quetzal

#endif // QUETZAL_FACTORYREGISTERING_HPP
