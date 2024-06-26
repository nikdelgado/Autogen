#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "test.h"
#include "{{path_package}}/conversions/Converter.h"
#include "n/iface/zmq/publisher/Publisher.h"
#include "common/iface/zmq/subscriber/Subscriber.h"

#include "common/api/abb_protobuf/types/UCI_ID.pb.h"
#include "common/iface/abb_protobuf/conversions/UCI_ID.h"
#include "common-mel/library/UCI_ID.h"

class TypesTestSuite : public BaseTest {};

/**
 * @brief UUID Utility Test 1
*/
TEST_F(TypesTestSuite, ExtensiveUUIDTest1)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::mel::UCI_ID;
    {%- raw %}
    abbType abbSender{{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, "Hello World"};
    {%- endraw %}

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::UCI_ID;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://UCI_ID", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("UCI_ID", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://UCI_ID", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"UCI_ID"}
    );

    std:: this_thread::sleep_for(std::chrono::milliseconds(200));

    subscriber.~Subscriber();
    publisherThread.join();

    // Create ABB and Protobuf sender objects
    abbType abbReciever{};
    protoType protobufReciever{};

    // deserialize Converted Protobuf object
    protobufReciever.ParseFromString(recievedMessage);

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::from_protobuf(abbReciever, protobufReciever);

    // Assert ABB sender and reciever objects are equivalent
    ASSERT_EQ(abbSender, abbReciever);

    std::array<std::uint8_t, 16> otherArr = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
    abbReciever.setUUID(otherArr);
    ASSERT_NE(abbSender, abbReciever);

    google::protobuf::ShutdownProtobufLibrary();
    
}

/**
 * @brief UUID Utility Test 2
*/
TEST_F(TypesTestSuite, ExtensiveUUIDTest2)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::mel::UCI_ID;
    {%- raw %}
    abbType abbSender{{255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"};
    {%- endraw %}

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::UCI_ID;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://UCI_ID", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("UCI_ID", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://UCI_ID", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"UCI_ID"}
    );

    std:: this_thread::sleep_for(std::chrono::milliseconds(200));

    subscriber.~Subscriber();
    publisherThread.join();

    // Create ABB and Protobuf sender objects
    abbType abbReciever{};
    protoType protobufReciever{};

    // deserialize Converted Protobuf object
    protobufReciever.ParseFromString(recievedMessage);

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::from_protobuf(abbReciever, protobufReciever);

    // Assert ABB sender and reciever objects are equivalent
    ASSERT_EQ(abbSender, abbReciever);

    std::array<std::uint8_t, 16> otherArr = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
    abbReciever.setUUID(otherArr);
    ASSERT_NE(abbSender, abbReciever);

    google::protobuf::ShutdownProtobufLibrary();
    
}

/**
 * @brief UUID Utility Test 3
*/
TEST_F(TypesTestSuite, ExtensiveUUIDTest3)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::mel::UCI_ID;
    {%- raw %}
    abbType abbSender{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, "FFFFFFFFFFFFFFFFFFFFFFF"};
    {%- endraw %}

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::UCI_ID;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://UCI_ID", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("UCI_ID", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://UCI_ID", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"UCI_ID"}
    );

    std:: this_thread::sleep_for(std::chrono::milliseconds(200));

    subscriber.~Subscriber();
    publisherThread.join();

    // Create ABB and Protobuf sender objects
    abbType abbReciever{};
    protoType protobufReciever{};

    // deserialize Converted Protobuf object
    protobufReciever.ParseFromString(recievedMessage);

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::from_protobuf(abbReciever, protobufReciever);

    // Assert ABB sender and reciever objects are equivalent
    ASSERT_EQ(abbSender, abbReciever);

    std::array<std::uint8_t, 16> otherArr = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
    abbReciever.setUUID(otherArr);
    ASSERT_NE(abbSender, abbReciever);

    google::protobuf::ShutdownProtobufLibrary();
    
}

