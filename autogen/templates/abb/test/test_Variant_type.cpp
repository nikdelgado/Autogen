#include <chrono>
#include <thread>
#include <variant>
#include "gtest/gtest.h"
#include "test.h"
#include "{{path_package}}/conversions/Converter.h"
#include "common/iface/zmq/publisher/Publisher.h"
#include "common/iface/zmq/subscriber/Subscriber.h"

#include "common/api/abb_protobuf/types/ExecuteTaskCmd.pb.h"
#include "common/iface/abb_protobuf/conversions/ExecuteTaskCmd.h"
#include "common-ir-mel/library/ExecuteTaskCmd_Extra Review.h"
#include "common-ir-mel_addendum/library/LadarTaskParam.h"
#include "common-ir-mel_addendum/library/RangeTaskParam.h"
#include "common-ir-mel/library/CommonIR_MEL.h"

class TypesTestSuite : public BaseTest {};

/**
    * @brief Variant Test 1
*/
TEST_F(TypesTestSuite, ExtensiveVariantTest1)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::irmel::ExecuteTaskCmd;
    abbType abbSender{};

    gra::iface::irmel::ScanParam scanParamObj{};

    const std::variant<gra::iface::irmel::ScanParam, gra::iface::irmel::StareTaskParam, gra::iface::irmel::TrackTaskParam,
        gra::iface::commonirmeladdendum::RangeTaskParam, gra::iface::commonirmeladdendum::LadarTaskParam> data = scanParamObj;

    abbSender.setData(data);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::ExecuteTaskCmd;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://ExecuteTaskCmd1", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("ExecuteTaskCmd1", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://ExecuteTaskCmd1", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"ExecuteTaskCmd1"}
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
    * @brief Variant Test 2
*/
TEST_F(TypesTestSuite, ExtensiveVariantTest2)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::irmel::ExecuteTaskCmd;
    abbType abbSender{};

    gra::iface::irmel::StareTaskParam stareTaskParamObj{};

    const std::variant<gra::iface::irmel::ScanParam, gra::iface::irmel::StareTaskParam, gra::iface::irmel::TrackTaskParam,
        gra::iface::commonirmeladdendum::RangeTaskParam, gra::iface::commonirmeladdendum::LadarTaskParam> data = stareTaskParamObj;

    abbSender.setData(data);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::ExecuteTaskCmd;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://ExecuteTaskCmd2", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("ExecuteTaskCmd2", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://ExecuteTaskCmd2", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"ExecuteTaskCmd2"}
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
    * @brief Variant Test 3
*/
TEST_F(TypesTestSuite, ExtensiveVariantTest3)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::irmel::ExecuteTaskCmd;
    abbType abbSender{};

    gra::iface::irmel::TrackTaskParam trackTaskParamObj{};

    const std::variant<gra::iface::irmel::ScanParam, gra::iface::irmel::StareTaskParam, gra::iface::irmel::TrackTaskParam,
        gra::iface::commonirmeladdendum::RangeTaskParam, gra::iface::commonirmeladdendum::LadarTaskParam> data = trackTaskParamObj;

    abbSender.setData(data);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::ExecuteTaskCmd;
    protoType protoSender{};

    // Convert ABB object
    ::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://ExecuteTaskCmd3", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("ExecuteTaskCmd3", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://ExecuteTaskCmd3", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"ExecuteTaskCmd3"}
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
    * @brief Variant Test 4
*/
TEST_F(TypesTestSuite, ExtensiveVariantTest4)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::irmel::ExecuteTaskCmd;
    abbType abbSender{};

    gra::iface::commonirmeladdendum::RangeTaskParam rangeTaskParamObj{};

    const std::variant<gra::iface::irmel::ScanParam, gra::iface::irmel::StareTaskParam, gra::iface::irmel::TrackTaskParam,
        gra::iface::commonirmeladdendum::RangeTaskParam, gra::iface::commonirmeladdendum::LadarTaskParam> data = rangeTaskParamObj;

    abbSender.setData(data);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::ExecuteTaskCmd;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://ExecuteTaskCmd4", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("ExecuteTaskCmd4", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://ExecuteTaskCmd4", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"ExecuteTaskCmd4"}
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
    * @brief Variant Test 5
*/
TEST_F(TypesTestSuite, ExtensiveVariantTest5)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using abbType = gra::iface::irmel::ExecuteTaskCmd;
    abbType abbSender{};

    gra::iface::commonirmeladdendum::LadarTaskParam ladarTaskParamObj{};

    const std::variant<gra::iface::irmel::ScanParam, gra::iface::irmel::StareTaskParam, gra::iface::irmel::TrackTaskParam,
        gra::iface::commonirmeladdendum::RangeTaskParam, gra::iface::commonirmeladdendum::LadarTaskParam> data = ladarTaskParamObj;

    abbSender.setData(data);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::ExecuteTaskCmd;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://ExecuteTaskCmd5", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("ExecuteTaskCmd5", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://ExecuteTaskCmd5", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"ExecuteTaskCmd5"}
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