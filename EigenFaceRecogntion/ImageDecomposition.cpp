#include "ImageDecomposition.hpp"

ImageDecomposition::ImageDecomposition(const std::string& name, const std::vector<double>& coeffs)
	: name_(name), coeffs_(coeffs) {

}

[[nodiscard]] const std::string& ImageDecomposition::GetName() const {
	return this->name_;
}

void ImageDecomposition::SetName(const std::string& name) {
	this->name_ = name;
}

[[nodiscard]] const std::vector<double>& ImageDecomposition::GetCoeffs() const {
	return this->coeffs_;
}

void ImageDecomposition::SetCoeffs(const std::vector<double>& coeffs) {
	this->coeffs_ = coeffs;
}

std::ostream& operator<<(std::ostream& out, const ImageDecomposition& trainImage) {
	const auto& coeffs = trainImage.coeffs_;

	out << trainImage.name_ << std::endl;

	if (coeffs.empty()) {
		return out << "[]" << std::endl;
	}

	out << "[" << coeffs.front();
	for (auto iter = std::next(coeffs.begin()); iter != coeffs.end(); iter = std::next(iter)) {
		out << ", " << *iter;
	}

	return out << "]" << std::endl;
}
