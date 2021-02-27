
#include "shapes.hpp"
#include <iostream>

p2t::Triangle::Triangle(const Point& a, const Point& b, const Point& c)
{
    m_points[0] = &a; m_points[1] = &b; m_points[2] = &c;
    m_neighbors[0] = nullptr; m_neighbors[1] = nullptr; m_neighbors[2] = nullptr;
    m_bEdgeConstrained[0] = m_bEdgeConstrained[1] = m_bEdgeConstrained[2] = false;
    m_bEdgeDelaunay[0] = m_bEdgeDelaunay[1] = m_bEdgeDelaunay[2] = false;
    m_bInterior = false;
}

const p2t::Point* p2t::Triangle::GetPoint(size_t n)
{
    return m_points[n];
}

p2t::Triangle* p2t::Triangle::GetNeighbor(size_t n)
{
    return m_neighbors[n];
}

// Update neighbor pointers
void p2t::Triangle::MarkNeighbor(const Point* p1, const Point* p2, Triangle* t)
{
    if ((p1 == m_points[2] && p2 == m_points[1]) || (p1 == m_points[1] && p2 == m_points[2]))
        m_neighbors[0] = t;
    else if ((p1 == m_points[0] && p2 == m_points[2]) || (p1 == m_points[2] && p2 == m_points[0]))
        m_neighbors[1] = t;
    else if ((p1 == m_points[0] && p2 == m_points[1]) || (p1 == m_points[1] && p2 == m_points[0]))
        m_neighbors[2] = t;
    else
        assert(false);
}

// Exhaustive search to update neighbor pointers
void p2t::Triangle::MarkNeighbor(Triangle& t)
{
    if (t.Contains(m_points[1], m_points[2]))
    {
        m_neighbors[0] = &t;
        t.MarkNeighbor(m_points[1], m_points[2], this);
    }
    else if (t.Contains(m_points[0], m_points[2]))
    {
        m_neighbors[1] = &t;
        t.MarkNeighbor(m_points[0], m_points[2], this);
    }
    else if (t.Contains(m_points[0], m_points[1]))
    {
        m_neighbors[2] = &t;
        t.MarkNeighbor(m_points[0], m_points[1], this);
    }
}

// Clears all references to all other triangles and points
void p2t::Triangle::Clear()
{
    Triangle *t;
    for (int i = 0; i < 3; i++)
    {
        t = m_neighbors[i];
        if (t != nullptr)
        {
            t->ClearNeighbor(this);
        }
    }
    ClearNeighbors();
    m_points[0] = m_points[1] = m_points[2] = nullptr;
}

void p2t::Triangle::ClearNeighbor(Triangle *triangle)
{
    if (m_neighbors[0] == triangle)
    {
        m_neighbors[0] = nullptr;
    }
    else if (m_neighbors[1] == triangle)
    {
        m_neighbors[1] = nullptr;
    }
    else
    {
        m_neighbors[2] = nullptr;
    }
}

void p2t::Triangle::ClearNeighbors()
{
    m_neighbors[0] = nullptr;
    m_neighbors[1] = nullptr;
    m_neighbors[2] = nullptr;
}

void p2t::Triangle::ClearDelunayEdges()
{
    m_bEdgeDelaunay[0] = m_bEdgeDelaunay[1] = m_bEdgeDelaunay[2] = false;
}

const p2t::Point* p2t::Triangle::OppositePoint(Triangle& t, const Point& p)
{
    const Point* cw = t.PointCW(p);
    double x = cw->x;
    double y = cw->y;
    x = p.x;
    y = p.y;
    return PointCW(*cw);
}

bool p2t::Triangle::Contains(const Point* p)
{
    return p == m_points[0] || p == m_points[1] || p == m_points[2];
}

bool p2t::Triangle::Contains(const Edge& e)
{
    return Contains(e.p) && Contains(e.q);
}

bool p2t::Triangle::Contains(const Point* p, const Point* q)
{
    return Contains(p) && Contains(q);
}

// Legalized triangle by rotating clockwise around point(0)
void p2t::Triangle::Legalize(const Point& point)
{
    m_points[1] = m_points[0];
    m_points[0] = m_points[2];
    m_points[2] = &point;
}

// Legalize triangle by rotating clockwise around oPoint
void p2t::Triangle::Legalize(const Point& opoint, const Point& npoint)
{
    if (&opoint == m_points[0])
    {
        m_points[1] = m_points[0];
        m_points[0] = m_points[2];
        m_points[2] = &npoint;
    }
    else if (&opoint == m_points[1])
    {
        m_points[2] = m_points[1];
        m_points[1] = m_points[0];
        m_points[0] = &npoint;
    }
    else if (&opoint == m_points[2])
    {
        m_points[0] = m_points[2];
        m_points[2] = m_points[1];
        m_points[1] = &npoint;
    }
    else
    {
        assert(false);
    }
}

int p2t::Triangle::Index(const Point* p)
{
    if (p == m_points[0])
    {
        return 0;
    }
    else if (p == m_points[1])
    {
        return 1;
    }
    else if (p == m_points[2])
    {
        return 2;
    }
    assert(false);
    return 0;
}

int p2t::Triangle::EdgeIndex(const Point* p1, const Point* p2)
{
    if (m_points[0] == p1)
    {
        if (m_points[1] == p2)
        {
            return 2;
        }
        else if (m_points[2] == p2)
        {
            return 1;
        }
    }
    else if (m_points[1] == p1)
    {
        if (m_points[2] == p2)
        {
            return 0;
        }
        else if (m_points[0] == p2)
        {
            return 2;
        }
    }
    else if (m_points[2] == p1)
    {
        if (m_points[0] == p2)
        {
            return 1;
        }
        else if (m_points[1] == p2)
        {
            return 0;
        }
    }
    return -1;
}

void p2t::Triangle::MarkConstrainedEdge(size_t n)
{
    m_bEdgeConstrained[n] = true;
}

void p2t::Triangle::MarkConstrainedEdge(const Edge& edge)
{
    MarkConstrainedEdge(edge.p, edge.q);
}

// Mark edge as constrained
void p2t::Triangle::MarkConstrainedEdge(const Point* p, const Point* q)
{
    if ((q == m_points[0] && p == m_points[1]) || (q == m_points[1] && p == m_points[0]))
    {
        m_bEdgeConstrained[2] = true;
    }
    else if ((q == m_points[0] && p == m_points[2]) || (q == m_points[2] && p == m_points[0]))
    {
        m_bEdgeConstrained[1] = true;
    }
    else if ((q == m_points[1] && p == m_points[2]) || (q == m_points[2] && p == m_points[1]))
    {
        m_bEdgeConstrained[0] = true;
    }
}

// The point counter-clockwise to given point
const p2t::Point* p2t::Triangle::PointCW(const Point& point)
{
    if (&point == m_points[0])
    {
        return m_points[2];
    }
    else if (&point == m_points[1])
    {
        return m_points[0];
    }
    else if (&point == m_points[2])
    {
        return m_points[1];
    }
    assert(false);
    return nullptr;
}

// The point counter-clockwise to given point
const p2t::Point* p2t::Triangle::PointCCW(const Point& point)
{
    if (&point == m_points[0])
    {
        return m_points[1];
    }
    else if (&point == m_points[1])
    {
        return m_points[2];
    }
    else if (&point == m_points[2])
    {
        return m_points[0];
    }
    assert(false);
    return nullptr;
}

// The neighbor clockwise to given point
p2t::Triangle* p2t::Triangle::NeighborCW(const Point& point)
{
    if (&point == m_points[0])
    {
        return m_neighbors[1];
    }
    else if (&point == m_points[1])
    {
        return m_neighbors[2];
    }
    return m_neighbors[0];
}

// The neighbor counter-clockwise to given point
p2t::Triangle* p2t::Triangle::NeighborCCW(const Point& point)
{
    if (&point == m_points[0])
    {
        return m_neighbors[2];
    }
    else if (&point == m_points[1])
    {
        return m_neighbors[0];
    }
    return m_neighbors[1];
}

bool p2t::Triangle::GetConstrainedEdgeCCW(const Point& p)
{
    if (&p == m_points[0])
    {
        return m_bEdgeConstrained[2];
    }
    else if (&p == m_points[1])
    {
        return m_bEdgeConstrained[0];
    }
    return m_bEdgeConstrained[1];
}

bool p2t::Triangle::GetConstrainedEdgeCW(const Point& p)
{
    if (&p == m_points[0])
    {
        return m_bEdgeConstrained[1];
    }
    else if (&p == m_points[1])
    {
        return m_bEdgeConstrained[2];
    }
    return m_bEdgeConstrained[0];
}

void p2t::Triangle::SetConstrainedEdgeCCW(const Point& p, bool b)
{
    if (&p == m_points[0])
    {
        m_bEdgeConstrained[2] = b;
    }
    else if (&p == m_points[1])
    {
        m_bEdgeConstrained[0] = b;
    }
    else
    {
        m_bEdgeConstrained[1] = b;
    }
}

void p2t::Triangle::SetConstrainedEdgeCW(const Point& p, bool b)
{
    if (&p == m_points[0])
    {
        m_bEdgeConstrained[1] = b;
    }
    else if (&p == m_points[1])
    {
        m_bEdgeConstrained[2] = b;
    }
    else
    {
        m_bEdgeConstrained[0] = b;
    }
}

bool p2t::Triangle::GetDelunayEdgeCCW(const Point& p)
{
    if (&p == m_points[0])
    {
        return m_bEdgeDelaunay[2];
    }
    else if (&p == m_points[1])
    {
        return m_bEdgeDelaunay[0];
    }
    return m_bEdgeDelaunay[1];
}

bool p2t::Triangle::GetDelunayEdgeCW(const Point& p)
{
    if (&p == m_points[0])
    {
        return m_bEdgeDelaunay[1];
    }
    else if (&p == m_points[1])
    {
        return m_bEdgeDelaunay[2];
    }
    return m_bEdgeDelaunay[0];
}

void p2t::Triangle::SetDelunayEdgeCCW(const Point& p, bool b)
{
    if (&p == m_points[0])
    {
        m_bEdgeDelaunay[2] = b;
    }
    else if (&p == m_points[1])
    {
        m_bEdgeDelaunay[0] = b;
    }
    else
    {
        m_bEdgeDelaunay[1] = b;
    }
}

void p2t::Triangle::SetDelunayEdgeCW(const Point& p, bool b)
{
    if (&p == m_points[0])
    {
        m_bEdgeDelaunay[1] = b;
    }
    else if (&p == m_points[1])
    {
        m_bEdgeDelaunay[2] = b;
    }
    else
    {
        m_bEdgeDelaunay[0] = b;
    }
}

bool p2t::Triangle::IsInterior()
{
    return m_bInterior;
}

void p2t::Triangle::set_interior(bool b)
{
    m_bInterior = b;
}

// The neighbor across to given point
p2t::Triangle& p2t::Triangle::NeighborAcross(const Point& opoint)
{
    if (&opoint == m_points[0])
    {
        return *m_neighbors[0];
    }
    else if (&opoint == m_points[1])
    {
        return *m_neighbors[1];
    }
    return *m_neighbors[2];
}

void p2t::Triangle::DebugPrint()
{
    using namespace std;
    cout << m_points[0]->x << "," << m_points[0]->y << " ";
    cout << m_points[1]->x << "," << m_points[1]->y << " ";
    cout << m_points[2]->x << "," << m_points[2]->y << endl;
}
