#include <algorithm>
#include <forward_list>
#include <iterator>
#include <vector>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
    using BucketList = forward_list<Type>;

public:
    explicit HashSet(
            size_t num_buckets,
            const Hasher& hasher = {}
    ) : hasher_(hasher), count(num_buckets) {
        buckets = vector<BucketList>(count);
    }

    void Add(const Type& value) {
        if (!Has(value)) {
            size_t num = hasher_(value) % count;
            auto& bucket = buckets[num];
            if (bucket.empty()) {
                bucket.push_front(value);
            } else {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    auto tmp = it;
                    if (++tmp == bucket.end()) {
                        bucket.insert_after(it, value);
                        return;
                    }
                }
            }
        }
    }

    bool Has(const Type& value) const {
        size_t num = hasher_(value) % count;
        return find(buckets[num].begin(), buckets[num].end(), value) != buckets[num].end();
    }

    void Erase(const Type& value) {
        size_t num = hasher_(value) % count;
        buckets[num].remove(value);
    }

    const BucketList& GetBucket(const Type& value) const {
        size_t num = hasher_(value) % count;
        return buckets[num];
    }

private:
    Hasher hasher_;
    size_t count;
    vector<BucketList> buckets;
};
