#if !defined(QUETZAL_ELEMENTSDIRECTCONSTITERATOR_HPP)
#define QUETZAL_ELEMENTSDIRECTCONSTITERATOR_HPP
//------------------------------------------------------------------------------
// common
// ElementsDirectConstIterator.hpp
//------------------------------------------------------------------------------

#include "id.hpp"
#include <iterator>
#include <functional>
#include <cassert>

namespace quetzal
{

    //--------------------------------------------------------------------------
    template<typename S, typename E, typename I>
    class ElementsDirectConstIterator
    {
    public:

        using source_type = S;
        using element_type = E;
        using i_type = I;
        using iterator_category = std::forward_iterator_tag;
        using value_type = element_type;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterate_function_type = std::function<i_type(const source_type&, i_type)>; // returns the next i given the current i
        using element_function_type = std::function<const element_type&(const source_type&, i_type)>; // returns a reference to the element at i

        ElementsDirectConstIterator(const source_type& source, i_type end); // end
        ElementsDirectConstIterator(const source_type& source, i_type begin, i_type end, iterate_function_type advance, element_function_type element);
        ElementsDirectConstIterator(const ElementsDirectConstIterator&) = default;
        ElementsDirectConstIterator(const ElementsDirectIterator<S, E, I>& other);
        ~ElementsDirectConstIterator() = default;

        ElementsDirectConstIterator& operator=(const ElementsDirectConstIterator&) = default;

        bool operator==(const ElementsDirectConstIterator& other);
        bool operator!=(const ElementsDirectConstIterator& other);

        ElementsDirectConstIterator& operator++();
        ElementsDirectConstIterator operator++(int);

        reference operator*();
        pointer operator->();

    private:

        const source_type* m_psource;
        i_type m_begin;
        i_type m_end;
        i_type m_i;
        iterate_function_type m_advance;
        element_function_type m_element;
    };

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectConstIterator<S, E, I>::ElementsDirectConstIterator(const source_type& source, i_type end) :
    m_psource(&source),
    m_begin(end),
    m_end(end),
    m_i(end),
    m_advance(),
    m_element()
{
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectConstIterator<S, E, I>::ElementsDirectConstIterator(const source_type& source, i_type begin, i_type end, iterate_function_type advance, element_function_type element) :
    m_psource(&source),
    m_begin(begin),
    m_end(end),
    m_i(begin),
    m_advance(advance),
    m_element(element)
{
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectConstIterator<S, E, I>::ElementsDirectConstIterator(const ElementsDirectIterator<S, E, I>& other) :
    m_psource(other.m_psource),
    m_begin(other.m_begin),
    m_end(other.m_end),
    m_i(other.m_i),
    m_advance(other.m_advance),
    m_element(other.m_element)
{
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
bool quetzal::ElementsDirectConstIterator<S, E, I>::operator==(const ElementsDirectConstIterator& other)
{
    return m_i == other.m_i;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
bool quetzal::ElementsDirectConstIterator<S, E, I>::operator!=(const ElementsDirectConstIterator& other)
{
    return !(*this == other);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectConstIterator<S, E, I>& quetzal::ElementsDirectConstIterator<S, E, I>::operator++()
{
    assert(m_psource != nullptr);
    m_i = m_advance(*m_psource, m_i);
    return *this;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectConstIterator<S, E, I> quetzal::ElementsDirectConstIterator<S, E, I>::operator++(int)
{
    ElementsDirectConstIterator i(*this);
    operator++();
    return i;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirectConstIterator<S, E, I>::reference quetzal::ElementsDirectConstIterator<S, E, I>::operator*()
{
    assert(m_psource != nullptr);
    return m_element(*m_psource, m_i);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirectConstIterator<S, E, I>::pointer quetzal::ElementsDirectConstIterator<S, E, I>::operator->()
{
    assert(m_psource != nullptr);
    return &m_element(*m_psource, m_i);
}

#endif // QUETZAL_ELEMENTSDIRECTCONSTITERATOR_HPP
