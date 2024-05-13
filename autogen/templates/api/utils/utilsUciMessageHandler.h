#pragma once

#include "MessageCallbackRegistry.h"

namespace {{ns_tpl}}
{
    /**
     * @brief helper class to simplify message handling in Managers that support one or more specific ASB messages.
     *
     */
    template <typename BaseType, typename... MsgTypes>
    class UciMessageHandler
    {
    public:
        UciMessageHandler()
        {
            if(!(registerCallback<MsgTypes>() && ...))
            {
            }
        }

        void handleMessageBytes(const MessageCallbackRegistry::MessageBytesType& bytes)
        {
            callbacks_.handleMessage<MsgTypes...>(bytes);
        }

    private:
        template <typename MsgType>
        bool registerCallback()
        {
            // TODO: the static_cast is the part that fails, then this doesn't give our custom error message.
            static_assert(std::is_same_v<decltype(static_cast<void (BaseType::*)(const MsgType&)>(&BaseType::handleUciMessage)),
                                         void (BaseType::*)(const MsgType&)>,
                          "Missing handleUciMessage for MsgType, see assertion details for class/message.");
            callbacks_.registerCallback<MsgType>([&](const MsgType& msg) { static_cast<BaseType*>(this)->handleUciMessage(msg); });
            return true;
        }

        MessageCallbackRegistry callbacks_;
    };
} // namespace {{ns_tpl}}