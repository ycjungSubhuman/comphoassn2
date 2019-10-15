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
    po::options_description desc("Bilateral Gaussian filtering");
    // clang-format off
    desc.add_options()
        ("help,H", "print this message")
        ("sigma_s,S", po::value<double>()->default_value(1.0), "stdev for Gaussian filter")
        ("sigma_r,S", po::value<double>()->default_value(1.0), "stdev for bilateral filter")
        ("g_size,G", po::value<size_t>()->default_value(21), "width,height for Gaussian filter")
        ("input,I", po::value<std::string>(), "Input image path")
        ("output,O", po::value<std::string>(), "Output image path");
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(1);
    }

    std::vector<std::string> required = {"sigma_s", "sigma_r", "g_size",
                                         "input", "output"};
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

int main(int argc, char **argv) {
    po::variables_map vm = get_options(argc, argv);

    cv::Mat image_input =
        to_float_image(cv::imread(vm["input"].as<std::string>()));
    cv::Mat kernel = get_gaussian_kernel(vm["sigma_s"].as<double>(),
                                         vm["g_size"].as<size_t>());
    BilateralSpatialConvolution convolution(kernel, vm["sigma_r"].as<double>());
    cv::Mat result = convolution(image_input);

    cv::Mat image_output = to_char_image(result);
    cv::imwrite(vm["output"].as<std::string>(), image_output);

    return 0;
}
