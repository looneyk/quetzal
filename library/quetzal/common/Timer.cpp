//------------------------------------------------------------------------------
// Timer.cpp
//------------------------------------------------------------------------------

#include "Timer.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::Timer::Timer() :
    m_tsBase(),
    m_ts(m_tsBase),
    m_tsPrev(m_tsBase),
    m_tsStop(),
    m_dtPaused(0.),
    m_dt(0.),
    m_bStopped(true)
{
}

//------------------------------------------------------------------------------
double quetzal::Timer::total() const
{
    if (m_bStopped)
    {
        return (m_tsStop - m_tsBase) - m_dtPaused;
    }

    return (m_ts - m_tsBase) - m_dtPaused;
}

//------------------------------------------------------------------------------
double quetzal::Timer::delta() const
{
    return m_dt;
}

//------------------------------------------------------------------------------
void quetzal::Timer::reset()
{
    m_tsBase.update();
    m_ts = m_tsBase;
    m_tsPrev = m_tsBase;
    m_bStopped = false;
    return;
}

//------------------------------------------------------------------------------
void quetzal::Timer::start()
{
    assert(m_bStopped);

    // Accumulate the time elapsed between stop and start pairs.
    //
    //                     |<-------d------->|
    // ----*---------------*-----------------*------------> time
    //  m_tsBase       m_tsStop        startTime

    if (m_bStopped)
    {
        m_ts.update();
        m_tsPrev = m_ts;
        m_dtPaused += m_ts - m_tsStop;
        m_bStopped = false;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::Timer::stop()
{
    if (!m_bStopped)
    {
        m_tsStop.update();
        m_bStopped = true;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::Timer::tick()
{
    if (m_bStopped)
    {
        m_dt = 0.;
        return;
    }

    m_ts.update();
    m_dt = m_ts - m_tsPrev;
    m_tsPrev = m_ts;
    return;
}
