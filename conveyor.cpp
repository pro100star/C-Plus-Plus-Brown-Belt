#include "test_runner.h"
#include <deque>
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>
using namespace std;


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        throw logic_error("Unimplemented");
    }

protected:
    void PassOn(unique_ptr<Email> email) const {
        if (ptr != nullptr) {
            ptr->Process(move(email));
        }
    }

private:
    unique_ptr<Worker> ptr;

public:
    void SetNext(unique_ptr<Worker> next) {
        if (ptr != nullptr) {
            ptr->SetNext(move(next));
        } else {
            ptr = move(next);
        }
    }
};


class Reader : public Worker {
public:
    Reader(istream& input) : is(input) {}

    void Run() override {
        unique_ptr<Email> email;
        string from, to, body;
        string s;
        size_t k = 0;
        while (getline(is, s)) {
            if (k == 0) {
                from = move(s);
                ++k;
            } else if (k == 1) {
                to = move(s);
                ++k;
            } else {
                body = move(s);
                k = 0;
                email = move(make_unique<Email>(Email{from, to, body}));
                PassOn(move(email));
            }
        }
    }

    void Process(unique_ptr<Email> email) override {
        PassOn(move(email));
    }

private:
    istream& is;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    Filter(Function func) {
        f = move(func);
    }

private:
    Function f;

public:
    void Process(unique_ptr<Email> email) override {
        if (f(*email)) {
            PassOn(move(email));
        }
    }
};


class Copier : public Worker {
public:
    Copier(string s) {
        to = move(s);
    }

    void Process(unique_ptr<Email> email) override {
        if (email->to != to) {
            unique_ptr<Email> tmp(new Email{email->from, to, email->body});
            PassOn(move(email));
            PassOn(move(tmp));
        } else {
            PassOn(move(email));
        }
    }

private:
    string to;
};


class Sender : public Worker {
public:
    Sender(ostream& output) : os(output) {}

    void Process(unique_ptr<Email> email) override {
        os << email->from << '\n';
        os << email->to << '\n';
        os << email->body << '\n';
        PassOn(move(email));
    }

private:
    ostream& os;
};


class PipelineBuilder {
public:
    explicit PipelineBuilder(istream& in) : ptr(new Reader(in)) {
    }

    PipelineBuilder& FilterBy(Filter::Function filter) {
        ptr->SetNext(make_unique<Filter>(move(filter)));
        return *this;
    }

    PipelineBuilder& CopyTo(string recipient) {
        ptr->SetNext(make_unique<Copier>(move(recipient)));
        return *this;
    }

    PipelineBuilder& Send(ostream& out) {
        ptr->SetNext(make_unique<Sender>(out));
        return *this;
    }

    unique_ptr<Worker> Build() {
        return move(ptr);
    }

private:
    unique_ptr<Worker> ptr;
};
