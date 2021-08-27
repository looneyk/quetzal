#if !defined(QUETZAL_XML_DECLARATION_HPP)
#define QUETZAL_XML_DECLARATION_HPP
//------------------------------------------------------------------------------
// XML file io
// Declaration.hpp
//------------------------------------------------------------------------------

//#include "Node.hpp"
#include <string>

namespace quetzal::xml
{

    //--------------------------------------------------------------------------
//    class Declaration : public Node
    class Declaration
    {
    public:

        Declaration();
        Declaration(const Declaration&) = default;
        ~Declaration() = default;

        Declaration& operator=(const Declaration&) = default;

        virtual bool empty() const;

        std::string version() const;
        std::string encoding() const;
        std::string standalone() const;

        void set_version(const std::string& s);
        void set_encoding(const std::string& s);
        void set_standalone(const std::string& s);

        virtual void read(std::istream& is);
        virtual void write(std::ostream& os) const;

    private:

        std::string m_version;
        std::string m_encoding;
        std::string m_standalone;
    };

    std::istream& operator>>(std::istream& is, Declaration& declaration); // Node ...
    std::ostream& operator<<(std::ostream& os, const Declaration& declaration); // Node ...

} // namespace quetzal::xml

#endif // QUETZAL_XML_DECLARATION_HPP
