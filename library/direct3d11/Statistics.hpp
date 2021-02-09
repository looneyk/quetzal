#if !defined(QUETZAL_DIRECT3D11_STATISTICS_HPP)
#define QUETZAL_DIRECT3D11_STATISTICS_HPP
//------------------------------------------------------------------------------
// direct3d11
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Statistics.hpp
// Keep track of and display the frame rate, time per frame, and vertex and triangle counts.
//------------------------------------------------------------------------------

#include <string>

namespace quetzal::direct3d11
{

    //----------------------------------------------------------------------
    class Statistics
    {
    public:

        Statistics();
        Statistics(const Statistics&) = delete;
        ~Statistics() = default;

        Statistics& operator=(const Statistics&) = delete;

        std::string text() const;

        void clear();
        void update(float dt);

        void add_vertices(size_t n);
        void add_triangles(size_t n);
        void add_counts(size_t nVertices, size_t nTriangles);

        void set_vertices(size_t n);
        void set_triangles(size_t n);
        void set_counts(size_t nVertices, size_t nTriangles);

    private:

        size_t m_nFrames;
        float m_dt;

        float m_fps;
        size_t m_nTriangles;
        size_t m_nVertices;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_STATISTICS_HPP
