//#pragma once
//#include <nlohmann/json.hpp>
//
//#include "TrainImage.hpp"
//
//void to_json(nlohmann::json& j, const TrainImage& trainImage) {
//    j = {
//        { "name", trainImage.GetName() },
//        { "coeffs", trainImage.GetCoeffs() }
//    };
//}
//
//void from_json(const nlohmann::json& j, TrainImage& trainImage) {
//    j.at("name").get_to(trainImage.GetName());
//    j.at("coeffs").get_to(trainImage.GetCoeffs());
//}
