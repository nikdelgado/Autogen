#include <uci/type/{{type_name}}.h>

#include <uci/base/accessorType.h>

{%- for subclass in derived %}
#include <uci/type/{{subclass.name}}.h>
#include "{{path_api}}/types/{{subclass.name}}.h"
#include "{{subclass.name}}.h"
{%- endfor %}

#include "{{path_package}}/utils/PopulateMutex.h"

#include "{{type_name}}Factory.h"

namespace {{ns_tpl}}
{
    bool Convert{{type_name}}Factory::from_uci(std::shared_ptr<{{ns_api}}::types::{{type_name}}>& dest, const ::uci::type::{{type_name}}& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        bool success = true;

        const auto accessor = src.getAccessorType();
        switch (accessor) {
        {%- for subclass in derived %}
            case {{subclass|uci.accessor_type}}:
            {
                {{ns_api}}::types::{{subclass.name}} sc{};
                using ConversionType = Converter<{{ns_api}}::types::{{subclass.name}},::uci::type::{{subclass.name}}>::type;
                success &= ConversionType::from_uci(sc, dynamic_cast<const ::uci::type::{{subclass.name}}&>(src));
                dest = std::make_shared<{{ns_api}}::types::{{subclass.name}}>(sc);
                break;
            }
        {%- endfor %}
            default:
                throw std::runtime_error("AccessorType: " + std::to_string(accessor) + " Invalid for {{type_name}}");
        }

        return success;
    }

    bool Convert{{type_name}}Factory::to_uci(::uci::type::{{type_name}}& dest, const std::shared_ptr<{{ns_api}}::types::{{type_name}}>& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        bool success = true;

        if (!src) return false;
        
        const decltype({{ns_api}}::types::{{type_name}}::ID()) id{src->abstractId()};
        switch (id) {
        {%- for subclass in derived %}
            case {{ns_api}}::types::{{subclass.name}}::ID():
            {
                using ConversionType = Converter<{{ns_api}}::types::{{subclass.name}},::uci::type::{{subclass.name}}>::type;
                success &= ConversionType::to_uci(
                    dynamic_cast<::uci::type::{{subclass.name}}&>(dest),
                    *std::dynamic_pointer_cast<{{ns_api}}::types::{{subclass.name}}>(src)
                );
                break;
            }
        {%- endfor %}
            default:
                throw std::runtime_error("ID: " + std::to_string(id) + " Invalid for {{type_name}}");
        }

        return success;
    }

    uint32_t Convert{{type_name}}Factory::uci_accessor_for(const std::shared_ptr<{{ns_api}}::types::{{type_name}}>& obj)
    {
        if (!obj) return 0;
        
        const decltype({{ns_api}}::types::{{type_name}}::ID()) id{obj->abstractId()};
        switch (id) {
        {%- for subclass in derived %}
            case {{ns_api}}::types::{{subclass.name}}::ID():
                return {{subclass|uci.accessor_type}};
                break;
        {%- endfor %}
            default:
                throw std::runtime_error("ID: " + std::to_string(id) + " Invalid for {{type_name}}");
        }
    }
} // namespace {{ns_tpl}}