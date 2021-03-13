#ifndef P2T_CDT_H
#define P2T_CDT_H

#include "SweepContext.hpp"

namespace p2t
{

    class CDT
    {
    public:

        CDT(const std::vector<Point>& polygon);
        ~CDT();

        void AddHole(const std::vector<Point>& polygon);

        // Add a steiner point
        void AddPoint(Point* point);

        // Triangulate - do this AFTER you've added the points, holes, and Steiner points
        void Triangulate();

        // Get resulting triangles
        const std::vector<Triangle>& GetTriangles();

    private:

        SweepContext sweep_context_;
        std::vector<p2t::Point*> m_face;
        std::vector<std::vector<p2t::Point*>> m_holes;
    };

}

#endif
