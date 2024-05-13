#include <uci/type/{{type_name}}.h>

#include "{{path_package}}/utils/PopulateMutex.h"

{%- if type_info|variant.choices|map(attribute='as_attr')|select('member.is_list')|list %}
#include "{{path_package}}/utils/Vector.h"
{%- endif %}

{%- for attr in type_info|variant.choices|map(attribute='as_attr')|select('member.is_custom')|list %}
#include {{attr|util.include(path_package)}}
{%- endfor %}

{%- for header in type_info|variant.includes %}
#include {{header}}
{%- endfor %}

{%- for header in type_info|variant.cpp_includes %}
#include {{header}}
{%- endfor %}

#include "{{type_name}}.h"

namespace {{ns_tpl}}
{
    bool Convert{{type_name}}::from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        switch (src.get{{type_name}}ChoiceOrdinal())
        {
            {%- for choice in type_info|variant.choices %}
            case ::uci::type::{{type_name}}::{{type_name}}Choice::{{type_name.upper()}}_CHOICE_{{choice.name.upper()}}:
            {
                {%-if choice.as_attr is member.is_native and choice.as_attr is not member.is_list %}
                dest.set{{choice.name}}(src.get{{choice.name}}());
				break;
                {%- else %}
				dest.defaultActivateChoice({{ns_api}}::types::{{type_name}}::Choice::{{choice.name}});
				return Converter<
                    std::remove_reference_t<decltype(dest.get{{choice.name}}())>,
                    std::remove_cv_t<std::remove_reference_t<decltype(src.get{{choice.name}}())>>
                >::type::from_uci(dest.get{{choice.name}}(), src.get{{choice.name}}());
				{%- endif %}
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
        switch (src.heldChoice())
        {
            {%- for choice in type_info|variant.choices %}
            case {{ns_api}}::types::{{type_name}}::Choice::{{choice.name}}:
            {
                dest.set{{type_name}}ChoiceOrdinal(::uci::type::{{type_name}}::{{type_name}}Choice::{{type_name.upper()}}_CHOICE_{{choice.name.upper()}});
                {%- if choice.as_attr is member.is_native and choice.as_attr is not member.is_list %}
                dest.set{{choice.name}}(src.get{{choice.name}}());
				break;
                {%- elif choice.as_attr is member.is_simple and choice.as_attr is not member.is_list %}
                dest.set{{choice.name}}(src.get{{choice.name}}().getValue());
				break;
                {%- else %}
                using ConversionType = Converter<
                    std::remove_cv_t<std::remove_reference_t<decltype(src.get{{choice.name}}())>>,
                    std::remove_reference_t<decltype(dest.get{{choice.name}}())>
                >::type;
                {%-   if choice.as_attr is member.is_custom and choice.as_attr is not member.is_list %}
                auto value = dest.get{{choice.name}}();
                if (!ConversionType::to_uci(value, src.get{{choice.name}}())) {
                    return false;
                }
                dest.set{{choice.name}}(value);
                return true;
                {%-   else %}
				return ConversionType::to_uci(dest.get{{choice.name}}(), src.get{{choice.name}}());
				{%-   endif %}
				{%- endif %}
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