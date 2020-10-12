#include "../headers/ISet.h"
#include "ISetImpl.cpp"

static ReturnCode validateSets(ISet const * set1, ISet const * set2, double accuracy) {
    if (set1 == nullptr || set2 == nullptr) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (set1->getDim() == 0 || set2->getDim() == 0) {
        return ReturnCode::RC_ZERO_DIM;
    }
    if (set1->getDim() != set2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }
    if (std::isnan(accuracy) || accuracy < 0) {
        return ReturnCode::RC_INVALID_PARAMS;
    }

    return ReturnCode::RC_SUCCESS;
}

ISet * ISet::createSet(ILogger * logger) {
    ISet * set = new(std::nothrow) ISetImpl();
    if (set == nullptr) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    return set;
}

ISet * ISet::_union(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(set1, set2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * union_set = set1->clone();
    IVector * cur_vec = nullptr;
    size_t cur_vec_ind = 0;

    while (r_code != ReturnCode::RC_OUT_OF_BOUNDS) {
        r_code = set2->get(cur_vec, cur_vec_ind);
        if (validateVector(cur_vec) == ReturnCode::RC_SUCCESS) {
            union_set->insert(cur_vec, norm, accuracy);
            delete cur_vec;
            cur_vec = nullptr;
        }
        cur_vec_ind++;
    }
    return union_set;
}

ISet * ISet::difference(ISet const * minuend, ISet const * subtrahend, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(minuend, subtrahend, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * diff_set = minuend->clone();
    IVector * cur_vec = nullptr;
    size_t cur_vec_ind = 0;

    while (r_code != ReturnCode::RC_OUT_OF_BOUNDS) {
        r_code = subtrahend->get(cur_vec, cur_vec_ind);
        if (validateVector(cur_vec) == ReturnCode::RC_SUCCESS) {
            diff_set->erase(cur_vec, norm, accuracy);
            delete cur_vec;
            cur_vec = nullptr;
        }
        cur_vec_ind++;
    }
    return diff_set;
}

ISet * ISet::symmetricDifference(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(set1, set2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * union_set = _union(set1, set2, norm, accuracy, logger);
    ISet * inter_set = intersection(set1, set2, norm, accuracy, logger);

    ISet * result_set = difference(union_set, inter_set, norm, accuracy, logger);

    delete union_set;
    delete inter_set;

    return result_set;
}

ISet * ISet::intersection(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(set1, set2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * result = set1->clone();
    IVector * cur_vec = nullptr;
    size_t cur_vec_ind = 0;

    while (r_code != ReturnCode::RC_OUT_OF_BOUNDS)
    {
        r_code = set1->get(cur_vec, cur_vec_ind);
        if (validateVector(cur_vec) == ReturnCode::RC_SUCCESS) {
            size_t ind;
            if (set2->find(cur_vec, norm, accuracy, ind) != ReturnCode::RC_SUCCESS) {
                result->erase(cur_vec, norm, accuracy);
            }
            delete cur_vec;
            cur_vec = nullptr;
        }
    }
    return result;
}

ISet::~ISet() {}