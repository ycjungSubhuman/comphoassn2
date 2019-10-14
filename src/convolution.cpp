#include "convolution.hpp"
#include "filter.hpp"
#include "util.hpp"
#include <cassert>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

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
                        image_padded.at<cv::Vec3f>(
                            row + width_half_kernel - krow,
                            col + width_half_kernel - kcol);
                }
            }
        }
    }

    return result;
}

SpectralConvolution::SpectralConvolution(cv::Mat kernel)
    : Convolution(std::move(kernel)) {}

cv::Mat SpectralConvolution::apply(const cv::Mat &image) {
    cv::Mat kernel = get_kernel();
    cv::Mat image_spectral, kernel_spectral;
    cv::dft(image, image_spectral);
    cv::Mat kernel_normalized =
        normalize_kernel(kernel, image.rows, image.cols);
    cv::dft(kernel_normalized, kernel_spectral);

    assert(image_spectral.size == kernel_spectral.size);

    cv::Mat result;
    cv::idft(image_spectral * kernel_spectral, result);

    return result;
}
} // namespace compho
