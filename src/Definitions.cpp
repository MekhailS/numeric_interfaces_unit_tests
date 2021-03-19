//
// Created by Mekhail on 05.03.2021.
//

#include "Definitions.h"
#include <cmath>

bool definitions::isValidNum(double num)
{
    return (-infinity() < num and num < infinity() and (not std::isnan(num)));
}

double definitions::nan()
{
    return std::numeric_limits<double>::quiet_NaN();
}

double definitions::infinity()
{
    return std::numeric_limits<double>::infinity();
}