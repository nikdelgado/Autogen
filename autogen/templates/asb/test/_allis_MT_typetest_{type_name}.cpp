#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "test.h"

// CAL types
#include <uci/base/AbstractServiceBusConnection.h>

// Asb Interfaces
#include "{{path_package}}/AsbConnection.h"
#include "{{path_package}}/AsbInterface.h"
#include "{{path_package}}/conversions/{{type_name}}.h"

class TypesTestSuite : public BaseTest
{
public:
    void save(const {{ns_api}}::types::{{type_name}}& msg)
    {
        std::unique_lock lock(bytesMutex_);
        bytes_ = msg.serialize();
    }

    std::vector<std::byte> bytes_{};
    std::mutex bytesMutex_{};
};

/**
    * @brief test {{type_name}}
    */
TEST_F(TypesTestSuite, Test{{type_name}})
{
    // Create Writer and Reader
    auto asb = std::make_shared<{{ns_api}}::AsbConnection>("Test Service");
    {%- set topic = type_name %}
    std::string topic = "{{topic}}";
    using OurType = {{ns_api}}::types::{{type_name}};
    auto writer = {{ns_api}}::Writer<OurType>{asb, topic};
    auto reader = {{ns_api}}::Reader<OurType>{
        asb, topic, [this](OurType& msg) { save(msg); }};

    // Send the default object
    const auto sent = OurType{};
    writer.send(sent);

    // Wait a little while to make sure we have time to received the message
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::unique_lock lock(bytesMutex_);
    ASSERT_GT(bytes_.size(), 0);

    const auto rec = OurType::deserialize(bytes_);
    ASSERT_TRUE(rec == sent);
}
    
    