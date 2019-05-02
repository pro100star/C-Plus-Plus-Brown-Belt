#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

struct Data {
    Record ptr;
    multimap<int, const Record*>::iterator first;
    multimap<string, const Record*>::iterator second;
    multimap<int, const Record*>::iterator third;
};

class Database {
public:
    bool Put(const Record& record) {
        auto [it, isSuccess] = id_s.insert({record.id, Data{record, {}, {}, {}}});
        if (!isSuccess) {
            return false;
        }
        auto& data = it->second;
        const Record* ptr = &data.ptr;
        data.first = timestamps.insert({record.timestamp, ptr});
        data.second = users.insert({record.user, ptr});
        data.third = karmas.insert({record.karma, ptr});
        return true;
    }

    const Record* GetById(const string& id) const {
        auto it = id_s.find(id);
        return it == id_s.end() ? nullptr : &it->second.ptr;
    }

    bool Erase(const string& id) {
        auto it = id_s.find(id);
        if (it == id_s.end()) {
            return false;
        }
        const auto& data = it->second;
        timestamps.erase(data.first);
        users.erase(data.second);
        karmas.erase(data.third);
        id_s.erase(it);
        return true;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        if (low > high) {
            return;
        }
        auto it_begin = timestamps.lower_bound(low);
        auto it_end = timestamps.upper_bound(high);
        for (auto it = it_begin; it != it_end; ++it) {
            if (!callback(*it->second)) {
                break;
            }
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        if (low > high) {
            return;
        }
        auto it_begin = karmas.lower_bound(low);
        auto it_end = karmas.upper_bound(high);
        for (auto it = it_begin; it != it_end; ++it) {
            if (!callback(*it->second)) {
                break;
            }
        }
    }

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        auto [it, end] = users.equal_range(user);
        while (it != end && callback(*it->second)) {
            ++it;
        }
    }

private:
    template<typename Type>
    using Index = multimap<Type, const Record*>;

    unordered_map<string, Data> id_s;
    Index<int> timestamps;
    Index<string> users;
    Index<int> karmas;
};
