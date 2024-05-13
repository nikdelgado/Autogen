#include <uci/type/{{type_name}}.h>
{%- if type_name is message.name_root %}
#include <uci/base/Listener.h>
{%- endif %}

#include "{{path_package}}/utils/PopulateMutex.h"
#include "{{path_package}}/UciTypeWrapper.h"

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

#include "{{type_name}}.h"

namespace {{ns_tpl}}
{
    {%- if type_name is message.name_root %}

    namespace {
        class {{type_name}}Listener : public ::uci::type::{{type_name}}::Listener
        {
        public:
            using OurType = {{ns_api}}::types::{{type_name}};

            {{type_name}}Listener(std::function<void(OurType&)>&& callback)
            : callback_(callback)
            {}
        
            /** @brief convert the UCI message to our type and call the callback */
            void handleMessage(const ::uci::type::{{type_name}}& message) override
            {
                auto ourType = FromUci<OurType, ::uci::type::{{type_name}}>(message);
                callback_(ourType);
            }
        private:
            // Callback function for the converted message type
		    std::function<void(OurType&)> callback_{};
        };
    }

	std::shared_ptr<::uci::type::{{type_name}}> UciMethods{{type_name}}::create_msg(::uci::base::AbstractServiceBusConnection* asb)
	{
		return detail::createMsg<::uci::type::{{type_name}}>(asb);
	}

    ::uci::base::Reader* UciMethods{{type_name}}::create_reader(const std::string& topic, ::uci::base::AbstractServiceBusConnection* asb)
    {
        return detail::createReader<::uci::type::{{type_name}}>(topic, asb);
    }

    ::uci::base::Writer* UciMethods{{type_name}}::create_writer(const std::string& topic, ::uci::base::AbstractServiceBusConnection* asb)
    {
        return detail::createWriter<::uci::type::{{type_name}}>(topic, asb);
    }

    ::uci::base::Listener* UciMethods{{type_name}}::create_listener(std::function<void({{ns_api}}::types::{{type_name}}&)>&& callback)
    {
        return (::uci::base::Listener*) new {{type_name}}Listener(std::move(callback));
    }

    void UciMethods{{type_name}}::add_listener(::uci::base::Reader* reader, ::uci::base::Listener* listener)
    {
        return detail::addListener<::uci::type::{{type_name}}>(reader, listener);
    }

    void UciMethods{{type_name}}::destroy_listener(::uci::base::Listener* listener)
    {
        delete listener;
    }
    
    void UciMethods{{type_name}}::write_msg(::uci::base::Writer* writer, ::uci::type::{{type_name}}& msg)
    {
        return detail::writeMsg<::uci::type::{{type_name}}>(writer, msg);
    }

    void UciMethods{{type_name}}::shutdown_reader(::uci::base::Reader* reader, ::uci::base::Listener* listener)
    {
        return detail::shutdownReader<::uci::type::{{type_name}}>(reader, listener);
    }

    void UciMethods{{type_name}}::shutdown_writer(::uci::base::Writer* writer)
    {
        return detail::shutdownWriter<::uci::type::{{type_name}}>(writer);
    }
    {%- endif %}

    {%- if type_info.attrs or type_info.extensions %}
    bool Convert{{type_name}}::from_uci({{ns_api}}::types::{{type_name}}& dest, const ::uci::type::{{type_name}}& src)
    {
        bool success = true;

        const auto lock = std::scoped_lock{utils::populateMutex};

        {%- if type_info.extensions|length %} 
        // superclass converter chaining removes the need
        // to put the dependent includes in the header
        {%-   for ex in type_info.extensions %}
        {
            using ConversionType = Converter<
                {{ns_api}}::types::{{ex|ext.type}},
                ::uci::type::{{ex|ext.type}}
            >::type;
            success &= ConversionType::from_uci(dest, src);
        }
        {%-   endfor %}
        {%- endif %}

        {%- for attr in type_info.attrs %}
        {%- set field_access = attr|member.getter + "()"%}
        {%- set field_setter = attr|member.setter %}
        {%- set asb_name = attr.name %}
        {

            {%- if attr is member.really_optional %}
            if (src.has{{asb_name}}())
            {
                {%- if attr is member.is_native %}
                auto value = src.get{{asb_name}}();
                {%- else %}
                auto value = {{attr|member.type_name(ns_api)}}{};
                using ConversionType = Converter<
                    std::remove_reference_t<decltype(value)>,
                    std::remove_cv_t<std::remove_reference_t<decltype(src.get{{asb_name}}())>>
                >::type;
                success &= ConversionType::from_uci(value, src.get{{asb_name}}());
                {%- endif %}
                dest.{{field_setter}}(value);
            }
            {%- elif attr is member.is_native and attr is not member.is_list %}
            dest.{{field_setter}}(src.get{{asb_name}}());
            {%- elif attr is member.is_simple and attr is not member.is_list %}
            dest.{{field_setter}}({{attr|member.type_name(ns_api)}}(src.get{{asb_name}}()));
            {%- else %}
            using ConversionType = Converter<
                std::remove_reference_t<decltype(dest.{{field_access}})>,
                std::remove_cv_t<std::remove_reference_t<decltype(src.get{{asb_name}}())>>
            >::type;
            {%-   if attr is member.is_list %}
            auto value = dest.{{field_access}};
            success &= ConversionType::from_uci(value, src.get{{asb_name}}());
            dest.{{field_setter}}(value);
            {%-   else %}
            success &= ConversionType::from_uci(dest.{{field_access}}, src.get{{asb_name}}());
            {%-   endif %}
            {%- endif %}
        }
        {%- endfor %}

        return success;
    }
    {%- else %}
    bool Convert{{type_name}}::from_uci({{ns_api}}::types::{{type_name}}& /*dest*/, const ::uci::type::{{type_name}}& /*src*/)
    {
        return true;
    }
    {%- endif %}

    {%- if type_info.attrs or type_info.extensions %}
    bool Convert{{type_name}}::to_uci(::uci::type::{{type_name}}& dest, const {{ns_api}}::types::{{type_name}}& src)
    {
        bool success = true;

        const auto lock = std::scoped_lock{utils::populateMutex};

        {%- if type_info.extensions %} 
        // superclass converter chaining removes the need
        // to put the dependent includes in the header
        {%-   for ex in type_info.extensions %}
        {
            using ConversionType = Converter<
                {{ns_api}}::types::{{ex|ext.type}},
                ::uci::type::{{ex|ext.type}}
            >::type;
            success &= ConversionType::to_uci(dest, src);
        }
        {%-   endfor %}
        {%- endif %}

        {%- for attr in type_info.attrs %}
        {%- set field_access = attr|member.getter + "()"%}
        {%- set asb_name = attr.name %}
        {

            {%- if attr is member.really_optional %}
            if (src.{{field_access}})
            {
                {%- if attr is member.is_native %}
                auto value = (src.{{field_access}}.value());
                dest.set{{asb_name}}(value);
                {%- else %}
                {%-   set value_getter = '.value()' if attr is not member.is_abstract else '' %}
                {%-   if attr is uci.direct_get_set_opt %}
                auto value = decltype(dest.get{{asb_name}}()){};
                {%-   else %}
                {%-     if attr is member.is_abstract %}
                auto& value = dest.enable{{asb_name}}(
                    Converter<
                        {{attr|member.type_name(ns_api)}},
                        ::uci::type::{{attr|member.base_type_name}}
                    >::type::uci_accessor_for(src.{{field_access}})
                );
                {%-     else %}
                auto& value = dest.enable{{asb_name}}({{attr|uci.accessor_type}});
                {%-     endif %}
                {%-   endif %}
                using ConversionType = Converter<
                    std::remove_cv_t<std::remove_reference_t<decltype(src.{{field_access}}{{value_getter}})>>,
                    std::remove_reference_t<decltype(value)>
                >::type;
                success &= ConversionType::to_uci(value, src.{{field_access}}{{value_getter}});
                {%-   if attr is uci.direct_get_set_opt %}
                dest.set{{asb_name}}(value);
                {%-   endif %}
                {%- endif %}
            }
            {%- elif attr is member.is_native and attr is not member.is_list %}
            dest.set{{asb_name}}(src.{{field_access}});
            {%- elif attr is member.is_simple and attr is not member.is_list %}
            dest.set{{asb_name}}(src.{{field_access}}.getValue());
            {%- elif attr is member.is_custom %}
            auto value = decltype(dest.get{{asb_name}}()){};
            using ConversionType = Converter<
                std::remove_cv_t<std::remove_reference_t<decltype(src.{{field_access}})>>,
                std::remove_reference_t<decltype(dest.get{{asb_name}}())>
            >::type;
            success &= ConversionType::to_uci(value, src.{{field_access}});
            dest.set{{asb_name}}(value);
            {%- else %}
            using ConversionType = Converter<
                std::remove_cv_t<std::remove_reference_t<decltype(src.{{field_access}})>>,
                std::remove_reference_t<decltype(dest.get{{asb_name}}())>
            >::type;
            success &= ConversionType::to_uci(dest.get{{asb_name}}(), src.{{field_access}});
            {%- endif %}
        }
        {%- endfor %}

        return success;
    }
    {%- else %}
    bool Convert{{type_name}}::to_uci(::uci::type::{{type_name}}& /*dest*/, const {{ns_api}}::types::{{type_name}}& /*src*/)
    {
        return true;
    }
    {%- endif %}
} // namespace {{ns_tpl}}