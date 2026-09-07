#pragma once
#include <atomic>
#include <concepts>
#include <cstddef>
#include <functional>
#include <utility>

template <typename T> class MySharedPtr {
  public:
    template <typename... Args>
        requires(!(sizeof...(Args) == 1 && (std::same_as<std::decay_t<Args>, MySharedPtr> && ...)))
    MySharedPtr(Args&&... args) {
        ptr_ = new T(std::forward<Args>(args)...);
        ctrl_ = new ControlBlock(); // strong_count = 1
    }
    MySharedPtr(const MySharedPtr& other) {
        ptr_ = other.ptr_;
        ctrl_ = other.ctrl_;
        if (ctrl_)
            ctrl_->strong_count++;
    }
    MySharedPtr(MySharedPtr&& other) noexcept : ptr_(other.ptr_), ctrl_(other.ctrl_) {
        other.ptr_ = nullptr;
        other.ctrl_ = nullptr;
    }
    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ctrl_ = other.ctrl_;
            if (ctrl_)
                ctrl_->strong_count++;
        }
        return *this;
    }
    MySharedPtr& operator=(MySharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ctrl_ = other.ctrl_;
            other.ptr_ = nullptr;
            other.ctrl_ = nullptr;
        }
        return *this;
    }
    ~MySharedPtr() { release(); }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    size_t use_count() const { return ctrl_ ? ctrl_->strong_count.load() : 0; }

  private:
    struct ControlBlock {
        std::atomic<size_t> strong_count{1};
        std::function<void(T*)> deleter;
    };
    void release() {
        if (ctrl_) {
            if (--ctrl_->strong_count == 0) {
                if (ctrl_->deleter)
                    ctrl_->deleter(ptr_);
                else
                    delete ptr_;
                delete ctrl_;
            }
        }
    }
    T* ptr_;
    ControlBlock* ctrl_;
};
