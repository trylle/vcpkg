#include "pch.h"
#include "vcpkg_Checks.h"
#include "vcpkg_System.h"

namespace vcpkg
{
    std::string LineInfo::toString() const
    {
        return Strings::format("%s(%d)", this->file_name, this->line_number);
    }
}

namespace vcpkg::Checks
{
    static void print_line_info_if_debug(const LineInfo& line_info)
    {
#if !defined(NDEBUG)
        System::println(System::color::error, line_info.toString());
#endif
    }

    void unreachable(const LineInfo& line_info)
    {
        System::println(System::color::error, "Error: Unreachable code was reached.");
        System::println(System::color::error, line_info.toString()); // Always print line_info here
#if !defined(NDEBUG)
        std::abort();
#else
        exit(EXIT_FAILURE);
#endif
    }

    void exit_with_message(const LineInfo& line_info, const char* errorMessage)
    {
        System::println(System::color::error, errorMessage);
        print_line_info_if_debug(line_info);
        exit(EXIT_FAILURE);
    }

    void throw_with_message(const LineInfo& line_info, const char* errorMessage)
    {
        print_line_info_if_debug(line_info);
        throw std::runtime_error(errorMessage);
    }

    void check_throw(const LineInfo& line_info, bool expression, const char* errorMessage)
    {
        if (!expression)
        {
            throw_with_message(line_info, errorMessage);
        }
    }

    void check_exit(const LineInfo& line_info, bool expression)
    {
        if (!expression)
        {
            print_line_info_if_debug(line_info);
            exit(EXIT_FAILURE);
        }
    }

    void check_exit(const LineInfo& line_info, bool expression, const char* errorMessage)
    {
        if (!expression)
        {
            exit_with_message(line_info, errorMessage);
        }
    }
}
