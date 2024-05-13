#include "{{type_name}}.h"
#include "{{path_package}}/utils/PopulateMutex.h"

{%- if type_info.attrs|select('member.is_native')|list %}
#include "{{path_package}}/utils/Native.h"
{%- endif %}

{%- if type_info.attrs|select('member.is_list')|list %}
#include "{{path_package}}/utils/Vector.h"
{%- endif %}

{%- for attr in type_info.attrs|select('member.is_custom')|list %}
#include {{attr|util.include(path_package)}}
{%- endfor %}

{%- for ex in type_info.extensions %}
#include "{{ex|ext.type}}.h"
{%- endfor %}

{#- both the type h/cpp includes are only needed in the context where conversion occurs#}
{%- for include in (
    type_info.attrs|map('cpp.includes', false)|sum(start=[])
    + type_info.attrs|map('h.includes')|sum(start=[])
)|sort|unique %}
#include {{include|incl.quote_fix(path_package)}}
{%- endfor %}

namespace {{ns_tpl}}
{
    {%- if type_info.attrs or type_info.extensions %}
    bool Convert{{type_name}}::from_protobuf({{type_name | member.ns_override}}{{type_name}}& dest, const common::api::abb_protobuf::types::{{type_name}}& src)
    {
        bool success = true;
        const auto lock = std::scoped_lock{utils::populateMutex};

        {%- for attr in type_info.attrs %}
        {%- set protobuf_getter = attr.name.lower() %}
        {%- set protobuf_setter = "set_" + attr.name.lower() %}
        {%- set abb_getter = "get" + attr.name | member.set_first_uppercase %}
        {%- set abb_setter = "set" + attr.name | member.set_first_uppercase %}
        {
            {%- if attr is member.is_simple and attr is not member.is_list %}
            {%- if loop.index == 1 %}
            dest.first = src.{{attr.name.lower()}}().value();
            {%- elif loop.index == 2 %}
            dest.second = src.{{attr.name.lower()}}().value();
            {%- else %}
            {%- endif %}
            {%- else %}
            using ConversionType = Converter<
                {%- if loop.index == 1 %}
                std::decay_t<decltype(dest.first)>, 
                std::decay_t<decltype(src.{{protobuf_getter}}())>
                {%- elif loop.index == 2%}
                std::decay_t<decltype(dest.second)>, 
                std::decay_t<decltype(src.{{protobuf_getter}}())>
                {%- else %}
                {%- endif %}
            >::converter_factory;

            {%- if loop.index == 1 %}
            auto temp_dest = dest.first;
            auto temp_src = src.{{protobuf_getter}}();
            {%- elif loop.index == 2 %}
            auto temp_dest = dest.second;
            auto temp_src = src.{{protobuf_getter}}();
            {%- else %}
            {%- endif %}

            success &= ConversionType::from_protobuf(temp_dest, temp_src);

            {%- if loop.index == 1 %}
            dest.first = temp_dest;
            {%- elif loop.index == 2 %}
            dest.second = temp_dest;
            {%- else %}
            {%- endif %}
            {%- endif %}
        }
        {%- endfor %}

        return success;
    }
    {%- else %}
    bool Convert{{type_name}}::from_protobuf({{type_name | member.ns_override}}{{type_name}}& /*dest*/, const common::api::abb_protobuf::types::{{type_name}}& /*src*/)
    {
        return true;
    }
    {%- endif %}

    {%- if type_info.attrs or type_info.extensions %}
    bool Convert{{type_name}}::to_protobuf(common::api::abb_protobuf::types::{{type_name}}& dest, const {{type_name | member.ns_override}}{{type_name}}& src)
    {
        bool success = true;

        const auto lock = std::scoped_lock{utils::populateMutex};

        {%- for attr in type_info.attrs %}
        {%- set protobuf_getter = attr.name.lower() %}
        {%- set protobuf_setter = "set_" + attr.name.lower() %}
        {%- set protobuf_mutable = "mutable_" + attr.name.lower() %}
        {%- set protobuf_adder = "add_" + attr.name.lower() %}
        {%- set abb_getter = "get" + attr.name | member.set_first_uppercase %}
        {%- set abb_setter = "set" + attr.name | member.set_first_uppercase %} 
        {
            {%- if attr is member.is_simple and attr is not member.is_list %}
            {%- if loop.index == 1 %}
            auto first_temp_data_member = dest.{{protobuf_mutable}}();
            first_temp_data_member->set_value(src.first);
            {%- elif loop.index == 2 %}
            auto second_temp_data_member = dest.{{protobuf_mutable}}();
            second_temp_data_member->set_value(src.second);
            {%- else %}
            {%- endif %}
            {%- else %}
            using ConversionType = Converter<
                {%- if loop.index == 1 %}
                std::decay_t<decltype(src.first)>,
                std::decay_t<decltype(dest.{{protobuf_getter}}())> 
                {%- elif loop.index == 2 %}
                std::decay_t<decltype(src.second)>,
                std::decay_t<decltype(dest.{{protobuf_getter}}())> 
                {%- else %}
                {%- endif%}
            >::converter_factory;
            auto temp_dest = dest.{{protobuf_mutable}}();
            {%- if loop.index == 1 %}
            auto temp_src = src.first;
            {%- elif loop.index == 2 %}
            auto temp_src = src.second;
            {%- else %}
            {%- endif %}
            success &= ConversionType::to_protobuf(*temp_dest, temp_src);
            {%- endif %}
        }
        {%- endfor %}

        return success;
    }
    {%- else %}
    bool Convert{{type_name}}::to_protobuf(common::api::abb_protobuf::types::{{type_name}}& /*dest*/, const {{type_name | member.ns_override}}{{type_name}}& /*src*/)
    {
        return true;
    }
    {%- endif %}
} // namespace {{ns_tpl}}