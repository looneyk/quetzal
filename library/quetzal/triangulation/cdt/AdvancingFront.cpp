
#include "AdvancingFront.hpp"

p2t::AdvancingFront::AdvancingFront() :
    head_(nullptr),
    tail_(nullptr),
    search_node_(nullptr)
{
}

void p2t::AdvancingFront::init(Node& head, Node& tail)
{
    head_ = &head;
    tail_ = &tail;
    search_node_ = &head;
}

p2t::Node* p2t::AdvancingFront::head()
{
  return head_;
}
void p2t::AdvancingFront::set_head(Node* node)
{
  head_ = node;
}

p2t::Node* p2t::AdvancingFront::tail()
{
  return tail_;
}
void p2t::AdvancingFront::set_tail(Node* node)
{
  tail_ = node;
}

p2t::Node* p2t::AdvancingFront::search()
{
  return search_node_;
}

void p2t::AdvancingFront::set_search(Node* node)
{
  search_node_ = node;
}

p2t::Node* p2t::AdvancingFront::LocateNode(double x)
{
  Node* node = search_node_;

  if (x < node->value)
  {
    while ((node = node->prev) != nullptr)
    {
      if (x >= node->value)
      {
        search_node_ = node;
        return node;
      }
    }
  }
  else
  {
    while ((node = node->next) != nullptr)
    {
      if (x < node->value)
      {
        search_node_ = node->prev;
        return node->prev;
      }
    }
  }
  return nullptr;
}

p2t::Node* p2t::AdvancingFront::FindSearchNode(double x)
{
  x;
  // TODO: implement BST index
  return search_node_;
}

p2t::Node* p2t::AdvancingFront::LocatePoint(const Point& point)
{
  double px = point.x;
  Node* node = FindSearchNode(px);

  double nx = node->point->x;
  if (px == nx)
  {
    if (&point != node->point)
    {
      // We might have two nodes with same x value for a short time
      if (&point == node->prev->point)
      {
        node = node->prev;
      }
      else if (&point == node->next->point)
      {
        node = node->next;
      }
      else
      {
        assert(false);
      }
    }
  }
  else if (px < nx)
  {
    while ((node = node->prev) != nullptr)
    {
      if (&point == node->point)
      {
        break;
      }
    }
  }
  else
  {
    while ((node = node->next) != nullptr)
    {
      if (&point == node->point)
        break;
    }
  }
  if(node)
      search_node_ = node;
  return node;
}
