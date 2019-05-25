#include "Common.h"
#include <algorithm>
#include <deque>
#include <mutex>
#include <unordered_map>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(
            shared_ptr<IBooksUnpacker> books_unpacker,
            const Settings& settings
    ) {
        this->settings = settings;
        this->books_unpacker = books_unpacker;
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> guard(m);
        auto it = names.find(book_name);
        if (it != names.end()) {
            books.erase(find(books.begin(), books.end(), it->second));
            books.push_back(it->second);
            return it->second;
        }
        /*
        auto it = find(books.begin(), books.end(), ptr);
        if (find(books.begin(), books.end(), ptr) != books.end()) {
            books.erase(it);
            books.push_back(ptr);
            return ptr;
        }*/
        auto ptr = BookPtr(books_unpacker->UnpackBook(book_name));
        size_t size_of_book = ptr->GetContent().size();
        if (size_of_book > settings.max_memory) {
            books.clear();
            names.clear();
            used_memory = 0;
            return ptr;
        }
        while (used_memory + size_of_book > settings.max_memory) {
            used_memory -= books.front()->GetContent().size();
            names.erase(books.front()->GetName());
            books.pop_front();
        }
        books.push_back(ptr);
        names[ptr->GetName()] = ptr;
        used_memory += size_of_book;
        return ptr;
    }

private:
    Settings settings;
    unordered_map<string, BookPtr> names;
    deque<BookPtr> books;
    shared_ptr<IBooksUnpacker> books_unpacker;
    size_t used_memory = 0;
    mutex m;
};


unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings& settings
) {
    return make_unique<LruCache>(books_unpacker, settings);
}
