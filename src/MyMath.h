//
// Created by Mekhail on 12.03.2021.
//

#pragma once
#include "Definitions.h"
#include "IVector.h"

namespace myMath
{
    IVector *combineVectors(const IVector *const &op1, const IVector *const &op2,
                            const std::function<double(double, double)>& funCombine);

    IVector *createZeroVector(size_t dim);

    size_t nextPowerOf2(size_t num);
}
