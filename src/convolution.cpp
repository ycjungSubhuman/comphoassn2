#include "convolution.hpp"
#include "filter.hpp"
#include "util.hpp"
#include <cassert>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <vector>

namespace compho {

Convolution::Convolution(cv::Mat kernel) : m_kernel(std::move(kernel)) {
    if (m_kernel.cols % 2 == 0) {
        throw std::runtime_error("Kernel size should be an odd number");
    }
}

cv::Mat Convolution::get_kernel() const { return m_kernel; }

cv::Mat Convolution::operator()(const cv::Mat &image) { return apply(image); }

namespace {
cv::Mat pad_image(const cv::Mat &image, int width_border) {
    cv::Mat image_padded =
        cv::Mat::zeros(image.rows + (2 * width_border),
                       image.cols + (2 * width_border), image.type());
    cv::Mat image_padded_roi = image_padded(
        cv::Rect(width_border, width_border, image.cols, image.rows));
    image.copyTo(image_padded_roi);

    return image_padded;
}
} // namespace

SpatialConvolution::SpatialConvolution(cv::Mat kernel)
    : Convolution(std::move(kernel)) {}

cv::Mat SpatialConvolution::apply(const cv::Mat &image) {
    COMPHO_REQUIRE(image.type() == CV_32FC3);

    cv::Mat kernel = get_kernel();
    int width_half_kernel = kernel.rows / 2;
    cv::Mat image_padded = pad_image(image, width_half_kernel);

    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, image.type());
    for (int col = 0; col < image.cols; col++) {
        for (int row = 0; row < image.rows; row++) {
            for (int kcol = 0; kcol < kernel.cols; kcol++) {
                for (int krow = 0; krow < kernel.rows; krow++) {
                    result.at<cv::Vec3f>(row, col) +=
                        kernel.at<float>(krow, kcol) *
                        image_padded.at<cv::Vec3f>(row + kernel.rows - krow,
                                                   col + kernel.cols - kcol);
                }
            }
        }
    }

    return result;
}

SpectralConvolution::SpectralConvolution(cv::Mat kernel)
    : Convolution(std::move(kernel)) {}

cv::Mat SpectralConvolution::apply(const cv::Mat &image) {
    COMPHO_REQUIRE(image.type() == CV_32FC3);

    cv::Mat kernel = get_kernel();

    std::vector<cv::Mat> bgr(3);
    std::vector<cv::Mat> bgr_filtered(3);
    int width_half_kernel = kernel.rows / 2;
    cv::Mat image_padded = pad_image(image, width_half_kernel);
    cv::split(image_padded, bgr.data());

    cv::Size size_dft;
    size_dft.width = cv::getOptimalDFTSize(image_padded.cols);
    size_dft.height = cv::getOptimalDFTSize(image_padded.rows);

    cv::Mat kernel_temp = cv::Mat::zeros(size_dft, kernel.type());

    cv::Mat kernel_spectral;
    cv::Mat kernel_normalized =
        normalize_kernel(kernel, size_dft.height, size_dft.width);
    cv::dft(kernel_normalized, kernel_spectral,
            cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

    for (size_t i = 0; i < 3; i++) {
        cv::Mat image_spectral;
        cv::Mat image_temp = cv::Mat::zeros(size_dft, kernel.type());
        cv::Mat image_roi =
            image_temp(cv::Rect(0, 0, image_padded.cols, image_padded.rows));
        bgr[i].copyTo(image_roi);
        cv::dft(image_temp, image_spectral, cv::DFT_COMPLEX_OUTPUT);

        cv::Mat multiplied;
        cv::mulSpectrums(image_spectral, kernel_spectral, multiplied, 0);
        cv::idft(multiplied, bgr_filtered[i],
                 cv::DFT_COMPLEX_INPUT | cv::DFT_REAL_OUTPUT);
    }

    cv::Mat result;
    cv::merge(bgr_filtered.data(), 3, result);

    return result(cv::Rect(width_half_kernel + 1, width_half_kernel + 1,
                           image.cols, image.rows));
}

BilateralSpatialConvolution::BilateralSpatialConvolution(cv::Mat kernel,
                                                         double sigma)
    : Convolution(std::move(kernel)), m_sigma(sigma) {}

cv::Mat BilateralSpatialConvolution::apply(const cv::Mat &image) {
    COMPHO_REQUIRE(image.type() == CV_32FC3);

    cv::Mat kernel = get_kernel();
    int width_half_kernel = kernel.rows / 2;
    cv::Mat image_padded = pad_image(image, width_half_kernel);

    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, image.type());
    for (int col = 0; col < image.cols; col++) {
        for (int row = 0; row < image.rows; row++) {
            cv::Vec3f center_image = image_padded.at<cv::Vec3f>(
                row + width_half_kernel, col + width_half_kernel);
            double factor_normalize = 0.0;
            for (int kcol = 0; kcol < kernel.cols; kcol++) {
                for (int krow = 0; krow < kernel.rows; krow++) {
                    cv::Vec3f off_image = image_padded.at<cv::Vec3f>(
                        row + kernel.rows - krow, col + kernel.cols - kcol);
                    double r = std::exp(
                        -std::pow(cv::norm(off_image - center_image), 2.0) /
                        (2.0 * std::pow(m_sigma, 2.0)));
                    result.at<cv::Vec3f>(row, col) +=
                        kernel.at<float>(krow, kcol) * r *
                        image_padded.at<cv::Vec3f>(row + kernel.rows - krow,
                                                   col + kernel.cols - kcol);
                    factor_normalize += r * kernel.at<float>(krow, kcol);
                }
            }
            result.at<cv::Vec3f>(row, col) /= factor_normalize;
        }
    }

    return result;
}
} // namespace compho
