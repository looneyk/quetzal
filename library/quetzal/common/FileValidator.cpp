//------------------------------------------------------------------------------
// common
// FileValidator.cpp
//------------------------------------------------------------------------------

#include "FileValidator.hpp"
#include "error_util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

using namespace std;

//------------------------------------------------------------------------------
quetzal::FileValidator::FileValidator(const std::filesystem::path& lhs, const std::filesystem::path& rhs) :
    m_lhs(lhs),
    m_rhs(rhs),
    m_t0()
{
    init_directory(m_lhs);
    init_directory(m_rhs);
}

//------------------------------------------------------------------------------
void quetzal::FileValidator::report_results()
{
    double t = Timestamp::since(m_t0);

    ostringstream oss;
    auto [nMissing, nDifferent, nTotal] = directory_differences();
    if (nMissing == 0 && nDifferent == 0)
    {
        oss << "OK: " << nTotal << " files equal (" << t << " seconds)" << endl;
    }
    else
    {
        oss << "Error: " << nMissing << " missing, " << nDifferent << " different, " << nTotal << " files (" << t << " seconds)" << endl;
    }

    write_debug_string(oss.str());
    cout << oss.str();
    this_thread::sleep_for(1s);
    return;
}

//------------------------------------------------------------------------------
void quetzal::FileValidator::init_directory(const std::filesystem::path& dir)
{
    if (!exists(dir))
    {
        create_directory(dir);
    }

    return;
}

//------------------------------------------------------------------------------
std::array<size_t, 3> quetzal::FileValidator::directory_differences()
{
    size_t nMissing = 0;
    size_t nDifferent = 0;
    size_t nTotal = 0;

    for (auto& p : filesystem::directory_iterator(m_lhs))
    {
        filesystem::path filepath = m_rhs / p.path().filename();
        if (!exists(filepath))
        {
            ++nMissing;
        }
        else
        {
            bool b = file_equal(p, filepath);
            if (!b)
            {
                ++nDifferent;
            }
        }

        ++nTotal;
    }

    return {nMissing, nDifferent, nTotal};
}

//------------------------------------------------------------------------------
bool quetzal::FileValidator::file_equal(const std::filesystem::path& lhs, const std::filesystem::path& rhs)
{
    size_t sizeRemaining = static_cast<size_t>(file_size(lhs));
    if (!exists(lhs) || !exists(rhs) || sizeRemaining != file_size(rhs))
    {
        return false;
    }

    ifstream ifs_lhs(lhs, ios_base::binary);
    ifstream ifs_rhs(rhs, ios_base::binary);

    const size_t sizeBuffer = 4096;
    char lhsBuffer[sizeBuffer];
    char rhsBuffer[sizeBuffer];

    while (sizeRemaining > 0)
    {
        size_t sizeRead = min(sizeBuffer, sizeRemaining);

        ifs_lhs.read(lhsBuffer, sizeRead);
        ifs_rhs.read(rhsBuffer, sizeRead);

        if (memcmp(lhsBuffer, rhsBuffer, sizeRead) != 0)
        {
            return false;
        }

        sizeRemaining -= sizeRead;
    }

    return true;
}
