#pragma once

namespace {{ns_package}}::conversions
{
    template<typename OurType, typename UciType>
    class Converter
    {
    public:
        // sub instances include a "using =" called type which maps to a class with the following methods
        // static bool to_uci(::uci::type::...& dest, const ...::types::...& src);
        // static bool from_uci(...::types::...& dest, const ::uci::type::...& src)
        // this allows all the converted methods to be compiled into the library and
        // remove any overload resolution as each template instantiation points to a specifically named
        // class/symbol.

        // sample looks like:
        // class ConvertABC {
        // public:
        //     static bool to_uci(::uci::type::some_uci_type& dest, const api::uci::types::ABC& src);
        //     static bool from_uci(api::uci::types::ABC& dest, const ::uci::type::some_uci_type& src)
        // };
        // template <>
        // class Converter<ABC, some_uci_type>
        // {
        // public:
        //     using type = ConverterABC;
        //     using uci_type = some_uci_type;
        // };

        // NOTE: uci_type is required for nested conversion like vector/containers

        // The assertion the the dtor will only throw at compile time if the using library
        // does not import the proper library, and a suggestion is returned in the assertion
        static_assert(
            sizeof(OurType) == -1 || sizeof(UciType) == -1,
            "Missing converter for typename OurType (see error message details). "
            "Try #include <{{src_path}}/conversions/[OurType].h"
        );
    };

} // namespace {{ns_package}}