#include "../headers/test.h"

ReturnCode compact_create_test() {
    ReturnCode r_code = ReturnCode::RC_SUCCESS;
    const double accuracy = 1e-5;
    const size_t dim2 = 2;

    double data1[dim2] = {0, 0};
    double data2[dim2] = {2, 2};
    IVector * vec1 = IVector::createVector(dim2, data1);
    IVector * vec2 = IVector::createVector(dim2, data2);
    ICompact * comp1 = ICompact::createCompact(vec1, vec2, accuracy);
    if (comp1 == nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    } else {
        IVector * begin = comp1->getBegin();
        IVector * end = comp1->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 0 ||
            end->getCoord(0) != 2 || end->getCoord(1) != 2) {
            r_code = ReturnCode::RC_UNKNOWN;
        }
        delete begin;
        delete end;
    }

    double data3[dim2] = {0, 1};
    IVector * vec3 = IVector::createVector(dim2, data3);
    ICompact * comp2 = ICompact::createCompact(vec3, vec3, accuracy);
    if (comp2 != nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    }

    delete vec1;
    delete vec2;
    delete vec3;
    delete comp1;
    delete comp2;
    return r_code;
}

ReturnCode compact_contain_test() {
    ReturnCode r_code = ReturnCode::RC_SUCCESS;
    const double accuracy = 1e-4;
    const size_t dim2 = 2;

    double data1[dim2] = {0, 0};
    double data2[dim2] = {5, 5};
    IVector * vec1 = IVector::createVector(dim2, data1);
    IVector * vec2 = IVector::createVector(dim2, data2);

    ICompact * comp1 = ICompact::createCompact(vec1, vec2, accuracy);
    if (comp1 == nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    } else {
        double a[dim2] = {2, 2};
        double b[dim2] = {3, 7};
        IVector * A = IVector::createVector(dim2, a);
        IVector * B = IVector::createVector(dim2, b);
        bool containsA = false;
        bool containsB = false;
        if (comp1->contains(A, containsA) != ReturnCode::RC_SUCCESS || !containsA ||
            comp1->contains(B,containsB) != ReturnCode::RC_SUCCESS || containsB) {
            r_code = ReturnCode::RC_UNKNOWN;
        }
        delete A;
        delete B;
    }

    delete vec1;
    delete vec2;
    delete comp1;
    return r_code;
}

ReturnCode compact_union_test() {
    ReturnCode r_code = ReturnCode::RC_SUCCESS;
    const double accuracy = 1e-4;
    const size_t dim2 = 2, dim3 = 3;

    double data1[dim2] = {1, 0};
    double data2[dim2] = {2, 2};
    IVector * vec1 = IVector::createVector(dim2, data1);
    IVector * vec2 = IVector::createVector(dim2, data2);
    ICompact * comp1 = ICompact::createCompact(vec1, vec2, accuracy);

    double data3[dim2] = {1, 2};
    double data4[dim2] = {2, 3};
    IVector * vec3 = IVector::createVector(dim2, data3);
    IVector * vec4 = IVector::createVector(dim2, data4);
    ICompact * comp2 = ICompact::createCompact(vec3, vec4, accuracy);

    ICompact * _u = ICompact::_union(comp1, comp2, accuracy);
    if ( _u == nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    } else {
        IVector * begin = _u->getBegin();
        IVector * end = _u->getEnd();
        if (begin->getCoord(0) != 1 || begin->getCoord(1) != 0 ||
            end->getCoord(0) != 2 || end->getCoord(1) != 3) {
            r_code = ReturnCode::RC_UNKNOWN;
        }
        delete begin;
        delete end;
    }

    double data_1[dim2] = {0, 0};
    double data_2[dim2] = {1, 2};
    IVector * vec_1 = IVector::createVector(dim2, data_1);
    IVector * vec_2 = IVector::createVector(dim2, data_2);
    ICompact * comp_1 = ICompact::createCompact(vec_1, vec_2, accuracy);

    double data_3[dim2] = {1, 3};
    double data_4[dim2] = {3, 3};
    IVector * vec_3 = IVector::createVector(dim2, data_3);
    IVector * vec_4 = IVector::createVector(dim2, data_4);
    ICompact * comp_2 = ICompact::createCompact(vec_3, vec_4, accuracy);

    ICompact * _u_ = ICompact::_union(comp_1, comp_2, accuracy);
    if ( _u_ != nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    }

    double data5[dim3] = {0, 0, 0};
    double data6[dim3] = {1, 1, 1};
    IVector * vec5 = IVector::createVector(dim3, data5);
    IVector * vec6 = IVector::createVector(dim3, data6);
    ICompact * comp3 = ICompact::createCompact(vec5, vec6, accuracy);

    ICompact * _uF = ICompact::_union(comp1, comp3, accuracy);
    if (_uF != nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    }

    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec_1;
    delete vec_2;
    delete vec_3;
    delete vec_4;
    delete vec5;
    delete vec6;

    delete comp1;
    delete comp2;
    delete comp_1;
    delete comp_2;
    delete comp3;

    delete _u;
    return r_code;
}

ReturnCode compact_intersection_test() {
    ReturnCode r_code = ReturnCode::RC_SUCCESS;
    const double accuracy = 1e-4;
    const size_t dim2 = 2, dim3 = 3;

    double data1[dim2] = {0, 0};
    double data2[dim2] = {2, 2};
    IVector * vec1 = IVector::createVector(dim2, data1);
    IVector * vec2 = IVector::createVector(dim2, data2);
    ICompact * comp1 = ICompact::createCompact(vec1, vec2, accuracy);

    double data3[dim2] = {1, 1.5};
    double data4[dim2] = {3, 4};
    IVector * vec3 = IVector::createVector(dim2, data3);
    IVector * vec4 = IVector::createVector(dim2, data4);
    ICompact * comp2 = ICompact::createCompact(vec3, vec4, accuracy);

    ICompact * inter = ICompact::intersection(comp1, comp2, accuracy);
    if ( inter == nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    } else {
        IVector * begin = inter->getBegin();
        IVector * end = inter->getEnd();
        if (begin->getCoord(0) != 1|| begin->getCoord(1) != 1.5 ||
            end->getCoord(0) != 2 || end->getCoord(1) != 2) {
            r_code = ReturnCode::RC_UNKNOWN;
        }
        delete begin;
        delete end;
    }

    double data5[dim3] = {0, 0, 0};
    double data6[dim3] = {1, 1, 1};
    IVector * vec5 = IVector::createVector(dim3, data5);
    IVector * vec6 = IVector::createVector(dim3, data6);
    ICompact * comp3 = ICompact::createCompact(vec5, vec6, accuracy);

    ICompact * interF = ICompact::intersection(comp1, comp3, accuracy);
    if (interF != nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    }

    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete comp1;
    delete comp2;
    delete inter;
    return r_code;
}

ReturnCode compact_convex_test() {
    ReturnCode r_code = ReturnCode::RC_SUCCESS;
    const double accuracy = 1e-4;
    const size_t dim2 = 2, dim3 = 3;

    double data1[dim2] = {0, 0};
    double data2[dim2] = {2, 2};
    IVector * vec1 = IVector::createVector(dim2, data1);
    IVector * vec2 = IVector::createVector(dim2, data2);
    ICompact * comp1 = ICompact::createCompact(vec1, vec2, accuracy);

    double data3[dim2] = {1, 1.5};
    double data4[dim2] = {3, 4};
    IVector * vec3 = IVector::createVector(dim2, data3);
    IVector * vec4 = IVector::createVector(dim2, data4);
    ICompact * comp2 = ICompact::createCompact(vec3, vec4, accuracy);

    ICompact * conv = ICompact::convex(comp1, comp2, accuracy);
    if (conv == nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    } else {
        IVector * begin = conv->getBegin();
        IVector * end = conv->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 0 ||
            end->getCoord(0) != 3 || end->getCoord(1) != 4) {
            r_code = ReturnCode::RC_UNKNOWN;
        }
        delete begin;
        delete end;
    }

    double data5[dim2] = {1, 0};
    double data6[dim2] = {3, 3};
    IVector * vec5 = IVector::createVector(dim2, data5);
    IVector * vec6 = IVector::createVector(dim2, data6);
    ICompact * comp3 = ICompact::createCompact(vec5, vec6, accuracy);

    double data7[dim2] = {0.5, 0.5};
    double data8[dim2] = {2.5, 3};
    IVector * vec7 = IVector::createVector(dim2, data7);
    IVector * vec8 = IVector::createVector(dim2, data8);
    ICompact * comp4 = ICompact::createCompact(vec7, vec8, accuracy);

    ICompact * conv2 = ICompact::convex(comp3, comp4, accuracy);
    if (conv == nullptr) {
        r_code = ReturnCode::RC_UNKNOWN;
    } else {
        IVector * begin = conv2->getBegin();
        IVector * end = conv2->getEnd();
        if (begin->getCoord(0) != 0.5 || begin->getCoord(1) != 0 ||
            end->getCoord(0) != 3 || end->getCoord(1) != 3) {
            r_code = ReturnCode::RC_UNKNOWN;
        }
        delete begin;
        delete end;
    }

    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;
    delete comp1;
    delete comp2;
    delete comp3;
    delete comp4;
    delete conv;
    delete conv2;
    return r_code;
}

void compact_testing_run() {
    int flag = 0;

    if (compact_union_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "compact union test failed" << std::endl;
    }
    if (compact_create_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "compact creation test failed" << std::endl;
    }
    if (compact_intersection_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "compact intersection test failed" << std::endl;
    }
    if (compact_contain_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "compact content test failed" << std::endl;
    }
    if (compact_convex_test() != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "compact convex test failed" << std::endl;
    }
    if (flag == 0) {
        std::cout << "ICompact testing passed successfully" << std::endl;
    } else {
        std::cout << "ICompact testing failed" << std::endl;
    }
}
