#pragma once

#include "Converter.h"
#include "{{path_package}}/{{type_name}}.pb.h"

{%- if type_name is member.has_include_override %}
{%- for file in type_name | member.include_override %}
#include "{{ file }}"
{%- endfor %}
{%- endif %}

namespace {{ns_tpl}}
{
    class Convert{{type_name}}
    {
    public:
        static bool to_protobuf({{path_package}}::{{type_name}}& dest, const {{type_name | member.ns_override}}{{type_name}}& src);
        static bool from_protobuf({{type_name | member.ns_override}}{{type_name}}& dest, const {{path_package}}::{{type_name}}& src);
    };

    template<>
    class Converter<{{type_name | member.ns_override}}{{type_name}}, {{path_package}}::{{type_name}}>
    {
    public:
        using converter_factory = Convert{{type_name}};
        using protobuf_ns = {{path_package}}::{{type_name}};
        using abb_ns = {{type_name | member.ns_override}}{{type_name}};
    };
} // namespace {{ns_tpl}}