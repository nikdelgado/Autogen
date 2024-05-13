#pragma once

#include <{{path_api}}/types/{{type_name}}.h>

#include "Converter.h"

namespace uci::type {
class {{type_name}};
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
} // namespace {{ns_tpl}}