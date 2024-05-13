#pragma once

{%- if type_name is message.name_root %}
#include <functional>
#include <memory>
#include <string>
{%- endif %}

#include <{{path_api}}/types/{{type_name}}.h>

#include "Converter.h"

namespace uci::base
{
class AbstractServiceBusConnection;
class Listener;
class Reader;
class Writer;
}

namespace uci::type
{
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

    {%- if type_name is message.name_root%}
    class UciMethods{{type_name}}
    {
    public:
        static std::shared_ptr<::uci::type::{{type_name}}> create_msg(::uci::base::AbstractServiceBusConnection* asb);
        static ::uci::base::Reader* create_reader(const std::string& topic, ::uci::base::AbstractServiceBusConnection* asb);
        static ::uci::base::Writer* create_writer(const std::string& topic, ::uci::base::AbstractServiceBusConnection* asb);
        static ::uci::base::Listener* create_listener(std::function<void({{ns_api}}::types::{{type_name}}&)>&& callback);
        static void add_listener(::uci::base::Reader* reader, ::uci::base::Listener* listener);
        static void destroy_listener(::uci::base::Listener* listener);
        static void write_msg(::uci::base::Writer* writer, ::uci::type::{{type_name}}& msg);
        static void shutdown_reader(::uci::base::Reader* reader, ::uci::base::Listener* listener);
        static void shutdown_writer(::uci::base::Writer* writer);
    };
    {%- endif %}

    template<>
    class Converter<{{ns_api}}::types::{{type_name}}, ::uci::type::{{type_name}}>
    {
    public:
        using type = Convert{{type_name}};
        {%- if type_name is message.name_root  %}
        using uci_methods = UciMethods{{type_name}};
        {%- endif %}
        using uci_type = ::uci::type::{{type_name}};
    };
} // namespace {{ns_tpl}}