#include <chrono>
    #include <thread>
    #include "gtest/gtest.h"
    #include "test.h"
    #include "{{path_package}}/conversions/Converter.h"
    #include "r/Publisher.h"
    #include "common/iface/zmq/subscriber/Subscriber.h"
    
    #include "common/api/abb_protobuf/types/ContributingSensor.pb.h"
    #include "common/api/abb_protobuf/types/ComponentLocation.pb.h"
    #include "common/api/abb_protobuf/types/SensorIDType.pb.h"
    #include "common/api/abb_protobuf/types/FrameHeader.pb.h"

    #include "common/iface/abb_protobuf/conversions/ContributingSensor.h"
    #include "common/iface/abb_protobuf/conversions/ComponentLocation.h"
    #include "common/iface/abb_protobuf/conversions/SensorIDType.h"
    #include "common/iface/abb_protobuf/conversions/FrameHeader.h"

    #include "common-ir-mel/library/CommonIR_MEL.h"
    #include "common-mel/library/Directional.h"
    #include "common-mel/library/UCI_ID.h"


    
    #include "common-mel/library/BIT_Status.h"
    
    class TypesTestSuite : public BaseTest {};
    
    /**
        * @brief Test Vector Utility Class 
    */
    TEST_F(TypesTestSuite, ExtensiveStdPairAliasTest)
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        
        // Create Abb Sender Object
        using abbType = gra::iface::irmel::FrameHeader;
        abbType abbSender{};

        gra::iface::mel::ForeignKey fkObj{"testKey" , "testSystemName"};
        gra::iface::mel::ComponentLocation componentLocationObj {1,2,3,fkObj};
        gra::iface::irmel::SensorIDType id = 123456789;

        gra::iface::irmel::ContributingSensor sensor = std::make_pair(componentLocationObj, id);

        abbSender.setFace(sensor);
    
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