#pragma once

#include <string_view>
#include "common/api/abb_protobuf/types/TypeMapEnum.pb.h"

namespace common::api::abb_protobuf::types
{
{%- set complex_types = [] %}
{%- for type_name, _type in class_map|dictsort %}
{%- if _type is class.is_complex_type and type_name != "Data" and type_name != "ContributingSensor"%}
{%- set _ = complex_types.append(type_name) %}
class {{type_name}};
{%- endif %}
{%- endfor %}
} // namespace common::api::abb_protobuf::types
{% set dict_list = [] | member.get_namespace_override_list %}
{%- for namespace, classes in dict_list.items()  %}
{%- if namespace == None %}
{%- for class_name in classes %}
{%- if class_name in complex_types %}
class {{ class_name }};
{%- endif %}
{%- endfor %}
{%- else %}
namespace {{ namespace }} {
    {%- for class_name in classes %}
    {%- if class_name in complex_types %}
    class {{ class_name }};
    {%- endif %}
    {%- endfor %}
} // namespace {{ namespace }}
{%- endif %}
{%- endfor %}


namespace {{ns_package}}
{
    template<common::api::abb_protobuf::TypeMapEnum T>
    class RecieveMap;

{%- for type_name, _type in class_map|dictsort %}
{%- if _type is class.is_complex_type and type_name != "Data" and type_name != "ContributingSensor" %}
    template<>
    class RecieveMap<common::api::abb_protobuf::TypeMapEnum::TypeMapEnum_{{type_name}}>
    {
    public:
        using AbbType = {{type_name | member.ns_override}}{{type_name}};
        using ProtoType = common::api::abb_protobuf::types::{{type_name}};
    };
    {%- endif %}
{% endfor %}

    template<class T>
    class SendMap;

{%- for type_name, _type in class_map|dictsort %}
{%- if _type is class.is_complex_type and type_name != "Data" and type_name != "ContributingSensor" %}
    template<>
    class SendMap<{{type_name | member.ns_override}}{{type_name}}>
    {
    public:
        const common:api::abb_protobuf::types::TypeMapEnum enumType = common::api::abb_protobuf::types::TypeMapEnum_{{type_name}};
    };
    {%- endif %}
{% endfor %}
} // namespace {{ns_package}}