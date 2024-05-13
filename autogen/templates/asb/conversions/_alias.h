#pragma once

#include <uci/type/{{type_name}}.h>
#include <{{path_api}}/types/{{type_name}}.h>

#include "Converter.h"


namespace uci::base {
class AbstractServiceBusConnection;
}

namespace {{ns_tpl}}
{
    class Convert{{type_name}}
    {
    public:
        static bool to_uci(::uci::type::{{type_name}}& dest, const {{ns_api}}::types::{{type_name}}& src);
        static bool from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}& src);
    };

    template<>
    class Converter<{{ns_api}}::types::{{type_name}}, ::uci::type::{{type_name}}>
    {
    public:
        using type = Convert{{type_name}};
		using uci_type = ::uci::type::{{type_name}};
    };

	{%- if type_info is not alias.is_string %}
    class Convert{{type_name}}Value
    {
    public:
        static bool to_uci(::uci::type::{{type_name}}Value& dest, const {{ns_api}}::types::{{type_name}}& src);
        static bool from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}Value& src);
    };

    template<>
    class Converter<{{ns_api}}::types::{{type_name}}, ::uci::type::{{type_name}}Value>
    {
    public:
        using type = Convert{{type_name}}Value;
        using uci_type = ::uci::type::{{type_name}}Value;
    };
	{%- endif %}
} // namespace {{ns_tpl}}