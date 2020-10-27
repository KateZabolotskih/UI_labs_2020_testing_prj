#include "../headers/ICompact.h"
#include <cmath>    // fabs (C++11)
#include <new>		// nothrow
#include <algorithm>// min, max
#include <assert.h> // assert
namespace {
    enum SEQUENCE {INVERSE = -1, EXPLICIT = 1};
    /* declaration */
    class ICompactImpl : public ICompact {
    private:
        size_t _dim {0};
        IVector * _begin {nullptr};
        IVector * _end {nullptr};
        ILogger * _logger {nullptr};
        double const _accuracy;

    public:
        class IteratorImpl : public ICompact::Iterator {
        private:
            IVector const * _begin {nullptr};
            IVector const * _end {nullptr};
            IVector const * _step {nullptr};
            IVector * _cur_point {nullptr};
            std::vector<size_t> _direction;
            SEQUENCE _orientation;
            ILogger * _logger {nullptr};
        public:
            ReturnCode setDirection(std::vector<size_t> const & direction)  override;
            ReturnCode doStep()                                             override;
            IVector * getPoint()                                      const override;

            IteratorImpl(IVector const * beg, IVector const * end, IVector const * step, std::vector<size_t> const & dir, SEQUENCE orientation);
            ~IteratorImpl();
        };

        Iterator * begin(IVector const * step)                            override;
        Iterator * end(IVector const * step)                              override;
        ICompact * clone()                                          const override;
        IVector * getBegin()                                        const override;
        IVector * getEnd()                                          const override;
        ReturnCode contains(IVector const * vec, bool & result)     const override;
        ReturnCode isSubset(ICompact const * comp, bool & result)   const override;
        ReturnCode intersects(ICompact const * comp, bool & result) const override;
        size_t getDim() const override;

        ICompactImpl(IVector const * begin, IVector const * end, double accuracy);
        ~ICompactImpl();
    };
}

ICompactImpl::IteratorImpl::IteratorImpl(IVector const * begin,
                                         IVector const * end,
                                         IVector const * step,
                                         std::vector<size_t> const & direction,
                                         SEQUENCE orientation) :
        _begin(begin),
        _end(end),
        _step(step),
        _direction(direction),
        _orientation(orientation) {
    if (_orientation == EXPLICIT) {
        _cur_point = begin->clone();
    } else {
        _cur_point = end->clone();
    }
    _logger = ILogger::createLogger(this);
}

ICompactImpl::IteratorImpl::~IteratorImpl() {
    if (_begin != nullptr) {
        delete _begin;
        _begin = nullptr;
    }
    if (_end != nullptr) {
        delete _end;
        _end = nullptr;
    }
    if (_cur_point!= nullptr) {
        delete _cur_point;
        _cur_point = nullptr;
    }
    if (_step != nullptr) {
        delete _step;
        _step = nullptr;
    }
    if (_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}

ReturnCode ICompactImpl::IteratorImpl::setDirection(std::vector<size_t> const & direction) {
    if (direction.size() != _begin->getDim()) {
        LOG(_logger, ReturnCode::RC_WRONG_DIM);
    }
    std::vector<bool> bool_vec(direction.size(), false);
    for (auto dir : direction) {
        bool_vec[dir] = true;
    }
    bool have_all_dirs = true;
    for (auto exist_axis : bool_vec) {
        if (!exist_axis) {
            have_all_dirs = false;
            break;
        }
    }
    if (!have_all_dirs) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS);
    }
    _direction = direction;
    return ReturnCode::RC_SUCCESS;
}

ReturnCode ICompactImpl::IteratorImpl::doStep() {
    size_t cur_axis = 0;
    double new_value;
    for (size_t i = 0; i < _begin->getDim(); i++) {
        new_value = _cur_point->getCoord(i) + _orientation * _step->getCoord(i);
        if (_orientation == INVERSE && new_value < _begin->getCoord(i)) {
            _cur_point->setCoord(i, _end->getCoord(i));
        }
        if (_orientation == EXPLICIT && new_value > _end->getCoord(i)) {
            _cur_point->setCoord(i, _begin->getCoord(i));
        }
        else {
            _cur_point->setCoord(i, new_value);
            cur_axis = i;
            break;
        }
    }

    if (cur_axis >= _begin->getDim()) {
        LOG(_logger, ReturnCode::RC_OUT_OF_BOUNDS);
        delete _cur_point;
        if (_orientation == EXPLICIT) {
            _cur_point = _end->clone();
        } else {
            _cur_point = _begin->clone();
        }
        return ReturnCode::RC_OUT_OF_BOUNDS;
    }
    return ReturnCode::RC_SUCCESS;
}

IVector * ICompactImpl::IteratorImpl::getPoint() const {
    return _cur_point->clone();
}

static IVector * buildStep(IVector const * step, size_t dim, ILogger * logger) {

}

static ICompact::Iterator* createIterator(IVector const * _begin, IVector const * _end, IVector const * _step, SEQUENCE orientation, ILogger * logger) {
    if (_begin == nullptr || _end == nullptr || _step == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    IVector const * begin = _begin->clone();
    IVector const * end   = _end->clone();
    if (begin == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        delete begin;
        return nullptr;
    }
    if (end == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        delete end;
        return nullptr;
    }

    IVector * step = buildStep(_step, begin->getDim(), logger);
    if (step == nullptr) {
        delete begin;
        delete end;
        return nullptr;
    }

    std::vector<size_t> direction(begin->getDim());
    for (size_t cur_axis = 0; cur_axis < begin->getDim(); cur_axis++) {
        direction[cur_axis] = cur_axis;
    }

    ICompact::Iterator * iterator = new(std::nothrow) ICompactImpl::IteratorImpl(begin, end, step, direction, orientation);
    if (iterator == nullptr) {
        delete begin;
        delete end;
        delete step;
    }

    return iterator;
}


ICompact::Iterator * ICompactImpl::begin(IVector const * step) {
    return createIterator(_begin, _end, step, EXPLICIT, _logger);
}

ICompact::Iterator * ICompactImpl::end(IVector const * step) {
    return createIterator(_begin, _end, step, INVERSE, _logger);
}

ICompact* ICompactImpl::clone() const {
    return ICompact::createCompact(_begin, _end, _accuracy, _logger);
}

IVector * ICompactImpl::getBegin() const {
    return _begin->clone();
}

IVector * ICompactImpl::getEnd() const {
    return _end->clone();
}

ReturnCode ICompactImpl::contains(IVector const * vec, bool & result) const {

}

ReturnCode ICompactImpl::isSubset(ICompact const * other, bool & result) const {

}

ReturnCode ICompactImpl::intersects(ICompact const * other, bool & result) const {

}

size_t ICompactImpl::getDim() const {
    return _dim;
}

ICompactImpl::ICompactImpl(IVector const * begin, IVector const * end, double accuracy) :
        _dim(begin->getDim()),
        _begin(begin->clone()),
        _end(end->clone()),
        _accuracy(accuracy) {
    _logger = ILogger::createLogger(this);
}

ICompactImpl::~ICompactImpl() {
    _dim = 0;
    if (_begin != nullptr) {
        delete _begin;
        _begin = nullptr;
    }
    if (_end != nullptr) {
        delete _end;
        _end = nullptr;
    }
    if (_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}