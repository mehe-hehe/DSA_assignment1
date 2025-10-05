#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
using namespace std;

class Polynomial {
public:
    Polynomial();
    virtual ~Polynomial();
    void insertTerm(int coefficient, int exponent);
    string toString() const;
    Polynomial add(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;
    Polynomial derivative() const;
};

struct Node {
    int coefficient;
    int exponent;
    Node* next;
    Node(int c, int e) : coefficient(c), exponent(e), next(nullptr) {}
};

struct PolyData {
    Node* head;
    PolyData() : head(nullptr) {}
};

static PolyData pData[100];
static int polyCount = 0;

Polynomial::Polynomial() {
    polyCount++;
}

Polynomial::~Polynomial() {
    Node* temp = pData[polyCount - 1].head;
    while (temp != nullptr) {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }
    pData[polyCount - 1].head = nullptr;
}

void Polynomial::insertTerm(int coefficient, int exponent) {
    PolyData& data = pData[polyCount - 1];
    Node*& head = data.head;

    if (coefficient == 0 || exponent < 0) {
        cout << "Negative exponents or zero coefficients are invalid\n";
        return;
    }

    Node* newTerm = new Node(coefficient, exponent);

    if (head == nullptr) {
        head = newTerm;
        return;
    }

    if (newTerm->exponent > head->exponent) {
        newTerm->next = head;
        head = newTerm;
        return;
    }

    Node* temp = head;
    Node* prev = nullptr;

    while (temp != nullptr && temp->exponent > newTerm->exponent) {
        prev = temp;
        temp = temp->next;
    }

    if (temp != nullptr && temp->exponent == newTerm->exponent) {
        temp->coefficient += newTerm->coefficient;
        delete newTerm;

        if (temp->coefficient == 0) {
            if (prev != nullptr)
                prev->next = temp->next;
            else
                head = temp->next;
            delete temp;
        }
    } else {
        newTerm->next = temp;
        if (prev != nullptr)
            prev->next = newTerm;
        else
            head = newTerm;
    }
}

string Polynomial::toString() const {
    const PolyData& data = pData[polyCount - 1];
    Node* head = data.head;

    if (head == nullptr)
        return "0";

    stringstream ss;
    Node* temp = head;
    bool firstTerm = true;

    while (temp != nullptr) {
        int c = temp->coefficient;
        int e = temp->exponent;

        if (!firstTerm) {
            if (c > 0)
                ss << " + ";
            else
                ss << " - ";
        } else {
            if (c < 0)
                ss << "-";
        }

        int absCoeff = abs(c);

        if (e == 0)
            ss << absCoeff;
        else if (e == 1) {
            if (absCoeff != 1)
                ss << absCoeff;
            ss << "x";
        } else {
            if (absCoeff != 1)
                ss << absCoeff;
            ss << "x^" << e;
        }

        firstTerm = false;
        temp = temp->next;
    }

    return ss.str();
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial result;
    const PolyData& data1 = pData[polyCount - 2];
    const PolyData& data2 = pData[polyCount - 1];

    Node* p1 = data1.head;
    Node* p2 = data2.head;

    while (p1 != nullptr || p2 != nullptr) {
        if (p1 != nullptr && (p2 == nullptr || p1->exponent > p2->exponent)) {
            result.insertTerm(p1->coefficient, p1->exponent);
            p1 = p1->next;
        } else if (p2 != nullptr && (p1 == nullptr || p2->exponent > p1->exponent)) {
            result.insertTerm(p2->coefficient, p2->exponent);
            p2 = p2->next;
        } else {
            result.insertTerm(p1->coefficient + p2->coefficient, p1->exponent);
            p1 = p1->next;
            p2 = p2->next;
        }
    }

    return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial result;
    const PolyData& data1 = pData[polyCount - 2];
    const PolyData& data2 = pData[polyCount - 1];

    for (Node* p1 = data1.head; p1 != nullptr; p1 = p1->next) {
        for (Node* p2 = data2.head; p2 != nullptr; p2 = p2->next) {
            int newCoeff = p1->coefficient * p2->coefficient;
            int newExp = p1->exponent + p2->exponent;
            result.insertTerm(newCoeff, newExp);
        }
    }
    return result;
}

Polynomial Polynomial::derivative() const {
    Polynomial result;
    const PolyData& data = pData[polyCount - 1];
    Node* temp = data.head;

    while (temp != nullptr) {
        if (temp->exponent > 0) {
            int newCoeff = temp->coefficient * temp->exponent;
            int newExp = temp->exponent - 1;
            result.insertTerm(newCoeff, newExp);
        }
        temp = temp->next;
    }
    return result;
}

int main() {
    Polynomial p1;
    Polynomial p2;

    p1.insertTerm(3, 4);
    p1.insertTerm(2, 2);
    p1.insertTerm(-1, 1);
    p1.insertTerm(5, 0);

    p2.insertTerm(1, 4);
    p2.insertTerm(1, 0);

    cout << "p1: " << p1.toString() << endl;
    cout << "p2: " << p2.toString() << endl;

    Polynomial sum = p1.add(p2);
    cout << "sum: " << sum.toString() << endl;

    Polynomial p3;
    p3.insertTerm(2, 1);

    Polynomial prod = sum.multiply(p3);
    cout << "prod: " << prod.toString() << endl;

    Polynomial deriv = p1.derivative();
    cout << "deriv: " << deriv.toString() << endl;

    return 0;
}
