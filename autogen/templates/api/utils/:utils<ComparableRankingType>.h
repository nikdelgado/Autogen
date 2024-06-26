#pragma once
#include "{{path_package}}/types/ComparableRankingType.h"

namespace {{ns_tpl}}
{
	/**
	 * @brief Less than operator for sorting ComparableRankingTypes
	 *
	 * @param lhs: the left hand side value
	 * @param rhs: the right hand side value
	 * @returns bool
	 */
	inline bool operator<(const types::ComparableRankingType& lhs, const types::ComparableRankingType& rhs)
	{
		return lhs.getPriority() == rhs.getPriority()
		? lhs.getPrecedenceWithinPriority() < rhs.getPrecedenceWithinPriority()
		: lhs.getPriority() < rhs.getPriority();
	}
} // namespace {{ns_tpl}}