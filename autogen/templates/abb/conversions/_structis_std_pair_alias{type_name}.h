#pragma once
{%- if type_name is message.name_root %}
#include <functional>
#include <memory>
#include <string>
{%- endif %}
#include "Converter.h"
#include "common/api/abb_protobuf/types/{{type_name}}.pb.h"


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
        static bool to_protobuf(common::api::abb_protobuf::types::{{type_name}}& dest, const {{type_name | member.ns_override}}{{type_name}}& src);
        static bool from_protobuf({{type_name | member.ns_override}}{{type_name}}& dest, const common::api::abb_protobuf::types::{{type_name}}& src);
    };

    template<>
    class Converter<{{type_name | member.ns_override}}{{type_name}}, common::api::abb_protobuf::types::{{type_name}}>
    {
    public:
        using converter_factory = Convert{{type_name}};
        using protobuf_namespace = common::api::abb_protobuf::types::{{type_name}};
        using abb_namespace = {{type_name | member.ns_override}}{{type_name}};
    };
} // namespace {{ns_tpl}}