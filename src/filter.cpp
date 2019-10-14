#include "filter.hpp"
#include "util.hpp"
#include <cassert>
#include <cmath>

namespace compho {
cv::Mat normalize_kernel(const cv::Mat &kernel, int rows, int cols) {
    COMPHO_REQUIRE(kernel.cols == kernel.rows);

    cv::Mat kernel_spectral = cv::Mat::zeros(rows, cols, kernel.type());
    int width_half_kernel = kernel.cols / 2;
    int width_rest_kernel = kernel.cols - width_half_kernel;

    {
        cv::Mat target_top_left = kernel_spectral(
            cv::Rect(0, 0, width_rest_kernel, width_rest_kernel));
        cv::Mat source_top_left =
            kernel(cv::Rect(width_half_kernel, width_half_kernel,
                            width_rest_kernel, width_rest_kernel));
        source_top_left.copyTo(target_top_left);
    }

    {
        cv::Mat target_top_right = kernel_spectral(
            cv::Rect(cols - width_rest_kernel, rows - width_rest_kernel,
                     width_rest_kernel, width_rest_kernel));
        cv::Mat source_top_right = kernel(cv::Rect(
            0, width_half_kernel, width_rest_kernel, width_rest_kernel));
        source_top_right.copyTo(target_top_right);
    }

    {
        cv::Mat target_bottom_left = kernel_spectral(cv::Rect(
            0, rows - width_rest_kernel, width_rest_kernel, width_rest_kernel));
        cv::Mat source_bottom_left = kernel(cv::Rect(
            width_half_kernel, 0, width_rest_kernel, width_rest_kernel));
        source_bottom_left.copyTo(target_bottom_left);
    }

    {
        cv::Mat target_bottom_right = kernel_spectral(cv::Rect(
            cols - width_rest_kernel, 0, width_rest_kernel, width_rest_kernel));
        cv::Mat source_bottom_right =
            kernel(cv::Rect(0, 0, width_rest_kernel, width_rest_kernel));
        source_bottom_right.copyTo(target_bottom_right);
    }

    assert(kernel.rows == kernel_spectral.rows &&
           kernel.cols == kernel_spectral.cols);
    return kernel_spectral;
}

cv::Mat get_gaussian_kernel(double sigma, size_t size) {
    cv::Mat result = cv::Mat::zeros(size, size, CV_32FC1);

    for (int col = 0; col < int(size); col++) {
        for (int row = 0; row < int(size); row++) {
            int x = 2.0 * col / size - 1.0;
            int y = 2.0 * row / size - 1.0;
            double nx = std::pow(double(x), 2.0) / (2.0 * std::pow(sigma, 2.0));
            double ny = std::pow(double(y), 2.0) / (2.0 * std::pow(sigma, 2.0));
            double v = std::exp(-(nx + ny));

            result.at<float>(col, row) = float(v);
        }
    }
    return result / cv::sum(result);
}
} // namespace compho
