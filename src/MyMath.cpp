//
// Created by Mekhail on 12.03.2021.
//

#include "MyMath.h"


IVector * myMath::combineVectors(const IVector *const &op1, const IVector *const &op2,
                                 const std::function<double(double, double)> &funCombine)
{
    if (op1 == nullptr or op2 == nullptr)
        return nullptr;

    if (op1->getDim() != op2->getDim())
        return nullptr;

    IVector* res = op1->clone();
    if (res == nullptr)
        return nullptr;

    for (int i = 0; i < res->getDim(); ++i)
    {
        double val1;
        double val2;
        op1->getCord(i, val1);
        op2->getCord(i, val2);

        double valRes = funCombine(val1, val2);
        if (not definitions::isValidNum(valRes))
        {
            delete res;
            return nullptr;
        }
        res->setCord(i, valRes);
    }
    return res;
}

IVector * myMath::createZeroVector(size_t dim)
{
    double* data = new double[dim]();
    auto* zeroVec = IVector::createVector(dim, data);

    delete[] data;
    return zeroVec;
}

size_t myMath::nextPowerOf2(size_t num)
{
    unsigned count = 0;

    if (num and not (num & (num - 1)))
        return num;

    while( num != 0)
    {
        num >>= 1;
        count += 1;
    }

    return 1 << count;
}
