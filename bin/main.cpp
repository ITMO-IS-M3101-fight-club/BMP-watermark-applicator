#include <charconv>
#include <cstring>
#include <iostream>

#include "ImageBMP.h"
#include "Watermark.h"

namespace {
const double kDefaultTransparencyCoef = 0.4;
struct ProgramOptions {
    char* original_file_path = nullptr;
    char* watermark_file_path = nullptr;
    char* result_file_path = nullptr;

    double transparency_coef = kDefaultTransparencyCoef;
};

double ParseDoubleFromChars(const char* str) {
    double res{};
    auto [ptr, ec] = std::from_chars(str, str + std::strlen(str), res);

    if (ec == std::errc()) {
        return res;
    } else if (ec == std::errc::invalid_argument) {
        std::cerr << "Error occurred while parsing double: provided value is not a digit\n";
    } else if (ec == std::errc::result_out_of_range) {
        std::cout << "Error occured while parsing double: provided value is out of double\n";
    }
    return kDefaultTransparencyCoef;
}

bool AppendOption(
    ProgramOptions& program_options,
    char* option,
    char* provided_value) {
    if (strcmp(option, "--original") == 0 || strcmp(option, "-o") == 0) {
        program_options.original_file_path = provided_value;
        return true;
    }

    if (strcmp(option, "--watermark") == 0 || strcmp(option, "-w") == 0) {
        program_options.watermark_file_path = provided_value;
        return true;
    }

    if (strcmp(option, "--result") == 0 || strcmp(option, "-r") == 0) {
        program_options.result_file_path = provided_value;
        return true;
    }

    if (strcmp(option, "--transparency") == 0 || strcmp(option, "-t") == 0) {
        program_options.transparency_coef = ParseDoubleFromChars(provided_value);
        return true;
    }

    std::cerr << "Uknown option: " << option << '\n';
    return false;
}

bool AppendOptions(int argc, char** argv, ProgramOptions& options) {
    for (int i = 1; i < argc; i += 2) {
        char* input = argv[i];
        if (input[0] == '\0') {
            std::cerr << "Incorrect input (empty)\n";
            return false;
        }
        if (input[0] == '-') {
            if (input[1] == '\0') {
                std::cerr << "Incorrect option\n";
                return false;
            }

            if (i + 1 >= argc) {
                std::cerr << "The option value is required\n";
                return false;
            }
            char* value = argv[i + 1];
            if (!AppendOption(options, input, value)) {
                return false;
            }
        }
    }
    return true;
}

bool CheckAllOptionsSet(const ProgramOptions& options) {
    if (nullptr == options.original_file_path || strcmp(options.original_file_path, "") == 0) {
        std::cerr << "Original file path should be provided\n";
        return false;
    }

    if (nullptr == options.watermark_file_path || strcmp(options.watermark_file_path, "") == 0) {
        std::cerr << "Watermark file path should be provided\n";
        return false;
    }

    if (nullptr == options.result_file_path || strcmp(options.result_file_path, "") == 0) {
        std::cerr << "Result file path should be provided\n";
        return false;
    }

    return true;
}

}  // namespace

int main(int argc, char** argv) {
    ProgramOptions program_options;

    if (!AppendOptions(argc, argv, program_options)) {
        std::cerr << "Wrong options\n";
        return 1;
    }

    if (!CheckAllOptionsSet(program_options)) {
        std::cerr << "Not all options are set\n";
        return 1;
    }

    const std::string original_file = program_options.original_file_path;
    const std::string watermark_file = program_options.watermark_file_path;
    const std::string result_file = program_options.result_file_path;

    ImageBMP image;
    ImageBMP watermark;

    if (!image.load(original_file)) {
        std::cerr << "Error: couldn't load original image\n";
        return 1;
    }

    if (!watermark.load(watermark_file)) {
        std::cerr << "Error: couldn't load watermark image\n";
        return 1;
    }

    if (!applyWatermark(image, watermark, program_options.transparency_coef)) {
        std::cerr << "Error: failed to apply watermark.\n";
        std::cerr << "Check image dimensions and alpha value.\n";
        return 1;
    }

    if (!image.save(result_file)) {
        std::cerr << "Error: cannot save result image.\n";
        return 1;
    }
    return 0;
}