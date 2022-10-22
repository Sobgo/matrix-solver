#include <iostream>
#include <vector>
#include <string>

class Fraction {
    public:
        int numerator;
        int denominator;

        Fraction(int num = 0, int den = 1);

        void shorten();

        void invert();

        bool isZero();

        int getPrintLength();

        Fraction operator * (Fraction f);
        Fraction operator + (Fraction f);
        bool operator == (Fraction f);
};

std::ostream &operator << (std::ostream &os, Fraction const &f);

class Matrix {
    private:
        int m;
        int n;
        std::vector < std::vector <Fraction> > matrix;
        std::vector <std::string> names;

    public: 
        Matrix(int n = 0, int m = 0);

        void addRow(std::vector <Fraction> row);

        void addNames(std::vector <std::string> name);

        void print();

        void printEquations();

        void solve(bool output = false);
};

struct Term {
    std::string coefficient;
    std::string name;
};

Matrix parseEquations(std::vector <std::string> in);
