#pragma once

#include "{{path_package}}/conversions/Converter.h"
#include "common/api/abb_protobuf/types/Duration.pb.h"
#include <chrono>

namespace {{ns_package}}::conversions
{
    class ConvertDuration
    {
    public:
        static bool to_protobuf(common::api::abb_protobuf::types::Duration& dest, const std::chrono::nanoseconds& src);
        static bool from_protobuf(std::chrono::nanoseconds& dest, const common::api::abb_protobuf::types::Duration& src);
    };

    template<>
    class Converter<std::chrono::nanoseconds, common::api::abb_protobuf::types::Duration>
    {
    public:
        using converter_factory = ConvertDuration;
        using protobuf_namespace = common::api::abb_protobuf::types::Duration;
    };
} // namespace {{ns_package}}::conversions