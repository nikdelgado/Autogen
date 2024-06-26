#pragma once
#include <cstddef>
#include <array>
#include <mutex>
#include <vector>
#include "{{path_package}}/utils/PopulateMutex.h"
#include "{{path_package}}/conversions/Converter.h"

namespace {{ns_package}}::conversions
{
    namespace detail
    {
        template<typename T>
        inline bool MaybeResize(google::protobuf::RepeatedPtrField<T>& container, typename google::protobuf::RepeatedPtrField<T>::size_type size)
        {
            container.Clear();
            
            while (container.size() < size) {
                container.Add();
            }
            return true;
        }

        template<typename ContainerType>
        inline bool MaybeResize(ContainerType& container, typename ContainerType::size_type size)
        {
            container.clear();
            container.resize(size);
            return true;
        }
        // Handling std::array which cannot be resized.
        template<typename T, std::size_t N>
        constexpr bool MaybeResize(std::array<T, N>&, std::size_t size) {
            return N == size; // This is a compile-time check, arrays can't be resized.
        }

        template<typename ConverterFn, typename ContainerType1, typename ContainerType2>
        inline bool Populate(ConverterFn fn, ContainerType1& dest, const ContainerType2& src)
        {
            const auto lock = std::scoped_lock{utils::populateMutex};

            if (!MaybeResize(dest, src.size())) {
                return false;
            }

            bool success = true;
            for(typename ContainerType2::size_type i = 0; i < src.size(); ++i)
            {
                if (!fn(dest[i], src[i])) {
                    success = false; // If any conversion fails, record it but continue processing.
                }
            }

            return success;
        }
    } // namespace detail

    template<typename AbbType, typename ProtobufType, typename ProtobufRepeatedField>
    class ConvertProtoVector
    {
    public:
        static bool to_protobuf(ProtobufRepeatedField& dest, const std::vector<AbbType>& src)
        {
            return detail::Populate(
                Converter<AbbType, ProtobufType>::converter_factory::to_protobuf,
                dest,
                src
            );
        }
        static bool from_protobuf(std::vector<AbbType>& dest, const ProtobufRepeatedField& src)
        {
            return detail::Populate(
                Converter<AbbType, ProtobufType>::converter_factory::from_protobuf,
                dest,
                src
            );
        }
    };
    
    template<typename AbbType, typename ProtobufType>
    class ConvertVectorVector
    {
    public:
        static bool to_protobuf(std::vector<ProtobufType>& dest, const std::vector<AbbType>& src)
        {
            return detail::Populate(
                Converter<AbbType, ProtobufType>::converter_factory::to_protobuf,
                dest,
                src
            );
        }

        static bool from_protobuf(std::vector<AbbType>& dest, const std::vector<ProtobufType>& src)
        {
            return detail::Populate(
                Converter<AbbType, ProtobufType>::converter_factory::from_protobuf,
                dest,
                src
            );
        }
    };

    template<typename AbbType, typename ProtobufType, typename ProtobufRepeatedField, std::size_t N>
    class ConvertArray
    {
    public:
        static bool to_protobuf(ProtobufRepeatedField& dest, const std::array<AbbType, N>& src)
        {
            return detail::Populate(
                Converter<AbbType, ProtobufType>::converter_factory::to_protobuf,
                dest,
                src
            );
        }

        static bool from_protobuf(std::array<AbbType, N>& dest, const ProtobufRepeatedField& src)
        {
            return detail::Populate(
                Converter<AbbType, ProtobufType>::converter_factory::from_protobuf,
                dest,
                src
            );
        }
    };

    template <typename AbbType, typename ProtobufType>
    class Converter<std::vector<AbbType>, google::protobuf::RepeatedPtrField<ProtobufType>> {
        public:
            using ProtobufRepeatedField = google::protobuf::RepeatedPtrField<ProtobufType>;
            using converter_factory = ConvertProtoVector<AbbType, ProtobufType, ProtobufRepeatedField>;
    };

    template <typename AbbType, typename ProtobufType, std::size_t N>
    class Converter<std::array<AbbType, N>, google::protobuf::RepeatedPtrField<ProtobufType>> {
        public:
            using ProtobufRepeatedField = google::protobuf::RepeatedPtrField<ProtobufType>;
            using converter_factory = ConvertArray<AbbType, ProtobufType, ProtobufRepeatedField, N>;          
    };

    template <typename AbbType, typename ProtobufType>
    class Converter<std::vector<AbbType>, std::vector<ProtobufType>>
    {
        public:
            using converter_factory = ConvertVectorVector<AbbType, ProtobufType>;
    };

    template <>
    class Converter<std::vector<double>, std::vector<double>>
    {
        public:
            using converter_factory = ConvertVectorVector<double, double>;
    };
    
} // namespace {{ns_package}}::conversions