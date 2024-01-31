#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <fmt/format.h>

#include "Consts.hpp"

int main(int argc, char** argv) {
	//const std::vector<std::string> names = {
	//	"Alisson", "Benzema", "Cancelo",
	//	"Casemiro", "Courtois", "DeBruyne",
	//	"Donnarumma", "Ederson", "Kane",
	//	"Kante", "Kimmich", "Lewandowski",
	//	"Mane", "Mbappe", "Messi",
	//	"Modirc", "Navas", "Neuer",
	//	"Neymar", "Oblak", "Ronaldo",
	//	"Salah", "Son", "VanDijk",
	//};

	//for (const auto& n : names) {
	//	const std::vector<std::string> paths = {
	//		fmt::format("TestImages\\{}\\1.jpg", n),
	//		fmt::format("TestImages\\{}\\2.jpg", n),
	//		fmt::format("TestImages\\{}\\3.jpg", n),
	//		fmt::format("TestImages\\{}\\4.jpg", n),
	//		//fmt::format("TestImages\\{}\\5.jpg", n),
	//	};
	//	const std::string save = fmt::format("TestImages\\{}\\mean.jpg", n);

	//	cv::Mat mean(BASIS_IMAGE_HEIGHT, BASIS_IMAGE_WIDTH, CV_64FC1, 0.0);
	//	for (const auto& p : paths) {
	//		auto basicImage = cv::imread(p, cv::IMREAD_UNCHANGED);

	//		cv::resize(basicImage, basicImage, cv::Size(BASIS_IMAGE_WIDTH, BASIS_IMAGE_HEIGHT));
	//		cv::cvtColor(basicImage, basicImage, cv::COLOR_BGR2GRAY);

	//		mean += basicImage;
	//	}
	//	mean /= paths.size();

	//	mean.convertTo(mean, CV_8UC1);
	//	cv::imwrite(save, mean);
	//}

	cv::Mat a = (
		cv::Mat_<double>(5, 10) << 1.0, 2.0, 3.0, 4.0, 5.0, 11.0, 2.0, 3.0, 5.0, 1.0,
								   1.0, 6.0, 4.0, 4.0, 2.0, 1.0, 2.0, 3.0, 12.0, 2.0,
								   2.0, 3.0, 2.0, 3.0, 2.0, 3.0, 7.0, 7.0, 7.0, 1.0,
								   9.0, 8.0, 6.0, 1.0, 1.0, 1.0, 2.0, 3.0, 4.0, 9.0,
								   3.0, 2.0, 6.0, 9.0, 11.0, 1.0, 11.0, 3.0, 4.0, 8.0
	);
	//int k = 0;
	//for (int i = 0; i < a.rows; ++i) {
	//	for (int j = 0; j < a.cols; ++j) {
	//		a.at<double>(i, j) = (double)k;
	//		if (j % 3 == 0) {
	//			++k;
	//		}
	//	}
	//}

	auto print = [](const cv::Mat& mat) {
		for (int i = 0; i < mat.rows; ++i) {
			for (int j = 0; j < mat.cols; ++j) {
				std::cout << mat.at<double>(i, j) << ' ';
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	};

	std::cout << "A = \n";
	print(a);

	cv::Mat a_t(a.t());
	std::cout << "A^T = \n";
	print(a_t);

	cv::Mat aa_t(a * a.t());
	std::cout << "A * A^T = \n";
	print(aa_t);

	cv::Mat a_ta(a.t() * a);
	std::cout << "A^T * A = \n";
	print(a_ta);

	cv::Mat eigenValues, eigenVectors;

	cv::eigen(aa_t, eigenValues, eigenVectors);
	std::cout << "Eigen(A * A^T) = \n";
	print(eigenValues);
	print(eigenVectors);

	cv::eigen(a_ta, eigenValues, eigenVectors);
	std::cout << "Eigen(A^T * A) = \n";
	print(eigenValues);
	print(eigenVectors);

	//
	cv::eigen(aa_t, eigenValues, eigenVectors);
	eigenVectors = eigenVectors * a;

	for (int i = 0; i < eigenVectors.rows; ++i) {
		const double norm = cv::norm(eigenVectors.row(i));
		for (int j = 0; j < eigenVectors.cols; ++j) {
			eigenVectors.at<double>(i, j) /= norm;
		}
	}

	std::cout << "New eigen = \n";
	print(eigenValues);
	print(eigenVectors);

	return EXIT_SUCCESS;
}
