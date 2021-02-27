
#include "sweep_context.hpp"
#include "advancing_front.hpp"
#include <algorithm>

namespace
{

    // Inital triangle factor, seed triangle will extend 30% of PointSet width to both left and right.
    const double kAlpha = 0.3;

    bool cmp(const p2t::Point* a, const p2t::Point* b)
    {
        if (a->y < b->y)
        {
           return true;
        }
        
        if (a->y == b->y)
        {
            if (a->x < b->x)
            {
                return true;
            }
        }

        return false;
    }

} // namespace

p2t::SweepContext::SweepContext() :
    points_(),
    triangles_(),
    front_(),
    head_(nullptr),
    tail_(nullptr),
    af_head_(nullptr),
    af_middle_(nullptr),
    af_tail_(nullptr),
    basin(),
    edge_event()
{
}

p2t::SweepContext::~SweepContext()
{
    delete head_;
    delete tail_;
    delete af_head_;
    delete af_middle_;
    delete af_tail_;

    for(unsigned int i = 0; i < edge_list.size(); ++i)
    {
        delete edge_list[i];
    }
}

void p2t::SweepContext::AddFace(std::vector<Point*> polyline)
{
    points_ = polyline;
    InitEdges(points_);
}

void p2t::SweepContext::AddHole(std::vector<Point*> polyline)
{
    InitEdges(polyline);
//    for(unsigned int i = 0; i < polyline.size(); ++i)
//    {
//        points_.push_back(pollyline[i]);
//    }
    points_.insert(end(points_), begin(polyline), end(polyline));
}

void p2t::SweepContext::AddPoint(Point* point)
{
    points_.push_back(point);
}

const std::vector<p2t::Triangle>& p2t::SweepContext::GetTriangles()
{
    return triangles_;
}

p2t::AdvancingFront& p2t::SweepContext::front()
{
    return front_;
}

size_t p2t::SweepContext::point_count()
{
    return points_.size();
}

void p2t::SweepContext::set_head(Point* p1)
{
    head_ = p1;
}

p2t::Point* p2t::SweepContext::head()
{
    return head_;
}

void p2t::SweepContext::set_tail(Point* p1)
{
    tail_ = p1;
}

p2t::Point* p2t::SweepContext::tail()
{
    return tail_;
}

void p2t::SweepContext::InitTriangulation()
{
    double xmin(points_[0]->x);
    double xmax(points_[0]->x);
    double ymin(points_[0]->y);
    double ymax(points_[0]->y);

    // Calculate bounds.
    for (unsigned int i = 0; i < points_.size(); ++i)
    {
        Point& p = *points_[i];
        if (p.x > xmax)
            xmax = p.x;
        if (p.x < xmin)
            xmin = p.x;
        if (p.y > ymax)
            ymax = p.y;
        if (p.y < ymin)
            ymin = p.y;
    }

    double dx = kAlpha * (xmax - xmin);
    double dy = kAlpha * (ymax - ymin);
    head_ = new Point(static_cast<size_t>(-1), xmax + dx, ymin - dy);
    tail_ = new Point(static_cast<size_t>(-1), xmin - dx, ymin - dy);

    // Sort points along y-axis
    std::sort(points_.begin(), points_.end(), cmp);

    CreateAdvancingFront();
}

void p2t::SweepContext::InitEdges(std::vector<Point*> polyline)
{
    size_t num_points = polyline.size();
    for (size_t i = 0; i < num_points; ++i)
    {
        size_t j = i < num_points - 1 ? i + 1 : 0;
        edge_list.push_back(new Edge(*polyline[i], *polyline[j]));
    }
}

const p2t::Point* p2t::SweepContext::GetPoint(size_t n)
{
    return points_[n];
}

p2t::Node& p2t::SweepContext::LocateNode(const Point& point)
{
  // TODO implement search tree
    return *front_.LocateNode(point.x);
}

void p2t::SweepContext::CreateAdvancingFront()
{
  // Initial triangle
    Triangle* triangle = new Triangle(*points_[0], *tail_, *head_);

    af_head_ = new Node(*triangle->GetPoint(1), *triangle);
    af_middle_ = new Node(*triangle->GetPoint(0), *triangle);
    af_tail_ = new Node(*triangle->GetPoint(2));
    front_.init(*af_head_, *af_tail_);

    // TODO: More intuitive if head is middles next and not previous?
    //       so swap head and tail
    af_head_->next = af_middle_;
    af_middle_->next = af_tail_;
    af_middle_->prev = af_head_;
    af_tail_->prev = af_middle_;
}

void p2t::SweepContext::MapTriangleToNodes(Triangle& t)
{
    for (int i = 0; i < 3; ++i)
    {
        if (!t.GetNeighbor(i))
        {
            Node* n = front_.LocatePoint(*t.PointCW(*t.GetPoint(i)));
            if (n)
                n->triangle = &t;
        }
    }
}

void p2t::SweepContext::MeshClean(Triangle& triangle)
{
    std::vector<Triangle*> triangles;
    triangles.push_back(&triangle);

    while(!triangles.empty())
    {
        Triangle *t = triangles.back();
        triangles.pop_back();

        if (t != nullptr && !t->IsInterior())
        {
            t->set_interior(true);
            triangles_.push_back(*t);
            for (int i = 0; i < 3; ++i)
            {
                if (!t->m_bEdgeConstrained[i])
                {
                    triangles.push_back(t->GetNeighbor(i));
                }
            }
        }
    }
}
