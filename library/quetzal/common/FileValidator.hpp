#if !defined(QUETZAL_COMMON_FILEVALIDATOR_HPP)
#define QUETZAL_COMMON_FILEVALIDATOR_HPP
//------------------------------------------------------------------------------
// common
// FileValidator.hpp
//------------------------------------------------------------------------------

#include "Timestamp.hpp"
#include <array>
#include <filesystem>

namespace quetzal
{

    //--------------------------------------------------------------------------
    class FileValidator
    {
    public:

        FileValidator(const std::filesystem::path& lhs, const std::filesystem::path& rhs);
        FileValidator(const FileValidator&) = default;
        FileValidator(FileValidator&&) = default;
        ~FileValidator() = default;

        FileValidator& operator=(const FileValidator&) = default;
        FileValidator& operator=(FileValidator&&) = default;

        void report_results();

    private:

        void init_directory(const std::filesystem::path& dir);

        // Returns missing, different, total
        std::array<size_t, 3> directory_differences();

        bool file_equal(const std::filesystem::path& lhs, const std::filesystem::path& rhs);

        std::filesystem::path m_lhs;
        std::filesystem::path m_rhs;
        Timestamp m_t0;
    };

} // namespace quetzal

#endif // QUETZAL_COMMON_FILEVALIDATOR_HPP
