#if !defined(QUETZAL_TIMER_HPP)
#define QUETZAL_TIMER_HPP
//------------------------------------------------------------------------------
// Timer.hpp
//------------------------------------------------------------------------------

#include "Timestamp.hpp"

namespace quetzal
{

    //--------------------------------------------------------------------------
    class Timer
    {
    public:

        Timer();

        double total() const; // Elapsed time excluding stopped time, seconds
        double delta() const; // Tick time, seconds

        void reset();
        void start();
        void stop();
        void tick();

    private:

        Timestamp m_tsBase;
        Timestamp m_ts;
        Timestamp m_tsPrev;
        Timestamp m_tsStop;
        double m_dtPaused;

        double m_dt;
        bool m_bStopped;
    };

} // namespace quetzal

#endif // QUETZAL_TIMER_HPP
