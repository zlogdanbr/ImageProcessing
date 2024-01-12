#pragma once
#include <string>
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/ann/ffn.hpp>

using namespace mlpack;
using namespace arma;
using namespace std;



void machine_learnit(std::string& f);