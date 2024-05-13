#pragma once

#include <type_traits>

#include <uci/base/DoubleAccessor.h>
#include <uci/base/IntAccessor.h>
#include <uci/base/UnsignedIntAccessor.h>

#include "{{path_package}}/utils/PopulateMutex.h"
#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    namespace detail
    {
        template<typename T, typename U>
        inline bool convert_assignable(T& dest, const U& src) noexcept
        {
            const auto lock = std::scoped_lock{utils::populateMutex};
            dest = src;
            return true;
        }
    }

    template<typename OurType, typename UciType>
    class ConvertAssignable
    {
    public:
        static bool to_uci(UciType& dest, const OurType& src)
        {
            return detail::convert_assignable(dest, src);
        }
        static bool from_uci(OurType& dest, const UciType& src)
        {
            return detail::convert_assignable(dest, src);
        }
    };

    class ConvertDouble
    {
    public:
        static bool to_uci(::uci::base::DoubleAccessor& dest, const double& src)
        {
            const auto lock = std::scoped_lock{utils::populateMutex};
            dest.setDoubleValue(src);
            return true;
        }
        static bool from_uci(double& dest, const ::uci::base::DoubleAccessor& src)
        {
            return detail::convert_assignable(dest, src);
        }
    };

    class ConvertUnsigned
    {
    public:
        static bool to_uci(::uci::base::UnsignedIntAccessor& dest, const uint32_t& src)
        {
            const auto lock = std::scoped_lock{utils::populateMutex};
            dest.setUnsignedIntValue(src);
            return true;
        }
        static bool from_uci(uint32_t& dest, const ::uci::base::UnsignedIntAccessor& src)
        {
            return detail::convert_assignable(dest, src);
        }
    };

    class ConvertSigned
    {
    public:
        static bool to_uci(::uci::base::IntAccessor& dest, const int32_t& src)
        {
            const auto lock = std::scoped_lock{utils::populateMutex};
            dest.setIntValue(src);
            return true;
        }
        static bool from_uci(int32_t& dest, const ::uci::base::IntAccessor& src)
        {
            return detail::convert_assignable(dest, src);
        }
    };
    
    template<typename T>
    class Converter<T, T>
    {
    public:
        using type = ConvertAssignable<T, T>;
        using uci_type = T;
    };

    template<>
    class Converter<double, ::uci::base::DoubleAccessor>
    {
    public:
        using type = ConvertDouble;
        using uci_type = ::uci::base::DoubleAccessor;
    };

    template<>
    class Converter<uint32_t, ::uci::base::UnsignedIntAccessor>
    {
    public:
        using type = ConvertUnsigned;
        using uci_type = ::uci::base::UnsignedIntAccessor;
    };

    template<>
    class Converter<int32_t, ::uci::base::IntAccessor>
    {
    public:
        using type = ConvertSigned;
        using uci_type = ::uci::base::IntAccessor;
    };
} // namespace {{ns_package}}::conversions