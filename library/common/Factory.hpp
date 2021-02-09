#if !defined(QUETZAL_FACTORY_HPP)
#define QUETZAL_FACTORY_HPP
//------------------------------------------------------------------------------
// Factory.hpp
//------------------------------------------------------------------------------

#include <functional>
#include <memory>
#include <map>
#include <sstream>
#include <typeinfo>
#include "Exception.hpp"

#include <iostream>

namespace quetzal
{

    //--------------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    class Factory
    {
    public:

        typedef IdType id_type;
        typedef BaseType base_type;
        typedef std::function<std::shared_ptr<BaseType>(Args...)> functor_type;
        typedef typename functor_type::result_type result_type;
        typedef std::map<id_type, functor_type> associations_type;

        Factory();
        Factory(const Factory& other) = delete;
        ~Factory() = default;

        Factory& operator=(const Factory& other) = delete;

        bool register_creator(const id_type& id, functor_type f);
        bool unregister_creator(const id_type& id);

        result_type create(const id_type& id, Args... args);

        const associations_type associations() const;

    private:

        associations_type m_associations;
        bool m_bErrorException;
    };

    //----------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    Factory<IdType, BaseType, Args...>::Factory() :
        m_associations(),
        m_bErrorException(true)
    {
    }

    //----------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    bool Factory<IdType, BaseType, Args...>::register_creator(const id_type& id, functor_type f)
    {
        return m_associations.insert(associations_type::value_type(id, f)).second != 0;
    }

    //----------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    bool Factory<IdType, BaseType, Args...>::unregister_creator(const id_type& id)
    {
        return m_associations.erase(id) != 0;
    }

    //----------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    typename Factory<IdType, BaseType, Args...>::result_type Factory<IdType, BaseType, Args...>::create(const id_type& id, Args... args)
    {
        const auto& i = m_associations.find(id);
        if (i == m_associations.end())
        {
            if (m_bErrorException)
            {
                std::ostringstream oss;
                oss << "Factory::create - unknown id: " << id;
                throw Exception(oss.str().c_str(), __FILE__, __LINE__);
            }

            return nullptr;
        }

        return i->second(args...);
    }

    //----------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    const typename Factory<IdType, BaseType, Args...>::associations_type  Factory<IdType, BaseType, Args...>::associations() const
    {
        return m_associations;
    }

    //----------------------------------------------------------------------
    template<typename IdType, typename BaseType, typename... Args>
    std::ostream& operator<<(std::ostream& os, const Factory<IdType, BaseType, Args...>& factory)
    {
        for (const auto& a : factory.associations())
        {
            os << a.first << " " << typeid(a.second).name() << endl;
        }

        return os;
    }

} // namespace quetzal

#endif // QUETZAL_FACTORY_HPP
