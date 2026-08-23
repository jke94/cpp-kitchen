/**
 * @file main.cpp
 * @brief Input file detector API (using magic numbers) and example usage.
 */

#include <array>
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief File input detector API definition.
 */
namespace fileInputDetector
{
    /**
     * @brief Enum representing supported file formats.
     */
    enum class FileFormat : int
    {
        Unknown = -1,
        PDF = 0,
        PNG = 1,
        JPEG = 2,
        TIFF = 3,
    };

    /**
     * @brief Detects the file format of the given file based on its magic number.
     * @param filePath The path to the file to be checked.
     * @return The detected file format as a FileFormat enum value.
     */
    FileFormat detectFileFormat(const std::string& filePath);

} // namespace fileInputDetector

/**
 * @brief Overloaded output stream operator for FileFormat enum.
 * @param os The output stream.
 * @param format The FileFormat enum value to be printed.
 * @return The output stream with the formatted file format string.
 */
std::ostream& operator<<(std::ostream& os, const fileInputDetector::FileFormat& format);

/**
 * @brief Main function demonstrating the usage of the file input detector API.
 * @param argc The number of command-line arguments.
 */
int main (int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    const std::string filePath = argv[1];

    if(!std::ifstream(filePath))
    {
        std::cerr << "Error: File not found: " << filePath << std::endl;
        return 1;
    }

    const fileInputDetector::FileFormat format = fileInputDetector::detectFileFormat(filePath);

    std::cout << "Detected file (" << filePath << ") format: " << format << std::endl;

    return 0;
}

std::ostream& operator<<(std::ostream& os, const fileInputDetector::FileFormat& format)
{
    switch (format)
    {
        case fileInputDetector::FileFormat::PDF:
            os << "PDF";
            break;
        case fileInputDetector::FileFormat::PNG:
            os << "PNG";
            break;
        case fileInputDetector::FileFormat::JPEG:
            os << "JPEG";
            break;
        case fileInputDetector::FileFormat::TIFF:
            os << "TIFF";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}

namespace fileInputDetector
{
    constexpr std::array<unsigned char, 8> PNG_SIGNATURE = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    };
    constexpr std::array<unsigned char, 3> JPEG_SIGNATURE = {
        0xFF, 0xD8, 0xFF
    };
    constexpr std::array<unsigned char, 4> TIFF_LITTLE_ENDIAN_SIGNATURE = {
        0x49, 0x49, 0x2A, 0x00
    };
    constexpr std::array<unsigned char, 4> TIFF_BIG_ENDIAN_SIGNATURE = {
        0x4D, 0x4D, 0x00, 0x2A
    };
    constexpr std::array<unsigned char, 5> PDF_SIGNATURE = {
        0x25, 0x50, 0x44, 0x46, 0x2D
    };

    FileFormat detectFileFormat(const std::string& filePath)
    {
        FileFormat format = FileFormat::Unknown;

        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile)
        {
            return format;
        }

        std::array<unsigned char, PNG_SIGNATURE.size()> header{};

        inputFile.read(
            reinterpret_cast<char *>(header.data()),
            static_cast<std::streamsize>(header.size())
        );
        
        const std::streamsize bytesRead = inputFile.gcount();

        const auto startsWith = [&, bytesRead](const auto &signature)
        {
            if (bytesRead < static_cast<std::streamsize>(signature.size()))
            {
                return false;
            }

            for (std::size_t index = 0; index < signature.size(); ++index)
            {
                if (header[index] != signature[index])
                {
                    return false;
                }
            }

            return true;
        };

        if (startsWith(PDF_SIGNATURE))
        {
            format = FileFormat::PDF;
        }
        else if (startsWith(PNG_SIGNATURE))
        {
            format = FileFormat::PNG;
        }
        else if (startsWith(JPEG_SIGNATURE))
        {
            format = FileFormat::JPEG;
        }
        else if (startsWith(TIFF_LITTLE_ENDIAN_SIGNATURE) ||
                 startsWith(TIFF_BIG_ENDIAN_SIGNATURE))
        {
            format = FileFormat::TIFF;
        }

        return format;
    }

}; // namespace fileInputDetector
