#include "../headers/ICompact.h"
#include "ICompactImpl.cpp"
#include <cmath>    // fabs (C++11), isnan
#include <new>      // nothrow
#include <algorithm>// min, max
#include <assert.h> // assert

ICompact::Iterator::~Iterator() {}

ICompact::~ICompact() {}

ICompact * ICompact::createCompact(IVector const * begin, IVector const * end, double accuracy, ILogger * logger) {

}

ICompact * ICompact::_union(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}

ICompact * ICompact::intersection(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}

ICompact* ICompact::convex(ICompact const * comp1, ICompact const * comp2, double accuracy, ILogger * logger) {

}