#pragma once

#include <string>

#include "{{path_package}}/utils/UUID.h"
#include "{{path_package}}/types/ID_Type.h"

namespace {{ns_tpl}}
{
        /**
         * @brief helper to avoid extra work for explicit ctors, etc creating id types.
         */
        template <typename IdType, typename... Args>
        IdType idType(const std::string& uuid, const std::string& label, Args... args)
        {
                return IdType(types::ID_Type{UUIDfromStr(uuid), label}, args...);
        }

} // namespace {{ns_tpl}}
