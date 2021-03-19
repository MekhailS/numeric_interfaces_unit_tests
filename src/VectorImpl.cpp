//
// Created by MikhailS on 06.03.2021.
//

#include "IVector.h"
#include "Definitions.h"

namespace
{
    class VectorImpl : public IVector
    {
    public:
        VectorImpl(size_t dim);

        static RC setLogger(ILogger* const logger);

        IVector * clone() const override;
        const double * getData() const override;

        RC getCord(size_t index, double &val) const override;
        RC setCord(size_t index, double val) override;
        RC scale(double multiplier) override;
        size_t getDim() const override;

        RC inc(IVector const* const& op) override;
        RC dec(IVector const* const& op) override;

        double norm(NORM n) const override;

        RC applyFunction(const std::function<double (double)> &fun) override;
        RC foreach(const std::function<void (double)> &fun) const override;

        size_t sizeAllocated() const override;

    private:

        const double *vecData() const;
        double *vecData();

        bool isValidIndex(size_t i) const;

        RC modifyByAnotherVector(IVector const* const& op, const std::function<double(double, double)>& funModify);

        size_t vecDim;
        static ILogger* pLogger;
    };

    ILogger* VectorImpl::pLogger = nullptr;

    VectorImpl::VectorImpl(size_t dim)
    {
        vecDim = dim;
    }

    RC VectorImpl::setLogger(ILogger *const logger)
    {
        pLogger = logger;
        return RC::SUCCESS;
    }

    IVector * VectorImpl::clone() const
    {
        return IVector::createVector(vecDim, vecData());
    }

    const double * VectorImpl::getData() const
    {
        return vecData();
    }

    RC VectorImpl::getCord(size_t index, double &val) const
    {
        if (not isValidIndex(index))
        {
            if (pLogger != nullptr)
                pLogger->sever(RC::INDEX_OUT_OF_BOUND, __FILE__, __FUNCTION__ , __LINE__);

            return RC::INDEX_OUT_OF_BOUND;
        }

        val = vecData()[index];
        return RC::SUCCESS;
    }

    RC VectorImpl::setCord(size_t index, double val)
    {
        RC err = RC::SUCCESS;
        if (not definitions::isValidNum(val))
            err = RC::NOT_NUMBER;

        if (not isValidIndex(index))
            err = RC::INDEX_OUT_OF_BOUND;

        if (err != RC::SUCCESS)
        {
            if (pLogger != nullptr)
                pLogger->sever(err, __FILE__, __FUNCTION__, __LINE__);

            return err;
        }

        vecData()[index] = val;
        return RC::SUCCESS;
    }

    RC VectorImpl::scale(double multiplier)
    {
        auto lambdaScale = [&multiplier](double val)
        {
            return multiplier * val;
        };
        // if multiplier is not valid then applyFunction() will return error
        return applyFunction(lambdaScale);
    }

    size_t VectorImpl::getDim() const
    {
        return vecDim;
    }

    RC VectorImpl::inc(IVector const* const& op)
    {
        auto lambdaInc = [](double valueFromThis, double valueFromOp)
        {
            return valueFromThis + valueFromOp;
        };

        return modifyByAnotherVector(op, lambdaInc);
    }

    RC VectorImpl::dec(IVector const* const& op)
    {
        auto lambdaDec = [](double valueFromThis, double valueFromOp)
        {
            return valueFromThis - valueFromOp;
        };

        return modifyByAnotherVector(op, lambdaDec);
    }

    double VectorImpl::norm(NORM n) const
    {
        std::function<void(double)> lambdaNorm;
        double res = 0;
        double* pRes = &res;
        switch (n)
        {
            case IVector::NORM::FIRST:
                lambdaNorm = [&pRes](double val)
                {
                    (*pRes) += abs(val);
                };
                break;

            case IVector::NORM::SECOND:
                lambdaNorm = [&pRes](double val)
                {
                    (*pRes) += val * val;
                };
                break;

            case IVector::NORM::CHEBYSHEV:
                lambdaNorm = [&pRes](double val)
                {
                    if ((*pRes) < abs(val))
                        (*pRes) = abs(val);
                };
                break;

            default:
                lambdaNorm = [](double val){};
        }
        foreach(lambdaNorm);
        switch(n)
        {
            case IVector::NORM::SECOND:
                res = sqrt(res);
                break;

            default:
                break;
        }
        return res;
    }

    RC VectorImpl::applyFunction(const std::function<double(double)> &fun)
    {
        auto rc = RC::SUCCESS;
        double* data = vecData();
        for (int i = 0; i<vecDim; ++i)
        {
            if (not definitions::isValidNum(fun(data[i])))
            {
                rc = RC::INFINITY_OVERFLOW;
                if (pLogger != nullptr)
                    pLogger->sever(rc, __FILE__, __FUNCTION__, __LINE__);

                return rc;
            }
        }

        for (int i = 0; i < vecDim; ++i)
            data[i] = fun(data[i]);

        return rc;
    }

    RC VectorImpl::foreach(const std::function<void(double)> &fun) const
    {
        const double* data = vecData();
        for (int i = 0; i < vecDim; ++i)
            fun(data[i]);

        return RC::SUCCESS;
    }

    size_t VectorImpl::sizeAllocated() const
    {
        return (sizeof(VectorImpl) + vecDim * sizeof(double));
    }

    const double * VectorImpl::vecData() const
    {
        return reinterpret_cast<const double *>
        (reinterpret_cast<const definitions::byte *>(this) + sizeof(VectorImpl));
    }

    double * VectorImpl::vecData()
    {
        return reinterpret_cast<double *>
        (reinterpret_cast<definitions::byte *>(this) + sizeof(VectorImpl));
    }

    bool VectorImpl::isValidIndex(size_t i) const
    {
        return (0 <= i and i < vecDim);
    }

    RC VectorImpl::modifyByAnotherVector(const IVector *const &op,
                                         const std::function<double(double, double)>& funModify)
    {
        auto rc = RC::SUCCESS;
        if (vecDim != op->getDim())
        {
            rc = RC::MISMATCHING_DIMENSIONS;
            if (pLogger != nullptr)
                pLogger->sever(rc, __FILE__, __FUNCTION__, __LINE__);

            return rc;
        }

        double* data = vecData();
        double const* dataOp = op->getData();
        for (int i = 0; i < vecDim; ++i)
        {
            double res = funModify(data[i], dataOp[i]);

            if (not definitions::isValidNum(res))
            {
                rc = RC::INFINITY_OVERFLOW;
                if (pLogger != nullptr)
                    pLogger->sever(rc, __FILE__, __FUNCTION__, __LINE__);

                return rc;
            }
        }

        for (int i = 0; i < vecDim; ++i)
        {
            data[i] = funModify(data[i], dataOp[i]);
        }
        return rc;
    }
}