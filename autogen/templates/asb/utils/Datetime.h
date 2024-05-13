#pragma once

#include <uci/type/DateTimeType.h>
#include <{{path_api}}/utils/Clock.h>

#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    class ConvertDateTime
    {
    public:
        static bool to_uci(::uci::type::DateTimeTypeValue& dest, const {{ns_api}}::utils::TimePoint& src);
        static bool from_uci({{ns_api}}::utils::TimePoint& dest, const ::uci::type::DateTimeTypeValue& src);
    };

    template<>
    class Converter<{{ns_api}}::utils::TimePoint, ::uci::type::DateTimeTypeValue>
    {
    public:
        using type = ConvertDateTime;
        using uci_type = ::uci::type::DateTimeTypeValue;
    };
} // namespace {{ns_tpl}}