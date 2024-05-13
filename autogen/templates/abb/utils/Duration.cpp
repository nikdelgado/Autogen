#include "{{path_package}}/utils/PopulateMutex.h"
#include "Duration.h"

namespace {{ns_package}}::conversions
{
    bool ConvertDuration::from_protobuf(std::chrono::nanoseconds& dest, const common::api::abb_protobuf::types::Duration& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest = std::chrono::nanoseconds(src.value());
		return true;
	}

    bool ConvertDuration::to_protobuf(common::api::abb_protobuf::types::Duration& dest, const std::chrono::nanoseconds& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest.set_value(src.count());
		return true;
	}
} // namespace {{ns_package}}::conversions