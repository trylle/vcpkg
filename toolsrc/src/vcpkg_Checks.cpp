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
    void unreachable(const LineInfo& line_info)
    {
        System::println(System::color::error, "Error: Unreachable code was reached: %s", line_info.toString());
#ifndef NDEBUG
        std::abort();
#else
        exit(EXIT_FAILURE);
#endif
    }

    void exit_with_message(const char* errorMessage)
    {
        System::println(System::color::error, errorMessage);
        exit(EXIT_FAILURE);
    }

    void throw_with_message(const char* errorMessage)
    {
        throw std::runtime_error(errorMessage);
    }

    void check_throw(bool expression, const char* errorMessage)
    {
        if (!expression)
        {
            throw_with_message(errorMessage);
        }
    }

    void check_exit(bool expression)
    {
        if (!expression)
        {
            exit(EXIT_FAILURE);
        }
    }

    void check_exit(bool expression, const char* errorMessage)
    {
        if (!expression)
        {
            exit_with_message(errorMessage);
        }
    }
}
