#pragma once

#include "common/api/abb_protobuf/types/UUID.pb.h"
#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    class ConvertUuid
    {
    public:    
        static bool from_protobuf(std::array<std::uint8_t, 16>& dest, const common::api::abb_protobuf::types::UUID& src);
        static bool to_protobuf(common::api::abb_protobuf::types::UUID& dest, const std::array<std::uint8_t, 16>& src);
    };

    template<>
    class Converter<std::array<std::uint8_t, 16>, common::api::abb_protobuf::types::UUID>
    {
    public:
        using converter_factory = ConvertUuid;
        using protobuf_namespace = common::api::abb_protobuf::types::UUID;
        using abb_namespace = std::array<std::uint8_t, 16>;
    };
} // namespace {{ns_package}}::conversions