
#include "sweep.hpp"
#include "sweep_context.hpp"
#include "advancing_front.hpp"
#include "quetzal/math/floating_point.hpp"
#include "quetzal/math/math_util.hpp"

using namespace quetzal;

namespace
{

    const double PiThreeFourth = 0.75 * math::Pi<double>;

    /**
     * Forumla to calculate signed area<br>
     * Positive if CCW<br>
     * Negative if CW<br>
     * 0 if colinear<br>
     * <pre>
     * A[P1,P2,P3]  =  (x1*y2 - y1*x2) + (x2*y3 - y2*x3) + (x3*y1 - y3*x1)
     *              =  (x1-x3)*(y2-y3) - (y1-y3)*(x2-x3)
     * </pre>
     */
    p2t::Orientation Orient2d(const p2t::Point& pa, const p2t::Point& pb, const p2t::Point& pc)
    {
        double detleft = (pa.x - pc.x) * (pb.y - pc.y);
        double detright = (pa.y - pc.y) * (pb.x - pc.x);
        double val = detleft - detright;
        if (math::float_eq0(val))
        {
            return p2t::Orientation::COLINEAR;
        }
        else if (val > 0)
        {
            return p2t::Orientation::CCW;
        }
        return p2t::Orientation::CW;
    }

    bool InScanArea(const p2t::Point& pa, const p2t::Point& pb, const p2t::Point& pc, const p2t::Point& pd)
    {
        double oadb = (pa.x - pb.x)*(pd.y - pb.y) - (pd.x - pb.x)*(pa.y - pb.y);
        if (math::float_ge0(oadb))
        {
            return false;
        }

        double oadc = (pa.x - pc.x)*(pd.y - pc.y) - (pd.x - pc.x)*(pa.y - pc.y);
        if (math::float_le0(oadc))
        {
            return false;
        }

        return true;
    }

} // namespace

p2t::Sweep::Sweep() :
    nodes_()
{
}

p2t::Sweep::~Sweep()
{
    for(size_t i = 0; i < nodes_.size(); ++i)
    {
        delete nodes_[i];
    }
}

void p2t::Sweep::Triangulate(SweepContext& tcx)
{
    tcx.InitTriangulation();
    SweepPoints(tcx);
    FinalizationPolygon(tcx);
}

void p2t::Sweep::SweepPoints(SweepContext& tcx)
{
    for (size_t i = 1; i < tcx.point_count(); ++i)
    {
        const Point& point = *tcx.GetPoint(i);
        Node* node = &PointEvent(tcx, point);
        for (size_t j = 0; j < point.edge_list.size(); ++j)
        {
            EdgeEvent(tcx, point.edge_list[j], node);
        }
    }
}

void p2t::Sweep::FinalizationPolygon(SweepContext& tcx)
{
    // Get an Internal triangle to start with
    Triangle* t = tcx.front().head()->next->triangle;
    const Point* p = tcx.front().head()->next->point;
    while (!t->GetConstrainedEdgeCW(*p))
    {
        t = t->NeighborCCW(*p);
    }

    // Collect interior triangles constrained by edges
    tcx.MeshClean(*t);
}

p2t::Node& p2t::Sweep::PointEvent(SweepContext& tcx, const Point& point)
{
    Node& node = tcx.LocateNode(point);
    Node& new_node = NewFrontTriangle(tcx, point, node);

    // Only need to check +epsilon since point never have smaller
    // x value than node due to how we fetch nodes from the front
    if (math::float_le(point.x, node.point->x))
    {
        Fill(tcx, node);
    }

    //tcx.AddNode(new_node);

    FillAdvancingFront(tcx, new_node);
    return new_node;
}

void p2t::Sweep::EdgeEvent(SweepContext& tcx, Edge* edge, Node* node)
{
    tcx.edge_event.constrained_edge = edge;
    tcx.edge_event.right = (edge->p->x > edge->q->x);

    if (IsEdgeSideOfTriangle(*node->triangle, *edge->p, *edge->q))
    {
        return;
    }

    // For now we will do all needed filling
    // TODO: integrate with flip process might give some better performance
    //       but for now this avoid the issue with cases that needs both flips and fills
    FillEdgeEvent(tcx, edge, node);
    EdgeEvent(tcx, *edge->p, *edge->q, node->triangle, *edge->q);
}

void p2t::Sweep::EdgeEvent(SweepContext& tcx, const Point& ep, const Point& eq, Triangle* triangle, const Point& point)
{
    if (IsEdgeSideOfTriangle(*triangle, ep, eq))
    {
        return;
    }

    const Point* p1 = triangle->PointCCW(point);
    Orientation o1 = Orient2d(eq, *p1, ep);
    if (o1 == Orientation::COLINEAR)
    {
        if (triangle->Contains(&eq, p1))
        {
            triangle->MarkConstrainedEdge(&eq, p1);
            // We are modifying the constraint maybe it would be better to
            // not change the given constraint and just keep a variable for the new constraint
            tcx.edge_event.constrained_edge->q = p1;
            triangle = &triangle->NeighborAcross(point);
            EdgeEvent(tcx, ep, *p1, triangle, *p1);
        }
        else
        {
            assert(false); // EdgeEvent - colinear points not supported
        }
        return;
    }

    const Point* p2 = triangle->PointCW(point);
    Orientation o2 = Orient2d(eq, *p2, ep);
    if (o2 == Orientation::COLINEAR)
    {
        if (triangle->Contains(&eq, p2))
        {
            triangle->MarkConstrainedEdge(&eq, p2);
            // We are modifying the constraint maybe it would be better to not change the given constraint and just keep a variable for the new constraint
            tcx.edge_event.constrained_edge->q = p2;
            triangle = &triangle->NeighborAcross(point);
            EdgeEvent(tcx, ep, *p2, triangle, *p2);
        }
        else
        {
            assert(false); // EdgeEvent - colinear points not supported
        }
        return;
    }

    if (o1 == o2)
    {
        // Need to decide if we are rotating CW or CCW to get to a triangle that will cross edge
        if (o1 == Orientation::CW)
        {
            triangle = triangle->NeighborCCW(point);
        }
        else
        {
            triangle = triangle->NeighborCW(point);
        }
        EdgeEvent(tcx, ep, eq, triangle, point);
    }
    else
    {
        // This triangle crosses constraint so lets flippin start!
        FlipEdgeEvent(tcx, ep, eq, triangle, point);
    }
}

bool p2t::Sweep::IsEdgeSideOfTriangle(Triangle& triangle, const Point& ep, const Point& eq)
{
    int n = triangle.EdgeIndex(&ep, &eq);
    if (n != -1) // assert ...
    {
        triangle.MarkConstrainedEdge(n);
        Triangle* t = triangle.GetNeighbor(n);
        if (t)
        {
            t->MarkConstrainedEdge(&ep, &eq);
        }
        return true;
    }

    return false;
}

p2t::Node& p2t::Sweep::NewFrontTriangle(SweepContext& tcx, const Point& point, Node& node)
{
    Triangle* triangle = new Triangle(point, *node.point, *node.next->point);

    triangle->MarkNeighbor(*node.triangle);

    Node* new_node = new Node(point);
    nodes_.push_back(new_node);

    new_node->next = node.next;
    new_node->prev = &node;
    node.next->prev = new_node;
    node.next = new_node;

    if (!Legalize(tcx, *triangle))
    {
        tcx.MapTriangleToNodes(*triangle);
    }

    return *new_node;
}

void p2t::Sweep::Fill(SweepContext& tcx, Node& node)
{
    Triangle* triangle = new Triangle(*node.prev->point, *node.point, *node.next->point);

    // TODO: should copy the constrained_edge value from neighbor triangles
    //       for now constrained_edge values are copied during the legalize
    triangle->MarkNeighbor(*node.prev->triangle);
    triangle->MarkNeighbor(*node.triangle);

    // Update the advancing front
    node.prev->next = node.next;
    node.next->prev = node.prev;

    // If it was legalized the triangle has already been mapped
    if (!Legalize(tcx, *triangle))
    {
        tcx.MapTriangleToNodes(*triangle);
    }

}

void p2t::Sweep::FillAdvancingFront(SweepContext& tcx, Node& n)
{

    // Fill right holes
    Node* node = n.next;

    while (node->next)
    {
        // if HoleAngle exceeds 90 degrees then break.
        if (LargeHole_DontFill(node)) break;
        Fill(tcx, *node);
        node = node->next;
    }

    // Fill left holes
    node = n.prev;

    while (node->prev)
    {
        // if HoleAngle exceeds 90 degrees then break.
        if (LargeHole_DontFill(node)) break;
        Fill(tcx, *node);
        node = node->prev;
    }

    // Fill right basins
    if (n.next && n.next->next)
    {
        double angle = BasinAngle(n);
        if (angle < PiThreeFourth)
        {
            FillBasin(tcx, n);
        }
    }
}

// True if HoleAngle exceeds 90 degrees.
bool p2t::Sweep::LargeHole_DontFill(Node* node)
{

    Node* nextNode = node->next;
    Node* prevNode = node->prev;
    if (!AngleExceeds90Degrees(*node->point, *nextNode->point, *prevNode->point))
        return false;

    // Check additional points on front.
    Node* next2Node = nextNode->next;
    // "..Plus.." because only want angles on same side as point being added.
    if ((next2Node != nullptr) && !AngleExceedsPlus90DegreesOrIsNegative(*node->point, *next2Node->point, *prevNode->point))
        return false;

    Node* prev2Node = prevNode->prev;
    // "..Plus.." because only want angles on same side as point being added.
    if ((prev2Node != nullptr) && !AngleExceedsPlus90DegreesOrIsNegative(*node->point, *nextNode->point, *prev2Node->point))
        return false;

    return true;
}

bool p2t::Sweep::AngleExceeds90Degrees(const Point& origin, const Point& pa, const Point& pb)
{
    double angle = Angle(origin, pa, pb);
    bool exceeds90Degrees = ((angle > math::PiHalf<double>) || (angle < -math::PiHalf<double>));
    return exceeds90Degrees;
}

bool p2t::Sweep::AngleExceedsPlus90DegreesOrIsNegative(const Point& origin, const Point& pa, const Point& pb)
{
    double angle = Angle(origin, pa, pb);
    bool exceedsPlus90DegreesOrIsNegative = (angle > math::PiHalf<double>) || (angle < 0);
    return exceedsPlus90DegreesOrIsNegative;
}

double p2t::Sweep::Angle(const Point& origin, const Point& pa, const Point& pb)
{
  /* Complex plane
   * ab = cosA +i*sinA
   * ab = (ax + ay*i)(bx + by*i) = (ax*bx + ay*by) + i(ax*by-ay*bx)
   * atan2(y,x) computes the principal value of the argument function
   * applied to the complex number x+iy
   * Where x = ax*bx + ay*by
   *       y = ax*by - ay*bx
   */
    double px = origin.x;
    double py = origin.y;
    double ax = pa.x - px;
    double ay = pa.y - py;
    double bx = pb.x - px;
    double by = pb.y - py;
    double x = ax * by - ay * bx;
    double y = ax * bx + ay * by;
    double angle = atan2(x, y);
    return angle;
}

double p2t::Sweep::BasinAngle(Node& node)
{
    double ax = node.point->x - node.next->next->point->x;
    double ay = node.point->y - node.next->next->point->y;
    return atan2(ay, ax);
}

double p2t::Sweep::HoleAngle(Node& node)
{
  /* Complex plane
   * ab = cosA +i*sinA
   * ab = (ax + ay*i)(bx + by*i) = (ax*bx + ay*by) + i(ax*by-ay*bx)
   * atan2(y,x) computes the principal value of the argument function
   * applied to the complex number x+iy
   * Where x = ax*bx + ay*by
   *       y = ax*by - ay*bx
   */
    double ax = node.next->point->x - node.point->x;
    double ay = node.next->point->y - node.point->y;
    double bx = node.prev->point->x - node.point->x;
    double by = node.prev->point->y - node.point->y;
    return atan2(ax * by - ay * bx, ax * bx + ay * by);
}

bool p2t::Sweep::Legalize(SweepContext& tcx, Triangle& t)
{
    // To legalize a triangle we start by finding if any of the three edges violate the Delaunay condition
    for (int i = 0; i < 3; ++i)
    {
        if (t.m_bEdgeDelaunay[i])
            continue;

        Triangle* ot = t.GetNeighbor(i);
        if (ot)
        {
            const Point& point = *t.GetPoint(i);
            const Point* op = ot->OppositePoint(t, point);
            int oi = ot->Index(op);

            // If this is a Constrained Edge or a Delaunay Edge(only during recursive legalization)
            // then we should not try to legalize
            if (ot->m_bEdgeConstrained[oi] || ot->m_bEdgeDelaunay[oi])
            {
                t.m_bEdgeConstrained[i] = ot->m_bEdgeConstrained[oi];
                continue;
            }

            bool inside = Incircle(point, *t.PointCCW(point), *t.PointCW(point), *op);
            if (inside)
            {
                // Lets mark this shared edge as Delaunay
                t.m_bEdgeDelaunay[i] = true;
                ot->m_bEdgeDelaunay[oi] = true;

                // Lets rotate shared edge one vertex CW to legalize it
                RotateTrianglePair(t, point, *ot, *op);

                // We now got one valid Delaunay Edge shared by two triangles
                // This gives us 4 new edges to check for Delaunay

                // Make sure that triangle to node mapping is done only one time for a specific triangle
                if (!Legalize(tcx, t))
                {
                    tcx.MapTriangleToNodes(t);
                }

                if (!Legalize(tcx, *ot))
                {
                    tcx.MapTriangleToNodes(*ot);
                }

                // Reset the Delaunay edges, since they only are valid Delaunay edges until we add a new triangle or point.
                // XXX: need to think about this. Can these edges be tried after we return to previous recursive level?
                t.m_bEdgeDelaunay[i] = false;
                ot->m_bEdgeDelaunay[oi] = false;

                // If triangle have been legalized no need to check the other edges since the recursive legalization will handle those so we can end here.
                return true;
            }
        }
    }
    return false;
}

bool p2t::Sweep::Incircle(const Point& pa, const Point& pb, const Point& pc, const Point& pd)
{
    double adx = pa.x - pd.x;
    double ady = pa.y - pd.y;
    double bdx = pb.x - pd.x;
    double bdy = pb.y - pd.y;

    double adxbdy = adx * bdy;
    double bdxady = bdx * ady;
    double oabd = adxbdy - bdxady;

    if (oabd <= 0)
        return false;

    double cdx = pc.x - pd.x;
    double cdy = pc.y - pd.y;

    double cdxady = cdx * ady;
    double adxcdy = adx * cdy;
    double ocad = cdxady - adxcdy;

    if (ocad <= 0)
        return false;

    double bdxcdy = bdx * cdy;
    double cdxbdy = cdx * bdy;

    double alift = adx * adx + ady * ady;
    double blift = bdx * bdx + bdy * bdy;
    double clift = cdx * cdx + cdy * cdy;

    double det = alift * (bdxcdy - cdxbdy) + blift * ocad + clift * oabd;
    return det > 0;
}

void p2t::Sweep::RotateTrianglePair(Triangle& t, const Point& p, Triangle& ot, const Point& op)
{
    Triangle* n1, *n2, *n3, *n4;
    n1 = t.NeighborCCW(p);
    n2 = t.NeighborCW(p);
    n3 = ot.NeighborCCW(op);
    n4 = ot.NeighborCW(op);

    bool ce1, ce2, ce3, ce4;
    ce1 = t.GetConstrainedEdgeCCW(p);
    ce2 = t.GetConstrainedEdgeCW(p);
    ce3 = ot.GetConstrainedEdgeCCW(op);
    ce4 = ot.GetConstrainedEdgeCW(op);

    bool de1, de2, de3, de4;
    de1 = t.GetDelunayEdgeCCW(p);
    de2 = t.GetDelunayEdgeCW(p);
    de3 = ot.GetDelunayEdgeCCW(op);
    de4 = ot.GetDelunayEdgeCW(op);

    t.Legalize(p, op);
    ot.Legalize(op, p);

    // Remap delaunay_edge
    ot.SetDelunayEdgeCCW(p, de1);
    t.SetDelunayEdgeCW(p, de2);
    t.SetDelunayEdgeCCW(op, de3);
    ot.SetDelunayEdgeCW(op, de4);

    // Remap constrained_edge
    ot.SetConstrainedEdgeCCW(p, ce1);
    t.SetConstrainedEdgeCW(p, ce2);
    t.SetConstrainedEdgeCCW(op, ce3);
    ot.SetConstrainedEdgeCW(op, ce4);

    // Remap neighbors
    // XXX: might optimize the markNeighbor by keeping track of what side should be assigned to what neighbor after the rotation.
    // Now mark neighbor does lots of testing to find the right side.
    t.ClearNeighbors();
    ot.ClearNeighbors();
    if (n1) ot.MarkNeighbor(*n1);
    if (n2) t.MarkNeighbor(*n2);
    if (n3) t.MarkNeighbor(*n3);
    if (n4) ot.MarkNeighbor(*n4);
    t.MarkNeighbor(ot);
}

void p2t::Sweep::FillBasin(SweepContext& tcx, Node& node)
{
    if (Orient2d(*node.point, *node.next->point, *node.next->next->point) == Orientation::CCW)
    {
        tcx.basin.left_node = node.next->next;
    }
    else
    {
        tcx.basin.left_node = node.next;
    }

    // Find the bottom and right node
    tcx.basin.bottom_node = tcx.basin.left_node;
    while (tcx.basin.bottom_node->next
        && tcx.basin.bottom_node->point->y >= tcx.basin.bottom_node->next->point->y)
    {
        tcx.basin.bottom_node = tcx.basin.bottom_node->next;
    }
    if (tcx.basin.bottom_node == tcx.basin.left_node)
    {
        // No valid basin
        return;
    }

    tcx.basin.right_node = tcx.basin.bottom_node;
    while (tcx.basin.right_node->next
        && tcx.basin.right_node->point->y < tcx.basin.right_node->next->point->y)
    {
        tcx.basin.right_node = tcx.basin.right_node->next;
    }
    if (tcx.basin.right_node == tcx.basin.bottom_node)
    {
        // No valid basins
        return;
    }

    tcx.basin.width = tcx.basin.right_node->point->x - tcx.basin.left_node->point->x;
    tcx.basin.left_highest = tcx.basin.left_node->point->y > tcx.basin.right_node->point->y;

    FillBasinReq(tcx, tcx.basin.bottom_node);
}

void p2t::Sweep::FillBasinReq(SweepContext& tcx, Node* node)
{
    // if shallow stop filling
    if (IsShallow(tcx, *node))
    {
        return;
    }

    Fill(tcx, *node);

    if (node->prev == tcx.basin.left_node && node->next == tcx.basin.right_node)
    {
        return;
    }
    else if (node->prev == tcx.basin.left_node)
    {
        Orientation o = Orient2d(*node->point, *node->next->point, *node->next->next->point);
        if (o == Orientation::CW)
        {
            return;
        }
        node = node->next;
    }
    else if (node->next == tcx.basin.right_node)
    {
        Orientation o = Orient2d(*node->point, *node->prev->point, *node->prev->prev->point);
        if (o == Orientation::CCW)
        {
            return;
        }
        node = node->prev;
    }
    else
    {
        // Continue with the neighbor node with lowest Y value
        if (node->prev->point->y < node->next->point->y)
        {
            node = node->prev;
        }
        else
        {
            node = node->next;
        }
    }

    FillBasinReq(tcx, node);
}

bool p2t::Sweep::IsShallow(SweepContext& tcx, Node& node)
{
    double height;

    if (tcx.basin.left_highest)
    {
        height = tcx.basin.left_node->point->y - node.point->y;
    }
    else
    {
        height = tcx.basin.right_node->point->y - node.point->y;
    }

    // if shallow stop filling
    if (tcx.basin.width > height)
    {
        return true;
    }
    return false;
}

void p2t::Sweep::FillEdgeEvent(SweepContext& tcx, Edge* edge, Node* node)
{
    if (tcx.edge_event.right)
    {
        FillRightAboveEdgeEvent(tcx, edge, node);
    }
    else
    {
        FillLeftAboveEdgeEvent(tcx, edge, node);
    }
}

void p2t::Sweep::FillRightAboveEdgeEvent(SweepContext& tcx, Edge* edge, Node* node)
{
    while (node->next->point->x < edge->p->x)
    {
        // Check if next node is below the edge
        if (Orient2d(*edge->q, *node->next->point, *edge->p) == Orientation::CCW)
        {
            FillRightBelowEdgeEvent(tcx, edge, *node);
        }
        else
        {
            node = node->next;
        }
    }
}

void p2t::Sweep::FillRightBelowEdgeEvent(SweepContext& tcx, Edge* edge, Node& node)
{
    if (node.point->x < edge->p->x)
    {
        if (Orient2d(*node.point, *node.next->point, *node.next->next->point) == Orientation::CCW)
        {
            // Concave
            FillRightConcaveEdgeEvent(tcx, edge, node);
        }
        else
        {
            // Convex
            FillRightConvexEdgeEvent(tcx, edge, node);
            // Retry this one
            FillRightBelowEdgeEvent(tcx, edge, node);
        }
    }
}

void p2t::Sweep::FillRightConcaveEdgeEvent(SweepContext& tcx, Edge* edge, Node& node)
{
    Fill(tcx, *node.next);
    if (node.next->point != edge->p)
    {
        // Next above or below edge?
        if (Orient2d(*edge->q, *node.next->point, *edge->p) == Orientation::CCW)
        {
            // Below
            if (Orient2d(*node.point, *node.next->point, *node.next->next->point) == Orientation::CCW)
            {
                // Next is concave
                FillRightConcaveEdgeEvent(tcx, edge, node);
            }
            else
            {
                // Next is convex
            }
        }
    }
}

void p2t::Sweep::FillRightConvexEdgeEvent(SweepContext& tcx, Edge* edge, Node& node)
{
    // Next concave or convex?
    if (Orient2d(*node.next->point, *node.next->next->point, *node.next->next->next->point) == Orientation::CCW)
    {
        // Concave
        FillRightConcaveEdgeEvent(tcx, edge, *node.next);
    }
    else
    {
        // Convex
        // Next above or below edge?
        if (Orient2d(*edge->q, *node.next->next->point, *edge->p) == Orientation::CCW)
        {
            // Below
            FillRightConvexEdgeEvent(tcx, edge, *node.next);
        }
        else
        {
            // Above
        }
    }
}

void p2t::Sweep::FillLeftAboveEdgeEvent(SweepContext& tcx, Edge* edge, Node* node)
{
    while (node->prev->point->x > edge->p->x)
    {
        // Check if next node is below the edge
        if (Orient2d(*edge->q, *node->prev->point, *edge->p) == Orientation::CW)
        {
            FillLeftBelowEdgeEvent(tcx, edge, *node);
        }
        else
        {
            node = node->prev;
        }
    }
}

void p2t::Sweep::FillLeftBelowEdgeEvent(SweepContext& tcx, Edge* edge, Node& node)
{
    if (node.point->x > edge->p->x)
    {
        if (Orient2d(*node.point, *node.prev->point, *node.prev->prev->point) == Orientation::CW)
        {
            // Concave
            FillLeftConcaveEdgeEvent(tcx, edge, node);
        }
        else
        {
            // Convex
            FillLeftConvexEdgeEvent(tcx, edge, node);
            // Retry this one
            FillLeftBelowEdgeEvent(tcx, edge, node);
        }
    }
}

void p2t::Sweep::FillLeftConvexEdgeEvent(SweepContext& tcx, Edge* edge, Node& node)
{
    // Next concave or convex?
    if (Orient2d(*node.prev->point, *node.prev->prev->point, *node.prev->prev->prev->point) == Orientation::CW)
    {
        // Concave
        FillLeftConcaveEdgeEvent(tcx, edge, *node.prev);
    }
    else
    {
        // Convex
        // Next above or below edge?
        if (Orient2d(*edge->q, *node.prev->prev->point, *edge->p) == Orientation::CW)
        {
            // Below
            FillLeftConvexEdgeEvent(tcx, edge, *node.prev);
        }
        else
        {
            // Above
        }
    }
}

void p2t::Sweep::FillLeftConcaveEdgeEvent(SweepContext& tcx, Edge* edge, Node& node)
{
    Fill(tcx, *node.prev);
    if (node.prev->point != edge->p)
    {
        // Next above or below edge?
        if (Orient2d(*edge->q, *node.prev->point, *edge->p) == Orientation::CW)
        {
            // Below
            if (Orient2d(*node.point, *node.prev->point, *node.prev->prev->point) == Orientation::CW)
            {
                // Next is concave
                FillLeftConcaveEdgeEvent(tcx, edge, node);
            }
            else
            {
                // Next is convex
            }
        }
    }
}

void p2t::Sweep::FlipEdgeEvent(SweepContext& tcx, const Point& ep, const Point& eq, Triangle* t, const Point& p)
{
    Triangle& ot = t->NeighborAcross(p);
    const Point& op = *ot.OppositePoint(*t, p);

    if (&ot == nullptr)
    {
        // If we want to integrate the fillEdgeEvent do it here
        // With current implementation we should never get here
        assert(false); // [BUG:FIXME] FLIP failed due to missing triangle
    }

    if (InScanArea(p, *t->PointCCW(p), *t->PointCW(p), op))
    {
        // Lets rotate shared edge one vertex CW
        RotateTrianglePair(*t, p, ot, op);
        tcx.MapTriangleToNodes(*t);
        tcx.MapTriangleToNodes(ot);

        if (p == eq && op == ep)
        {
            if (eq == *tcx.edge_event.constrained_edge->q && ep == *tcx.edge_event.constrained_edge->p)
            {
                t->MarkConstrainedEdge(&ep, &eq);
                ot.MarkConstrainedEdge(&ep, &eq);
                Legalize(tcx, *t);
                Legalize(tcx, ot);
            }
            else
            {
                // XXX: I think one of the triangles should be legalized here?
            }
        }
        else
        {
            Orientation o = Orient2d(eq, op, ep);
            t = &NextFlipTriangle(tcx, o, *t, ot, p, op);
            FlipEdgeEvent(tcx, ep, eq, t, p);
        }
    }
    else
    {
        const Point& newP = NextFlipPoint(ep, eq, ot, op);
        FlipScanEdgeEvent(tcx, ep, eq, *t, ot, newP);
        EdgeEvent(tcx, ep, eq, t, p);
    }
}

p2t::Triangle& p2t::Sweep::NextFlipTriangle(SweepContext& tcx, Orientation o, Triangle& t, Triangle& ot, const Point& p, const Point& op)
{
    if (o == Orientation::CCW)
    {
        // ot is not crossing edge after flip
        int edge_index = ot.EdgeIndex(&p, &op);
        assert(edge_index >= 0);

        ot.m_bEdgeDelaunay[edge_index] = true;
        Legalize(tcx, ot);
        ot.ClearDelunayEdges();
        return t;
    }

    // t is not crossing edge after flip
    int edge_index = t.EdgeIndex(&p, &op);
    assert(edge_index >= 0);

    t.m_bEdgeDelaunay[edge_index] = true;
    Legalize(tcx, t);
    t.ClearDelunayEdges();
    return ot;
}

const p2t::Point& p2t::Sweep::NextFlipPoint(const Point& ep, const Point& eq, Triangle& ot, const Point& op)
{
    Orientation o2d = Orient2d(eq, op, ep);
    if (o2d == Orientation::CW)
    {
        // Right
        return *ot.PointCCW(op);
    }
    else if (o2d == Orientation::CCW)
    {
        // Left
        return *ot.PointCW(op);
    }

	assert(false); // [Unsupported] Opposing point on constrained edge
	return ep; // Arbitarary
}

void p2t::Sweep::FlipScanEdgeEvent(SweepContext& tcx, const Point& ep, const Point& eq, Triangle& flip_triangle, Triangle& t, const Point& p)
{
    Triangle& ot = t.NeighborAcross(p);
    const Point& op = *ot.OppositePoint(t, p);

    if (&t.NeighborAcross(p) == nullptr)
    {
        // If we want to integrate the fillEdgeEvent do it here
        // With current implementation we should never get here
        assert(false); // [BUG:FIXME] FLIP failed due to missing triangle
    }

    if (InScanArea(eq, *flip_triangle.PointCCW(eq), *flip_triangle.PointCW(eq), op))
    {
        // flip with new edge op->eq
        FlipEdgeEvent(tcx, eq, op, &ot, op);
        // TODO: Actually I just figured out that it should be possible to improve this by getting the next ot and op before the the above flip and continue the flipScanEdgeEvent here
        // set new ot and op here and loop back to inScanArea test
        // also need to set a new flip_triangle first
        // Turns out at first glance that this is somewhat complicated so it will have to wait.
    }
    else
    {
        const Point& newP = NextFlipPoint(ep, eq, ot, op);
        FlipScanEdgeEvent(tcx, ep, eq, flip_triangle, ot, newP);
    }
}
