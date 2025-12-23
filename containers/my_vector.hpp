#pragma once
#include <cstddef>
#include <memory>

constexpr size_t DEFAULT_SIZE = 10;

template <typename T> class MyVector {
  public:
    MyVector() : begin_(alloc.allocate(DEFAULT_SIZE)), end_(begin_), cap_(begin_ + DEFAULT_SIZE) {}
    MyVector(const MyVector& other) {
        size_t size = other.end_ - other.begin_;
        size_t capacity = other.cap_ - other.begin_;

        begin_ = alloc.allocate(capacity);
        end_ = begin_ + size;
        cap_ = begin_ + capacity;
        std::copy(other.begin_, other.end_, begin_);
    }

    MyVector(MyVector&& other) {
        begin_ = other.begin_;
        end_ = other.end_;
        cap_ = other.cap_;
        other.begin_ = other.end_ = other.cap_ = nullptr;
    }

    MyVector& operator=(MyVector&& other) {
        if (&other == this) {
            return *this;
        }
        if (begin_) {
            for (T* p = begin_; p != end_; ++p)
                std::allocator_traits<decltype(alloc)>::destroy(alloc, p);
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
        std::allocator_traits<decltype(alloc)>::construct(alloc, end_, element);
        ++end_;
    }

    void push_back(T&& element) {
        if (end_ == cap_)
            allocate_more_storage();
        std::allocator_traits<decltype(alloc)>::construct(alloc, end_, std::move(element));
        ++end_;
    }

    void pop_back() {
        if (end_ != begin_) {
            end_--;
            std::allocator_traits<decltype(alloc)>::destroy(alloc, end_);
        }
    }
    bool empty() const { return size() == 0; }

    size_t size() const {
        if (begin_ == end_) {
            return 0;
        }
        return (end_ - begin_);
    }

    size_t capacity() const noexcept {
        if (!begin_) {
            return 0;
        }

        return static_cast<size_t>(cap_ - begin_);
    }

    void reserve(size_t reserve_cap) {
        size_t old_cap = capacity();
        if (reserve_cap <= old_cap)
            return;

        T* new_begin = alloc.allocate(reserve_cap);
        T* new_end = new_begin;

        try {
            for (T* p = begin_; p != end_; ++p, ++new_end) {
                std::allocator_traits<decltype(alloc)>::construct(alloc, new_end,
                                                                  std::move_if_noexcept(*p));
            }
        } catch (...) {
            for (T* p = new_begin; p != new_end; ++p) {
                std::allocator_traits<decltype(alloc)>::destroy(alloc, p);
            }
            alloc.deallocate(new_begin, reserve_cap);
            throw;
        }

        for (T* p = begin_; p != end_; ++p) {
            std::allocator_traits<decltype(alloc)>::destroy(alloc, p);
        }
        if (begin_) {
            alloc.deallocate(begin_, old_cap);
        }

        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + reserve_cap;
    }


  private:
    std::allocator<T> alloc;
    T* begin_;
    T* end_;
    T* cap_;

    void swap(MyVector& other) noexcept {
        std::swap(begin_, other.begin_);
        std::swap(end_, other.end_);
        std::swap(cap_, other.cap_);
    }

    void allocate_more_storage() {
        size_t old_capacity = cap_ - begin_;
        size_t new_capacity = old_capacity * 2;

        T* new_begin = alloc.allocate(new_capacity);
        T* new_end = new_begin + (end_ - begin_);
        T* new_cap = new_begin + new_capacity;

        for (T* p = begin_; p != end_; ++p)
            std::allocator_traits<decltype(alloc)>::construct(alloc, new_begin + (p - begin_),
                                                              std::move(*p));

        for (T* p = begin_; p != end_; ++p)
            std::allocator_traits<decltype(alloc)>::destroy(alloc, p);
        alloc.deallocate(begin_, old_capacity);

        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_cap;
    }

    void destroy_storage() {
        if (begin_ == nullptr)
            return;
        for (T* p = begin_; p != end_; ++p) {
            std::allocator_traits<decltype(alloc)>::destroy(alloc, p);
        }
        alloc.deallocate(begin_, cap_ - begin_);
        begin_ = end_ = cap_ = nullptr;
    }
};
