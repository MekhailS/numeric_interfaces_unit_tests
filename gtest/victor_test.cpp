//
// Created by vikto on 19.03.2021.
//

#include "gtest/gtest.h"



#define VECTOR_IMPLEMENTATION_NAME VectorImpl

#include "../include/IVector.h"
#include "../include/ILogger.h"
#include "../src/VectorImpl.cpp"
#include <cmath>


class setLogger: public ::testing::Test
{
public:
    virtual void SetUp()
    {
        ILogger* logger = ILogger::createLogger();
        IVector::setLogger(logger);
    }

    virtual void TearDown()
    {

    }
};

TEST_F(setLogger, setLogger_Succ) {
    ASSERT_EQ(true, true);
}

//----------------------------------------test_moveInstance_function---------------------------------------


class moveInstance: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

};


TEST_F(moveInstance, moveInstanceVecDim5WithoutIntersection_SuccMove) {
    IVector* vecDim5;
    double data5[5] = {1, 2, 3, 4, 5};
    vecDim5 = IVector::createVector(5, data5);

    size_t sizeAllocatedBeforeMove = vecDim5->sizeAllocated();
    size_t dimBeforeMove = vecDim5->getDim();

    IVector* dst = (IVector*)new uint8_t[sizeAllocatedBeforeMove];

    size_t diffPointers = abs(reinterpret_cast<uint8_t*>(dst) -
                              reinterpret_cast<const uint8_t*>(vecDim5));
    ASSERT_GE(diffPointers, sizeAllocatedBeforeMove);

    RC error = IVector::moveInstance(dst, vecDim5);

    ASSERT_EQ(error, RC::SUCCESS);
    ASSERT_EQ(vecDim5, nullptr);
    ASSERT_EQ(dst->sizeAllocated(), sizeAllocatedBeforeMove);
    ASSERT_EQ(dst->getDim(), dimBeforeMove);

    for (size_t i = 0; i < dimBeforeMove; i++) {
        double val;
        dst->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }
}

TEST_F(moveInstance, moveInstanceVecDim5WithIntersection_SuccMove) {
    IVector* vecDim5;
    double data5[5] = {1, 2, 3, 4, 5};
    vecDim5 = IVector::createVector(5, data5);
    size_t dimBeforeMove = 5;
    size_t sizeAllocatedBeforeMove = sizeof(VECTOR_IMPLEMENTATION_NAME) + dimBeforeMove * sizeof(double);

    uint8_t * buffer = new (std::nothrow)uint8_t[2*(sizeAllocatedBeforeMove)];
    IVector::copyInstance((IVector*)buffer, vecDim5);

    IVector* dst = new (buffer + 3) VECTOR_IMPLEMENTATION_NAME(dimBeforeMove);

    RC error = IVector::moveInstance(dst, vecDim5);

    ASSERT_EQ(error, RC::SUCCESS);
    ASSERT_EQ(vecDim5, nullptr);
    ASSERT_EQ(dst->sizeAllocated(), sizeAllocatedBeforeMove);
    ASSERT_EQ(dst->getDim(), dimBeforeMove);

    for (size_t i = 0; i < dimBeforeMove; i++) {
        double val;
        dst->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }

    delete buffer;
}

TEST_F(moveInstance, dstIsNullptr_FailMove) {
    IVector* vecDim5;
    size_t dimBeforeMove = 5;
    double data5[5] = {1, 2, 3, 4, 5};
    vecDim5 = IVector::createVector(5, data5);

    IVector* dst = nullptr;

    RC error = IVector::moveInstance(dst, vecDim5);
    ASSERT_NE(error, RC::SUCCESS);
    ASSERT_EQ(dst, nullptr);
    ASSERT_NE(vecDim5, nullptr);

    ASSERT_EQ(vecDim5->getDim(), dimBeforeMove);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val;
        vecDim5->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }

    delete vecDim5;
}

TEST_F(moveInstance, srcIsNullptr_FailMove) {
    size_t sizeAllocatedBeforeMove = sizeof(VECTOR_IMPLEMENTATION_NAME) * 5 * sizeof(double);

    IVector* dst = (IVector*)new uint8_t[sizeAllocatedBeforeMove];

    IVector* src = nullptr;

    RC error = IVector::copyInstance(dst, src);
    ASSERT_NE(error, RC::SUCCESS);
    ASSERT_EQ(src, nullptr);
    ASSERT_NE(dst, nullptr);

}

TEST_F(moveInstance, srcAndDestAreNullptr_FailCopy) {
    IVector* dst = nullptr;
    IVector* src = nullptr;

    RC error = IVector::moveInstance(dst, src);
    ASSERT_NE(error, RC::SUCCESS);
    ASSERT_EQ(src, nullptr);
    ASSERT_EQ(dst, nullptr);
}

//----------------------------------------test_copyInstance_function---------------------------------------

class copyInstance: public ::testing::Test
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

    double data5[5] = {1, 2, 3, 4, 5};
    IVector* vecDim5;
};


TEST_F(copyInstance, copyVecDim5_SuccCopy) {
    size_t sizeAllocatedBeforeCopy = vecDim5->sizeAllocated();
    size_t dimBeforeCopy = vecDim5->getDim();

    IVector* dst = (IVector*)new uint8_t[sizeAllocatedBeforeCopy];

    size_t diffPointers = abs(reinterpret_cast<uint8_t*>(dst) -
                               reinterpret_cast<const uint8_t*>(vecDim5));
    ASSERT_GE(diffPointers, sizeAllocatedBeforeCopy);

    RC error = IVector::copyInstance(dst, vecDim5);
    ASSERT_EQ(error, RC::SUCCESS);
    ASSERT_EQ(dst->sizeAllocated(), sizeAllocatedBeforeCopy);
    ASSERT_EQ(dst->getDim(), dimBeforeCopy);
    ASSERT_NE(vecDim5, nullptr);
    ASSERT_EQ(vecDim5->getDim(), dimBeforeCopy);
    ASSERT_EQ(vecDim5->sizeAllocated(), sizeAllocatedBeforeCopy);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val;
        vecDim5->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val1, val2;
        vecDim5->getCord(i, val1);
        dst->getCord(i, val2);
        ASSERT_EQ(val1, val2);
    }

    delete dst;
}


TEST_F(copyInstance, destIsNullptr_FailCopy) {
    size_t dimBeforeCopy = vecDim5->getDim();
    IVector* dst = nullptr;

    RC error = IVector::copyInstance(dst, vecDim5);
    ASSERT_NE(error, RC::SUCCESS);
    ASSERT_EQ(dst, nullptr);
    ASSERT_NE(vecDim5, nullptr);

    ASSERT_EQ(vecDim5->getDim(), dimBeforeCopy);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val;
        vecDim5->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }

}


TEST_F(copyInstance, srcIsNullptr_FailCopy) {
    size_t sizeAllocatedBeforeCopy = vecDim5->sizeAllocated();

    IVector* dst = (IVector*)new uint8_t[sizeAllocatedBeforeCopy];

    IVector* src = nullptr;

    RC error = IVector::copyInstance(dst, src);
    ASSERT_NE(error, RC::SUCCESS);
    ASSERT_EQ(src, nullptr);
    ASSERT_NE(dst, nullptr);

}


TEST_F(copyInstance, srcAndDestAreNullptr_FailCopy) {
    IVector* dst = nullptr;
    IVector* src = nullptr;

    RC error = IVector::copyInstance(dst, src);
    ASSERT_NE(error, RC::SUCCESS);
    ASSERT_EQ(src, nullptr);
    ASSERT_EQ(dst, nullptr);
}

//-------------------------------------------test_clone_function------------------------------------------

class clone: public ::testing::Test
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

    double data5[5] = {1, 2, 3, 4, 5};
    IVector* vecDim5;
};


TEST_F(clone, cloneVecDim5_SuccClone) {
    IVector* cloned = vecDim5->clone();

    ASSERT_EQ(cloned->getDim(), vecDim5->getDim());
    ASSERT_EQ(cloned->sizeAllocated(), vecDim5->sizeAllocated());

    size_t diffPointers = abs(reinterpret_cast<uint8_t*>(cloned) -
            reinterpret_cast<const uint8_t*>(vecDim5));
    ASSERT_GE(diffPointers, cloned->sizeAllocated());


    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val1, val2;
        vecDim5->getCord(i, val1);
        cloned->getCord(i, val2);
        ASSERT_EQ(val1, val2);
    }

}

//----------------------------------------test_sizeAllocated_function---------------------------------------

class sizeAllocated: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }
};


TEST_F(sizeAllocated, sizeAllocatedOfElems5_SuccGet) {
    double data5[5] = {1, 2, 3, 4, 5};
    size_t sizeData = 5;
    IVector* vecDim5 = IVector::createVector(sizeData, data5);

    ASSERT_EQ(vecDim5->sizeAllocated(), sizeof(VECTOR_IMPLEMENTATION_NAME) + sizeData * sizeof(double));
}


TEST_F(sizeAllocated, sizeAllocatedOfElems1_SuccGet) {
    double data1[1] = {1};
    size_t sizeData = 1;
    IVector* vecDim1 = IVector::createVector(sizeData, data1);

    ASSERT_EQ(vecDim1->sizeAllocated(), sizeof(VECTOR_IMPLEMENTATION_NAME) + sizeData * sizeof(double));
}

//--------------------------------------------test_add_function--------------------------------------------

class add: public ::testing::Test
{
public:
    virtual void SetUp()
    {
        vecDim5 = IVector::createVector(5, data5);
        another_vecDim5 = IVector::createVector(5, another_data5);
        vecDim1 = IVector::createVector(1, data1);
    }

    virtual void TearDown()
    {
        delete vecDim5;
        delete another_vecDim5;
        delete vecDim1;
    }

    double data5[5] = {1, 2, 3, 4, 5};
    double another_data5[5] = {5, 4, 3, 2, 1};
    double data1[1] = {1};
    IVector* vecDim5;
    IVector* another_vecDim5;
    IVector* vecDim1;
};


TEST_F(add, addTwoSameDimensionalVec_SuccAdd) {
    IVector* res = IVector::add(vecDim5, another_vecDim5);

    ASSERT_NE(res, nullptr);
    ASSERT_EQ(res->getDim(), vecDim5->getDim());
    ASSERT_EQ(res->getDim(), another_vecDim5->getDim());

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double resOfAdd;
        res->getCord(i, resOfAdd);
        ASSERT_EQ(data5[i] + another_data5[i], resOfAdd);
    }
}


TEST_F(add, addFirstParameterIsNullptr_FailAdd) {
    IVector* res = IVector::add(nullptr, another_vecDim5);

    ASSERT_EQ(res, nullptr);
    ASSERT_NE(another_vecDim5, nullptr);

    for (size_t i = 0; i < another_vecDim5->getDim(); i++) {
        double val;
        another_vecDim5->getCord(i, val);
        ASSERT_EQ(val, another_data5[i]);
    }
}


TEST_F(add, addSecondParameterIsNullptr_FailAdd) {
    IVector* res = IVector::add(vecDim5, nullptr);

    ASSERT_EQ(res, nullptr);
    ASSERT_NE(vecDim5, nullptr);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val;
        vecDim5->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }
}


TEST_F(add, addBothParametersAreNullptr_FailAdd) {
    IVector* res = IVector::add(nullptr, nullptr);

    ASSERT_EQ(res, nullptr);

}


TEST_F(add, addVectorsWithValueNearToInfinity_FailAdd) {
    //Maximum finite value
    auto nearInf = std::numeric_limits<double>::max();

    size_t idx = 2;
    data5[idx] = nearInf;
    another_data5[idx] = nearInf;
    vecDim5->setCord(idx, nearInf);
    another_vecDim5->setCord(idx, nearInf);

    IVector* res = IVector::add(vecDim5, another_vecDim5);

    ASSERT_EQ(res, nullptr);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val1, val2;
        vecDim5->getCord(i, val1);
        another_vecDim5->getCord(i, val2);
        ASSERT_EQ(val1, data5[i]);
        ASSERT_EQ(val2, another_data5[i]);
    }

}

//--------------------------------------------test_sub_function--------------------------------------------

class sub: public ::testing::Test
{
public:
    virtual void SetUp()
    {
        vecDim5 = IVector::createVector(5, data5);
        another_vecDim5 = IVector::createVector(5, another_data5);
        vecDim1 = IVector::createVector(1, data1);
    }

    virtual void TearDown()
    {
        delete vecDim5;
        delete another_vecDim5;
        delete vecDim1;
    }

    double data5[5] = {1, 2, 3, 4, 5};
    double another_data5[5] = {5, 4, 3, 2, 1};
    double data1[1] = {1};
    IVector* vecDim5;
    IVector* another_vecDim5;
    IVector* vecDim1;
};


TEST_F(sub, subTwoSameDimensionalVec_SuccSub) {
    IVector* res = IVector::sub(vecDim5, another_vecDim5);

    ASSERT_NE(res, nullptr);
    ASSERT_EQ(res->getDim(), vecDim5->getDim());
    ASSERT_EQ(res->getDim(), another_vecDim5->getDim());

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double resOfSub;
        res->getCord(i, resOfSub);
        ASSERT_EQ(data5[i] - another_data5[i], resOfSub);
    }
}


TEST_F(sub, subFirstParameterIsNullptr_FailSub) {
    IVector* res = IVector::sub(nullptr, another_vecDim5);

    ASSERT_EQ(res, nullptr);
    ASSERT_NE(another_vecDim5, nullptr);

    for (size_t i = 0; i < another_vecDim5->getDim(); i++) {
        double val;
        another_vecDim5->getCord(i, val);
        ASSERT_EQ(val, another_data5[i]);
    }
}


TEST_F(sub, subSecondParameterIsNullptr_FailSub) {
    IVector* res = IVector::sub(vecDim5, nullptr);

    ASSERT_EQ(res, nullptr);
    ASSERT_NE(vecDim5, nullptr);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val;
        vecDim5->getCord(i, val);
        ASSERT_EQ(val, data5[i]);
    }
}


TEST_F(sub, subBothParametersAreNullptr_FailSub) {
    IVector* res = IVector::sub(nullptr, nullptr);

    ASSERT_EQ(res, nullptr);

}


TEST_F(sub, subVectorsWithValueNearToInfinity_FailSub) {
    //Maximum finite value
    auto nearInf = std::numeric_limits<double>::max();

    size_t idx = 2;
    data5[idx] = nearInf;
    another_data5[idx] = -nearInf;
    vecDim5->setCord(idx, nearInf);
    another_vecDim5->setCord(idx, -nearInf);

    IVector* res = IVector::sub(vecDim5, another_vecDim5);

    ASSERT_EQ(res, nullptr);

    for (size_t i = 0; i < vecDim5->getDim(); i++) {
        double val1, val2;
        vecDim5->getCord(i, val1);
        another_vecDim5->getCord(i, val2);
        ASSERT_EQ(val1, data5[i]);
        ASSERT_EQ(val2, another_data5[i]);
    }

}