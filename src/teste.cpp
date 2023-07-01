#include <iostream>
#include <vector>

template<typename T>
class Matrix {
public:
    Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows* cols) {}

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const std::vector<T>& getData() const { return data; }

    // Implementação do operador de adição para somar duas matrizes do mesmo tipo
    Matrix<T> operator+(const Matrix<T>& other) const {
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; i++) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    // Sobrecarga do operador de adição para somar uma Matrix<int> com uma Matrix<float>
    template<typename U>
    Matrix<float> operator+(const Matrix<U>& other) const {
        Matrix<float> result(rows, cols);
        for (int i = 0; i < rows * cols; i++) {
            result.data[i] = static_cast<float>(data[i]) + static_cast<float>(other.data[i]);
        }
        return result;
    }

    T operator()(int row, int col) const {
        return data[row * cols + col];
    }

    // Outras funções e operadores
    std::vector<T> data;
private:
    int rows;
    int cols;
    
};

int main() {
    Matrix<int> matrix1(2, 2);
    Matrix<float> matrix2(2, 2);

    // Preencha as matrizes matrix1 e matrix2 com os valores desejados

    // Soma de uma Matrix<int> com uma Matrix<float>
    Matrix<float> result = matrix1 + matrix2;

    // Imprimir o resultado
    for (int i = 0; i < result.getRows(); i++) {
        for (int j = 0; j < result.getCols(); j++) {
            std::cout << result(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
