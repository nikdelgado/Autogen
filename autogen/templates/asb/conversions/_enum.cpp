#include <uci/type/{{type_name}}.h>

#include "{{path_package}}/utils/PopulateMutex.h"

#include "{{type_name}}.h"

namespace {{ns_tpl}}
{
    bool Convert{{type_name}}::from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        switch (src.getValue())
        {
            
            {%- for attr in type_info.attrs %}
            case ::uci::type::{{type_name}}::{{attr|enum.name}}:
            {
                dest = {{ns_api}}::types::{{type_name}}::{{attr|enum.name}};
				break;
			} 
            {%- endfor %}
            default:
            {
			    return false;
			}
        }

		return true;
	}

    bool Convert{{type_name}}::to_uci(::uci::type::{{type_name}}& dest, const {{ns_api}}::types::{{type_name}}& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        switch (src)
        {
            {%- for attr in type_info.attrs %}
            case {{ns_api}}::types::{{type_name}}::{{attr|enum.name}}:
            {
                dest.setValue(::uci::type::{{type_name}}::{{attr|enum.name}});
				break;
			} 
            {%- endfor %}
            default:
            {
			    return false;
			}
        }

		return true;
    }
} // namespace {{ns_tpl}}