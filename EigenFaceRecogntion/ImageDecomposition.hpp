#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "Consts.hpp"

class ImageDecomposition {
public:
	explicit ImageDecomposition(const std::string& name = "", const std::vector<double>& coeffs = {});

	~ImageDecomposition() = default;

	[[nodiscard]] const std::string& GetName() const;
	void SetName(const std::string& name);

	[[nodiscard]] const std::vector<double>& GetCoeffs() const;
	void SetCoeffs(const std::vector<double>& coeffs);

	friend std::ostream& operator<<(std::ostream& out, const ImageDecomposition& trainImage);

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ImageDecomposition, name_, coeffs_);

private:
	std::string name_;
	std::vector<double> coeffs_;
};
