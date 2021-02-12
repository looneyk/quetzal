
#include "shapes.hpp"
#include <iostream>

p2t::Triangle::Triangle(const Point& a, const Point& b, const Point& c)
{
    points_[0] = &a; points_[1] = &b; points_[2] = &c;
    neighbors_[0] = nullptr; neighbors_[1] = nullptr; neighbors_[2] = nullptr;
    constrained_edge[0] = constrained_edge[1] = constrained_edge[2] = false;
    delaunay_edge[0] = delaunay_edge[1] = delaunay_edge[2] = false;
    interior_ = false;
}

const p2t::Point* p2t::Triangle::GetPoint(size_t n)
{
    return points_[n];
}

p2t::Triangle* p2t::Triangle::GetNeighbor(size_t n)
{
    return neighbors_[n];
}

// Update neighbor pointers
void p2t::Triangle::MarkNeighbor(const Point* p1, const Point* p2, Triangle* t)
{
    if ((p1 == points_[2] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[2]))
        neighbors_[0] = t;
    else if ((p1 == points_[0] && p2 == points_[2]) || (p1 == points_[2] && p2 == points_[0]))
        neighbors_[1] = t;
    else if ((p1 == points_[0] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[0]))
        neighbors_[2] = t;
    else
        assert(false);
}

// Exhaustive search to update neighbor pointers
void p2t::Triangle::MarkNeighbor(Triangle& t)
{
    if (t.Contains(points_[1], points_[2]))
    {
        neighbors_[0] = &t;
        t.MarkNeighbor(points_[1], points_[2], this);
    }
    else if (t.Contains(points_[0], points_[2]))
    {
        neighbors_[1] = &t;
        t.MarkNeighbor(points_[0], points_[2], this);
    }
    else if (t.Contains(points_[0], points_[1]))
    {
        neighbors_[2] = &t;
        t.MarkNeighbor(points_[0], points_[1], this);
    }
}

// Clears all references to all other triangles and points
void p2t::Triangle::Clear()
{
    Triangle *t;
    for (int i = 0; i < 3; i++)
    {
        t = neighbors_[i];
        if (t != nullptr)
        {
            t->ClearNeighbor(this);
        }
    }
    ClearNeighbors();
    points_[0] = points_[1] = points_[2] = nullptr;
}

void p2t::Triangle::ClearNeighbor(Triangle *triangle)
{
    if (neighbors_[0] == triangle)
    {
        neighbors_[0] = nullptr;
    }
    else if (neighbors_[1] == triangle)
    {
        neighbors_[1] = nullptr;
    }
    else
    {
        neighbors_[2] = nullptr;
    }
}

void p2t::Triangle::ClearNeighbors()
{
    neighbors_[0] = nullptr;
    neighbors_[1] = nullptr;
    neighbors_[2] = nullptr;
}

void p2t::Triangle::ClearDelunayEdges()
{
    delaunay_edge[0] = delaunay_edge[1] = delaunay_edge[2] = false;
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
    return p == points_[0] || p == points_[1] || p == points_[2];
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
    points_[1] = points_[0];
    points_[0] = points_[2];
    points_[2] = &point;
}

// Legalize triagnle by rotating clockwise around oPoint
void p2t::Triangle::Legalize(const Point& opoint, const Point& npoint)
{
    if (&opoint == points_[0])
    {
        points_[1] = points_[0];
        points_[0] = points_[2];
        points_[2] = &npoint;
    }
    else if (&opoint == points_[1])
    {
        points_[2] = points_[1];
        points_[1] = points_[0];
        points_[0] = &npoint;
    }
    else if (&opoint == points_[2])
    {
        points_[0] = points_[2];
        points_[2] = points_[1];
        points_[1] = &npoint;
    }
    else
    {
        assert(false);
    }
}

int p2t::Triangle::Index(const Point* p)
{
    if (p == points_[0])
    {
        return 0;
    }
    else if (p == points_[1])
    {
        return 1;
    }
    else if (p == points_[2])
    {
        return 2;
    }
    assert(false);
    return 0;
}

int p2t::Triangle::EdgeIndex(const Point* p1, const Point* p2)
{
    if (points_[0] == p1)
    {
        if (points_[1] == p2)
        {
            return 2;
        }
        else if (points_[2] == p2)
        {
            return 1;
        }
    }
    else if (points_[1] == p1)
    {
        if (points_[2] == p2)
        {
            return 0;
        }
        else if (points_[0] == p2)
        {
            return 2;
        }
    }
    else if (points_[2] == p1)
    {
        if (points_[0] == p2)
        {
            return 1;
        }
        else if (points_[1] == p2)
        {
            return 0;
        }
    }
    return -1;
}

void p2t::Triangle::MarkConstrainedEdge(size_t n)
{
    constrained_edge[n] = true;
}

void p2t::Triangle::MarkConstrainedEdge(const Edge& edge)
{
    MarkConstrainedEdge(edge.p, edge.q);
}

// Mark edge as constrained
void p2t::Triangle::MarkConstrainedEdge(const Point* p, const Point* q)
{
    if ((q == points_[0] && p == points_[1]) || (q == points_[1] && p == points_[0]))
    {
        constrained_edge[2] = true;
    }
    else if ((q == points_[0] && p == points_[2]) || (q == points_[2] && p == points_[0]))
    {
        constrained_edge[1] = true;
    }
    else if ((q == points_[1] && p == points_[2]) || (q == points_[2] && p == points_[1]))
    {
        constrained_edge[0] = true;
    }
}

// The point counter-clockwise to given point
const p2t::Point* p2t::Triangle::PointCW(const Point& point)
{
    if (&point == points_[0])
    {
        return points_[2];
    }
    else if (&point == points_[1])
    {
        return points_[0];
    }
    else if (&point == points_[2])
    {
        return points_[1];
    }
    assert(false);
    return nullptr;
}

// The point counter-clockwise to given point
const p2t::Point* p2t::Triangle::PointCCW(const Point& point)
{
    if (&point == points_[0])
    {
        return points_[1];
    }
    else if (&point == points_[1])
    {
        return points_[2];
    }
    else if (&point == points_[2])
    {
        return points_[0];
    }
    assert(false);
    return nullptr;
}

// The neighbor clockwise to given point
p2t::Triangle* p2t::Triangle::NeighborCW(const Point& point)
{
    if (&point == points_[0])
    {
        return neighbors_[1];
    }
    else if (&point == points_[1])
    {
        return neighbors_[2];
    }
    return neighbors_[0];
}

// The neighbor counter-clockwise to given point
p2t::Triangle* p2t::Triangle::NeighborCCW(const Point& point)
{
    if (&point == points_[0])
    {
        return neighbors_[2];
    }
    else if (&point == points_[1])
    {
        return neighbors_[0];
    }
    return neighbors_[1];
}

bool p2t::Triangle::GetConstrainedEdgeCCW(const Point& p)
{
    if (&p == points_[0])
    {
        return constrained_edge[2];
    }
    else if (&p == points_[1])
    {
        return constrained_edge[0];
    }
    return constrained_edge[1];
}

bool p2t::Triangle::GetConstrainedEdgeCW(const Point& p)
{
    if (&p == points_[0])
    {
        return constrained_edge[1];
    }
    else if (&p == points_[1])
    {
        return constrained_edge[2];
    }
    return constrained_edge[0];
}

void p2t::Triangle::SetConstrainedEdgeCCW(const Point& p, bool b)
{
    if (&p == points_[0])
    {
        constrained_edge[2] = b;
    }
    else if (&p == points_[1])
    {
        constrained_edge[0] = b;
    }
    else
    {
        constrained_edge[1] = b;
    }
}

void p2t::Triangle::SetConstrainedEdgeCW(const Point& p, bool b)
{
    if (&p == points_[0])
    {
        constrained_edge[1] = b;
    }
    else if (&p == points_[1])
    {
        constrained_edge[2] = b;
    }
    else
    {
        constrained_edge[0] = b;
    }
}

bool p2t::Triangle::GetDelunayEdgeCCW(const Point& p)
{
    if (&p == points_[0])
    {
        return delaunay_edge[2];
    }
    else if (&p == points_[1])
    {
        return delaunay_edge[0];
    }
    return delaunay_edge[1];
}

bool p2t::Triangle::GetDelunayEdgeCW(const Point& p)
{
    if (&p == points_[0])
    {
        return delaunay_edge[1];
    }
    else if (&p == points_[1])
    {
        return delaunay_edge[2];
    }
    return delaunay_edge[0];
}

void p2t::Triangle::SetDelunayEdgeCCW(const Point& p, bool b)
{
    if (&p == points_[0])
    {
        delaunay_edge[2] = b;
    }
    else if (&p == points_[1])
    {
        delaunay_edge[0] = b;
    }
    else
    {
        delaunay_edge[1] = b;
    }
}

void p2t::Triangle::SetDelunayEdgeCW(const Point& p, bool b)
{
    if (&p == points_[0])
    {
        delaunay_edge[1] = b;
    }
    else if (&p == points_[1])
    {
        delaunay_edge[2] = b;
    }
    else
    {
        delaunay_edge[0] = b;
    }
}

bool p2t::Triangle::IsInterior()
{
    return interior_;
}

void p2t::Triangle::set_interior(bool b)
{
    interior_ = b;
}

// The neighbor across to given point
p2t::Triangle& p2t::Triangle::NeighborAcross(const Point& opoint)
{
    if (&opoint == points_[0])
    {
        return *neighbors_[0];
    }
    else if (&opoint == points_[1])
    {
        return *neighbors_[1];
    }
    return *neighbors_[2];
}

void p2t::Triangle::DebugPrint()
{
    using namespace std;
    cout << points_[0]->x << "," << points_[0]->y << " ";
    cout << points_[1]->x << "," << points_[1]->y << " ";
    cout << points_[2]->x << "," << points_[2]->y << endl;
}
