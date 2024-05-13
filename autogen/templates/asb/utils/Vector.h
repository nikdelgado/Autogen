#include <vector>

#include <uci/base/BoundedList.h>

#include "{{path_package}}/utils/PopulateMutex.h"
#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    namespace detail
    {
        template<typename ConverterFn, typename ContainerType1, typename ContainerType2>
        inline bool Populate(ConverterFn fn, ContainerType1& dest, const ContainerType2& src)
        {
            const auto lock = std::scoped_lock{utils::populateMutex};
            dest.clear();
            dest.resize(src.size());

            bool success = true;
            for(typename ContainerType2::size_type i = 0; i < src.size(); ++i)
            {
                success &= fn(dest[i], src[i]);
            }

            return success;
        }

        template <typename ConverterFn, typename AccessorFn, typename ContainerType1, typename ContainerType2>
        inline bool PopulateAbstract(ConverterFn fn, AccessorFn fn2, ContainerType1& dest, const ContainerType2& src)
        {
                const auto lock = std::scoped_lock{utils::populateMutex};
                dest.clear();
                for(typename ContainerType2::size_type i = 0; i < src.size(); ++i)
                {
                        // UCI Bounded list needs to know what types new data are
                        dest.resize(i + 1, fn2(src[i]));
                }
                bool success = true;
                for(typename ContainerType2::size_type i = 0; i < src.size(); ++i)
                {
                        success &= fn(dest[i], src[i]);
                }
                return success;
        }
    } // namespace  detail

    template<typename OurType, typename UciType, typename UciBoundedList>
    class ConvertVector
    {
    public:
        static bool to_uci(UciBoundedList& dest, const std::vector<OurType>& src)
        {
            return detail::Populate(
                Converter<OurType, UciType>::type::to_uci,
                dest,
                src
            );
        }
        static bool from_uci(std::vector<OurType>& dest, const UciBoundedList& src)
        {
            return detail::Populate(
                Converter<OurType, UciType>::type::from_uci,
                dest,
                src
            );
        }
    };

    template <typename OurType, typename UciType, typename UciBoundedList>
    class ConvertVectorAbstract
    {
    public:
            static bool to_uci(UciBoundedList& dest, const std::vector<std::shared_ptr<OurType>>& src)
            {
                    return detail::PopulateAbstract(Converter<std::shared_ptr<OurType>, UciType>::type::to_uci,
                                                                                    Converter<std::shared_ptr<OurType>, UciType>::type::uci_accessor_for, dest, src);
            }
            static bool from_uci(std::vector<std::shared_ptr<OurType>>& dest, const UciBoundedList& src)
            {
                    return detail::Populate(Converter<std::shared_ptr<OurType>, UciType>::type::from_uci, dest, src);
            }
    };
    template <typename OurType, typename UciType, ::uci::base::accessorType::AccessorType UCIAccessorType>
    class Converter<std::vector<std::shared_ptr<OurType>>, uci::base::BoundedList<UciType, UCIAccessorType>>
    {
    public:
            using uci_type = ::uci::base::BoundedList<UciType, UCIAccessorType>;
            using type = ConvertVectorAbstract<OurType, UciType, uci_type>;
    };

    template<typename OurType, typename UciType, ::uci::base::accessorType::AccessorType UCIAccessorType>
    class Converter<std::vector<OurType>, ::uci::base::BoundedList<UciType, UCIAccessorType>>
    {
    public:
        using uci_type = ::uci::base::BoundedList<UciType, UCIAccessorType>;
        using type = ConvertVector<OurType, UciType, uci_type>;
    };
} // namespace {{ns_package}}::conversions