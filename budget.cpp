#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

class Date {
public:
    Date() = default;
    Date(int year, int month, int day): year_(year), month_(month), day_(day){}

    time_t AsTimestamp() const;

private:
    int day_, month_, year_;
};

time_t Date::AsTimestamp() const {
    std::tm t;
    t.tm_sec   = 0;
    t.tm_min   = 0;
    t.tm_hour  = 0;
    t.tm_mday  = day_;
    t.tm_mon   = month_ - 1;
    t.tm_year  = year_ - 1900;
    t.tm_isdst = 0;
    return mktime(&t);
}

istream& operator>>(istream &is, Date& date) {
    int year, month, day;

    is >> year;
    is.ignore(1);

    if (is.peek() == '0') {
        is.ignore(1);
    }
    is >> month;
    is.ignore(1);

    if (is.peek() == '0') {
        is.ignore(1);
    }
    is >> day;

    date = Date(year, month, day);
    return is;
}

int ComputeDaysDiff(const Date& date_from, const Date& date_to) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

class Budget {
private:
    struct Income {
        Income(double income_, double expense_) : income(income_), expense(expense_) {}

        double income;
        double expense;
    };

    vector<Income> income;
    const Date BEGIN_DATE;

public:
    Budget() : BEGIN_DATE(Date(2000, 1, 1)) {
        int hundred_year = ComputeDaysDiff(BEGIN_DATE, Date(2100, 1, 1)) + 1;
        income = vector<Income>(hundred_year, Income(0, 0));
    }

    double ComputeIncome(const Date &from, const Date &to) const {
        double sum = 0;
        int begin = ComputeDaysDiff(BEGIN_DATE, from);
        int end = ComputeDaysDiff(BEGIN_DATE, to);
        for (int i = begin; i <= end; ++i) {
            sum += income[i].income - income[i].expense;
        }
        return sum;
    }

    void Earn(const Date& from, const Date& to, size_t value) {
        int begin = ComputeDaysDiff(BEGIN_DATE, from);
        int end = ComputeDaysDiff(BEGIN_DATE, to);
        int diff = end - begin + 1;
        for (int i = begin; i <= end; ++i) {
            income[i].income += value * 1.0 / diff;
        }
    }

    void PayTax(const Date &from, const Date &to, size_t percentage) {
        int begin = ComputeDaysDiff(BEGIN_DATE, from);
        int end = ComputeDaysDiff(BEGIN_DATE, to);
        int diff = end - begin + 1;
        for (int i = begin; i <= end; ++i) {
            income[i].income *= (100 - percentage) / 100.0;
        }
    }

    void Spend(const Date &from, const Date &to, size_t value) {
        int begin = ComputeDaysDiff(BEGIN_DATE, from);
        int end = ComputeDaysDiff(BEGIN_DATE, to);
        int diff = end - begin + 1;
        for (int i = begin; i <= end; ++i) {
            income[i].expense += value * 1.0 / diff;
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    cout.precision(25);

    const string COMPUTE_INCOME = "ComputeIncome";
    const string EARN = "Earn";
    const string PAY_TAX = "PayTax";
    const string SPEND = "Spend";

    Budget budget;

    size_t q;
    cin >> q;
    for (size_t i = 0; i < q; ++i) {
        string query;
        cin >> query;

        Date from, to;
        cin >> from >> to;

        if (query == COMPUTE_INCOME) {
            cout << budget.ComputeIncome(from, to) << '\n';
        } else if (query == EARN) {
            size_t value;
            cin >> value;
            budget.Earn(from, to, value);
        } else if (query == PAY_TAX) {
            size_t percentage;
            cin >> percentage;
            budget.PayTax(from, to, percentage);
        } else if (query == SPEND) {
            size_t value;
            cin >> value;
            budget.Spend(from, to, value);
        }
    }

    return 0;
}
