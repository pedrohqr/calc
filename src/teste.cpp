#include <iostream>
#include <stdexcept>
#include "Matrix.h"

int main() {
    // Cria uma matriz 3x3 de inteiros
    //Matrix<int> matrix(3, 3);
    math::IntMatrix matrix(3, 3);

    // Atribui valores à matriz
    matrix(0, 0) = 1;
    matrix(0, 1) = 2;
    matrix(0, 2) = 3;
    matrix(1, 0) = 4;
    matrix(1, 1) = 5;
    matrix(1, 2) = 6;
    matrix(2, 0) = 7;
    matrix(2, 1) = 8;
    matrix(2, 2) = 9;

    // Acessa e imprime os valores da matriz
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
