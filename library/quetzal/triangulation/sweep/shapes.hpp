#ifndef P2T_SHAPES_H
#define P2T_SHAPES_H

#include <vector>
#include <cassert>

namespace p2t
{

    struct Edge;

    struct Point
    {

        Point() :
            m_id(static_cast<size_t>(-1)), // nullid
            x(0.0),
            y(0.0)
        {
        }

        Point(size_t id, double xx, double yy) :
            m_id(id),
            x(xx),
            y(yy)
        {
        }

        size_t m_id;
        double x, y;

        // The edges this point constitutes an upper ending point
        mutable std::vector<Edge*> edge_list; // don't like this mutable ...

    };

    inline bool operator==(const Point& a, const Point& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    inline bool operator!=(const Point& a, const Point& b)
    {
        return !(a == b);
    }

    // Represents a simple polygon's edge
    struct Edge
    {

        Edge(Point& a, Point& b) :
            p(&a),
            q(&b)
        {
            assert(a != b);

            if (a.y > b.y)
            {
                q = &a;
                p = &b;
            }
            else if (a.y == b.y)
            {
                if (a.x > b.x)
                {
                    q = &a;
                    p = &b;
                }
            }

            q->edge_list.push_back(this);
        }

        const Point* p;
        const Point* q;
    };

    // Triangle-based data structures are know to have better performance than quad-edge structures
    // See: J. Shewchuk, "Triangle: Engineering a 2D Quality Mesh Generator and Delaunay Triangulator"
    //      "Triangulations in CGAL"
    class Triangle
    {
    public:

        Triangle(const Point& a, const Point& b, const Point& c);

        const Point* GetPoint(size_t n);
        const Point* PointCW(const Point& point);
        const Point* PointCCW(const Point& point);
        const Point* OppositePoint(Triangle& t, const Point& p);

        Triangle* GetNeighbor(size_t n);
        void MarkNeighbor(const Point* p1, const Point* p2, Triangle* t);
        void MarkNeighbor(Triangle& t);

        void MarkConstrainedEdge(size_t n);
        void MarkConstrainedEdge(const Edge& edge);
        void MarkConstrainedEdge(const Point* p, const Point* q);

        int Index(const Point* p);
        int EdgeIndex(const Point* p1, const Point* p2);

        Triangle* NeighborCW(const Point& point);
        Triangle* NeighborCCW(const Point& point);
        bool GetConstrainedEdgeCCW(const Point& p);
        bool GetConstrainedEdgeCW(const Point& p);
        void SetConstrainedEdgeCCW(const Point& p, bool b);
        void SetConstrainedEdgeCW(const Point& p, bool b);
        bool GetDelunayEdgeCCW(const Point& p);
        bool GetDelunayEdgeCW(const Point& p);
        void SetDelunayEdgeCCW(const Point& p, bool b);
        void SetDelunayEdgeCW(const Point& p, bool b);

        bool Contains(const Point* p);
        bool Contains(const Edge& e);
        bool Contains(const Point* p, const Point* q);
        void Legalize(const Point& point);
        void Legalize(const Point& opoint, const Point& npoint);

        // Clears all references to all other triangles and points
        void Clear();
        void ClearNeighbor(Triangle *triangle );
        void ClearNeighbors();
        void ClearDelunayEdges();

        bool IsInterior();
        void set_interior(bool b);

        Triangle& NeighborAcross(const Point& opoint);

        void DebugPrint();

        // Flags to determine if an edge is a Constrained edge
        bool constrained_edge[3];
        // Flags to determine if an edge is a Delauney edge
        bool delaunay_edge[3];

    private:

        const Point* points_[3];
        Triangle* neighbors_[3];

        // Has this triangle been marked as an interior triangle?
        bool interior_;
    };

}

#endif
