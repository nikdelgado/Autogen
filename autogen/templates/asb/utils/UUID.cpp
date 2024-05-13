#include "{{path_package}}/utils/PopulateMutex.h"

#include "UUID.h"

namespace {{ns_package}}::conversions
{
    bool ConvertUuid::from_uci({{ns_api}}::utils::UUID& dest, const ::uci::base::UUID& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        if (!src.isNil()) {
		    dest = {{ns_api}}::utils::UUIDfromStr(src.toString());
        }
		return true;
	}

    bool ConvertUuid::to_uci(::uci::base::UUID& dest, const {{ns_api}}::utils::UUID& src)
    {
        const auto lock = std::scoped_lock{utils::populateMutex};
        if (!src.is_nil()) {
		    dest = ::uci::base::UUID::fromString({{ns_api}}::utils::UUIDtoStr(src));
        }
		return true;
	}
} // namespace {{ns_package}}::conversions
