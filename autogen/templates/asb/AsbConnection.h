#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include <uci/base/AbstractServiceBusConnection.h>

namespace {{ns_tpl}}
{
    /** @brief Default timeout in seconds */
    constexpr std::chrono::seconds DEFAULT_TIMEOUT = std::chrono::seconds(10);

	class AsbConnection
	{
	public:
		/** @brief Constructor */
		explicit AsbConnection(const std::string& serviceName)
		{
			openConnection(serviceName);
		}

		/** @brief Destructor */
		~AsbConnection()
		{
			asb_->shutdown();
		}

		/** @brief return true if the asb connection state is NORMAL **/
		bool isNormal() const {
			return asb_ && asb_->getStatus().state == ::uci::base::AbstractServiceBusConnection::NORMAL;
		}

		/** @brief Open the ASB connection with the given service name */
		void openConnection(const std::string& serviceName, const std::chrono::seconds& timeout = DEFAULT_TIMEOUT)
		{
			// Quick return if the connection has already been opened
			if(hasConnection_)
			{
				return;
			}

			std::mutex asbMutex;
			std::condition_variable asbConditionVariable;
			::uci::base::AbstractServiceBusConnection* asb{};
			std::thread asbThread(
				[&asbConditionVariable, serviceName, &asb]()
				{
					asb = uci_getAbstractServiceBusConnection(serviceName, "");
					asbConditionVariable.notify_one();
				});

		    std::unique_lock<std::mutex> asbLock(asbMutex);
			if(asbConditionVariable.wait_for(asbLock, timeout) == std::cv_status::timeout)
			{
				if(asbThread.joinable())
				{
					asbThread.detach();
				}
				throw std::runtime_error("ActiveMQ Timeout");
			}
			else if(asbThread.joinable())
			{
				asbThread.join();
			}

			asb_ = asb;
			hasConnection_ = true;
		}

		/** @brief Gets the raw pointer to the connection. */
		::uci::base::AbstractServiceBusConnection* getPointer()
		{
			if(!hasConnection_)
			{
				throw std::logic_error("Attempting to access the ASB connection pointer before opening.");
			}

			return asb_;
		}

		// Don't support copy or assignment operators at risk of deleting the same memory twice
		AsbConnection(const AsbConnection& rhs) = delete;
		AsbConnection& operator=(AsbConnection const& rhs) = delete;
		AsbConnection(AsbConnection&& rhs) = delete;
		AsbConnection& operator=(AsbConnection&& rhs) = delete;

	private:
		// Raw Pointer to the ASB
		::uci::base::AbstractServiceBusConnection* asb_{nullptr};

		// Connection state of this instance
		bool hasConnection_{false};
	};
} // namespace {{ns_tpl}}