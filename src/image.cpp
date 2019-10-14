#include "image.hpp"

namespace compho {
cv::Mat to_float_image(const cv::Mat &image) {
    cv::Mat result;
    image.convertTo(result, CV_32FC3, 1.0 / 255.0);

    return result;
}

cv::Mat to_char_image(const cv::Mat &image) {
    cv::Mat result;
    image.convertTo(result, CV_8UC1, 255.0);

    return result;
}
} // namespace compho
