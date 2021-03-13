#if !defined(CDT_ADVANCINGFRONT_HPP)
#define CDT_ADVANCINGFRONT_HPP

#include "shapes.hpp"

namespace p2t
{

    // Advancing front node
    struct Node
    {

        Node(const Point& p) :
            point(&p),
            triangle(nullptr),
            next(nullptr),
            prev(nullptr),
            value(p.x)
        {
        }

        Node(const Point& p, Triangle& t) :
            point(&p),
            triangle(&t),
            next(nullptr),
            prev(nullptr),
            value(p.x)
        {
        }

        const Point* point;
        Triangle* triangle;

        Node* next;
        Node* prev;

        double value;
    };

    class AdvancingFront
    {
    public:

        AdvancingFront();
        ~AdvancingFront() = default;

        void init(Node& head, Node& tail);

        Node* head();
        void set_head(Node* node);
        Node* tail();
        void set_tail(Node* node);
        Node* search();
        void set_search(Node* node);

        // Locate insertion point along advancing front
        Node* LocateNode(double x);

        Node* LocatePoint(const Point& point);

    private:

        Node* FindSearchNode(double x);

        Node* head_;
        Node* tail_;
        Node* search_node_;
    };

}

#endif // CDT_ADVANCINGFRONT_HPP
