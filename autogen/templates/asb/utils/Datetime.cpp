#include "{{path_package}}/utils/PopulateMutex.h"

#include "Datetime.h"

namespace {{ns_package}}::conversions
{
    bool ConvertDateTime::from_uci({{ns_api}}::utils::TimePoint& dest, const ::uci::type::DateTimeTypeValue& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest = {{ns_api}}::utils::TimePoint({{ns_api}}::utils::Duration(src));
		return true;
	}

    bool ConvertDateTime::to_uci(::uci::type::DateTimeTypeValue& dest, const {{ns_api}}::utils::TimePoint& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
		dest = src.time_since_epoch().count();
		return true;
	}
} // namespace {{ns_package}}::conversions