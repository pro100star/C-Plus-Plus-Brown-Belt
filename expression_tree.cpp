#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

string Print(const Expression* e) {
    if (!e) {
        return "Null expression provided";
    }
    stringstream output;
    output << e->ToString() << " = " << e->Evaluate();
    return output.str();
}

class ValueExpr : public Expression {
private:
    int value;

public:
    ValueExpr(int val) {
        value = val;
    }

    int Evaluate() const override {
        return value;
    }

    string ToString() const override {
        return to_string(value);
    }
};

class BinaryExpr : public Expression {
protected:
    ExpressionPtr left;
    ExpressionPtr right;
};

class SumExpr : public BinaryExpr {
public:
    SumExpr(ExpressionPtr l, ExpressionPtr r) {
        left = move(l);
        right = move(r);
    }

    int Evaluate() const override {
        return left->Evaluate() + right->Evaluate();
    }

    string ToString() const override {
        return '(' + left->ToString() + ')' + '+' + '(' + right->ToString() + ')';
    }
};

class ProductExpr : public BinaryExpr {
public:
    ProductExpr(ExpressionPtr l, ExpressionPtr r) {
        left = move(l);
        right = move(r);
    }

    int Evaluate() const override {
        return left->Evaluate() * right->Evaluate();
    }

    string ToString() const override {
        return  '(' + left->ToString() + ')' + '*' + '(' + right->ToString() + ')';
    }
};

ExpressionPtr Value(int value) {
    return make_unique<ValueExpr>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<SumExpr>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<ProductExpr>(move(left), move(right));
}
