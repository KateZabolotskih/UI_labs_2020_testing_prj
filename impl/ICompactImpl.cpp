#include "../headers/ICompact.h"
#include <cmath>    // fabs (C++11)
#include <new>		// nothrow
#include <algorithm>// min, max
#include <assert.h> // assert
namespace {
    /* declaration */
    class ICompactImpl : public ICompact {
    private:
        size_t m_dim {0};
        IVector * m_begin {nullptr};
        IVector * m_end {nullptr};
        ILogger * m_logger {nullptr};
        double const m_tolerance;

    public:
        class IteratorImpl : public ICompact::Iterator {
        private:
            IVector const * m_begin {nullptr};
            IVector const * m_end {nullptr};
            IVector const * m_step {nullptr};
            IVector * m_current {nullptr};
            std::vector<size_t> m_direction;
            // true  - direct   traversal
            // false - reversed traversal
            bool m_traversal;
            ILogger * m_logger {nullptr};

        public:
            ReturnCode setDirection(std::vector<size_t> const & direction)  override;
            ReturnCode doStep()                                             override;
            IVector * getPoint()                                      const override;

            IteratorImpl(IVector const * beg, IVector const * end, IVector const * step, std::vector<size_t> const & dir, bool traversal);
            ~IteratorImpl();
        };

        Iterator * begin(IVector const * step) override;
        Iterator * end(IVector const * step)   override;

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

/* implementation */
ICompactImpl::IteratorImpl::IteratorImpl(
        IVector const * begin,
        IVector const * end,
        IVector const * step,
        std::vector<size_t> const & direction,
        bool traversal) :
        m_begin(begin),
        m_end(end),
        m_step(step),
        m_current(traversal ? begin->clone() : end->clone()),
        m_direction(direction),
        m_traversal(traversal) {
    m_logger = ILogger::createLogger(this);
}

ICompactImpl::IteratorImpl::~IteratorImpl() {
    if (m_begin != nullptr) {
        delete m_begin;
        m_begin = nullptr;
    }
    if (m_end != nullptr) {
        delete m_end;
        m_end = nullptr;
    }
    if (m_current != nullptr) {
        delete m_current;
        m_current = nullptr;
    }
    if (m_step != nullptr) {
        delete m_step;
        m_step = nullptr;
    }
    if (m_logger != nullptr) {
        m_logger->releaseLogger(this);
    }
}

ReturnCode ICompactImpl::IteratorImpl::doStep() {

}

ReturnCode ICompactImpl::IteratorImpl::setDirection(std::vector<size_t> const & direction) {

}

IVector * ICompactImpl::IteratorImpl::getPoint() const {
    return m_current->clone();
}

static IVector * buildStep(IVector const * temp, size_t dim, ILogger * logger) {

}

static ICompact::Iterator * createIterator(IVector const * _begin, IVector const * _end, IVector const * temp, bool traversal, ILogger * logger) {

}

ICompact::Iterator * ICompactImpl::begin(IVector const * temp) {
    return createIterator(m_begin, m_end, temp, true, m_logger);
}

ICompact::Iterator * ICompactImpl::end(IVector const * temp) {
    return createIterator(m_begin, m_end, temp, false, m_logger);
}

ICompact* ICompactImpl::clone() const {
    return ICompact::createCompact(m_begin, m_end, m_tolerance, m_logger);
}

IVector * ICompactImpl::getBegin() const {
    return m_begin->clone();
}

IVector * ICompactImpl::getEnd() const {
    return m_end->clone();
}

ReturnCode ICompactImpl::contains(IVector const * vec, bool & result) const {

}

ReturnCode ICompactImpl::isSubset(ICompact const * other, bool & result) const {

}

ReturnCode ICompactImpl::intersects(ICompact const * other, bool & result) const {

}

size_t ICompactImpl::getDim() const {
    return m_dim;
}

ICompactImpl::ICompactImpl(IVector const * begin, IVector const * end, double accuracy) :
        m_dim(begin->getDim()),
        m_begin(begin->clone()),
        m_end(end->clone()),
        m_tolerance(accuracy) {
    m_logger = ILogger::createLogger(this);
}

ICompactImpl::~ICompactImpl() {
    m_dim = 0;

    if (m_begin != nullptr) {
        delete m_begin;
        m_begin = nullptr;
    }
    if (m_end != nullptr) {
        delete m_end;
        m_end = nullptr;
    }
    if (m_logger != nullptr) {
        m_logger->releaseLogger(this);
    }
}