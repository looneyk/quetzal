//------------------------------------------------------------------------------
// svg
// Document.cpp
//------------------------------------------------------------------------------

#include "Document.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::svg::Document::Document() :
    m_document(),
    m_pelement(&m_document.element()),
    m_bAutoViewBox(false),
    m_sizeBorder(),
    m_pointMin(),
    m_pointMax(),
    m_bFirst(true)
{
    m_document.element().set_name("svg");
    m_document.element().set_attribute("xmlns", "http://www.w3.org/2000/svg");
    m_document.element().set_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
}

//------------------------------------------------------------------------------
quetzal::svg::Document::Document(const dimensions_type& sizeBorder) :
    Document()
{
    m_bAutoViewBox = true;
    m_sizeBorder = sizeBorder;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::set_viewport(const dimensions_type& size)
{
    m_document.element().set_attribute("width", to_string(size.x()));
    m_document.element().set_attribute("height", to_string(size.y()));
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::set_view_box(const point_type& position, const dimensions_type& size)
{
    ostringstream oss;
    oss << position.x() << " " << position.y() << " " << size.x() << " " << size.y();
    m_document.element().set_attribute("viewBox", oss.str());
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::set_border(const dimensions_type& size)
{
    m_sizeBorder = size;
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::write(const string& filename)
{
    if (m_bAutoViewBox)
    {
//        dimensions_type size = {m_pointMax.x() - m_pointMin.x(), m_pointMax.y() - m_pointMin.y()};
// ...
dimensions_type size;
//size.set(m_pointMax.x() - m_pointMin.x(), m_pointMax.y() - m_pointMin.y());
size.set_x(m_pointMax.x() - m_pointMin.x());
size.set_y(m_pointMax.y() - m_pointMin.y());
        if (size.x() < size.y())
        {
            size.x() = size.y();
        }
        else
        {
            size.y() = size.x();
        }

        m_pointMin -= m_sizeBorder;
        size += 2.0 * m_sizeBorder;
        set_view_box(m_pointMin, size);
    }

    ofstream ofs(filename);
    assert(ofs.good());
    ofs << m_document;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::close_symbol()
{
    m_pelement = m_pelement->parent();
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::close_group()
{
    m_pelement = m_pelement->parent();
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::close_defs()
{
    m_pelement = m_pelement->parent();
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::close_svg()
{
    m_pelement = m_pelement->parent();
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::set_attributes(std::shared_ptr<xml::Element> pelement)
{
    pelement;
    return;
}

//------------------------------------------------------------------------------
void quetzal::svg::Document::update_size(point_type point)
{
    if (m_bAutoViewBox)
    {
        if (m_bFirst)
        {
            m_pointMin = point;
            m_pointMax = point;
            m_bFirst = false;
        }
        else
        {
            if (point.x() < m_pointMin.x())
            {
                m_pointMin.x() = point.x();
            }
            if (point.y() < m_pointMin.y())
            {
                m_pointMin.y() = point.y();
            }
            if (point.x() > m_pointMax.x())
            {
                m_pointMax.x() = point.x();
            }
            if (point.y() > m_pointMax.y())
            {
                m_pointMax.y() = point.y();
            }
        }
    }

    return;
}
