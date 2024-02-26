#pragma once
#include <vector>
#include <random>
#include <opencv2/opencv.hpp>

#include "Consts.hpp"

template<int Size>
std::vector<double> ConvertVecToVector(const cv::Vec<double, Size>& vec) {
    std::vector<double> result(Size);

    for (int i = 0; i < Size; i++) {
        result[i] = vec[i];
    }

    return result;
}

template<int Size>
cv::Vec<double, Size> ConvertVectorToVec(const std::vector<double>& vec) {
    return cv::Vec<double, Size>(vec.data());
}

inline std::vector<std::vector<double>> ConvertMatToVectors(const cv::Mat& mat) {
    if (mat.type() != CV_64FC1) {
        throw std::runtime_error("Input matrix should be of type CV_64F (double)");
    }

    int numRows = mat.rows;
    int numCols = mat.cols;

    std::vector<std::vector<double>> result(numRows, std::vector<double>(numCols));

    for (int i = 0; i < numRows; i++) {
        const auto* rowPtr = mat.ptr<double>(i);

        for (int j = 0; j < numCols; j++) {
            result[i][j] = rowPtr[j];
        }
    }

    return result;
}

inline cv::Mat ConvertVectorsToMat(const std::vector<std::vector<double>>& vec) {
    const int numRows = static_cast<int>(vec.size());
    const int numCols = (numRows > 0) ? static_cast<int>(vec[0].size()) : 0;

    cv::Mat mat(numRows, numCols, CV_64FC1);

    for (int i = 0; i < numRows; i++) {
        const std::vector<double>& row = vec[i];

        auto* rowPtr = mat.ptr<double>(i);
        for (int j = 0; j < numCols; j++) {
            rowPtr[j] = row[j];
        }
    }

    return mat;
}

inline std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

inline int GenerateRandomInt(int min, int max) {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}
