#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "Consts.hpp"

class ImageDecomposition {
public:
	explicit ImageDecomposition(const std::string& name = "", const std::vector<double>& coeffs = {})
		: name_(name), coeffs_(coeffs) {

	}

	~ImageDecomposition() = default;

	[[nodiscard]] const std::string& GetName() const {
		return this->name_;
	}

	void SetName(const std::string& name) {
		this->name_ = name;
	}

	[[nodiscard]] const std::vector<double>& GetCoeffs() const {
		return this->coeffs_;
	}

	void SetCoeffs(const std::vector<double>& coeffs) {
		this->coeffs_ = coeffs;
	}

	friend std::ostream& operator<<(std::ostream& out, const ImageDecomposition& trainImage) {
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

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ImageDecomposition, name_, coeffs_);

private:
	std::string name_;
	std::vector<double> coeffs_;
};
