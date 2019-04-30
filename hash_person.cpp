using namespace std;

struct Address {
    string city, street;
    int building;

    bool operator==(const Address& other) const {
        return building == other.building && city == other.city && street == other.street;
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;

    bool operator==(const Person& other) const {
        return height == other.height && weight == other.weight && name == other.name && address == other.address;
    }
};

struct AddressHasher {
    size_t operator()(const Address& obj) const {
        const size_t cnst = 37;
        size_t first = s_hash(obj.city) * cnst * cnst;
        size_t second = s_hash(obj.street) * cnst;
        size_t third = i_hash(obj.building);
        return first + second + third;
    }

    hash<string> s_hash;
    hash<int> i_hash;
};

struct PersonHasher {
    size_t operator()(const Person& obj) const {
        const size_t cnst = 37;
        size_t f = a_hash(obj.address) * cnst * cnst * cnst;
        size_t s = s_hash(obj.name) * cnst * cnst;
        size_t t = d_hash(obj.weight) * cnst;
        size_t c = i_hash(obj.height);
        return f + s + t + c;
    }

    hash<string> s_hash;
    hash<int> i_hash;
    hash<double> d_hash;
    AddressHasher a_hash;
};
