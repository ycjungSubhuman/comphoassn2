#include <boost/program_options.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>

#include "convolution.hpp"
#include "filter.hpp"
#include "image.hpp"

namespace po = boost::program_options;
using namespace compho;

po::variables_map get_options(int argc, char **argv) {
    po::options_description desc("Unsharp masking");
    // clang-format off
    desc.add_options()
        ("help,H", "print this message")
        ("alpha,A", po::value<double>()->default_value(0.5), "high -> very sharp. low -> not so sharp")
        ("sigma_s,S", po::value<double>()->default_value(1.0), "stdev for Gaussian filter")
        ("convolution,C", po::value<std::string>()->default_value("spatial"), "Convolution implementation to use")
        ("g_size,G", po::value<size_t>()->default_value(21), "width,height for Gaussian filter")
        ("input,I", po::value<std::string>(), "Input image path")
        ("output,O", po::value<std::string>(), "Output image path")
        ("kernel_output", po::value<std::string>(), "kernel image output path");
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(1);
    }

    std::vector<std::string> required = {"alpha", "sigma_s",     "g_size",
                                         "input", "convolution", "output"};
    bool pass = true;
    for (std::string &key : required) {
        if (!vm.count(key)) {
            std::cerr << "Required field missing: " << key << std::endl;
            pass = false;
        }
    }

    if (!pass) {
        std::cerr << desc << std::endl;
        exit(2);
    }

    return vm;
}

std::unique_ptr<Convolution> get_convolution(const std::string &key,
                                             const cv::Mat &kernel) {
    if (key == "spatial") {
        return std::make_unique<SpatialConvolution>(kernel);
    } else if (key == "spectral") {
        return std::make_unique<SpectralConvolution>(kernel);
    } else {
        throw std::runtime_error("Unknown convolution key: " + key);
    };
}

int main(int argc, char **argv) {
    po::variables_map vm = get_options(argc, argv);

    cv::Mat image_input =
        to_float_image(cv::imread(vm["input"].as<std::string>()));
    cv::Mat kernel = get_gaussian_kernel(vm["sigma_s"].as<double>(),
                                         vm["g_size"].as<size_t>());
    std::unique_ptr<Convolution> convolution =
        get_convolution(vm["convolution"].as<std::string>(), kernel);
    double alpha = vm["alpha"].as<double>();
    cv::Mat result =
        image_input + alpha * (image_input - convolution->apply(image_input));

    cv::Mat image_output = to_char_image(result);
    cv::imwrite(vm["output"].as<std::string>(), image_output);

    if (vm.count("kernel_output")) {
        cv::imwrite(vm["kernel_output"].as<std::string>(),
                    to_char_image(100 * kernel));
        std::cout << kernel << std::endl;
    }

    return 0;
}
