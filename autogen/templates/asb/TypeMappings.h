#pragma once

#include <string_view>

namespace {{ns_api}}::types
{
{%- for type_name, _type in class_map|dictsort if _type is message.root %}
class {{type_name}};
{%- endfor %}
} // namespace {{ns_api}}::types

namespace uci::type
{
{%- for type_name, _type in class_map|dictsort if _type is message.root %}
class {{type_name}};
{%- endfor %}
} // namespace uci::type

namespace {{ns_package}}
{
    template<class T> class TypeMap;

{%- for type_name, _type in class_map|dictsort if _type is message.root %}
    template<>
    class TypeMap<{{ns_api}}::types::{{type_name}}>
    {
    public:
        using UciType = ::uci::type::{{type_name}};
        static constexpr std::string_view topic_key = "{{type_name[:-2]}}";
    };
{%- endfor %}
} // namespace {{ns_package}}