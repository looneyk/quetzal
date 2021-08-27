#if !defined(QUETZAL_ELEMENTSDIRECTITERATOR_HPP)
#define QUETZAL_ELEMENTSDIRECTITERATOR_HPP
//------------------------------------------------------------------------------
// common
// ElementsDirectIterator.hpp
//------------------------------------------------------------------------------

#include "id.hpp"
#include <iterator>
#include <functional>
#include <cassert>

namespace quetzal
{

    template<typename S, typename E, typename I>
    class ElementsConstIterator;

    //--------------------------------------------------------------------------
    template<typename S, typename E, typename I>
    class ElementsDirectIterator
    {
    public:

        using source_type = S;
        using element_type = E;
        using i_type = I;
        using iterator_category = std::forward_iterator_tag;
        using value_type = element_type;
        using difference_type = ptrdiff_t;
        using pointer_type = value_type*;
        using reference_type = value_type&;
        using iterate_function_type = std::function<i_type(source_type&, i_type)>; // returns the next i given the current i
        using element_function_type = std::function<element_type&(source_type&, i_type)>; // returns a reference to the element at i

        ElementsDirectIterator(source_type& source, i_type end); // end
        ElementsDirectIterator(source_type& source, i_type begin, i_type end, iterate_function_type advance, element_function_type element);
        ElementsDirectIterator(const ElementsDirectIterator&) = default;
        ~ElementsDirectIterator() = default;

        ElementsDirectIterator& operator=(const ElementsDirectIterator&) = default;

        bool operator==(const ElementsDirectIterator& other) const;
        bool operator!=(const ElementsDirectIterator& other) const;

        ElementsDirectIterator& operator++();
        ElementsDirectIterator operator++(int);

        reference_type operator*();
        pointer_type operator->();

    private:

        template<typename S, typename E, typename I>
        friend class ElementsConstIterator;

        source_type* m_psource;
        i_type m_begin;
        i_type m_end;
        i_type m_i;
        iterate_function_type m_advance;
        element_function_type m_element;
    };

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectIterator<S, E, I>::ElementsDirectIterator(source_type& source, i_type end) :
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
quetzal::ElementsDirectIterator<S, E, I>::ElementsDirectIterator(source_type& source, i_type begin, i_type end, iterate_function_type advance, element_function_type element) :
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
bool quetzal::ElementsDirectIterator<S, E, I>::operator==(const ElementsDirectIterator& other) const
{
    return m_i == other.m_i;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
bool quetzal::ElementsDirectIterator<S, E, I>::operator!=(const ElementsDirectIterator& other) const
{
    return !(*this == other);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectIterator<S, E, I>& quetzal::ElementsDirectIterator<S, E, I>::operator++()
{
    assert(m_psource != nullptr);
    m_i = m_advance(*m_psource, m_i);
    return *this;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
quetzal::ElementsDirectIterator<S, E, I> quetzal::ElementsDirectIterator<S, E, I>::operator++(int)
{
    ElementsDirectIterator i(*this);
    operator++();
    return i;
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirectIterator<S, E, I>::reference_type quetzal::ElementsDirectIterator<S, E, I>::operator*()
{
    assert(m_psource != nullptr);
    return m_element(*m_psource, m_i);
}

//------------------------------------------------------------------------------
template<typename S, typename E, typename I>
typename quetzal::ElementsDirectIterator<S, E, I>::pointer_type quetzal::ElementsDirectIterator<S, E, I>::operator->()
{
    assert(m_psource != nullptr);
    return &m_element(*m_psource, m_i);
}

#endif // QUETZAL_ELEMENTSDIRECTITERATOR_HPP
