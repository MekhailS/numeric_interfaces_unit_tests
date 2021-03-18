//
// Created by Mekhail on 12.03.2021.
//

#include "gtest/gtest.h"

#include "../include/IVector.h"
#include <math.h>

class CreateVector : public ::testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(CreateVector, SimpleVectorDataFromStack_SuccCreate)
{
    double simpleVectorData[] = {1, 2, 3, 4, 5};
    auto vec = IVector::createVector(5, simpleVectorData);
    ASSERT_NE(vec, nullptr);
    ASSERT_EQ(5, vec->getDim());
}

TEST_F(CreateVector, TryCreateFromNullptr_FailCreate)
{
    auto vec = IVector::createVector(1, nullptr);
    ASSERT_EQ(vec, nullptr);
}

TEST_F(CreateVector, TryCreateWithZeroDim_FailCreate)
{
    double simpleVectorData[] = {1};
    auto vec = IVector::createVector(0, simpleVectorData);
    ASSERT_EQ(vec, nullptr);
}

TEST_F(CreateVector, TryCreateWithNotValidValues_FailCreate)
{
    double notValidValues[] = {NAN};
    auto vec = IVector::createVector(1, notValidValues);
    ASSERT_EQ(vec, nullptr);
}


class getDim: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }
};

TEST_F(getDim, Dim5Vector_SuccGet)
{
    double data5[5] = {1, 2, 3, 4, 5};
    IVector* vecDim5 = IVector::createVector(5, data5);

    ASSERT_EQ(vecDim5->getDim(), 5);
}

TEST_F(getDim, Dim1Vector_SuccGet)
{
    double data1[1] = {1};
    IVector* vecDim1 = IVector::createVector(1, data1);

    ASSERT_EQ(vecDim1->getDim(), 1);
}


class getCord : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        vecDim5 = IVector::createVector(5, data5);
        vecDim1 = IVector::createVector(1, data1);
    }

    virtual void TearDown()
    {
        delete vecDim5;
        delete vecDim1;
    }

    double data5[5] = {1, 2, 3, 4, 5};
    double data1[1] = {1};
    IVector* vecDim5;
    IVector* vecDim1;

    //constexpr static const double MAGIC_CONST = 666;
    const double MAGIC_CONST = 666;
};

TEST_F(getCord, GetAllByValidIdxFromVecDim5_SuccGet)
{
    for (int i = 0; i < vecDim5->getDim(); ++i)
    {
        double val = MAGIC_CONST;
        RC rc = vecDim5->getCord(i, val);

        ASSERT_EQ(val, data5[i]);
        ASSERT_EQ(rc, RC::SUCCESS);
    }
}

TEST_F(getCord, GetByZeroIdxFromVecDim1_SuccGet)
{
    double val = MAGIC_CONST;
    RC rc = vecDim1->getCord(0, val);

    ASSERT_EQ(val, data1[0]);
    ASSERT_EQ(rc, RC::SUCCESS);
}

TEST_F(getCord, GetByNotValidIdxFromVecDim5_FailGet)
{
    size_t notValidIdx[] = {vecDim5->getDim()+1, 100, 1000};
    for (auto i : notValidIdx)
    {
        double val = MAGIC_CONST;
        RC rc = vecDim5->getCord(i, val);

        ASSERT_EQ(val, MAGIC_CONST);
        ASSERT_NE(rc, RC::SUCCESS);
    }
}

TEST_F(getCord, GetByNotValidIdxFromVecDim1_FailGet)
{
    size_t notValidIdx[] = {vecDim5->getDim()+1, 100, 1000};
    for (auto i : notValidIdx)
    {
        double val = MAGIC_CONST;
        RC rc = vecDim1->getCord(i, val);

        ASSERT_EQ(val, MAGIC_CONST);
        ASSERT_NE(rc, RC::SUCCESS);
    }
}


class setCord : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        vecDim5 = IVector::createVector(5, data5);
        vecDim1 = IVector::createVector(1, data1);
    }

    virtual void TearDown()
    {
        delete vecDim5;
        delete vecDim1;
    }

    bool hasVecDim5Changed()
    {
        for (int i = 0; i < vecDim5->getDim(); ++i)
        {
            double val;
            vecDim5->getCord(i, val);

            if (data5[i] != val)
                return true;
        }
        return false;
    }

    bool hasVecDim1Changed()
    {
        double val;
        vecDim1->getCord(0, val);

        return (data1[0] != val);
    }

    double data5[5] = {1, 2, 3, 4, 5};
    double data1[1] = {1};
    IVector* vecDim5;
    IVector* vecDim1;

    //constexpr static const double MAGIC_CONST = 666;
    const double MAGIC_CONST = 666;
};

TEST_F(setCord, SetAllByValidIdxInVecDim5_SuccSet)
{
    for (int i = 0; i < vecDim5->getDim(); ++i)
    {
        double valToSet = i;
        RC rcSet = vecDim5->setCord(i, valToSet);

        double valSet;
        vecDim5->getCord(i, valSet);
        ASSERT_EQ(i, valSet);

        ASSERT_EQ(rcSet, RC::SUCCESS);
    }
}

TEST_F(setCord, SetByZeroIdxInVecDim1_SuccSet)
{
    double valToSet = MAGIC_CONST;
    RC rcSet = vecDim1->setCord(0, valToSet);

    double valSet;
    vecDim1->getCord(0, valSet);
    ASSERT_EQ(valSet, valToSet);

    ASSERT_EQ(rcSet, RC::SUCCESS);
}

TEST_F(setCord, SetByNotValidIdxInVecDim5_FailSet)
{
    size_t notValidIdx[] = {vecDim5->getDim()+1, 100, 1000};
    for (auto i : notValidIdx)
    {
        double val = i;
        RC rc = vecDim5->setCord(i, val);

        ASSERT_NE(rc, RC::SUCCESS);
    }
    ASSERT_FALSE(hasVecDim5Changed());
}

TEST_F(setCord, SetByNotValidIdxInVecDim1_FailSet)
{
    size_t notValidIdx[] = {vecDim5->getDim()+1, 100, 1000};
    for (auto i : notValidIdx)
    {
        double val = MAGIC_CONST;
        RC rc = vecDim1->setCord(i, val);

        ASSERT_NE(rc, RC::SUCCESS);
    }
    ASSERT_FALSE(hasVecDim1Changed());
}

TEST_F(setCord, SetNANInVecDim5_FailSet)
{
    for (int i = 0; i < vecDim5->getDim(); ++i)
    {
        double valToSet = NAN;
        RC rcSet = vecDim5->setCord(i, valToSet);

        double valSet;
        vecDim5->getCord(i, valSet);

        ASSERT_EQ(valSet, data5[i]);
        ASSERT_NE(rcSet, RC::SUCCESS);
    }
}


class scale : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        vecDim5 = IVector::createVector(5, data5);
    }

    virtual void TearDown()
    {
        delete vecDim5;
    }

    bool hasVecDim5Changed()
    {
        for (int i = 0; i < vecDim5->getDim(); ++i)
        {
            double val;
            vecDim5->getCord(i, val);

            if (data5[i] != val)
                return true;
        }
        return false;
    }

    bool hasVecDim5Scaled(double scale)
    {
        for (int i = 0; i < vecDim5->getDim(); ++i)
        {
            double val;
            vecDim5->getCord(i, val);

            if (scale*data5[i] != val)
                return false;
        }
        return true;
    }

    double data5[5] = {1, 2, 3, 4, 5};
    IVector* vecDim5;

    //constexpr static const double MAGIC_CONST = 666;
    const double MAGIC_CONST = 666;
};

TEST_F(scale, ScaleVecDim5By2_SuccScale)
{
    double scale = 2;
    vecDim5->scale(scale);

    ASSERT_TRUE(hasVecDim5Scaled(scale));
    ASSERT_TRUE(hasVecDim5Changed());
}

TEST_F(scale, ScaleVecDim5By1000_SuccScale)
{
    double scale = -1000;
    vecDim5->scale(scale);

    ASSERT_TRUE(hasVecDim5Scaled(scale));
    ASSERT_TRUE(hasVecDim5Changed());
}

TEST_F(scale, ScaleVecDim5ByMinus1000_SuccScale)
{
    double scale = -1000;
    vecDim5->scale(scale);

    ASSERT_TRUE(hasVecDim5Scaled(scale));
    ASSERT_TRUE(hasVecDim5Changed());
}

TEST_F(scale, ScaleVecDim5ByNan_FailScaleNothingChanged)
{
    double scale = NAN;
    vecDim5->scale(scale);

    ASSERT_FALSE(hasVecDim5Scaled(scale));
    ASSERT_FALSE(hasVecDim5Changed());
}

TEST_F(scale, ScaleVecDim5ByINFTY_FailScaleNothingChanged)
{
    double scale = INFINITY;
    vecDim5->scale(scale);

    ASSERT_FALSE(hasVecDim5Scaled(scale));
    ASSERT_FALSE(hasVecDim5Changed());
}