#pragma once
#include <cstddef>
#include <memory>

template <typename T> class MyVector {
  public:
    using Alloc = std::allocator<T>;
    using Traits = std::allocator_traits<Alloc>;

    MyVector() : begin_(nullptr), end_(nullptr), cap_(nullptr) {}
    MyVector(const MyVector& other) : begin_(nullptr), end_(nullptr), cap_(nullptr) {
        size_t capacity = static_cast<size_t>(other.cap_ - other.begin_);

        begin_ = alloc.allocate(capacity);
        end_ = begin_;
        cap_ = begin_ + capacity;

        try {
            for (T* p = other.begin_; p != other.end_; ++p, ++end_) {
                Traits::construct(alloc, end_, *p);
            }
        } catch (...) {
            destroy_storage();
            throw;
        }
    }

    MyVector(MyVector&& other) noexcept {
        begin_ = other.begin_;
        end_ = other.end_;
        cap_ = other.cap_;
        other.begin_ = other.end_ = other.cap_ = nullptr;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        if (begin_) {
            for (T* p = begin_; p != end_; ++p)
                Traits::destroy(alloc, p);
            alloc.deallocate(begin_, cap_ - begin_);
        }

        begin_ = other.begin_;
        end_ = other.end_;
        cap_ = other.cap_;
        other.begin_ = other.end_ = other.cap_ = nullptr;

        return *this;
    }


    MyVector& operator=(const MyVector& other) {
        if (&other == this) {
            return *this;
        }

        MyVector temp(other);
        swap(temp);
        return *this;
    }


    ~MyVector() { destroy_storage(); }

    T& operator[](size_t index) { return *(begin_ + index); }
    const T& operator[](size_t index) const { return *(begin_ + index); }
    void push_back(const T& element) {
        if (end_ == cap_) {
            allocate_more_storage();
        }
        Traits::construct(alloc, end_, element);
        ++end_;
    }

    void push_back(T&& element) {
        if (end_ == cap_)
            allocate_more_storage();
        Traits::construct(alloc, end_, std::move(element));
        ++end_;
    }

    void pop_back() {
        if (end_ != begin_) {
            end_--;
            Traits::destroy(alloc, end_);
        }
    }
    bool empty() const noexcept { return size() == 0; }

    size_t size() const noexcept { return begin_ ? static_cast<size_t>(end_ - begin_) : 0; }

    size_t capacity() const noexcept { return begin_ ? static_cast<size_t>(cap_ - begin_) : 0; }

    void reserve(size_t reserve_cap) {
        size_t old_cap = capacity();
        if (reserve_cap <= old_cap)
            return;

        T* new_begin = alloc.allocate(reserve_cap);
        T* new_end = new_begin;

        try {
            for (T* p = begin_; p != end_; ++p, ++new_end) {
                Traits::construct(alloc, new_end, std::move_if_noexcept(*p));
            }
        } catch (...) {
            for (T* p = new_begin; p != new_end; ++p) {
                Traits::destroy(alloc, p);
            }
            alloc.deallocate(new_begin, reserve_cap);
            throw;
        }

        for (T* p = begin_; p != end_; ++p) {
            Traits::destroy(alloc, p);
        }
        if (begin_) {
            alloc.deallocate(begin_, old_cap);
        }

        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + reserve_cap;
    }


  private:
    Alloc alloc;
    T* begin_;
    T* end_;
    T* cap_;

    void swap(MyVector& other) noexcept {
        std::swap(begin_, other.begin_);
        std::swap(end_, other.end_);
        std::swap(cap_, other.cap_);
    }

    void allocate_more_storage() {
        size_t new_cap = capacity() ? capacity() * 2 : 1;
        reserve(new_cap);
    }

    void destroy_storage() {
        if (begin_ == nullptr)
            return;
        for (T* p = begin_; p != end_; ++p) {
            Traits::destroy(alloc, p);
        }
        alloc.deallocate(begin_, cap_ - begin_);
        begin_ = end_ = cap_ = nullptr;
    }
};
