#pragma once

#include <memory>
#include <vector>

#include "AsbInterface.h"

namespace {{ns_tpl}}
{
    class AsbSink
    {
    public:
        using MessageBytesType = std::vector<std::byte>;

        AsbSink(const std::shared_ptr<AsbConnection>& asb, std::function<void(const MessageBytesType&)> callback) :
            asb_{asb},
            callback_{std::move(callback)}
        {
        }

        /** @brief Callback function for each message type */
        template <typename MessageType>
        void handleMessage(const MessageType &newMsg)
        {
            // Serialize the message and call the callback
            const auto bytes = newMsg.serialize();
            callback_(bytes);
        }

        /** @brief Builds all the Readers and attaches the AsbSink::handleMessage function to them */
        template<typename... MsgTypes>
        void createReaders(const ReadWriteTopics& topicMap)
        {
            (createReaderImpl<MsgTypes>(topicMap) && ...);
        }

        void resetReaders()
        {
            readers_.clear();
        }

    protected:
    
        template<typename MessageType>
        bool createReaderImpl(const ReadWriteTopics& topicMap) {
            readers_.push_back(
                std::make_unique<Reader<MessageType>>(
                    asb_,
                    std::get<0>(detail::getReadWriteTopic(topicMap, TypeMap<MessageType>::topic_key)),
                    [this](const MessageType& newMsg){
                        handleMessage<MessageType>(newMsg);
                    }
                )
            );
            return true;
        }

		// The ASB
		const std::shared_ptr<AsbConnection> asb_{};

        /** @brief Callback for the message bytes of all readers */
        std::function<void(const MessageBytesType&)> callback_{};

        /** @brief {{type}} Readers, just holding ownership here */
        std::vector<std::unique_ptr<detail::BaseIo>> readers_;
    };
} // namespace {{ns_tpl}}