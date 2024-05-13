#pragma once

#include <memory>
#include <mutex>
#include <string_view>
#include <type_traits>

#include "AsbConnection.h"
#include "TypeMappings.h"
#include "UciTypeWrapper.h"

// NOTE: intentionally not using any static members from Uci (cal) types here, they are compiled into
// the struct cpp files to hit any exposed uci/cal includes.
namespace uci::base
{
	class Reader;
	class Writer;
	class Listener;
}

namespace {{ns_tpl}}
{
	using ReadWriteTopic = std::pair<std::string, std::string>;
	using ReadWriteTopics = std::unordered_map<std::string, ReadWriteTopic>;

	namespace detail
	{
		inline ReadWriteTopic getReadWriteTopic(const ReadWriteTopics& topicMap, std::string_view messageKey)
		{
			const std::string key{messageKey};
			if(topicMap.count(key) == 0)
			{
				return {key, key};
			}

			return topicMap.at(key);
		}

		/**
		 * @brief Base class to store writers in a single vector for lifecycle management
		 */
		class BaseIo
		{
		public:
			virtual ~BaseIo() = default;
		};
	}

	template<typename OurType>
	class Writer : public detail::BaseIo
	{
	public:
	    // Define the corresponding UCI Message type for convenience
	    using UciMessageType = typename TypeMap<OurType>::UciType;

		/** @brief Constructor */
		Writer(const std::shared_ptr<AsbConnection>& asb, const std::string& topic)
			: asb_{asb}, writer_{conversions::Converter<OurType, UciMessageType>::uci_methods::create_writer(topic, asb->getPointer())}
		{
			
		}

		/** @brief Destructor */
		~Writer()
		{
			conversions::Converter<OurType, UciMessageType>::uci_methods::shutdown_writer(writer_);
		}

		/** @brief Convert the message type to the UCI message and send it */
		void send(const OurType& message)
		{
			auto msg = ToUci<OurType, UciMessageType>(asb_.get()->getPointer(), message);
			conversions::Converter<OurType, UciMessageType>::uci_methods::write_msg(writer_, *msg);
		}

		// Don't support copy or assignment operators at risk of deleting the same memory twice
		Writer(const Writer& rhs) = delete;
		Writer& operator=(Writer const& rhs) = delete;
		Writer(Writer&& rhs) = delete;
		Writer& operator=(Writer&& rhs) = delete;

	private:
		// The ASB
		const std::shared_ptr<AsbConnection> asb_{};
		// Writer
		typename ::uci::base::Writer* const writer_{nullptr};
	};

	template<typename OurType>
	class Reader : public detail::BaseIo
	{
	public:
	    // Define the corresponding UCI Message type for convenience
	    using UciMessageType = TypeMap<OurType>::UciType;

		/** @brief Constructor */
		Reader(const std::shared_ptr<AsbConnection>& asb, const std::string& topic, std::function<void(OurType&)> callback)
		: reader_{conversions::Converter<OurType, UciMessageType>::uci_methods::create_reader(topic, asb->getPointer())},
		  listener_{conversions::Converter<OurType, UciMessageType>::uci_methods::create_listener(std::move(callback))}
		{
			conversions::Converter<OurType, UciMessageType>::uci_methods::add_listener(reader_, listener_);
		}

		/** @brief Destructor */
		~Reader()
		{
			conversions::Converter<OurType, UciMessageType>::uci_methods::shutdown_reader(reader_, listener_);
			conversions::Converter<OurType, UciMessageType>::uci_methods::destroy_listener(listener_);
		}

		// Don't support copy or assignment operators at risk of deleting the same memory twice
		Reader(const Reader& rhs) = delete;
		Reader& operator=(Reader const& rhs) = delete;
		Reader(Reader&& rhs) = delete;
		Reader& operator=(Reader&& rhs) = delete;

	private:
		// Reader
		::uci::base::Reader* const reader_{nullptr};
		// Listener
		::uci::base::Listener* listener_{nullptr};
	};
} // namespace {{ns_tpl}}