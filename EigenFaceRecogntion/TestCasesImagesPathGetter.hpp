#pragma once
#include "IImagesPathGetter.hpp"

#include <fmt/format.h>

struct TestCasesImagesPathGetter : public IImagesPathGetter {
public:
	explicit TestCasesImagesPathGetter() = delete;

	explicit TestCasesImagesPathGetter(int count)
		: count_(count) {
		for (auto i = 0; i < this->count_; ++i) {
			this->paths_.insert(fmt::format("TestImages\\{}.jpg", i + 1));
		}
	}

	~TestCasesImagesPathGetter() override = default;

	[[nodiscard]] std::set<std::string> GetFilePaths() const override {
		return this->paths_;
	}

private:
	std::set<std::string> paths_{};
	int count_;
};
