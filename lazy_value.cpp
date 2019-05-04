#include <functional>
#include <string>
using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init) : init_(move(init)) {}

    bool HasValue() const {
        return value.has_value();
    }

    const T& Get() const {
        if (!value) {
            value = init_();
        }
        return *value;
    }

private:
    function<T()> init_;
    mutable optional<T> value;
};
