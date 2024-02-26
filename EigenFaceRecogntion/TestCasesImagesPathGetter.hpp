#pragma once
#include "IImagesPathGetter.hpp"

struct TestCasesImagesPathGetter : public IImagesPathGetter {
public:
	explicit TestCasesImagesPathGetter() = delete;
	explicit TestCasesImagesPathGetter(int count);

	~TestCasesImagesPathGetter() override = default;

	[[nodiscard]] std::set<std::string> GetFilePaths() const override;

private:
	std::set<std::string> paths_{};
	int count_;
};
