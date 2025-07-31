#include "common/argparse.hpp"

int main(int argc, char* argv[]) {
    // 创建 ArgumentParser 对象
    argparse::ArgumentParser program("my_program");

    // 添加参数
    program.add_argument("--input", "-i")
        .help("Input file path")
        .required();

    program.add_argument("--output", "-o")
        .help("Output file path")
        .default_value("output.txt");

    program.add_argument("--verbose", "-v")
        .help("Enable verbose mode")
        .default_value(false)
        .implicit_value(true);

    // 解析命令行参数
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    // 获取参数值
    auto input_file = program.get<std::string>("--input");
    auto output_file = program.get<std::string>("--output");
    bool verbose = program.get<bool>("--verbose");

    std::cout << "Input: " << input_file << std::endl;
    std::cout << "Output: " << output_file << std::endl;
    std::cout << "Verbose: " << verbose << std::endl;

    return 0;
}