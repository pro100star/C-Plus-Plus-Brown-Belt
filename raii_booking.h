#pragma once

namespace RAII {
    template <typename Provider>
    class Booking {
    private:
        Provider* provider;

    public:
        Booking(Provider* pr, int) : provider(pr) {}

        Booking(Booking&& other) : provider(other.provider) {
            other.provider = nullptr;
        }

        Booking&operator= (const Booking&) = delete;

        Booking&operator= (Booking&& other) {
            provider = other.provider;
            other.provider = nullptr;
            return *this;
        }

        ~Booking() {
            if (provider) {
                provider->CancelOrComplete(*this);
            }
        }
    };
}
