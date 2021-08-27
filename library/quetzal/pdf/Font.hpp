#if !defined(QUETZAL_PDF_FONT_HPP)
#define QUETZAL_PDF_FONT_HPP
//------------------------------------------------------------------------------
// Font.hpp
//------------------------------------------------------------------------------

#include <string>
#include <vector>

namespace quetzal
{

namespace pdf
{

    typedef unsigned int id_type;

    //--------------------------------------------------------------------------
    class Font
    {
    public:

        typedef std::vector<unsigned int> metrics_type;

        Font();
        Font(id_type id, const std::string& name, const metrics_type& metrics);
        Font(const Font&) = default;
        virtual ~Font() = default;

        Font& operator=(const Font&) = default;

        id_type id() const;
        std::string name() const;

        double width(double size, const std::string& s) const;

    private:

        id_type m_id;
        std::string m_name; // BaseFont
        metrics_type m_metrics;
    };

    void init_fonts();
    unsigned int fonts();
    const Font* font(id_type id);
    const Font* font(const std::string& name);

    std::vector<std::string> word_wrap(const Font& font, double size, double wMax, const std::string& s, bool bRightJustify); // Font, or separate? ...

} // namespace pdf

} // namespace quetzal

#endif // QUETZAL_PDF_FONT_HPP
