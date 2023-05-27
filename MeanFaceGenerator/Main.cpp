#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <fmt/format.h>

#include "Consts.hpp"

int main(int argc, char** argv) {
	const std::vector<std::string> names = {
		"Alisson", "Benzema", "Cancelo",
		"Casemiro", "Courtois", "DeBruyne",
		"Donnarumma", "Ederson", "Kane",
		"Kante", "Kimmich", "Lewandowski",
		"Mane", "Mbappe", "Messi",
		"Modirc", "Navas", "Neuer",
		"Neymar", "Oblak", "Ronaldo",
		"Salah", "Son", "VanDijk",
	};

	for (const auto& n : names) {
		const std::vector<std::string> paths = {
			fmt::format("TestImages\\{}\\1.jpg", n),
			fmt::format("TestImages\\{}\\2.jpg", n),
			fmt::format("TestImages\\{}\\3.jpg", n),
			fmt::format("TestImages\\{}\\4.jpg", n),
			//fmt::format("TestImages\\{}\\5.jpg", n),
		};
		const std::string save = fmt::format("TestImages\\{}\\mean.jpg", n);

		cv::Mat mean(BASIS_IMAGE_HEIGHT, BASIS_IMAGE_WIDTH, CV_64FC1, 0.0);
		for (const auto& p : paths) {
			auto basicImage = cv::imread(p, cv::IMREAD_UNCHANGED);

			cv::resize(basicImage, basicImage, cv::Size(BASIS_IMAGE_WIDTH, BASIS_IMAGE_HEIGHT));
			cv::cvtColor(basicImage, basicImage, cv::COLOR_BGR2GRAY);

			mean += basicImage;
		}
		mean /= paths.size();

		mean.convertTo(mean, CV_8UC1);
		cv::imwrite(save, mean);
	}

	return EXIT_SUCCESS;
}
