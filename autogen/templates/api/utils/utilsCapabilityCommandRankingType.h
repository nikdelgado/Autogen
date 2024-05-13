#pragma once

#include "{{path_package}}/types/CapabilityCommandRankingType.h"
#include "{{path_package}}/utils/ComparableRankingType.h"

namespace {{ns_tpl}}
{
	/**
	 * @brief Less than operator for sorting CapabilityCommandRankingType
	 *
	 * @param lhs: the left hand side value
	 * @param rhs: the right hand side value
	 * @returns bool
	 */
	inline bool operator<(const types::CapabilityCommandRankingType& lhs, const types::CapabilityCommandRankingType& rhs)
	{
		return lhs.getRank() < rhs.getRank();
	}
} // namespace {{ns_tpl}}