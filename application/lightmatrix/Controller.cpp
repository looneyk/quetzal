//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Controller.cpp
//------------------------------------------------------------------------------

#include "Controller.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::lightmatrix::Controller::Controller(Frame& frame) :
    m_frame(frame),
    m_shot0(),
    m_shot1(),
    m_pShotFront(&m_shot0),
    m_pShotBack(&m_shot1),
    m_bTransition(),
    m_tTransition(),
    m_tDissolve(),
    m_bFirst(true)
{
}

//------------------------------------------------------------------------------
const quetzal::lightmatrix::Frame& quetzal::lightmatrix::Controller::frame() const
{
    return m_frame;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::load_pattern(const string& path, float tDissolve)
{
    m_pShotBack->load_pattern(path);

    if (m_bFirst)
    {
        m_pShotFront->load_pattern(path); // ...
        m_bFirst = false;
        return;
    }

    m_tDissolve = tDissolve;
    m_tTransition = true;
    m_bTransition = true;

    swap(m_pShotBack, m_pShotFront);

    // reset frame size ...

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::set_radial(bool bRadial)
{
    m_pShotFront->set_radial(bRadial);

    m_pShotBack->set_radial(bRadial); // layout and pixel positions should be handled here in controller ...

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::toggle_shift_color()
{
    m_pShotFront->toggle_shift_color();
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::toggle_shift_brightness()
{
    m_pShotFront->toggle_shift_brightness();
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::cycle_smoothing()
{
    m_pShotFront->cycle_smoothing();
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::set_direction(float dx, float dy)
{
    m_pShotFront->set_direction(dx, dy);
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::increment_velocity(float vx, float vy)
{
    m_pShotFront->increment_velocity(vx, vy);
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Controller::update(float dt)
{
    if (m_bTransition)
    {
        m_tTransition += dt;
        if (m_tTransition >= m_tDissolve)
        {
            m_bTransition = false;
        }
    }

    m_pShotFront->update(dt);

    if (m_bTransition)
    {
        m_pShotBack->update(dt);
        float t = m_tTransition / m_tDissolve;
        blend(m_pShotBack->frame(), 1.0f - t, m_pShotFront->frame(), t, m_frame);
    }
    else
    {
        m_frame = m_pShotFront->frame();
    }

    return;
}
