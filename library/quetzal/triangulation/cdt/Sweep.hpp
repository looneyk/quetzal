#if !defined(CDT_SWEEP_HPP)
#define CDT_SWEEP_HPP
/*
 * Sweep-line, Constrained Delauney Triangulation (CDT) See: Domiter, V. and
 * Zalik, B.(2008)'Sweep-line algorithm for constrained Delaunay triangulation',
 * International Journal of Geographical Information Science
 *
 * "FlipScan" Constrained Edge Algorithm invented by Thomas Åhlén, thahlen@gmail.com
 */

#include <vector>

namespace p2t
{

    enum class Orientation
    {
        CW,
        CCW,
        COLINEAR
    };

    class SweepContext;
    struct Node;
    struct Point;
    struct Edge;
    class Triangle;

    class Sweep
    {
    public:

        Sweep();
        ~Sweep();

        void Triangulate(SweepContext& tcx);

    private:

        // Start sweeping the Y-sorted point set from bottom to top
        void SweepPoints(SweepContext& tcx);

        // Find closes node to the left of the new point and create a new triangle. If needed new holes and basins will be filled to.
        Node& PointEvent(SweepContext& tcx, const Point& point);

        void EdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

        void EdgeEvent(SweepContext& tcx, const Point& ep, const Point& eq, Triangle* triangle, const Point& point);

        // Creates a new front triangle and legalize it
        Node& NewFrontTriangle(SweepContext& tcx, const Point& point, Node& node);

        /**
        * Adds a triangle to the advancing front to fill a hole.
        * @param node - middle node, that is the bottom of the hole
        */
        void Fill(SweepContext& tcx, Node& node);

        // Returns true if triangle was legalized
        bool Legalize(SweepContext& tcx, Triangle& t);

        /**
        * <b>Requirement</b>:<br>
        * 1. a,b and c form a triangle.<br>
        * 2. a and d is know to be on opposite side of bc<br>
        * <pre>
        *                a
        *                +
        *               / \
        *              /   \
        *            b/     \c
        *            +-------+
        *           /    d    \
        *          /           \
        * </pre>
        * <b>Fact</b>: d has to be in area B to have a chance to be inside the circle formed by
        *  a,b and c<br>
        *  d is outside B if orient2d(a,b,d) or orient2d(c,a,d) is CW<br>
        *  This preknowledge gives us a way to optimize the incircle test
        * @param a - triangle point, opposite d
        * @param b - triangle point
        * @param c - triangle point
        * @param d - point opposite a
        * @return true if d is inside circle, false if on circle edge
        */
        bool Incircle(const Point& pa, const Point& pb, const Point& pc, const Point& pd);

        /**
        * Rotates a triangle pair one vertex CW
        *<pre>
        *       n2                    n2
        *  P +-----+             P +-----+
        *    | t  /|               |\  t |
        *    |   / |               | \   |
        *  n1|  /  |n3           n1|  \  |n3
        *    | /   |    after CW   |   \ |
        *    |/ oT |               | oT \|
        *    +-----+ oP            +-----+
        *       n4                    n4
        * </pre>
        */
        void RotateTrianglePair(Triangle& t, const Point& p, Triangle& ot, const Point& op);

        // Fills holes in the Advancing Front
        void FillAdvancingFront(SweepContext& tcx, Node& n);

        // Decision-making about when to Fill hole.
        // Contributed by ToolmakerSteve2
        bool LargeHole_DontFill(Node* node);
        bool AngleExceeds90Degrees(const Point& origin, const Point& pa, const Point& pb);
        bool AngleExceedsPlus90DegreesOrIsNegative(const Point& origin, const Point& pa, const Point& pb);
        double Angle(const Point& origin, const Point& pa, const Point& pb);

        /**
        * @param node - middle node
        * @return the angle between 3 front nodes
        */
        double HoleAngle(Node& node);

        // The basin angle is decided against the horizontal line [1,0]
        double BasinAngle(Node& node);

        /**
        * Fills a basin that has formed on the Advancing Front to the right of given node.<br>
        * First we decide a left,bottom and right node that forms the boundaries of the basin. Then we do a reqursive fill.
        *
        * @param node - starting node, this or next node will be left node
        */
        void FillBasin(SweepContext& tcx, Node& node);

        /**
        * Recursive algorithm to fill a Basin with triangles
        *
        * @param node - bottom_node
        * @param cnt - counter used to alternate on even and odd numbers
        */
        void FillBasinReq(SweepContext& tcx, Node* node);

        bool IsShallow(SweepContext& tcx, Node& node);

        bool IsEdgeSideOfTriangle(Triangle& triangle, const Point& ep, const Point& eq);

        void FillEdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

        void FillRightAboveEdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

        void FillRightBelowEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

        void FillRightConcaveEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

        void FillRightConvexEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

        void FillLeftAboveEdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

        void FillLeftBelowEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

        void FillLeftConcaveEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

        void FillLeftConvexEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

        void FlipEdgeEvent(SweepContext& tcx, const Point& ep, const Point& eq, Triangle* t, const Point& p);

        /**
        * After a flip we have two triangles and know that only one will still be
        * intersecting the edge. So decide which to contiune with and legalize the other
        *
        * @param o - should be the result of an orient2d( eq, op, ep )
        * @param t - triangle 1
        * @param ot - triangle 2
        * @param p - a point shared by both triangles
        * @param op - another point shared by both triangles
        * @return returns the triangle still intersecting the edge
        */
        Triangle& NextFlipTriangle(SweepContext& tcx, Orientation o, Triangle&  t, Triangle& ot, const Point& p, const Point& op);

        /**
         * When we need to traverse from one triangle to the next we need
         * the point in current triangle that is the opposite point to the next triangle.
         */
        const Point& NextFlipPoint(const Point& ep, const Point& eq, Triangle& ot, const Point& op);

        /**
         * Scan part of the FlipScan algorithm<br>
         * When a triangle pair isn't flippable we will scan for the next
         * point that is inside the flip triangle scan area. When found
         * we generate a new flipEdgeEvent
         *
         * @param ep - last point on the edge we are traversing
         * @param eq - first point on the edge we are traversing
         * @param flipTriangle - the current triangle sharing the point eq with edge
         */
        void FlipScanEdgeEvent(SweepContext& tcx, const Point& ep, const Point& eq, Triangle& flip_triangle, Triangle& t, const Point& p);

        void FinalizationPolygon(SweepContext& tcx);

        std::vector<Node*> nodes_;
    };

}

#endif // CDT_SWEEP_HPP
