#pragma once

#include <memory>
#include <mutex>
#include <type_traits>

#include "conversions/Converter.h"

namespace uci::base {
class AbstractServiceBusConnection;
class Listener;
class Reader;
class Writer;
}


namespace {{ns_tpl}}
{
    namespace detail
    {
        template<typename UciType>
        std::shared_ptr<UciType> createMsg(::uci::base::AbstractServiceBusConnection* asb) {
            return {&(UciType::create(asb)), [](UciType* val){UciType::destroy(*val);} };
        }

        template<typename UciType>
        ::uci::base::Writer* createWriter(const std::string& topic, ::uci::base::AbstractServiceBusConnection* asb) {
            return &UciType::createWriter(topic, asb);
        }

        template<typename UciType>
        ::uci::base::Reader* createReader(const std::string& topic, ::uci::base::AbstractServiceBusConnection* asb) {
            return &UciType::createReader(topic, asb);
        }

        template<typename UciType>
        void addListener(::uci::base::Reader* reader, ::uci::base::Listener* listener) {
            // dynamic_cast not supported due to non-virtual reader base class, c-style required, void first because of virtual inheritance...
            ((typename UciType::Reader*)((void*)reader))->addListener(*((typename UciType::Listener*)(listener)));
        }

        template<typename UciType>
        void writeMsg(::uci::base::Writer* writer, UciType& msg) {
            // dynamic_cast not supported due to non-virtual reader base class, c-style required, void first because of virtual inheritance...
            ((typename UciType::Writer*)((void*)writer))->write(msg);
        }

        template<typename UciType>
        void shutdownReader(::uci::base::Reader* reader, ::uci::base::Listener* listener){
            // dynamic_cast not supported due to non-virtual reader base class, c-style required, void first because of virtual inheritance...
            typename UciType::Reader* typedReader = (typename UciType::Reader*)((void*)reader);
            typedReader->removeListener(*((typename UciType::Listener*)listener));
            typedReader->close();
            UciType::destroyReader(*typedReader);
        }

        template<typename UciType>
        void shutdownWriter(::uci::base::Writer* writer) {
            // dynamic_cast not supported due to non-virtual reader base class, c-style required, void first because of virtual inheritance...
            typename UciType::Writer* typedWriter = (typename UciType::Writer*)((void*)writer);
            typedWriter->close();
            UciType::destroyWriter(*typedWriter);
        }
    }

    template <typename OurType, typename UciType>
    [[nodiscard]] inline std::shared_ptr<UciType> ToUci(::uci::base::AbstractServiceBusConnection* asb, const OurType& ourType)
    {
        auto dest = conversions::Converter<OurType, UciType>::uci_methods::create_msg(asb);
        // TODO(steve): Error check the Populate function
		conversions::Converter<OurType, UciType>::type::to_uci(*dest, ourType);

        return dest;
    }

    template <typename OurType, typename UciType>
    [[nodiscard]] inline OurType FromUci(const UciType& uciType)
    {
        auto ourType = OurType{};
        // TODO(steve): Error check the Populate function
		conversions::Converter<OurType, UciType>::type::from_uci(ourType, uciType);

        return ourType;
    }
} // namespace {{ns_tpl}}