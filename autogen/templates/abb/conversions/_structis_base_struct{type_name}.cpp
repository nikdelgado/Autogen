#include "{{type_name}}.h"

{%- if type_name is member.has_include_override %}
{%- for file in type_name | member.include_override %}
#include "{{ file }}"
{%- endfor %}
{%- endif %}

#include "{{path_package}}/utils/PopulateMutex.h"

{%- if type_info.attrs|select('member.is_native')|list %}
#include "{{path_package}}/utils/Native.h"
{%- endif %}

{%- if type_info.attrs|select('member.is_list')|list %}
#include "{{path_package}}/utils/Vector.h"
{%- endif %}

// include dependencies
{%- set included_files = [] %}
{%- for attr in type_info.attrs|select('member.is_custom')|list %}
{%- set include_path = attr|util.include(path_package) %}

{%- if include_path not in included_files %}
{%- set _ = included_files.append(include_path) %}
{%- endif %}

{%- endfor %}

{%- for ex in type_info.extensions %}
{%- set include_path = "\"" + (ex|ext.type) + ".h\"" %}

{%- if include_path not in included_files %}
{%- set _ = included_files.append(include_path) %}
{%- endif %}

{%- endfor %}

{%- for include in (
    type_info.attrs|map('cpp.includes', false)|sum(start=[])
    + type_info.attrs|map('h.includes')|sum(start=[])
)|sort|unique %}
{%- set include_path = include|incl.quote_fix(path_package) %}

{%- if include_path not in included_files %}
{%- set _ = included_files.append(include_path) %}
{%- endif %}

{%- endfor %}

{%- for include_path in included_files|sort|unique %}
#include {{ include_path }}
{%- endfor %}

namespace {{ns_tpl}}
{
    {%- if type_info.attrs or type_info.extensions %}
    bool Convert{{type_name}}::from_protobuf({{type_name | member.ns_override}}{{type_name}}& dest, const common::api::abb_protobuf::types::{{type_name}}& src)
    {
        bool success = true;

        const auto lock = std::scoped_lock{utils::populateMutex};

        {%- for attr in type_info.attrs %}
        {%- set protobuf_getter = attr.name.lower().replace("_", "") %}
        {%- set protobuf_setter = "set_" + attr.name.lower().replace("_", "") %}

        {# UUID has non uniform naming convention, need special case #}
        {%- if attr.name == "uuid" %}
        {%- set abb_getter = "get" + attr.name.upper() %}
        {%- set abb_setter = "set" + attr.name.upper() %}
        {%- elif attr.name == "mfaState" %}
        {%- set abb_getter = "getMFA_State" %}
        {%- set abb_setter = "setMFA_State" %}
        {%- elif attr.name == "mfaComponents" %}
        {%- set abb_getter = "getMFAComponents" %}
        {%- set abb_setter = "setmfaComponents" %}
        {%- else %}
        {%- set abb_getter = "get" + attr.name | member.set_first_uppercase %}
        {%- set abb_setter = "set" + attr.name | member.set_first_uppercase %}
        {%- endif %}
        
        {
            {# Types below have non-uniform naming conventions, need special cases #}
            {%- if type_name == "BaseDirectional" or type_name == "AzEl" or type_name == "RangeAzEl" %}
            dest.{{attr.name}} = src.{{protobuf_getter}}();

            {%- elif type_name == "Directional" %}
            dest.set{{attr.name}}(src.{{protobuf_getter}}());

            {%- elif attr is member.is_native and attr is not member.is_list %}
            dest.{{abb_setter}}(src.{{protobuf_getter}}());

            {%- elif attr is member.is_list and attr is member.is_primitive_list %}
            {%- if "{http://www.w3.org/2001/XMLSchema}double" in attr.types[0].qname %}
            std::vector<double> temp_vector;

            const auto& {{protobuf_getter}} = src.{{protobuf_getter}}();
            std::copy({{protobuf_getter}}.begin(), {{protobuf_getter}}.end(), std::back_inserter(temp_vector));

            {%- else %}
            std::vector<common::api::abb_protobuf::types::{{attr.types[0].qname}}> temp_vector;

            for (int i = 0; i < src.{{protobuf_getter}}().size(); ++i) {
                temp_vector.push_back(src.{{protobuf_getter}}(i));
            }

            {%- endif %}
            using ConversionType = Converter<
                std::decay_t<decltype(dest.{{abb_getter}}())>,
                std::decay_t<decltype(temp_vector)>
            >::converter_factory;

            auto temp_dest = dest.{{abb_getter}}();

            success &= ConversionType::from_protobuf(temp_dest, temp_vector);

            dest.{{abb_setter}}(temp_dest);

            {%- else %}
            using ConversionType = Converter<
                std::decay_t<decltype(dest.{{abb_getter}}())>, 
                std::decay_t<decltype(src.{{protobuf_getter}}())>
            >::converter_factory;
            
            auto temp_dest = dest.{{abb_getter}}();
            auto temp_src = src.{{protobuf_getter}}();

            success &= ConversionType::from_protobuf(temp_dest, temp_src);

            dest.{{abb_setter}}(temp_dest);
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
        {%- set protobuf_getter = attr.name.lower().replace("_", "") %}
        {%- set protobuf_setter = "set_" + attr.name.lower().replace("_", "") %}
        {%- set protobuf_mutable = "mutable_" + attr.name.lower().replace("_", "") %}
        {%- set protobuf_adder = "add_" + attr.name.lower().replace("_", "") %}
        
        {# UUID has non uniform naming convention, need special case #}
        {%- if attr.name == "uuid" %}
        {%- set abb_getter = "get" + attr.name.upper() %}
        {%- set abb_setter = "set" + attr.name.upper() %}
        {%- elif attr.name == "mfaState" %}
        {%- set abb_getter = "getMFA_State" %}
        {%- set abb_setter = "setMFA_State" %}
        {%- elif attr.name == "mfaComponents" %}
        {%- set abb_getter = "getMFAComponents" %}
        {%- set abb_setter = "setmfaComponents" %}
        {%- else %}
        {%- set abb_getter = "get" + attr.name | member.set_first_uppercase %}
        {%- set abb_setter = "set" + attr.name | member.set_first_uppercase %}
        {%- endif %} 
        {
            {# Types below have non-uniform naming conventions, need special cases #}
            {%- if type_name == "BaseDirectional" or type_name == "AzEl" or type_name == "RangeAzEl" %}
            dest.{{protobuf_setter}}(src.{{attr.name}});

            {%- elif type_name == "Directional" %}
            dest.{{protobuf_setter}}(src.get{{attr.name}}());

            {%- elif attr is member.is_native and attr is not member.is_list %}
            dest.{{protobuf_setter}}(src.{{abb_getter}}());

            {%- elif attr is member.is_simple and attr is not member.is_list %}
            dest.{{protobuf_setter}}(src.{{abb_getter}}.getValue());

            {%- elif attr is member.is_list and attr is member.is_primitive_list %}
            {%- if "{http://www.w3.org/2001/XMLSchema}double" in attr.types[0].qname %}
            std::vector<double> temp_vector;
            {%- else %}
            std::vector<common::api::abb_protobuf::types::{{attr.types[0].qname}}> temp_vector;
            {%- endif %}

            using ConversionType = Converter<
                std::decay_t<decltype(src.{{abb_getter}}())>,
                std::decay_t<decltype(temp_vector)>
            >::converter_factory;

            auto temp_src = src.{{abb_getter}}();

            success &= ConversionType::to_protobuf(temp_vector, temp_src);

            std::for_each(temp_vector.begin(), temp_vector.end(), [&dest] (const auto& element){
                dest.add_{{attr.name.lower().replace("_", "")}}(element);
            });
            {%- else %}
            using ConversionType = Converter<
                std::decay_t<decltype(src.{{abb_getter}}())>,
                std::decay_t<decltype(dest.{{protobuf_getter}}())> 
            >::converter_factory;
            {%- if attr is member.is_enum %}
            auto temp_dest = dest.{{protobuf_getter}}();
            auto temp_src = src.{{abb_getter}}();

            success &= ConversionType::to_protobuf(temp_dest, temp_src);
            
            dest.{{protobuf_setter}}(temp_dest);
            {%- else %}
            auto temp_dest = dest.{{protobuf_mutable}}();
            const auto &temp_src = src.{{abb_getter}}();

            success &= ConversionType::to_protobuf(*temp_dest, temp_src);
            {%- endif %}
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