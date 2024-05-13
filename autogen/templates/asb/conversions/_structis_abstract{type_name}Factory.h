#pragma once

#include <{{path_api}}/types/{{type_name}}.h>

#include "Converter.h"

namespace uci::type
{
class {{type_name}};    
}

namespace {{ns_tpl}}
{
    class Convert{{type_name}}Factory
    {
    public:
        static bool to_uci(::uci::type::{{type_name}}& dest, const std::shared_ptr<{{ns_api}}::types::{{type_name}}>& src);
        static bool from_uci(std::shared_ptr<{{ns_api}}::types::{{type_name}}>& dest, const ::uci::type::{{type_name}}& src);
        static uint32_t uci_accessor_for(const std::shared_ptr<{{ns_api}}::types::{{type_name}}>& obj);
    };

    template<>
    class Converter<std::shared_ptr<{{ns_api}}::types::{{type_name}}>, ::uci::type::{{type_name}}>
    {
    public:
        using type = Convert{{type_name}}Factory;
        using uci_type = ::uci::type::{{type_name}};
    };
} // namespace {{ns_package}}