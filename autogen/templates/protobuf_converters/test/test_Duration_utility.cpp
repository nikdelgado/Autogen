#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "test.h"
#include "{{path_package}}/conversions/Converter.h"
#include "common/iface/zmq/publisher/Publisher.h"
#include "n/iface/zmq/subscriber/Subscriber.h"

#include "common/api/abb_protobuf/types/BIT_Status.pb.h"
#include "common/iface/abb_protobuf/conversions/BIT_Status.h"

#include "common-mel/library/BIT_Status.h"

class TypesTestSuite : public BaseTest {};

/**
    * @brief Test Vector Utility Class 
*/
TEST_F(TypesTestSuite, ExtensiveDurationTest1)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create UCI_ID object, populate fields
    gra::iface::mel::UCI_ID uciObj{};

    auto time = std::chrono::nanoseconds(9999);
    double percentComplete = 99;

    // Create Active BIT Object, populate fields
    gra::iface::mel::ActiveBIT activeBITObj{uciObj, time, percentComplete};

    // Create BIT Status Object
    using abbType = gra::iface::mel::BIT_Status;
    abbType abbSender{};

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::BIT_Status;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://BIT_Status1", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("BIT_Status1", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://BIT_Status1", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"BIT_Status1"}
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

    google::protobuf::ShutdownProtobufLibrary();
}
    
/**
    * @brief Test Vector Utility Class 
*/
TEST_F(TypesTestSuite, ExtensiveDurationTest2)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create UCI_ID object, populate fields
    gra::iface::mel::UCI_ID uciObj{};

    auto time = std::chrono::nanoseconds(0);
    double percentComplete = 99;

    // Create Active BIT Object, populate fields
    gra::iface::mel::ActiveBIT activeBITObj{uciObj, time, percentComplete};

    // Create BIT Status Object
    using abbType = gra::iface::mel::BIT_Status;
    abbType abbSender{};

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::BIT_Status;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://BIT_Status2", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("BIT_Status2", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://BIT_Status2", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"BIT_Status2"}
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

    google::protobuf::ShutdownProtobufLibrary();
}
    