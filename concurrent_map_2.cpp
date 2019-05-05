#include <future>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
using namespace std;

template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentMap {
public:
    using MapType = unordered_map<K, V, Hash>;

    struct WriteAccess {
        lock_guard<mutex> guard;
        V& ref_to_value;
    };

    struct ReadAccess {
        lock_guard<mutex> guard;
        const V& ref_to_value;
    };

    explicit ConcurrentMap(size_t bucket_count) : maps(vector<pair<mutex, unordered_map<K, V, Hash>>>(bucket_count)) {}

    WriteAccess operator[](const K& key) {
        auto& p = maps[hasher(key) % maps.size()];
        return {lock_guard(p.first), p.second[key]};
    }

    ReadAccess At(const K& key) const {
        auto& p = maps[hasher(key) % maps.size()];
        if (auto it = p.second.find(key); it != p.second.end()) {
            return {lock_guard(p.first), it->second};
        }
        throw out_of_range("");
    }

    bool Has(const K& key) const {
        auto& p = maps[hasher(key) % maps.size()];
        lock_guard guard(p.first);
        return p.second.find(key) != p.second.end();
    }

    MapType BuildOrdinaryMap() const {
        MapType result;
        for (auto& p : maps) {
            for (const auto& [key, value] : p.second) {
                lock_guard guard(p.first);
                result[key] = value;
            }
        }
        return result;
    }

private:
    Hash hasher;
    mutable vector<pair<mutex, unordered_map<K, V, Hash>>> maps;
};
