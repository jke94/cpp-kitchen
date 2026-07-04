#include <cstddef>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace apiProcessor
{
    using Handler = void*;

    struct Bytes
    {
        const std::byte* data;
        std::size_t size;
    };

    enum class DataType
    {
        Unknown = 0,
        Int,
        String,
        ByteArray
    };

    struct Value
    {
        DataType type;

        union
        {
            int intValue;
            const char* stringValue;
            Bytes bytesValue;
        };
    };

    extern "C"
    {
        void createHandler(Handler& handler);

        void process(Handler handler, const Value& value);

        void freeHandler(Handler& handler);
    }
}

using namespace apiProcessor;

namespace
{
    using InternalValue =
        std::variant<
            int,
            std::string,
            std::vector<std::byte>>;

    InternalValue toInternal(
        const Value& value
    )
    {
        switch (value.type)
        {
        case DataType::Int:
            return value.intValue;

        case DataType::String:
            return std::string(
                value.stringValue != nullptr ? value.stringValue: "");

        case DataType::ByteArray:
            return std::vector<std::byte>(
                value.bytesValue.data,
                value.bytesValue.data +
                value.bytesValue.size);

        default:
            throw std::runtime_error("Unknown data type");
        }
    }
}

namespace apiProcessor
{
    void createHandler(Handler& handler)
    {
        handler = new int(1994);

        std::cout << "Created handler" << handler << std::endl;
    }

    void freeHandler(Handler& handler)
    {
        std::cout
            << "Freeing handler: "
            << handler
            << std::endl;

        delete static_cast<int*>(handler);

        handler = nullptr;
    }

    void process(Handler handler, const Value& value)
    {
        InternalValue internal =
            toInternal(value);

        std::visit(
            [handler](const auto& item)
            {
                using T =
                    std::decay_t<decltype(item)>;

                if constexpr (
                    std::is_same_v<T, int>)
                {
                    std::cout
                        << "Processing (Handler: "
                        << handler
                        << ") int: "
                        << item
                        << std::endl;
                }
                else if constexpr (
                    std::is_same_v<T, std::string>)
                {
                    std::cout
                        << "Processing (Handler: "
                        << handler
                        << ") string: "
                        << item
                        << std::endl;
                }
                else if constexpr (
                    std::is_same_v<
                        T,
                        std::vector<std::byte>>)
                {
                    std::cout
                        << "Processing (Handler: "
                        << handler
                        << ") array item: ";

                    for (auto byte : item)
                    {
                        std::cout
                            << '\''
                            << static_cast<char>(byte)
                            << '\'';
                    }

                    std::cout
                        << std::endl;
                }
            },
            internal);
    }
}

int main()
{
    Handler handler{};

    createHandler(handler);
 
    Value value{};

    // Send an integer to the API

    value.type = DataType::Int;
    value.intValue = 42;
    process(handler, value);

    // Send a string to the API

    value.type = DataType::String;
    value.stringValue = "Hello, C++17!";
    process(handler, value);

    // Send a byte array to the API

    const std::byte bytes[] =
    {
        std::byte{'H'},
        std::byte{'e'},
        std::byte{'l'},
        std::byte{'l'},
        std::byte{'o'},
        std::byte{'!'}
    };
    value.type = DataType::ByteArray;
    value.bytesValue.data = bytes;
    value.bytesValue.size = sizeof(bytes);
    process(handler, value);

    // Free the handler

    freeHandler(handler);

    return 0;
}
