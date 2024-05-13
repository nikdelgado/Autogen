#include "{{path_package}}/utils/PopulateMutex.h"
#include "{{type_name}}.h"

namespace {{ns_tpl}}
{
	bool Convert{{type_name}}::from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}& src)
	{
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest.setValue(src);
		return true;
	}

    bool Convert{{type_name}}::to_uci(::uci::type::{{type_name}}& dest, const {{ns_api}}::types::{{type_name}}& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        dest.set{{type_info|alias.accessor_type}}Value(src.getValue());
		return true;
	}
	

	{%- if type_info is not alias.is_string %}
	bool Convert{{type_name}}Value::from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}Value& src)
	{
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest.setValue(src);
		return true;
	}

    bool  Convert{{type_name}}Value::to_uci(::uci::type::{{type_name}}Value& dest, const {{ns_api}}::types::{{type_name}}& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        dest = src.getValue();
		return true;
	}
	{%- endif %}
} // namespace {{ns_tpl}}