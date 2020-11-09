#include "../headers/ICompact.h"
#include "ICompactImpl.cpp"
#include <cmath>    // fabs (C++11), isnan
#include <new>      // nothrow
#include <algorithm>// min, max
#include <assert.h> // assert

ICompact::Iterator::~Iterator() {}

ICompact::~ICompact() {}

ICompact * ICompact::createCompact(IVector const * begin, IVector const * end, double accuracy, ILogger * logger) {
    if (!begin || !end) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    if (begin->getDim() != end->getDim()) {
        LOG(logger, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }
    size_t dim = begin->getDim();
    for (int i = 0; i < dim; ++i) {
        if (end->getCoord(i) - begin->getCoord(i) < accuracy) {
            LOG(logger, ReturnCode::RC_INVALID_PARAMS);
            return nullptr;
        }
    }
    if (std::isnan(accuracy) || accuracy < 0.0) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }
    bool is_equals = false;
    IVector::equals(begin, end, IVector::Norm::NORM_1, accuracy, is_equals);
    if (is_equals) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }
    IVector * begin_copy = begin->clone();
    if (!begin_copy) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    IVector * end_copy = end->clone();
    if (!end_copy) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        delete begin_copy;
        return nullptr;
    }
    ICompactImpl * result = new(std::nothrow) ICompactImpl(begin_copy, end_copy, accuracy);
    if (!result) {
        delete begin_copy;
        delete end_copy;
        LOG(logger, ReturnCode::RC_NULL_PTR);
    }
    return (ICompact *)result;
}

ICompact * ICompact::_union(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}

ICompact * ICompact::intersection(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}

ICompact* ICompact::convex(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}