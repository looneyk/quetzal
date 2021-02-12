#if !defined(QUETZAL_COM_PTR_HPP)
#define QUETZAL_COM_PTR_HPP
//------------------------------------------------------------------------------
// COM smart pointer template class
// com_ptr.hpp
// Copyright © 2000-2008 Keith Michael Looney
//------------------------------------------------------------------------------

#include <unknwn.h>
#include <utility>
#include <cassert>

//#include "logger.hpp"
//#include <iostream>

namespace quetzal
{

    //--------------------------------------------------------------------------
    template<typename T>
    class com_ptr
    {
    public:

        typedef T element_type;

        com_ptr() noexcept;
        com_ptr(const com_ptr& other);
        com_ptr(element_type* p); // Copy semantics
        ~com_ptr();

        com_ptr& operator=(com_ptr other) noexcept;
        com_ptr& operator=(element_type* p); // Copy semantics

        operator element_type*() const noexcept; // deprecate? use get instead? ...

        element_type* get() const noexcept;

        element_type& operator*() const;
        element_type* operator->() const noexcept;

        explicit operator bool() const noexcept;

        bool operator==(T* p) const; // also check for identical interfaces? ...
        bool operator!=(T* p) const;

//        element_type** getp() const; // experimental ...

        // These will release a non-null pointer. Should be used only when a new assignment is to be made.
        element_type*& p();
        void** ppv();

        void attach(T*& p) noexcept; // Take ownership of the interface pointer. Does not increment reference count. Sets original pointer to null. Move semantics

        void reset() noexcept;
        void swap(com_ptr& other) noexcept;

//        T** operator&(); // Do not use. VD T** operator~(): This is a landmine if you put com_ptr in an STL container

        HRESULT CreateInstance(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_ALL);

        template<typename U>
        HRESULT QueryInterface(com_ptr<U>& cp) const;

        template<typename U>
        HRESULT QueryInterface(U** pp) const;

    private:

        inline void checked_addref();
        inline void checked_release();

        template<typename U>
        inline void checked_release(U*& p);

        element_type* m_p;
    };

    template<typename T>
    void swap(com_ptr<T>& a, com_ptr<T>& b) noexcept;

    template<typename T>
    bool operator==(T* p, const com_ptr<T>& cp);

    template<typename T>
    bool operator!=(T* p, const com_ptr<T>& cp);

} // namespace quetzal

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>::com_ptr() noexcept :
    m_p(nullptr)
{
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>::com_ptr(const com_ptr& other) :
    m_p(other.m_p)
{
    checked_addref();
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>::com_ptr(element_type* p) :
    m_p(p)
{
    checked_addref();
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>::~com_ptr()
{
    checked_release();
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>& quetzal::com_ptr<T>::operator=(com_ptr other) noexcept
{
    swap(other);
    return *this;
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>& quetzal::com_ptr<T>::operator=(element_type* p)
{
    com_ptr<T> other(p);
    swap(other);
    return *this;
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>::operator element_type*() const noexcept
{
    return m_p;
}

//--------------------------------------------------------------------------
template<typename T>
typename quetzal::com_ptr<T>::element_type* quetzal::com_ptr<T>::get() const noexcept
{
    return m_p;
}

//--------------------------------------------------------------------------
template<typename T>
typename quetzal::com_ptr<T>::element_type& quetzal::com_ptr<T>::operator*() const
{
    assert(m_p != nullptr);
    return *m_p;
}

//--------------------------------------------------------------------------
template<typename T>
typename quetzal::com_ptr<T>::element_type* quetzal::com_ptr<T>::operator->() const noexcept
{
    assert(m_p != nullptr);
    return m_p;
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::com_ptr<T>::operator bool() const noexcept
{
    return m_p != nullptr;
}

//--------------------------------------------------------------------------
template<typename T>
bool quetzal::com_ptr<T>::operator==(T* p) const
{
    if (m_p == p)
    {
        return true;
    }

    if (m_p == nullptr || p == nullptr)
    {
        return false;
    }

    com_ptr<IUnknown> punk1;
    QueryInterface(punk1);

    com_ptr<IUnknown> punk2;
    p->QueryInterface(IID_IUnknown, punk2.ppv());

    return punk1 == punk2;
}

//--------------------------------------------------------------------------
template<typename T>
bool quetzal::com_ptr<T>::operator!=(T* p) const
{
    return !operator==(p);
}

//--------------------------------------------------------------------------
//template<typename T>
//typename quetzal::com_ptr<T>::element_type** quetzal::com_ptr<T>::getp() const
//{
//    return &m_p;
//}

//--------------------------------------------------------------------------
template<typename T>
typename quetzal::com_ptr<T>::element_type*& quetzal::com_ptr<T>::p()
{
    checked_release();
    return m_p;
}

//--------------------------------------------------------------------------
template<typename T>
void** quetzal::com_ptr<T>::ppv()
{
    checked_release();
    return reinterpret_cast<void**>(&m_p);
}

//--------------------------------------------------------------------------
template<typename T>
void quetzal::com_ptr<T>::attach(T*& p) noexcept
{
    checked_release();
    m_p = p;
    p = nullptr;
    return;
}

//--------------------------------------------------------------------------
template<typename T>
void quetzal::com_ptr<T>::reset() noexcept
{
    checked_release();
    return;
}
//--------------------------------------------------------------------------
template<typename T>
void quetzal::com_ptr<T>::swap(com_ptr& other) noexcept
{
    using std::swap;
    swap(m_p, other.m_p);
    return;
}

//--------------------------------------------------------------------------
template<typename T>
HRESULT quetzal::com_ptr<T>::CreateInstance(const CLSID& clsid, IUnknown* pOuter, DWORD dwClsContext)
{
    return CoCreateInstance(clsid, pOuter, dwClsContext, __uuidof(T), ppv());
}

//--------------------------------------------------------------------------
template<typename T>
template<typename U>
HRESULT quetzal::com_ptr<T>::QueryInterface(com_ptr<U>& cp) const
{
    assert(m_p != nullptr);
    return m_p->QueryInterface(&cp.p());
}

//--------------------------------------------------------------------------
template<typename T>
template<typename U>
HRESULT quetzal::com_ptr<T>::QueryInterface(U** pp) const
{
    assert(pp != nullptr);
    checked_release(*pp);

    assert(m_p != nullptr);
    return m_p->QueryInterface(__uuidof(U), reinterpret_cast<void**>(pp));
}

//--------------------------------------------------------------------------
template<typename T>
void quetzal::com_ptr<T>::checked_addref()
{
    if (m_p != nullptr)
    {
//        log.info() << static_cast<void*>(m_p) << " AddRef" << std::endl;
        m_p->AddRef();
    }

    return;
}

//--------------------------------------------------------------------------
template<typename T>
void quetzal::com_ptr<T>::checked_release()
{
    if (m_p != nullptr)
    {
//        log.info() << static_cast<void*>(m_p) << " Release" << std::endl;
        m_p->Release();
        m_p = nullptr;
    }

    return;
}

//--------------------------------------------------------------------------
template<typename T>
template<typename U>
void quetzal::com_ptr<T>::checked_release(U*& p)
{
    if (p != nullptr)
    {
//        log.info() << static_cast<void*>(p) << " Release" << std::endl;
        p->Release();
        p = nullptr;
    }

    return;
}

//--------------------------------------------------------------------------
template<typename T>
void quetzal::swap(com_ptr<T>& a, com_ptr<T>& b) noexcept
{
    a.swap(b);
    return;
}

//--------------------------------------------------------------------------
template<typename T>
bool quetzal::operator==(T* p, const com_ptr<T>& cp)
{
    return cp == p;
}

//--------------------------------------------------------------------------
template<typename T>
bool quetzal::operator!=(T* p, const com_ptr<T>& cp)
{
    return !operator=(p, cp);
}

#endif // QUETZAL_COM_PTR_HPP
