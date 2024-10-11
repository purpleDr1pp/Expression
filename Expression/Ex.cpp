#include <iostream>
#include <memory>
#include <string>

class Expression {
public:
    virtual int Evaluate() const = 0;
    virtual std::string ToString() const = 0;
    virtual ~Expression() {}
};

using ExpressionPtr = std::shared_ptr<Expression>;

class ConstExpr : public Expression {
private:
    int num;
public:
    ConstExpr(int(n)) : num(n) {};
    std::string ToString() const override {
        return std::to_string(num);
    }
    int Evaluate() const override {
        return num;
    }
};


class BinaryOperation : public Expression {
protected:
    ExpressionPtr num1, num2;
public:
    BinaryOperation(ExpressionPtr n1, ExpressionPtr n2) : num1(n1), num2(n2) {};
};

class SumExpr : public BinaryOperation {
public:
    SumExpr(ExpressionPtr l, ExpressionPtr r) : BinaryOperation(l, r) {}

    int Evaluate() const override {
        return num1->Evaluate() + num2->Evaluate();
    }

    std::string ToString() const override {
        return num1->ToString() + " + " + num2->ToString();
    }
};

class ProductExpr : public BinaryOperation {
private:
    static std::string Parentheses(ExpressionPtr ex) {
        if (dynamic_cast<SumExpr*>(ex.get())) {
            return std::string("(") + ex->ToString() + ")";
        }
        else {
            return ex->ToString();
        }
    }
public:
    ProductExpr(ExpressionPtr l, ExpressionPtr r) : BinaryOperation(l, r) {}

    int Evaluate() const override {
        return num1->Evaluate() * num2->Evaluate();
    }

    std::string ToString() const override {
        return Parentheses(num1) + " * " + Parentheses(num2);
    }

};

ExpressionPtr Const(int x) {
    return ExpressionPtr(new ConstExpr(x));
}

ExpressionPtr Sum(ExpressionPtr l, ExpressionPtr r) {
    return ExpressionPtr(new SumExpr(l, r));
}

ExpressionPtr Product(ExpressionPtr l, ExpressionPtr r) {
    return ExpressionPtr(new ProductExpr(l, r));
}


int main() {
    ExpressionPtr ex1 = Sum(Product(Const(3), Const(4)), Const(5));
    std::cout << ex1->ToString() << "\n";  // 3 * 4 + 5
    std::cout << ex1->Evaluate() << "\n";  // 17

    ExpressionPtr ex2 = Product(Const(6), ex1);
    std::cout << ex2->ToString() << "\n";  // 6 * (3 * 4 + 5)
    std::cout << ex2->Evaluate() << "\n";  // 102
}