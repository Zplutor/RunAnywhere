#pragma once

#include <cstdint>
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace ra::mod::calculator {

class EvaluateResult {
public:
    boost::multiprecision::cpp_dec_float_100 decimal_value;
};

}