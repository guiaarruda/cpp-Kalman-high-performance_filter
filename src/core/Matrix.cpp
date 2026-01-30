#include"Matrix.hpp"

Matrix::Matrix(size_t r, size_t c, double initial_value)
    : rows(r), columns(c), data(r * c, initial_value) {}

double& Matrix::operator()(size_t row, size_t column){
    if(row >= rows || column >= columns) {
        throw std::out_of_range("Matrix index out of range");
    }
    //2D -> 1D
    return data[row * columns + column];
}

const double& Matrix::operator()(size_t row, size_t column) const{
    if(row >= rows || column >= columns){
        throw std::out_of_range("Matrix index out of range");
    }
    return data[row * columns + column];
}

void Matrix::print() const {
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < columns; j++){
            std::cout << (*this)(i, j) << " ";
        }
        std::cout << '\n';
    }
}

Matrix Matrix::operator*(const Matrix& other) const {
    if(columns != other.rows){
        throw std::invalid_argument("Matrix dimensions are incompatible");
    }


    Matrix result(rows, other.columns, 0.0);

    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < other.columns; j++){
            for(size_t k = 0; k < columns; k++){
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if(rows != other.rows || columns != other.columns){
        throw std::invalid_argument("Matrix dimensions doesn't match!");
    }
    Matrix result(rows, columns);
    for(size_t i = 0; i < rows * columns; i++){
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}
Matrix Matrix::operator-(const Matrix& other) const {
    if(rows != other.rows || columns != other.columns){
        throw std::invalid_argument("Matrix dimensions doesn't match!");
    }
    Matrix result(rows, columns);
    for(size_t i = 0; i < rows * columns; i++){
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}

Matrix Matrix::transpose() const{
    Matrix result(columns, rows);

    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < columns; j++){
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}
