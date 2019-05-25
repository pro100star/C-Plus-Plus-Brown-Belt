#include "test_runner.h"

#include <cstddef>

using namespace std;

template <typename T>
class UniquePtr {
private:
    T* object = nullptr;

public:
    UniquePtr() = default;

    UniquePtr(T * ptr) {
        object = ptr;
    }

    UniquePtr(const UniquePtr&) = delete;

    T * Release() {
        T* result = object;
        object = nullptr;
        return result;
    }


    UniquePtr(UniquePtr&& other) {
        object = other.Release();
    }

    UniquePtr& operator = (const UniquePtr&) = delete;

    UniquePtr& operator = (nullptr_t) {
        if (object) {
            delete object;
        }
        object = nullptr;
        return *this;
    }

    UniquePtr& operator = (UniquePtr&& other) {
        if (object) {
            delete object;
        }
        object = other.Release();
        return *this;
    }

    ~UniquePtr() {
        delete object;
    }

    T& operator * () const {
        return *object;
    }

    T * operator -> () const {
        return object;
    }

    void Reset(T * ptr) {
        if (object) {
            delete object;
        }
        object = ptr;
    }

    void Swap(UniquePtr& other) {
        swap(*this, other);
    }

    T * Get() const {
        return object;
    }
};
