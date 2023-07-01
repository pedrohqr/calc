#include <iostream>
#include <stdexcept>
#include "Matrix.h"

int main() {
    // Cria uma matriz 3x3 de inteiros
    //Matrix<int> matrix(3, 3);
    math::IntMatrix matrix(2, 2);
    math::IntMatrix matrix2(2, 2);

    // Atribui valores à matriz
    matrix(0, 0) = 1;
    matrix(0, 1) = 2;
    matrix(1, 0) = 3;
    matrix(1, 1) = 4;
    matrix2(0, 0) = 5;
    matrix2(0, 1) = 6;
    matrix2(1, 0) = 7;
    matrix2(1, 1) = 8;

    // Acessa e imprime os valores da matriz
    matrix = matrix * matrix2;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::cout << matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
