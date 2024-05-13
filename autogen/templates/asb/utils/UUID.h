#include <uci/base/UUID.h>
#include <{{path_api}}/utils/UUID.h>

#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    class ConvertUuid
    {
    public:    
        static bool from_uci({{ns_api}}::utils::UUID& dest, const ::uci::base::UUID& src);
        static bool to_uci(::uci::base::UUID& dest, const {{ns_api}}::utils::UUID& src);
    };

    template<>
    class Converter<{{ns_api}}::utils::UUID, ::uci::base::UUID>
    {
    public:
        using type = ConvertUuid;
        using uci_type = ::uci::base::UUID;
    };
} // namespace {{ns_package}}::conversions