//
// Created by Danil on 19.03.2021.
//

#include "gtest/gtest.h"

#include "IVector.h"
#include "VectorImpl.cpp"
#include <limits>
#include <numeric>


class getData : public ::testing::Test {
public:
    virtual void SetUp() {
        vec5 = IVector::createVector(5, data5);
    }

    virtual void TearDown() {
        delete vec5;
    }

    double data5[5] = {1, 2, 3, 4, 5};
    IVector *vec5;
};

TEST_F(getData, GetingDataWith5Elements_SuccGet) {
    double const *outputData5 = vec5->getData();
    ASSERT_NE(outputData5, nullptr);

    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(outputData5[i], data5[i]);
    }
}

class inc : public ::testing::Test {
public:
    virtual void SetUp() {
        logger = ILogger::createLogger("log1.txt");
        IVector::setLogger(logger);
        Vec5 = IVector::createVector(5, dataVec5);
        AnotherVec5 = IVector::createVector(5, dataAnotherVec5);
        Vec1 = IVector::createVector(1, dataVec1);
    }

    virtual void TearDown() {
        delete Vec5;
        delete Vec1;
        delete AnotherVec5;
        delete logger;
    }

    bool hasDefaultVec5Changed() {
        double const *outputData = Vec5->getData();
        for (int i = 0; i < 5; i++) {
            if (outputData[i] != dataVec5[i])
                return true;
        }
        return false;
    }

    ILogger* logger = nullptr;
    double dataVec5[5] = {4, 3, 3, 4, 5};
    double dataAnotherVec5[5] = {3, 2, 8, 6, 3};
    double dataVec1[1] = {1};

    IVector* Vec5;
    IVector* AnotherVec5;
    IVector* Vec1;
};

TEST_F(inc, IncTwoVectorsSecondIsNullptr_ReturnNotSuccessRC) {
    RC rc = Vec5->inc(nullptr);
    ASSERT_NE(rc, RC::SUCCESS);
}

TEST_F(inc, IncTwoVectorsWithEqualDim_SuccInc) {
    RC rc = Vec5->inc(AnotherVec5);
    ASSERT_EQ(rc, RC::SUCCESS);

    double const *outputData = Vec5->getData();
    for (int i = 0; i < 5; i++) {
        ASSERT_TRUE(std::isfinite(outputData[i]));
    }

}

TEST_F(inc, IncTwoVectorsWithNonEqualDim_ReturnNotSuccessRC) {
    RC rc = Vec5->inc(Vec1);
    ASSERT_NE(rc, RC::SUCCESS);

    ASSERT_FALSE(hasDefaultVec5Changed());
}



TEST_F(inc, IncTwoVectorsOneElemIsNearInfinity_ReturnNotSuccessRCAndUnchangedVector) {
    auto nearInf = std::numeric_limits<double>::max();

    size_t idx = 2;
    dataVec5[idx] = nearInf;
    dataAnotherVec5[idx] = nearInf;
    Vec5->setCord(idx, nearInf);
    AnotherVec5->setCord(idx, nearInf);


    RC rc = Vec5->inc(AnotherVec5);
    ASSERT_NE(rc, RC::SUCCESS);

    ASSERT_FALSE(hasDefaultVec5Changed());
}


class dec : public ::testing::Test {
public:
    virtual void SetUp() {
        logger = ILogger::createLogger("log1.txt");
        IVector::setLogger(logger);
        Vec5 = IVector::createVector(5, dataVec5);
        AnotherVec5 = IVector::createVector(5, dataAnotherVec5);
        Vec1 = IVector::createVector(1, dataVec1);
    }

    virtual void TearDown() {
        delete Vec5;
        delete Vec1;
        delete AnotherVec5;
        delete logger;
    }

    bool hasDefaultVec5Changed() {
        double const *outputData = Vec5->getData();
        for (int i = 0; i < 5; i++) {
            if (outputData[i] != dataVec5[i])
                return true;
        }
        return false;
    }

    ILogger* logger = nullptr;
    double dataVec5[5] = {4, 3, 3, 4, 5};
    double dataAnotherVec5[5] = {3, 2, 8, 6, 3};
    double dataVec1[1] = {1};

    IVector* Vec5;
    IVector* AnotherVec5;
    IVector* Vec1;
};

TEST_F(dec, DecTwoVectorsSecondIsNullptr_ReturnNotSuccessRC) {
    RC rc = Vec5->dec(nullptr);
    ASSERT_NE(rc, RC::SUCCESS);
}

TEST_F(dec, DecTwoVectorsWithEqualDim_SuccDec) {
    RC rc = Vec5->dec(AnotherVec5);
    ASSERT_EQ(rc, RC::SUCCESS);

    double const *outputData = Vec5->getData();
    for (int i = 0; i < 5; i++) {
        ASSERT_TRUE(std::isfinite(outputData[i]));
    }

}

TEST_F(dec, DecTwoVectorsWithNonEqualDim_ReturnNotSuccessRC) {
    RC rc = Vec5->dec(Vec1);
    ASSERT_NE(rc, RC::SUCCESS);

    ASSERT_FALSE(hasDefaultVec5Changed());
}



TEST_F(dec, DecTwoVectorsOneElemIsNearInfinity_ReturnNotSuccessRCAndUnchangedVector) {
    auto nearMinusInf = -std::numeric_limits<double>::max();
    auto nearPlusInf = std::numeric_limits<double>::max();

    size_t idx = 0;
    dataVec5[idx] = nearMinusInf;
    dataAnotherVec5[idx] = nearPlusInf;
    Vec5->setCord(idx, nearMinusInf);
    AnotherVec5->setCord(idx, nearPlusInf);


    RC rc = Vec5->dec(AnotherVec5);
    ASSERT_NE(rc, RC::SUCCESS);

    ASSERT_FALSE(hasDefaultVec5Changed());
}


class dot : public ::testing::Test {
public:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

TEST_F(dot, DotOfTwoCorrectVectors_SuccessResult) {
    double data2[5] = {1, 2, 3, 4, 5};
    double data1[5] = {6, 8, 2, 3, 1};

    IVector *vec1 = IVector::createVector(5, data1);
    IVector *vec2 = IVector::createVector(5, data2);

    double result = IVector::dot(vec1, vec2);
    double correctResult = std::inner_product(std::begin(data1), std::end(data1), std::begin(data2), 0.0);

    ASSERT_EQ(result, correctResult);

    delete vec1;
    delete vec2;

}

TEST_F(dot, DotOfTwoNullptrVectors_ReturnQuiteNan) {
    ASSERT_TRUE(std::isnan(IVector::dot(nullptr, nullptr)));
}

TEST_F(dot, DotOfTwoVectorsWithNonEqualDim_ReturnQuiteNan) {
    double data2[5] = {1, 2, 3, 4, 5};
    double data1[4] = {6, 8, 2, 3};

    IVector *vec1 = IVector::createVector(5, data1);
    IVector *vec2 = IVector::createVector(4, data2);

    double result = IVector::dot(vec1, vec2);

    ASSERT_TRUE(std::isnan(result));

    delete vec1;
    delete vec2;
}

TEST_F(dot, DotOfTwoVectorsOneHasInfinityItem_ReturnQuiteNan) {
    double data2[5] = {1, 2, 3, 4, 5};
    double data1[5] = {6, 8, std::numeric_limits<double>::max(), 3, 3};

    IVector *vec1 = IVector::createVector(5, data1);
    IVector *vec2 = IVector::createVector(5, data2);

    double result = IVector::dot(vec1, vec2);

    ASSERT_TRUE(std::isnan(result));

    delete vec1;
    delete vec2;
}

TEST_F(dot, DotOfTwoVectorsOneHasMinusInfinityItem_ReturnQuiteNan) {
    double data2[5] = {1, 2, 3, 4, 5};
    double data1[5] = {6, 8, -std::numeric_limits<double>::max(), 3, 3};

    IVector *vec1 = IVector::createVector(5, data1);
    IVector *vec2 = IVector::createVector(5, data2);

    double result = IVector::dot(vec1, vec2);

    ASSERT_TRUE(std::isnan(result));

    delete vec1;
    delete vec2;
}


class norm : public ::testing::Test {
public:
    virtual void SetUp()
    {
        ILogger* logger = ILogger::createLogger("log1.txt");
        IVector::setLogger(logger);
    }

    virtual void TearDown()
    {
    }
    double data[5] = {1, 2, 3, 4, 5};
    double dataRandom[5] = {1, -3, -5, 2, -100};
    double dataInf[5] = {1, 2, std::numeric_limits<double>::max(), -2, std::numeric_limits<double>::max()};

    int get_file_size(const char* filename)
    {
        FILE *p_file = nullptr;
        p_file = fopen(filename,"rb");
        if (p_file == nullptr)
            return 0;

        fseek(p_file,0,SEEK_END);
        int size = ftell(p_file);
        fclose(p_file);
        return size;
    }
};

TEST_F(norm, FirstNorm_ReturnSuccResult)
{
    IVector *vec1 = IVector::createVector(5, data);

    ASSERT_EQ(vec1->norm(IVector::NORM::FIRST), 15);

    delete vec1;
}

TEST_F(norm, SecondNorm_ReturnSuccResult)
{

    IVector *vec1 = IVector::createVector(5, data);

    ASSERT_EQ(vec1->norm(IVector::NORM::SECOND), sqrt(55));

    delete vec1;
}

TEST_F(norm, ChebyshevNorm_ReturnSuccResult)
{

    IVector *vec1 = IVector::createVector(5, data);

    ASSERT_EQ(vec1->norm(IVector::NORM::CHEBYSHEV), 5);

    delete vec1;
}

TEST_F(norm, ChebysevNormOfVectorWithNegElem_ReturnSuccResult)
{
    IVector *vec1 = IVector::createVector(5, dataRandom);

    ASSERT_EQ(vec1->norm(IVector::NORM::CHEBYSHEV), 100);

    delete vec1;
}

TEST_F(norm, FirstNormOfVectorWithNegElem_ReturnSuccResult)
{
    IVector *vec1 = IVector::createVector(5, dataRandom);

    ASSERT_EQ(vec1->norm(IVector::NORM::FIRST), 111);

    delete vec1;
}

TEST_F(norm, FirstNormOfVectorWithInfElem_ReturnNAN)
{
    IVector *vec1 = IVector::createVector(5, dataInf);

    ASSERT_TRUE(std::isnan(vec1->norm(IVector::NORM::FIRST)));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);

    delete vec1;
}

TEST_F(norm, SecondNormOfVectorWithInfElem_ReturnNAN)
{
    IVector *vec1 = IVector::createVector(5, dataInf);

    ASSERT_TRUE(std::isnan(vec1->norm(IVector::NORM::SECOND)));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);

    delete vec1;
}

TEST_F(norm, AmountEnumInFunction_ReturnNAN)
{
    IVector *vec1 = IVector::createVector(5, dataRandom);

    ASSERT_TRUE(std::isnan(vec1->norm(IVector::NORM::AMOUNT)));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);

    delete vec1;
}



class equals : public ::testing::Test {
public:
    virtual void SetUp()
    {
        vec1 = IVector::createVector(5, dataVec1);
        vec2 = IVector::createVector(5, dataVec2);
        logger = ILogger::createLogger(logFileName);
        IVector::setLogger(logger);
    }

    virtual void TearDown()
    {
        delete vec1;
        delete vec2;
        delete logger;
    }
    const char* logFileName = "log1.txt";
    ILogger* logger;
    IVector* vec1;
    IVector* vec2;
    double dataVec1[5] = {1.2312, 1.3516, 1.67712, 5.915851, 8.3415157};
    double dataVec2[5] = {1.2412, 1.3616, 1.68712, 5.905851, 8.3315157};

    int get_file_size(const char* filename)
    {
        FILE *p_file = nullptr;
        p_file = fopen(filename,"rb");
        if (p_file == nullptr)
            return 0;

        fseek(p_file,0,SEEK_END);
        int size = ftell(p_file);
        fclose(p_file);
        return size;
    }
};

TEST_F(equals, NullPtrChecker_ReturnFalseAngLogErrorInFile)
{
    ASSERT_FALSE(IVector::equals(nullptr, nullptr, IVector::NORM::FIRST, 0.1));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);
}

TEST_F(equals, AmountNormArg_ReturnFalseAngLogErrorInFile)
{
    ASSERT_FALSE(IVector::equals(vec1, vec2, IVector::NORM::AMOUNT, 0.1));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);
}

TEST_F(equals, InfTolArg_ReturnFalseAngLogErrorInFile)
{
    ASSERT_FALSE(IVector::equals(vec1, vec2, IVector::NORM::FIRST, std::numeric_limits<double>::infinity()));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);
}

TEST_F(equals, NanTolArg_ReturnFalseAngLogErrorInFile)
{
    ASSERT_FALSE(IVector::equals(vec1, vec2, IVector::NORM::FIRST, std::numeric_limits<double>::quiet_NaN()));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") != 0);
}

TEST_F(equals, TwoEqualsVectorsFirstNorm_ReturnSuccResult)
{
    ASSERT_TRUE(IVector::equals(vec1, vec2, IVector::NORM::FIRST, 0.1));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") == 0);
}

TEST_F(equals, TwoEqualsVectorsSecondNorm_ReturnSuccResult)
{
    ASSERT_TRUE(IVector::equals(vec1, vec2, IVector::NORM::SECOND, 0.1));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") == 0);
}

TEST_F(equals, TwoEqualsVectorsChebyshevNorm_ReturnSuccResult)
{
    ASSERT_TRUE(IVector::equals(vec1, vec2, IVector::NORM::CHEBYSHEV, 0.1));
    ASSERT_TRUE(get_file_size("..\\..\\cmake-build-debug\\gtest\\log1.txt") == 0);
}
