#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "test.h"
#include "{{path_package}}/conversions/Converter.h"
#include "r.h"
#include "common/iface/zmq/subscriber/Subscriber.h"

#include "common/api/abb_protobuf/types/BIT_Status.pb.h"
#include "common/iface/abb_protobuf/conversions/BIT_Status.h"

#include "buf/types/CandidateObjectMessage.pb.h"
#include "common/iface/abb_protobuf/conversions/CandidateObjectMessage.h"

#include "common/api/abb_protobuf/types/LadarRange.pb.h"
#include "common/iface/abb_protobuf/conversions/LadarRange.h"

#include "common/api/abb_protobuf/types/FrameHeader.pb.h"
#include "common/iface/abb_protobuf/conversions/FrameHeader.h"

#include "common-mel/library/ActiveBIT.h"
#include "common-mel/library/CompletedBIT.h"
#include "common-mel/library/Fault.h"
#include "common-mel/library/BIT_Status.h"
#include "common-ir-mel/library/CommonIR_MEL.h"
#include "common-mel/library/UCI_ID.h"
#include "common-ir-mel_addendum/library/LadarMeasurementReport.h"

class TypesTestSuite : public BaseTest {};

/**
 * @brief Test Vector Utility Class 
*/
TEST_F(TypesTestSuite, ExtensiveVectorTest1)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create UCI_ID object, populate fields
    {%- raw %}
    gra::iface::mel::UCI_ID uciObj{{255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}, "FFFFFFFFFFFFFFFFFF"};
    {%- endraw %}
    auto time = std::chrono::nanoseconds(1);
    double percentComplete = 99;

    // Create Active BIT Object, populate fields
    gra::iface::mel::ActiveBIT activeBITObj{uciObj, time, percentComplete};

    // Create BIT Status Object
    using abbType = gra::iface::mel::BIT_Status;
    abbType abbSender{};

    // Fill BIT Status message with 50 activeBIT Objects
    std::vector<gra::iface::mel::ActiveBIT>range(50);
    std::for_each(range.begin(), range.end(), [&abbSender] (gra::iface::mel::ActiveBIT& activeBITObj) {
        abbSender.addActiveBIT(activeBITObj);
    });

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::BIT_Status;
    protoType protoSender{};

    // Convert ABB object
    ::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        publisher(context, "inproc://BIT_Status", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("BIT_Status", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://BIT_Status", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"BIT_Status"}
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
 * @brief Test Vector Utility Class: array<objects> <--> RepeatedPtrFieldConversion
*/
TEST_F(TypesTestSuite, ExtensiveVectorTest2)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB Sender object
    using abbType = gra::iface::irmel::CandidateObjectMessage;
    abbType abbSender{};

    // Load array with candidate Objects
    gra::iface::irmel::CandidateObject candidateObj{};

    std::array<gra::iface::irmel::CandidateObject, 900> candidateObjectArr;

    std::fill(candidateObjectArr.begin(), candidateObjectArr.end(), candidateObj);

    abbSender.setCandidateObjects(candidateObjectArr);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::CandidateObjectMessage;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://CandidateObjectMessage", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("CandidateObjectMessage", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    ::iface::zmq::Subscriber subscriber(
        context, "inproc://CandidateObjectMessage", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"CandidateObjectMessage"}
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
    ::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::from_protobuf(abbReciever, protobufReciever);

    // Assert ABB sender and reciever objects are equivalent
    ASSERT_EQ(abbSender, abbReciever);

    google::protobuf::ShutdownProtobufLibrary();
    
}

/**
 * @brief Test Vector Utility Class: vector<double> <--> RepeatedFieldConversion
*/
TEST_F(TypesTestSuite, ExtensiveVectorTest3)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB Sender object
    using abbType = gra::iface::commonirmeladdendum::LadarRange;
    abbType abbSender{};

    // Load the vector with data
    std::vector<double> testData = {99,99,99,99,99,99};

    abbSender.setTrackerErrorCov(testData);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::LadarRange;
    protoType protoSender{};

    // Convert ABB object
    ::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://LadarRange", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("LadarRange", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://LadarRange", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"LadarRange"}
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
 * @brief Test Vector Utility Class: vector<enum type> <--> RepeatedPtrField Conversion
*/
TEST_F(TypesTestSuite, ExtensiveVectorTest4)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB Sender object
    using abbType = gra::iface::irmel::FrameHeader;
    abbType abbSender{};

    // initialize enum
    gra::iface::irmel::ImageFlag flag = gra::iface::irmel::ImageFlag::StareSnapshot;

    // load vector of enums
    std::vector<gra::iface::irmel::ImageFlag> imageFlagsArr = {flag, flag, flag, flag, flag};

    abbSender.setFlags(imageFlagsArr);

    // Create Protobuf sender object
    using protoType = common::api::abb_protobuf::types::FrameHeader;
    protoType protoSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<abbType, protoType>
        ::converter_factory::to_protobuf(protoSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protoSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://FrameHeader", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("FrameHeader", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://FrameHeader", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"FrameHeader"}
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


