//
// Created by Mekhail on 04.03.2021.
//
#pragma once

#include <limits>
#include <cstdint>
#include <math.h>

namespace definitions
{
    using byte = uint8_t;
    double infinity();

    double nan();

    bool isValidNum(double num);
}
