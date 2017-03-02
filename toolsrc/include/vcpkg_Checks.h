#pragma once

#include "vcpkg_Strings.h"

namespace vcpkg
{
    struct LineInfo
    {
        int line_number;
        const char* file_name;

        constexpr LineInfo() : line_number(0), file_name(nullptr) {}
        constexpr LineInfo(const int line_number, const char* file_name) : line_number(line_number), file_name(file_name) {}

        std::string toString() const;
    };
}

#define __LINE_INFO__ vcpkg::LineInfo(__LINE__, __FILE__)

namespace vcpkg::Checks
{
    __declspec(noreturn) void unreachable(const LineInfo& line_info);

    // Part of the reason these exist is to not include extra headers in this one to avoid circular #includes. 
    _declspec(noreturn) void exit_with_message(const LineInfo& line_info, const char* errorMessage);

    template <class...Args>
    _declspec(noreturn) void exit_with_message(const LineInfo& line_info, const char* errorMessageTemplate, const Args&... errorMessageArgs)
    {
        exit_with_message(line_info, Strings::format(errorMessageTemplate, errorMessageArgs...).c_str());
    }

    _declspec(noreturn) void throw_with_message(const LineInfo& line_info, const char* errorMessage);

    template <class...Args>
    _declspec(noreturn) void throw_with_message(const LineInfo& line_info, const char* errorMessageTemplate, const Args&... errorMessageArgs)
    {
        throw_with_message(line_info, Strings::format(errorMessageTemplate, errorMessageArgs...).c_str());
    }

    void check_throw(const LineInfo& line_info, bool expression, const char* errorMessage);

    template <class...Args>
    void check_throw(const LineInfo& line_info, bool expression, const char* errorMessageTemplate, const Args&... errorMessageArgs)
    {
        if (!expression)
        {
            // Only create the string if the expression is false
            throw_with_message(line_info, Strings::format(errorMessageTemplate, errorMessageArgs...).c_str());
        }
    }

    void check_exit(const LineInfo& line_info, bool expression);

    void check_exit(const LineInfo& line_info, bool expression, const char* errorMessage);

    template <class...Args>
    void check_exit(const LineInfo& line_info, bool expression, const char* errorMessageTemplate, const Args&... errorMessageArgs)
    {
        if (!expression)
        {
            // Only create the string if the expression is false
            exit_with_message(line_info, Strings::format(errorMessageTemplate, errorMessageArgs...).c_str());
        }
    }
}
