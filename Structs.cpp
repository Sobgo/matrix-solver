#include <numeric> // gcd, lcm
#include <cmath> // log10
#include <algorithm> // sort
#include "./Structs.h"

Fraction::Fraction (int num, int den) {
    numerator = num;
    denominator = den;

    if (denominator == 0) {
        std::cerr << "Error: denominator is 0";
        exit(1);
    }
    
    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }

    shorten();
}

void Fraction::shorten () {
    int d = std::gcd(numerator, denominator);
    numerator /= d;
    denominator /= d;
}

void Fraction::invert () {
    if (numerator == 0) return;

    if (numerator < 0) {
        numerator *= -1;
        denominator*= -1;
    }

    std::swap(numerator, denominator);
}

// returns char count of cout << operator for this fraction
int Fraction::getPrintLength () {
    int length;
    if (numerator == 0) length = 1;
    else length = floor(log10(abs(numerator)) + 1);

    if (denominator != 1) length += 1 + floor(log10(denominator) + 1);
    if (numerator < 0) length += 1; // sign

    return length;
}

// same as this == Fraction(0, 1)
bool Fraction::isZero() {
    return numerator == 0;
}

// Fraction * Fraction
Fraction Fraction::operator * (Fraction f) {
    Fraction temp(numerator, denominator);
    
    temp.numerator *= f.numerator;
    temp.denominator *= f.denominator;

    temp.shorten();

    return temp;
}

// Fraction + Fraction
Fraction Fraction::operator + (Fraction f) {
    Fraction temp(numerator, denominator);

    int m = std::lcm(temp.denominator, f.denominator);

    temp.numerator *= (m / temp.denominator);
    f.numerator *= (m / f.denominator);

    temp.numerator += f.numerator;
    temp.denominator = m;

    temp.shorten();

    return temp;
}

// Fraction == Fraction
bool Fraction::operator == (Fraction f) {
    shorten();
    f.shorten();
    return (numerator == f.numerator && denominator == f.denominator);
}

std::ostream &operator << (std::ostream &os, Fraction const &f) { 
    if (f.denominator > 1) {
        return os << f.numerator << "/" << f.denominator;;
    }
    else {
        return os << f.numerator;;
    }
}

// ---------------- Matrix ----------------

Matrix::Matrix (int n, int m) {
    matrix.resize(m, std::vector <Fraction> (n));
    this->m = m;
    this->n = n;
    int t;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> t;
            matrix[i][j].numerator = t;
        }
    }
}

void Matrix::addRow (std::vector <Fraction> row) {
    m += 1;
    n = std::max((int)row.size(), n);
    matrix.push_back(row);
}

void Matrix::addNames (std::vector <std::string> names) {
    this->names = names;
}

void Matrix::print () {
    if (m < 1 || n < 1) {
        std::cout << "Cannot display empty matrix\n";
        return;
    }

    // calculations
    std::vector <int> rowLengths(n);

    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            if (matrix[j][i].getPrintLength() > rowLengths[i]) {
                rowLengths[i] = matrix[j][i].getPrintLength();
            }
        }
    }

    // names above matrix collumns
    if (names.size() != 0) {
        for (int i = 0; i < n; ++i) {
            int pos = (rowLengths[i] - names[i].size() + 4)/2;
            for (int j = 0; j < pos; ++j) {
                std::cout << " ";
            }
            std::cout << names[i];
            for (int j = pos + 1; j < rowLengths[i] + 3; ++j) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }

    // top
    std::cout << "+";
    for (int i = 0; i < n; ++i) { 
        std::cout << "-"; 
        for (int j = 0; j < rowLengths[i]; ++j) {
            std::cout << "-";
        }
        std::cout << "-+";
    }
    std::cout << "\n";

    // m rows
    for (int  j = 0; j < m; ++j) {
        
        std::cout << "|";

        if (n > 0) {
            std::cout << " " << matrix[j][0] << " ";
            for (int k = rowLengths[0] - matrix[j][0].getPrintLength(); k > 0; --k) {
                std::cout << " ";
            }
        }

        if (n > 1) {
            std::cout << " ";
        }

        for (int i = 1; i < n - 1; ++i) {
            std::cout << " " << matrix[j][i] << "  ";
            for (int k = rowLengths[i] - matrix[j][i].getPrintLength(); k > 0; --k) {
                std::cout << " ";
            }
        }

        if (n > 1) {
            std::cout << " " << matrix[j][n-1] << " ";
            for (int k = rowLengths[n-1] - matrix[j][n-1].getPrintLength(); k > 0; --k) {
                std::cout << " ";
            }
        }

        std::cout << "|\n";
    }

    // bottom
    std::cout << "+";
    for (int i = 0; i < n; ++i) { 
        std::cout << "-"; 
        for (int j = 0; j < rowLengths[i]; ++j) {
            std::cout << "-";
        }
        std::cout << "-+";
    }
    std::cout << "\n";
}

void Matrix::printEquations() {
    for (int j = 0; j < m; ++j) {
        // print first non zero
        int prev = n;
        bool first = true;
        for (int i = 0; i < n; ++i) {
            if (matrix[j][i].isZero()) continue;
            if (!(matrix[j][i] == Fraction(1, 1))) {
                std::cout << matrix[j][i];
            }
            std::cout << names[i] << " = ";
            prev = i;
            break;
        }
        for (int i = prev + 1; i < n; ++i) {
            if (names[i] != "") {
                if (matrix[j][i].isZero()) continue;

                if (!first) {
                    if (matrix[j][i].numerator < 0) {
                        std::cout << "+ ";
                        if (!(matrix[j][i] == Fraction(1, 1))) {
                            std::cout << matrix[j][i] * Fraction(-1, 1);
                        } 
                    }
                    else {
                        std::cout << "- ";
                        if (!(matrix[j][i] == Fraction(1, 1))) {
                            std::cout << matrix[j][i];
                        } 
                    }
                    std::cout << names[i] << " ";
                    first = false;
                }
                else {
                    std::cout << matrix[j][i] << names[i] << " ";
                }
            }
            else {
                if (prev != i - 1) {
                    if (matrix[j][i].numerator > 0) {
                        std::cout << "+ ";
                        if (!(matrix[j][i] == Fraction(1, 1))) {
                            std::cout << matrix[j][i];
                        } 
                    }
                    else {
                        std::cout << "- ";
                        if (!(matrix[j][i] == Fraction(1, 1))) {
                            std::cout << matrix[j][i] * Fraction(-1, 1);
                        } 
                    }
                    std::cout << names[i];
                }
                else {
                    std::cout << matrix[j][i] << names[i];
                }
            }
        }
        std::cout << "\n";
        first = true;
    }
}

/// @brief transforms matrix to row canonical form
/// @param output print steps to stdout
void Matrix::solve(bool output) {
    
    // part 1: transform to row echelon form
    // achived by gaussian elimination

    int y = 0; // vertical marker (current top)

    for (int i = 0; i < n; ++i) {
        for (int j = y; j < m; ++j) {
            // find first row with non zero value in column i
            if (!matrix[j][i].isZero()) {
                // swap it so it will be at positon y (top)
                swap(matrix[j], matrix[y]);
                
                // for all rows bellow if non zero value at position i
                for (int l = j + 1; l < m; ++l) {
                    if (!matrix[l][i].isZero()) {
                        // then calculate such number mp that when
                        // multiplied by value from y (top) row 
                        // and added to value at l position
                        // that value will become zero
                        Fraction mp = matrix[y][i]; mp.invert();
                        mp = mp * matrix[l][i] * Fraction(-1,1);
                        
                        // for all values in this row add mp multiplied by value form y row 
                        for (int k = i; k < n; ++k) {
                            matrix[l][k] = matrix[l][k] + mp * matrix[y][k];
                        }
                    }
                }
                y++;
                break;
            }
        }
    }

    if (output) {
        std::cout << "\nRow echelon form:\n"; print();
    }

    // part 2: reduce (reduced row echelon form)
    
    int x = -1; // horizontal marker (current first column)

    for (int j = 0; j < m; ++j) {
        for (int i = x + 1; i < n; ++i) {
            // for each row
            // find leading coefficient (pivot)
            if (!matrix[j][i].isZero()) {  
                x = i;
                // then calculate such number mp that when
                // leading coefficient is multiplied by it
                // it will become 1               
                Fraction mp = matrix[j][i]; mp.invert();
        
                // multiply all coeffcients in this row by mp
                for (int k = i; k < n; ++k) {
                    matrix[j][k] = matrix[j][k] * mp;
                }
                break;
            }
        }
    }

    if (output) {
        std::cout << "\nReduced row echelon form:\n"; print();
    }

    // part 3: strictly reduce (row canonical form)

    for (int j = m - 1; j > 0; --j) {
        for (int i = 0; i < n; ++i) {
            // find leading coeffcient in row
            // (this time from bottom to top)
            if (!matrix[j][i].isZero()) {  
                // then for each row above this row
                for (int l = j - 1; l >= 0; --l) {
                    // calculate such number mp that when
                    // added to value at l position
                    // that value will become zero
                    Fraction mp = matrix[l][i] * Fraction(-1,1);
                    // for each value in row l
                    // add mp multiplied by value from row j (bottom)
                    for (int k = i; k < n; ++k) {
                        matrix[l][k] = matrix[l][k] + matrix[j][k] * mp;
                    }
                }
                break;
            }
        }
    }

    if (output) {
        std::cout << "\nRow canonical form:\n"; print();
    }
}

// parse strings to vactors of terms and then to matrix
Matrix parseEquations(std::vector <std::string> in) {
    Matrix out;
    std::vector <std::string> names;
    std::vector < std::vector <Term> > grouped;
    std::vector <Term> temp;

    for (std::string &s : in) {
        Term t;

        // remove all spaces
        s.erase(remove_if(s.begin(), s.end(), [](unsigned char x){return isspace(x);}), s.end());

        // separate terms
        for (char &c : s) {
            if (isalpha(c)) {
                t.name += c;
            }
            else if (isdigit(c) || c == '/') {
                t.coefficient += c;
            }
            else if (c == '+' || c == '=' || c == '-') {
                if (t.coefficient != "" && t.coefficient != "-") {
                    temp.push_back(t);
                }
                else if (t.name != "") {
                    t.coefficient += "1";
                    temp.push_back(t);
                }

                t = {"", ""};

                if (c == '-') {
                    t.coefficient += c;
                }
            }
        }

        // process last term
        if (t.coefficient != ""  || t.coefficient != "-") {
            temp.push_back(t);
        }
        else if (t.name != "") {
            t.coefficient += "1";
            temp.push_back(t);
        }

        grouped.push_back(temp);

        for (Term term : temp) {
            // if term name is not in names, add it
            if (term.name != "" && find(names.begin(), names.end(), term.name) == names.end()) {
                names.push_back(term.name);
            }
        }

        temp.clear();

        // sort names lexicographically ex. x, y, z
        sort(names.begin(), names.end());
    }

    // sort each vector in grouped lexicographically by name
    for (std::vector <Term> &v : grouped) {
        sort(v.begin(), v.end(), [](Term a, Term b) {
            if (a.name == "") return false;
            if (b.name == "") return true;
            return a.name < b.name;
        });
    }

    // for each vector in grouped add all missing names from names with coefficient 0
    int iter;
    for (std::vector <Term> &v : grouped) {
        iter = 0;
        for (std::string &name : names) {
            if (v[iter].name != name) {
                v.insert(v.begin() + iter, { "0", name });
            }
            iter++;
        }
    }

    // convert to matrix
    for (std::vector <Term> v : grouped) {
        std::vector <Fraction> row;
        for (Term &t : v) {
            auto pos = t.coefficient.find('/');
            if (pos != std::string::npos) {
                row.push_back({
                    stoi(t.coefficient.substr(0, pos)),
                    stoi(t.coefficient.substr(pos + 1))
                });
            }
            else {
                row.push_back({
                    stoi(t.coefficient),
                    1
                });
            }
        }
        out.addRow(row);
    }
    // set last name to empty string to prevent seg fault
    names.push_back("");

    out.addNames(names);
    return out;
}