#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "test.h"
#include "{{path_package}}/conversions/Converter.h"
#include "common/iface/zmq/publisher/Publisher.h"
#include "common/iface/zmq/subscriber/Subscriber.h"


#include "common/api/abb_protobuf/types/{{type_name}}.pb.h"
#include "{{path_package}}/conversions/{{type_name}}.h"

{%- if type_name is member.has_include_override %}
{%- for file in type_name | member.include_override %}
#include "{{ file }}"
{%- endfor %}
{%- endif %}

class TypesTestSuite : public BaseTest {};

/**
 * @brief test {{type_name}}
*/
TEST_F(TypesTestSuite, Test{{type_name}})
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create ABB sender object
    using melType = {{type_name | member.ns_override}}{{type_name}};
    melType abbSender{};

    // Create Protobuf sender object
    using protobufType = common::api::abb_protobuf::types::{{type_name}};
    protobufType protobufSender{};

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<melType, protobufType>
        ::converter_factory::to_protobuf(protobufSender, abbSender);

    // Serialize Converted Protobuf object
    std::string sentMessage;
    protobufSender.SerializeToString(&sentMessage);

    zmq::context_t context{1};

    std::thread publisherThread([&]() {
        common::iface::zmq::Publisher publisher(context, "inproc://{{type_name}}", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        publisher.send("{{type_name}}", sentMessage.data(), sentMessage.size());
    });

    std::string recievedMessage;

    common::iface::zmq::Subscriber subscriber(
        context, "inproc://{{type_name}}", false,
        [&](const zmq::multipart_t& multipart) {
            if (multipart.size() >= 2) {
                recievedMessage.assign(multipart[1].data<char>(), multipart[1].size());
            }
        },
        std::vector<std::string_view>{"{{type_name}}"}
    );

    std:: this_thread::sleep_for(std::chrono::milliseconds(200));

    subscriber.~Subscriber();
    publisherThread.join();

    // Create ABB and Protobuf sender objects
    melType abbReciever{};
    protobufType protobufReciever{};

    // deserialize Converted Protobuf object
    protobufReciever.ParseFromString(recievedMessage);

    // Convert ABB object
    common::iface::abb_protobuf::conversions::Converter<melType, protobufType>
        ::converter_factory::from_protobuf(abbReciever, protobufReciever);

    // Assert ABB sender and reciever objects are equivalent
    ASSERT_EQ(abbSender, abbReciever);
    google::protobuf::ShutdownProtobufLibrary();
    
}
    
    