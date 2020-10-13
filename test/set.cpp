#include "../headers/test.h"

void getParams(std::vector<IVector *> & vec_s, double & accuracy, IVector::Norm & norm) {
    accuracy = 1e-5;
    norm = IVector::Norm::NORM_1;

    const size_t dim1 = 4, dim2 = 6;
    double data1[dim1] = {1.5,        4.67, 2.754, 5.566};
    double data2[dim1] = {1.5 + 1e-8, 4.67, 2.754, 5.566};
    double data3[dim1] = {5.1,        6.78, 8.187, 6.326};

    double data4[dim2] = {-2.1, 2.56, -2.689, 4.806, 0.111, 1.5};
    double data5[dim2] = { 1.1, 5.89,  2.689, 0.0,   0.431, 1.5};

    IVector * vec1 = IVector::createVector(dim1, data1);
    IVector * vec2 = IVector::createVector(dim1, data2);
    IVector * vec3 = IVector::createVector(dim1, data3);
    IVector * vec4 = IVector::createVector(dim2, data4);
    IVector * vec5 = IVector::createVector(dim2, data5);

    std::vector<IVector *> vec = {vec1, vec2, vec3, vec4, vec5};
    vec_s = vec;
}

void clear_vecs(std::vector<IVector *> & vec_s) {
    for (auto vec : vec_s) {
        delete vec;
    }
}

ReturnCode _insert_test() {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm);

    ISet * set = ISet::createSet();

    auto r_code = set->insert(vec_s[0], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getDim() != vec_s[0]->getDim()) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->insert(vec_s[1], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 1) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->insert(vec_s[3], norm, accuracy);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->insert(nullptr, norm, accuracy);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set;
    clear_vecs(vec_s);
    vec_s.clear();

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _erase_byVec_test() {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm);
    ISet * set = ISet::createSet(nullptr);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    auto r_code = set->erase(vec_s[3], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 1) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->erase(vec_s[1], norm, accuracy);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->erase(vec_s[4], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 0) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set;
    clear_vecs(vec_s);
    vec_s.clear();

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _erase_byInd_test() {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm);
    ISet * set = ISet::createSet(nullptr);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    size_t index;
    ReturnCode r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        r_code = set->erase(index);
        if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 1) {
            return ReturnCode::RC_UNKNOWN;
        }
    } else {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->erase(10);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->find(vec_s[4], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        r_code = set->erase(index);
        if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 0) {
            return ReturnCode::RC_UNKNOWN;
        }
    } else {
        return ReturnCode::RC_UNKNOWN;
    }

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _find_test() {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm);
    ISet * set = ISet::createSet(nullptr);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    size_t index;
    ReturnCode r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->find(vec_s[1], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    r_code = set->find(nullptr, norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _union_test() {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm);
    ISet * set = ISet::createSet(nullptr);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    size_t index;
    ReturnCode r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->find(vec_s[1], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    r_code = set->find(nullptr, norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    return ReturnCode::RC_SUCCESS;

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _diff_test() {

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _symmDiff_test() {
    return ReturnCode::RC_SUCCESS;

}

ReturnCode _intersection_test() {
    return ReturnCode::RC_SUCCESS;

}

ReturnCode _clone_test() {
    return ReturnCode::RC_SUCCESS;

}


void set_testing_run() {
    int flag = 0;

    if (_insert_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set insertion test failed" << std::endl;
    }
    if (_erase_byVec_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set erase vector test failed" << std::endl;
    }
   if ( _erase_byInd_test() != ReturnCode::RC_SUCCESS) {
       flag = 1;
       std::cout << "set erase vector by index test failed" << std::endl;
   }
   if (_find_test() != ReturnCode::RC_SUCCESS) {
       flag = 1;
       std::cout << "set find test failed" << std::endl;
   }
   if (flag == 0) {
       std::cout << "ISet testing passed successfully" << std::endl;
   } else {
       std::cout << "ISet testing failed" << std::endl;
   }
}

