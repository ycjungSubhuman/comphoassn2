#include <opencv2/core.hpp>

namespace compho {

/**
 * convert (w/2,h/2)-centerd kernel to zero-centered filter with proper
 * zero paddings
 */
cv::Mat normalize_kernel(const cv::Mat &kernel, int rows, int cols);

/**
 * sigma 1.0 -> 1.0 of size
 *
 * Returns cv::Mat of CV_32FC1
 */
cv::Mat get_gaussian_kernel(double sigma, size_t size);

} // namespace compho
