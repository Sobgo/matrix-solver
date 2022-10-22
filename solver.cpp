#include "./Structs.h"

Matrix inputEquations () {
    std::vector <std::string> equations;
    std::string temp;
    
    int count;
    std::cout << "How many equations? "; std::cin >> count; std::cin.ignore();

    std::cout << "Enter equations:\n";

    for (int i = 0; i < count; ++i) {
        getline(std::cin, temp);
        equations.push_back(temp);
    }

    return parseEquations(equations);
}

int main () {

    // 0 = coeficcients matrix, 1 = equations
    std::cout << "Choose input method [0 = coefficients matrix, 1 = equations]: ";
    int method; std::cin >> method;

    if (method == 0) {
        /**
         * Expected input:
         * Matrix size - two numbers n and m (horizontal then vertical size)
         * followed by n rows of m digits - coefficients
         * Example:
         * for:
         * 2z = 1
         * 2x + y - z = 1
         * x + 2y -z = 0
         * 3y + 3z = 1
         * input is:
         * 4 4
         * 0 0 2 1
         * 2 1 -1 1
         * 1 2 -1 0
         * 0 3 3 1
        **/
        int n, m; std::cin >> n >> m;
        Matrix matrix(n,m);
        std::cout << "\nInput:\n"; matrix.print(); 
        matrix.solve(true);
    }
    
    else if (method == 1) {
        /**
         * Expected input:
         * n - number of equations
         * followed by n equations
         * Example:
         * 4
         * 2z = 1
         * 2x + y - z = 1
         * x + 2y - z = 0
         * 3y + 3z = 1
        **/
        Matrix matrix = inputEquations();
        std::cout << "\nInput:\n"; matrix.print(); 
        matrix.solve(true);
       
        // prints matrix in the form of equations
        std::cout << "\nSolution:\n"; matrix.printEquations();
    }

    return 0;
}
