#ifndef SWEEP_CONTEXT_H
#define SWEEP_CONTEXT_H

#include <list>
#include <vector>
#include "advancing_front.hpp"

namespace p2t
{

    struct Point;
    class Triangle;
    struct Node;
    struct Edge;
    class AdvancingFront;

    class SweepContext
    {
    public:

        SweepContext();
        ~SweepContext();

        void set_head(Point* p1);

        Point* head();

        void set_tail(Point* p1);

        Point* tail();

        size_t point_count();

        Node& LocateNode(const Point& point);

        void CreateAdvancingFront();

        // Try to map a node to all sides of this triangle that don't have a neighbor
        void MapTriangleToNodes(Triangle& t);

        const Point* GetPoint(size_t n);

        void AddFace(std::vector<Point*> polyline);
        void AddHole(std::vector<Point*> polyline);
        void AddPoint(Point* point);

        AdvancingFront& front();

        void MeshClean(Triangle& triangle);

        const std::vector<Triangle>& GetTriangles();

        std::vector<Edge*> edge_list;

        struct Basin
        {

            Basin() :
                left_node(nullptr),
                bottom_node(nullptr),
                right_node(nullptr),
                width(0.0),
                left_highest(false)
            {
            }

            void Clear()
            {
                left_node = nullptr;
                bottom_node = nullptr;
                right_node = nullptr;
                width = 0.0;
                left_highest = false;
            }

            Node* left_node;
            Node* bottom_node;
            Node* right_node;
            double width;
            bool left_highest;
        };

        struct EdgeEvent
        {

            EdgeEvent() :
                constrained_edge(nullptr),
                right(false)
            {
            }

            Edge* constrained_edge;
            bool right;
        };

    private:

        void InitTriangulation();
        void InitEdges(std::vector<Point*> polyline);

        friend class Sweep;

        std::vector<Point*> points_;
        std::vector<Triangle> triangles_;

        AdvancingFront front_;
        Point* head_; // head point used with advancing front
        Point* tail_; // tail point used with advancing front

        Node* af_head_;
        Node* af_middle_;
        Node* af_tail_;

        Basin basin;
        EdgeEvent edge_event;
    };

}

#endif
