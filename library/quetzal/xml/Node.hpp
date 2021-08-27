#if !defined(QUETZAL_XML_NODE_HPP)
#define QUETZAL_XML_NODE_HPP
//------------------------------------------------------------------------------
// XML file io
// Node.hpp
//------------------------------------------------------------------------------

#include <iosfwd>

namespace quetzal::xml
{

    //--------------------------------------------------------------------------
    class Node
    {
    public:

        Node();
        Node(const Node& other) = default;
        virtual ~Node() = default;

        Node& operator=(const Node& other) = default;

        virtual bool empty() const = 0;
        virtual void read(std::istream& is) const = 0;
        virtual void write(std::ostream& os) const = 0;

    private:
    };

    std::istream& operator>>(std::istream& is, const Node& node);
    std::ostream& operator<<(std::ostream& os, const Node& node);

} // namespace quetzal::xml

#endif // QUETZAL_XML_NODE_HPP
