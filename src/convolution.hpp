#include <opencv2/core.hpp>

namespace compho {

class Convolution {
  public:
    Convolution(cv::Mat kernel);

    cv::Mat get_kernel() const;

    virtual cv::Mat apply(const cv::Mat &image) = 0;
    cv::Mat operator()(const cv::Mat &image);

  private:
    cv::Mat m_kernel;
};

class SpatialConvolution : public Convolution {
  public:
    SpatialConvolution(cv::Mat kernel);
    cv::Mat apply(const cv::Mat &image) override;
};

class SpectralConvolution : public Convolution {
  public:
    SpectralConvolution(cv::Mat kernel);
    cv::Mat apply(const cv::Mat &image) override;
};
} // namespace compho

