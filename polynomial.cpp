class IndexProxy {
    public:
        IndexProxy(Polynomial& poly_, size_t index_) : index(index_), poly(poly_) {}

        bool operator == (const T& value) const {
            if (poly.Degree() >= index) {
                return as_const(poly)[index] == value;
            }
            return value == 0;
        }

        IndexProxy& operator = (T value) {
            if (index > poly.Degree() && value != 0) {
                poly.coeffs_.resize(index + 1);
            }
            poly.coeffs_[index] = move(value);
            poly.Shrink();
            return *this;
        }

        friend ostream& operator << (ostream& os, IndexProxy& ip) {
            if (ip.poly.Degree() >= ip.index) {
                os << as_const(ip.poly)[ip.index];
            } else {
                os << 0;
            }
            return os;
        }

        size_t index;
        Polynomial<T>& poly;

        operator T() const {
            return as_const(poly)[index];
        }
    };
    
