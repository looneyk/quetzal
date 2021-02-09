#if !defined(QUETZAL_LOG_HPP)
#define QUETZAL_LOG_HPP
//------------------------------------------------------------------------------
// Log.hpp
//------------------------------------------------------------------------------

#include "system_path.hpp"
#include "LogLevel.hpp"
#include "DateTime.hpp"
#include <iostream>
#include <string>
#include <cassert>

namespace quetzal
{

    // Standard log path
#if defined(LOG_PATH)
    const std::string log_path(LOG_PATH); // path ...
#else
    const std::string log_path(system_path(FOLDERID_Documents) + "\\Quetzal\\log"); // path ...
#endif

    //--------------------------------------------------------------------------
    template<typename T>
    class Log
    {
    public:

        Log(LogLevel level, const std::string& name);
        Log(const Log&) = delete;
        ~Log();

        Log& operator=(const Log&) = delete;

        std::ostream& log(LogLevel level);
        std::ostream& fatal();
        std::ostream& error();
        std::ostream& warn();
        std::ostream& info();
        std::ostream& debug();
        std::ostream& trace();

    private:

        class NullStream : public std::ostream
        {
        private:

            class NullBuffer : public std::streambuf
            {
            public:

                int overflow(int c)
                {
                    return c;
                }
            };
            
            NullBuffer m_nb;

        public:

            NullStream() : std::ostream(&m_nb)
            {
            }
        };

        LogLevel m_level;
        T m_os;
        NullStream m_osNull;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    quetzal::Log<T>::Log(LogLevel level, const std::string& name) :
        m_os(name, std::ios_base::app),
        m_level(level),
        m_osNull()
    {
    }

    //--------------------------------------------------------------------------
    template<typename T>
    quetzal::Log<T>::~Log()
    {
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::log(LogLevel level)
    {
        if (level <= m_level)
        {
            assert(m_os);

            m_os << DateTime().to_string() << " " << to_string(level) << " ";

            return m_os;
        }

        return m_osNull;
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::fatal()
    {
        return log(LogLevel::Fatal);
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::error()
    {
        return log(LogLevel::Error);
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::warn()
    {
        return log(LogLevel::Warn);
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::info()
    {
        return log(LogLevel::Info);
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::debug()
    {
        return log(LogLevel::Debug);
    }

    //--------------------------------------------------------------------------
    template<typename T>
    std::ostream& quetzal::Log<T>::trace()
    {
        return log(LogLevel::Trace);
    }

} // namespace quetzal

#endif // QUETZAL_LOG_HPP
