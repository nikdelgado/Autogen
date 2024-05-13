#include "{{path_package}}/utils/PopulateMutex.h"

#include "Duration.h"

namespace {{ns_package}}::conversions
{
    bool ConvertDuration::from_uci({{ns_api}}::utils::Duration& dest, const ::uci::type::DurationTypeValue& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest = {{ns_api}}::utils::Duration(src);
		return true;
	}

    bool ConvertDuration::to_uci(::uci::type::DurationTypeValue& dest, const {{ns_api}}::utils::Duration& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest = src.count();
		return true;
	}
} // namespace {{ns_package}}::conversions