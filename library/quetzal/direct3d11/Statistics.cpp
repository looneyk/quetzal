//------------------------------------------------------------------------------
// direct3d11
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Statistics.cpp
//------------------------------------------------------------------------------

#include "Statistics.hpp"
#include <sstream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::direct3d11::Statistics::Statistics() :
    m_nFrames(0),
    m_dt(0.0f),
    m_fps(0.0f),
    m_nTriangles(0),
    m_nVertices(0)
{
}

//------------------------------------------------------------------------------
string quetzal::direct3d11::Statistics::text() const
{
    ostringstream oss;
    oss << m_fps << " f/s\n";
    oss << (1000.f / m_fps) << " ms/f\n";
    oss << m_nTriangles << " triangles\n";
    oss << m_nVertices << " vertices";

    return oss.str();
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::clear()
{
    // ...

    m_nVertices = 0;
    m_nTriangles = 0;

    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::update(float dt)
{
    ++m_nFrames;

    // Accumulate how much time has passed.
    m_dt += dt;

    // Compute frame statistics once per second.
    // Note that the time between frames can vary so these stats are averages over a second.
    if (m_dt >= 1.0f)
    {
        // m_fps = m_nFrames / m_dt, but m_dt is approximately equal to 1.0
        m_fps = static_cast<float>(m_nFrames);

        // Reset time counter and frame count to prepare for computing the average stats over the next second
        m_nFrames = 0;
        m_dt = 0.0f; // -= floor(m_dt)?
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::add_vertices(size_t n)
{
    m_nVertices += n;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::add_triangles(size_t n)
{
    m_nTriangles += n;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::add_counts(size_t nVertices, size_t nTriangles)
{
    m_nVertices += nVertices;
    m_nTriangles += nTriangles;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::set_vertices(size_t n)
{
    m_nVertices = n;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::set_triangles(size_t n)
{
    m_nTriangles = n;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Statistics::set_counts(size_t nVertices, size_t nTriangles)
{
    m_nVertices = nVertices;
    m_nTriangles = nTriangles;
    return;
}
