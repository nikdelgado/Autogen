#pragma once

#include "Converter.h"
{%- for header in type_info|variant.includes %}
#include {{header}}
{%- endfor %}

#include "common/api/abb_protobuf/types/{{type_name}}.pb.h"

{%- if type_name is member.has_include_override %}
{%- for file in type_name | member.include_override %}
#include "{{ file }}"
{%- endfor %}
{%- endif %}

using {{type_name}} = std::variant<{%- for choice in type_info|variant.choices %}{%- if loop.last %}{{choice.raw_type | member.ns_override}}{{choice.raw_type}}{%- else %}{{choice.raw_type | member.ns_override}}{{choice.raw_type}}, {%- endif %}{%- endfor %}>;

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
        using protobuf_ns = common::api::abb_protobuf::types::{{type_name}};
        using abb_ns = {{type_name | member.ns_override}}{{type_name}};
    };
} // namespace {{ns_tpl}}