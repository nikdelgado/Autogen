#include <uci/type/DurationType.h>
#include <{{path_api}}/utils/Clock.h>

#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    class ConvertDuration
    {
    public:
        static bool to_uci(::uci::type::DurationTypeValue& dest, const {{ns_api}}::utils::Duration& src);
        static bool from_uci({{ns_api}}::utils::Duration& dest, const ::uci::type::DurationTypeValue& src);
    };

    template<>
    class Converter<{{ns_api}}::utils::Duration, ::uci::type::DurationTypeValue>
    {
    public:
        using type = ConvertDuration;
        using uci_type = ::uci::type::DurationTypeValue;
    };
} // namespace {{ns_package}}::conversions