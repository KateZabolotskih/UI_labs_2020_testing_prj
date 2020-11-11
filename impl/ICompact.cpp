#include "../headers/ICompact.h"
#include "ICompactImpl.cpp"
#include <cmath>    // fabs (C++11), isnan
#include <new>      // nothrow
#include <algorithm>// min, max
#include <assert.h> // assert


static ReturnCode validateCompacts(const ICompact * compact1, const ICompact * compact2, double accuracy = 0) {
    if (!compact1 || !compact2) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (compact2->getDim() != compact2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }
    if (compact1->getDim() == 0) {
        return ReturnCode::RC_WRONG_DIM;
    }
    if (std::isnan(accuracy)) {
        return ReturnCode::RC_NAN;
    }

    if (accuracy < 0.0) {
        return ReturnCode::RC_INVALID_PARAMS;
    }
    return ReturnCode::RC_SUCCESS;
}

static ReturnCode validateVecs(const IVector * vec1, const IVector * vec2, double accuracy = 0) {
    if (!vec2 || !vec1) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (vec1->getDim() != vec2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }
    if (vec1->getDim() == 0) {
        return ReturnCode::RC_WRONG_DIM;
    }
    return ReturnCode::RC_SUCCESS;
}

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
    for (int i = 0; i < begin->getDim(); ++i) {
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

static bool areCollinear (IVector * vec1, IVector * vec2, double accuracy, size_t & axesNum) {
    size_t numCoords = 0;
    for (int i = 0; i < vec1->getDim(); ++i) {
        if (std::fabs(vec1->getCoord(i) - vec2->getCoord(i)) < accuracy) {
            numCoords++;
        } else {
            axesNum = i;
        }
    }
    return numCoords == (vec1->getDim() - 1);
}

ICompact * ICompact::_union(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {
    auto r_code = validateCompacts(comp1, comp2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    bool issubset = false;
    r_code = comp2->isSubset(comp1, issubset);
    if (r_code == ReturnCode::RC_SUCCESS && issubset) {
        ICompact * result = comp1->clone();
        if (result == nullptr) {
            LOG(logger, ReturnCode::RC_NO_MEM);
            return nullptr;
        }
        return result;
    }
    r_code = comp1->isSubset(comp2, issubset);
    if (r_code == ReturnCode::RC_SUCCESS && issubset) {
        ICompact* result = comp2->clone();
        if (result == nullptr) {
            LOG(logger, ReturnCode::RC_NO_MEM);
            return nullptr;
        }
        return result;
    }
    IVector * begin1 = comp1->getBegin();
    IVector * begin2 = comp2->getBegin();
    r_code = validateVecs(begin1, begin2);
    if (r_code != ReturnCode::RC_SUCCESS) {
        delete begin1;
        delete begin2;
        LOG(logger, r_code);
        return nullptr;
    }
    size_t beginAxes;
    if (!areCollinear(begin1, begin2, accuracy, beginAxes)) {
        delete begin1;
        delete begin2;
        return nullptr;
    }

    IVector * end1   = comp1->getEnd();
    IVector * end2   = comp2->getEnd();
    r_code = validateVecs(end1, end2);
    if (r_code != ReturnCode::RC_SUCCESS) {
        delete end1;
        delete end2;
        LOG(logger, r_code);
        return nullptr;
    }
    size_t endAxes;
    if (!areCollinear(end1, end2, accuracy, endAxes)) {
        delete end1;
        delete end2;
        return nullptr;
    }
    if (beginAxes != endAxes) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        delete begin1;
        delete begin2;
        delete end1;
        delete end2;
        return nullptr;
    }
    bool intersects;
    comp1->intersects(comp2, intersects);
    if (!intersects) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        delete begin1;
        delete begin2;
        delete end1;
        delete end2;
        return nullptr;
    }

    begin1->setCoord(beginAxes, std::min(begin1->getCoord(beginAxes), begin2->getCoord(beginAxes)));
    end1->setCoord(endAxes, std::max(end1->getCoord(endAxes), end2->getCoord(endAxes)));

    ICompact * resComp = ICompact::createCompact(begin1, end1, accuracy);
    if (!resComp) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
    }
    delete end2;
    delete begin1;
    delete begin2;
    delete end1;
    return resComp;
}

ICompact * ICompact::intersection(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}

ICompact* ICompact::convex(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}