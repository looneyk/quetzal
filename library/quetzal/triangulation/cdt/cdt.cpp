
#include "cdt.hpp"
#include "Sweep.hpp"

using namespace std;

p2t::CDT::CDT(const vector<Point>& polygon) :
    sweep_context_(),
    m_face(),
    m_holes()
{
    vector<Point*> points;

    for (const auto& point : polygon)
    {
        points.push_back(new p2t::Point(point));
    }

    sweep_context_.AddFace(points);
}

p2t::CDT::~CDT()
{
    for (size_t i = 0; i < m_face.size(); ++i)
    {
        delete m_face[i];
    }

    for (size_t i = 0; i < m_holes.size(); ++i)
    {
        vector<Point*> hole = m_holes[i];
        for (size_t j = 0; j < hole.size(); ++j)
        {
            delete hole[j];
        }
    }
}

void p2t::CDT::AddHole(const vector<Point>& polygon)
{
    vector<Point*> points;

    for (const auto& point : polygon)
    {
        points.push_back(new p2t::Point(point));
    }

    m_holes.push_back(points);

    sweep_context_.AddHole(points);
}

void p2t::CDT::AddPoint(Point* point)
{
    sweep_context_.AddPoint(point);
}

void p2t::CDT::Triangulate()
{
    Sweep sweep;
    sweep.Triangulate(sweep_context_);
}

const vector<p2t::Triangle>& p2t::CDT::GetTriangles()
{
    return sweep_context_.GetTriangles();
}
