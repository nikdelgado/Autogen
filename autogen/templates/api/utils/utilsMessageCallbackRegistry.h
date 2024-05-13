#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "{{path_package}}/types/ClassificationEnum.h"
#include "{{path_package}}/types/HeaderType.h"
#include "{{path_package}}/types/MessageModeEnum.h"
#include "{{path_package}}/types/MessageType.h"
#include "{{path_package}}/types/OwnerProducerChoiceType.h"
#include "{{path_package}}/types/OwnerProducerEnum.h"
#include "{{path_package}}/types/SecurityInformationType.h"
#include "{{path_package}}/types/ServiceID_Type.h"
#include "{{path_package}}/types/SystemID_Type.h"
#include "{{path_package}}/types/UCI_SchemaVersionStringType.h"
#include "{{path_package}}/utils/Clock.h"
#include "{{path_package}}/utils/ID_Types.h"
#include "{{path_package}}/utils/UUID.h"

namespace {{ns_tpl}}
{
	class MessageCallbackRegistry
	{
	public:
		template <typename MessageType>
		using CallbackType = std::function<void(const MessageType&)>;
		using MessageBytesType = std::vector<std::byte>;
		using MessageIdType = std::uint32_t;

		MessageCallbackRegistry() = default;

		MessageCallbackRegistry(types::SystemID_Type systemId, types::ServiceID_Type serviceId, types::UCI_SchemaVersionStringType version)
			: updateMessageHeader_{true}, systemId_{std::move(systemId)}, serviceId_{std::move(serviceId)}, version_{std::move(version)}
		{
		}

		template <typename MessageType>
		void registerCallback(const CallbackType<MessageType>& callback) noexcept
		{
			std::shared_ptr<Function> callbackPtr(new BasicFunction(callback));
			if(registry_.count(MessageType::ID()) == 0)
			{
				registry_.insert(CallbackRegistryType::value_type(MessageType::ID(), {std::move(callbackPtr)}));
			}
			else
			{
				registry_.at(MessageType::ID()).push_back(std::move(callbackPtr));
			}
		}

		template <typename... MsgTypes>
		void handleMessage(const MessageBytesType& message) const
		{
			const auto messageId = [&message]
			{
				MessageIdType id{};
				std::copy_n(message.data(), sizeof(id), reinterpret_cast<std::byte*>(&id));
				return id;
			}();

			if(registry_.count(messageId) == 0)
			{
				// No Registered Callback for message
				return;
			}

			if(!(executeCallback<MsgTypes>(messageId, message) || ...))
			{
				throw std::runtime_error("Unsupported message type, api_uci::MessageCallbackRegistry requires complete templated message list.");
			}
		}

	private:
		// Base class for all functions so that we can store all functions
		// in a single container.
		class Function
		{
		public:
			Function() = default;
			Function(const Function&) = default;
			Function(Function&&) = default;
			virtual ~Function() = default;
			Function& operator=(const Function&) = default;
			Function& operator=(Function&&) = default;
		};

		// Derived class template for functions with a particular signature.
		template <typename FunctionSignature>
		class BasicFunction final : public Function
		{
		public:
			explicit BasicFunction(FunctionSignature function) : function_(function)
			{
			}

			[[nodiscard]] const FunctionSignature& get() const noexcept
			{
				return function_;
			}

		private:
			FunctionSignature function_;
		};

		template <typename MessageType>
		[[nodiscard]] std::vector<CallbackType<MessageType>> getCallbacks(MessageIdType messageId) const
		{
			if(messageId != MessageType::ID())
			{
				throw std::runtime_error("Getting the callback from the wrong message type");
			}

			auto& registryCallbacks = registry_.at(messageId);

			auto callbacks = std::vector<CallbackType<MessageType>>{};
			callbacks.reserve(registryCallbacks.size());
			for(auto& registryCallback : registryCallbacks)
			{
				callbacks.push_back(std::dynamic_pointer_cast<BasicFunction<CallbackType<MessageType>>>(registryCallback)->get());
			}

			return callbacks;
		}

		template <typename MessageType>
		bool executeCallback(MessageIdType messageId, const MessageBytesType& messageBytes) const
		{
			if(messageId != MessageType::ID())
			{
				return false;
			}

			auto message = MessageType::deserialize(messageBytes);

            if (updateMessageHeader_)
            {
			    updateMessageHeader(message);
            }

			for(auto& callback : getCallbacks<MessageType>(messageId))
			{
				callback(message);
			}
			return true;
		}

		/** @brief Update the Message Header and Security Attributes */
		void updateMessageHeader(types::MessageType& message) const
		{
			auto& header = message.getMessageHeader();

            // Set the SystemID
            header.setSystemId(systemId_);
            // Set the ServiceID
            header.setServiceId(serviceId_);

			// Set the Timestamp
			header.setTimestamp(Clock::now());
			// Set the SchemaVersion
			header.setSchemaVersion(version_);
			// Set the OperatingMode
			header.setMode(types::MessageModeEnum::UCI_SIMULATION);

			auto& securityInfo = message.getSecurityInformation();
			// Only two security attributes are required, Classification and OwnerProducer
			securityInfo.setClassification(types::ClassificationEnum::UCI_U);
			// Owner producers is a list, resize ensures that there is space for one and only one entry
			securityInfo.getOwnerProducer().resize(1);
			securityInfo.getOwnerProducer()[0].setGovernmentIdentifier(types::OwnerProducerEnum::UCI_USA);
		}

		bool updateMessageHeader_{false};

		/** @brief System ID reported by the ASB */
		types::SystemID_Type systemId_{};

		/** @brief Service ID reported by the ASB */
		types::ServiceID_Type serviceId_{};

		/** @brief schema version as reported by the ASB */
		types::UCI_SchemaVersionStringType version_{};

		/** @brief the callback registry */
		using CallbackRegistryType = std::unordered_map<MessageIdType, std::vector<std::shared_ptr<Function>>>;
		CallbackRegistryType registry_{};
	};
} // namespace {{ns_tpl}}
