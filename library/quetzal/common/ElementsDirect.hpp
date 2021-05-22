#if !defined(QUETZAL_ELEMENTSDIRECT_HPP)
#define QUETZAL_ELEMENTSDIRECT_HPP
//------------------------------------------------------------------------------
// common
// ElementsDirect.hpp
//------------------------------------------------------------------------------

#include "ElementsDirectIterator.hpp"
#include "ElementsDirectConstIterator.hpp"
#include "id.hpp"
#include <functional>
#include <cassert>

namespace quetzal
{

    //--------------------------------------------------------------------------
    template<typename S, typename E, typename I>
    class ElementsDirect
    {
    public:

        using source_type = S;
        using element_type = E;
        using i_type = I;
        using value_type = element_type;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = ElementsDirectIterator<source_type, element_type, i_type>;
        using const_iterator = ElementsDirectConstIterator<source_type, element_type, i_type>;
        using reverse_iterator = ElementsDirectIterator<source_type, element_type, i_type>;
        using const_reverse_iterator = ElementsDirectConstIterator<source_type, element_type, i_type>;

        using size_function_type = std::function<size_t(const source_type&)>;
        using terminal_function_type = std::function<i_type(const source_type&)>; // returns the i at the beginning or end of the range given a source element id
        using iterate_function_type = std::function<i_type(const source_type&, i_type)>; // returns the next i given the current i
        using element_function_type = std::function<element_type&(source_type&, i_type)>; // returns a reference to the element at i
        using const_element_function_type = std::function<const element_type&(const source_type&, i_type)>; // returns a const_reference to the element at i

        ElementsDirect(size_function_type size, terminal_function_type first, terminal_function_type last, terminal_function_type end,
                iterate_function_type forward, iterate_function_type reverse, element_function_type element, const_element_function_type const_element);
        ElementsDirect(const ElementsDirect&) = default;
        ElementsDirect(ElementsDirect&&) = default;
        ~ElementsDirect() = default;

        ElementsDirect& operator=(const ElementsDirect&) = default;
        ElementsDirect& operator=(ElementsDirect&&) = default;

        bool empty() const;
        size_t size() const;

        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        reverse_iterator rbegin();
        reverse_iterator rend();

        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;

        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        // Internal use
        const source_type* source() const;
        void set_source(const source_type& source) const;
        void set_source(source_type& source);
        void check_source(const source_type* const psource) const;

    private:

        mutable source_type* m_psource;
        size_function_type m_size;
        terminal_function_type m_first;
        terminal_function_type m_last;
        terminal_function_type m_end;
        iterate_function_type m_forward;
        iterate_function_type m_reverse;
        element_function_type m_element;
        const_element_function_type m_const_element;
    };

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirect<S, E, I>::ElementsDirect(size_function_type size,
        terminal_function_type first, terminal_function_type last, terminal_function_type end,
        iterate_function_type forward, iterate_function_type reverse, element_function_type element, const_element_function_type const_element) :
    m_psource(nullptr),
    m_size(size),
    m_first(first),
    m_last(last),
    m_end(end),
    m_forward(forward),
    m_reverse(reverse),
    m_element(element),
    m_const_element(const_element)
{
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
bool quetzal::ElementsDirect<S, E, I>::empty() const
{
    return cbegin() == cend();
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
size_t quetzal::ElementsDirect<S, E, I>::size() const
{
    assert(m_psource != nullptr);
    return m_size(*m_psource);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::iterator quetzal::ElementsDirect<S, E, I>::begin()
{
    assert(m_psource != nullptr);
    return iterator(*m_psource, m_first(*m_psource), m_end(*m_psource), m_forward, m_element);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::iterator quetzal::ElementsDirect<S, E, I>::end()
{
    assert(m_psource != nullptr);
    return iterator(*m_psource, m_end(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_iterator quetzal::ElementsDirect<S, E, I>::begin() const
{
    return cbegin();
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_iterator quetzal::ElementsDirect<S, E, I>::end() const
{
    return cend();
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_iterator quetzal::ElementsDirect<S, E, I>::cbegin() const
{
    assert(m_psource != nullptr);
    return const_iterator(*m_psource, m_first(*m_psource), m_end(*m_psource), m_forward, m_const_element);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_iterator quetzal::ElementsDirect<S, E, I>::cend() const
{
    assert(m_psource != nullptr);
    return const_iterator(*m_psource, m_end(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::reverse_iterator quetzal::ElementsDirect<S, E, I>::rbegin()
{
    assert(m_psource != nullptr);
    return reverse_iterator(*m_psource, m_last(*m_psource), m_end(*m_psource), m_reverse, m_element);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::reverse_iterator quetzal::ElementsDirect<S, E, I>::rend()
{
    assert(m_psource != nullptr);
    return reverse_iterator(*m_psource, m_end(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_reverse_iterator quetzal::ElementsDirect<S, E, I>::rbegin() const
{
    return crbegin();
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_reverse_iterator quetzal::ElementsDirect<S, E, I>::rend() const
{
    return crend();
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_reverse_iterator quetzal::ElementsDirect<S, E, I>::crbegin() const
{
    assert(m_psource != nullptr);
    return const_reverse_iterator(*m_psource, m_last(*m_psource), m_end(*m_psource), m_reverse, m_const_element);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_reverse_iterator quetzal::ElementsDirect<S, E, I>::crend() const
{
    assert(m_psource != nullptr);
    return const_reverse_iterator(*m_psource, m_end(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::reference quetzal::ElementsDirect<S, E, I>::front()
{
    assert(m_psource != nullptr);
    return m_element(*m_psource, m_first(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_reference quetzal::ElementsDirect<S, E, I>::front() const
{
    assert(m_psource != nullptr);
    return m_element(*m_psource, m_first(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::reference quetzal::ElementsDirect<S, E, I>::back()
{
    assert(m_psource != nullptr);
    return m_element(*m_psource, m_last(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirect<S, E, I>::const_reference quetzal::ElementsDirect<S, E, I>::back() const
{
    assert(m_psource != nullptr);
    return m_element(*m_psource, m_last(*m_psource));
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
const typename quetzal::ElementsDirect<S, E, I>::source_type* quetzal::ElementsDirect<S, E, I>::source() const
{
    return m_psource;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
void quetzal::ElementsDirect<S, E, I>::set_source(const source_type& source) const
{
    m_psource = const_cast<S*>(&source);
    return;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
void quetzal::ElementsDirect<S, E, I>::set_source(source_type& source)
{
    m_psource = &source;
    return;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
void quetzal::ElementsDirect<S, E, I>::check_source(const source_type* const psource) const
{
    assert(m_psource == psource);
    return;
}

#endif // QUETZAL_ELEMENTSDIRECT_HPP
