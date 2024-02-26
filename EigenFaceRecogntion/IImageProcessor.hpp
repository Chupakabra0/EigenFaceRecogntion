#include <opencv2/opencv.hpp>

struct IImageProcessor {
public:
	virtual ~IImageProcessor() = default;
	virtual [[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) = 0;
};
