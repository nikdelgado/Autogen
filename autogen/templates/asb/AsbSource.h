#pragma once

#include <memory>
#include <string>

#include <{{path_api}}/types/MessageType_cpp.h>
#include <{{path_api}}/types/SystemID_Type.h>
#include <{{path_api}}/types/ServiceID_Type.h>
#include <{{path_api}}/utils/MessageCallbackRegistry.h>
#include <{{path_api}}/utils/ID_Types.h>
#include <{{path_api}}/utils/UUID.h>

#include "AsbInterface.h"

namespace {{ns_tpl}}
{
    class AsbSource
    {
    public:
        explicit AsbSource(const std::shared_ptr<AsbConnection>& asb) :
            asb_{asb},
            outgoingCallbackRegistry_{ {{ns_api}}::utils::idType<{{ns_api}}::types::SystemID_Type>(asb.get()->getPointer()->getMySystemUUID().toString(), asb.get()->getPointer()->getMySystemLabel()),
                                       {{ns_api}}::types::ServiceID_Type{ {{ns_api}}::types::ID_Type{ {{ns_api}}::utils::UUIDfromStr(asb.get()->getPointer()->getMyServiceUUID().toString())} },
                                       asb.get()->getPointer()->getOmsSchemaVersion()}
        {
        }

        AsbSource(const std::shared_ptr<AsbConnection>& asb, const {{ns_api}}::types::SystemID_Type& systemId, const {{ns_api}}::types::ServiceID_Type& serviceId)
			: asb_{asb},
			  outgoingCallbackRegistry_{systemId, serviceId, asb.get()->getPointer()->getOmsSchemaVersion()}
		{
		}

        /** @brief Builds all the writers and adds their send functions to the callback registry */
        template<typename... MsgTypes>
        void createWriters(const ReadWriteTopics& topicMap)
        {
            ( createWriterImpl<MsgTypes>(topicMap) && ...);
        }

        void resetWriters()
        {
            writers_.clear();
        }       

    protected:
        template<typename MessageType>
        bool createWriterImpl(const ReadWriteTopics& topicMap)
        {
            auto writer = std::make_unique<Writer<MessageType>>(
                asb_,
                std::get<1>(detail::getReadWriteTopic(topicMap, TypeMap<MessageType>::topic_key))
            );
			outgoingCallbackRegistry_.registerCallback<MessageType>(std::bind(&Writer<MessageType>::send, writer.get(), std::placeholders::_1));
            writers_.push_back(std::move(writer));
            return true;
        }

		// The ASB
		const std::shared_ptr<AsbConnection> asb_{};

        /** @brief Callback Registry for all messages going out to the ASB */
        {{ns_api}}::utils::MessageCallbackRegistry outgoingCallbackRegistry_{};

        /** @brief array of writers, tracks ownership/lifecycle **/
        std::vector<std::unique_ptr<detail::BaseIo>> writers_;
    };
} // namespace {{ns_tpl}}
