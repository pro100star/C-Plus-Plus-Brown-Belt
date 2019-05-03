#include "ini.h"
#include <iostream>

using namespace Ini;
using namespace std;

Section& Document::AddSection(string name) {
    return sections[move(name)];
}

const Section& Document::GetSection(const string &name) const {
    return sections.at(name);
}

size_t Document::SectionCount() const {
    return sections.size();
}

void Document::AddSection(std::string &name, Ini::Section &section) {
    sections[move(name)] = move(section);
}

Document Ini::Load(istream& input) {
    Document document;
    string s;
    string name;
    unordered_map<string, string> section;
    bool flag = false;
    while (getline(input, s)) {
        if (s.empty()) {
            continue;
        }
        if (s.front() == '[' && s.back() == ']') {
            if (flag) {
                document.AddSection(name, section);
            }
            name = s.substr(1, s.size() - 2);
            flag = true;
        } else {
            size_t index = s.find('=');
            string key = s.substr(0, index);
            string value = s.substr(index + 1, string::npos);
            section[move(key)] = move(value);
        }
    }
    if (flag) {
        document.AddSection(name, section);
    }
    return document;
}
