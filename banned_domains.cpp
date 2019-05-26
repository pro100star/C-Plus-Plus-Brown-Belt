#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

bool IsPrefix(const string& lhs, const string& rhs) {
    if (lhs.size() > rhs.size()) {
        return false;
    }
    size_t i = 0;
    while (i < lhs.size()) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
        ++i;
    }
    return i == rhs.size() || rhs[i] == '.';
}

vector<string> ReadDomains() {
    size_t count;
    cin >> count;
    string s;
    getline(cin, s);

    vector<string> domains;
    for (size_t i = 0; i < count; ++i) {
        string domain;
        getline(cin, domain);
        domains.push_back(domain);
    }
    return domains;
}


int main() {
    vector<string> banned_domains = ReadDomains();
    vector<string> domains_to_check = ReadDomains();

    for (string& s : banned_domains) {
        reverse(s.begin(), s.end());
    }

    sort(banned_domains.begin(), banned_domains.end());
    banned_domains.erase(unique(banned_domains.begin(), banned_domains.end(), IsPrefix), banned_domains.end());
    reverse(begin(banned_domains), end(banned_domains));

    for (string& s : domains_to_check) {
        reverse(s.begin(), s.end());
        auto it = lower_bound(banned_domains.begin(), banned_domains.end(), s, greater<>());
        if (it != banned_domains.end() && IsPrefix(*it, s)) {
            cout << "Bad" << endl;
        } else {
            cout << "Good" << endl;
        }
    }
    return 0;
}
