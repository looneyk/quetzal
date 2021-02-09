#if !defined(QUETZAL_DIRECT3D11_SHAREDRESOURCEPOOL_HPP)
#define QUETZAL_DIRECT3D11_SHAREDRESOURCEPOOL_HPP
//------------------------------------------------------------------------------
// SharedResourcePool.h
//------------------------------------------------------------------------------

#include <map>
#include <memory>
#include <mutex>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Pool manager ensures that only a single TData instance is created for each unique TKey.
    // This is used to avoid duplicate resource creation, so that for instance a caller can
    // create any number of SpriteBatch instances, but these can internally share shaders and
    // vertex buffer if more than one SpriteBatch uses the same underlying D3D device.
    template<typename TKey, typename TData, typename... TConstructorArgs>
    class SharedResourcePool
    {
    public:

        SharedResourcePool();
        SharedResourcePool(const SharedResourcePool&) = delete;

        SharedResourcePool& operator=(const SharedResourcePool&) = delete;

        // Allocates or looks up the shared TData instance for the specified key.
        std::shared_ptr<TData> DemandCreate(TKey key, TConstructorArgs... args);

    private:

        // Keep track of all allocated TData instances.
        struct ResourceMap : public std::map<TKey, std::weak_ptr<TData>>
        {
            std::mutex m_mutex;
        };

        std::shared_ptr<ResourceMap> m_resource_map;

        // Wrap TData with our own subclass, so we can hook the destructor to remove instances from our pool before they are freed
        class WrappedData : public TData
        {
        public:

            WrappedData(TKey key, const std::shared_ptr<ResourceMap>& resource_map, TConstructorArgs... args);
            ~WrappedData();

            TKey m_key;
            std::shared_ptr<ResourceMap> m_resource_map;
        };
    };

    //----------------------------------------------------------------------
    template<typename TKey, typename TData, typename... TConstructorArgs>
    SharedResourcePool<TKey, TData, TConstructorArgs...>::SharedResourcePool() :
        m_resource_map(std::make_shared<ResourceMap>())
    {
    }

    //----------------------------------------------------------------------
    template<typename TKey, typename TData, typename... TConstructorArgs>
    std::shared_ptr<TData> SharedResourcePool<TKey, TData, TConstructorArgs...>::DemandCreate(TKey key, TConstructorArgs... args)
    {
        std::lock_guard<std::mutex> lock(m_resource_map->m_mutex);

        // Look for an existing instance
        auto i = m_resource_map->find(key);

        if (i != m_resource_map->end())
        {
            auto p = i->second.lock();

            if (p)
            {
                return p;
            }
            else
            {
                m_resource_map->erase(i);
            }
        }

        // Allocate a new instance
        auto p = std::make_shared<WrappedData>(key, m_resource_map, args...);

        m_resource_map->insert(std::make_pair(key, p));

        return p;
    }

    //----------------------------------------------------------------------
    template<typename TKey, typename TData, typename... TConstructorArgs>
    SharedResourcePool<TKey, TData, TConstructorArgs...>::WrappedData::WrappedData(TKey key, const std::shared_ptr<ResourceMap>& resource_map, TConstructorArgs... args) :
        TData(args...),
        m_key(key),
        m_resource_map(resource_map)
    {
    }

    //----------------------------------------------------------------------
    template<typename TKey, typename TData, typename... TConstructorArgs>
    SharedResourcePool<TKey, TData, TConstructorArgs...>::WrappedData::~WrappedData()
    {
        std::lock_guard<std::mutex> lock(m_resource_map->m_mutex);

        auto i = m_resource_map->find(m_key);

        // Check for weak reference expiry before erasing, in case DemandCreate runs on
        // a different thread at the same time as a previous instance is being destroyed.
        // We mustn't erase replacement objects that have just been added!
        if (i != m_resource_map->end() && i->second.expired())
        {
            m_resource_map->erase(i);
        }
    }

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_SHAREDRESOURCEPOOL_HPP
