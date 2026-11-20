#include"Matrix.hpp"
#include<cmath>

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

    const size_t n = other.columns;
    Matrix result(rows, n, 0.0);

    // i-k-j loop order: the innermost loop walks rows of `other` and `result`
    // contiguously in memory, instead of striding down a column of `other`.
    // Raw indexing skips the bounds check, which is already guaranteed above.
    for(size_t i = 0; i < rows; i++){
        double* result_row = &result.data[i * n];
        for(size_t k = 0; k < columns; k++){
            const double a_ik = data[i * columns + k];
            const double* other_row = &other.data[k * n];
            for(size_t j = 0; j < n; j++){
                result_row[j] += a_ik * other_row[j];
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
            result.data[j * rows + i] = data[i * columns + j];
        }
    }
    return result;
}

Matrix Matrix::identity(size_t size){
    Matrix I(size, size, 0.0);
    for(size_t i = 0; i < size; i++){
        I(i,i) = 1.0;
    }
    return I;
}

Matrix Matrix::inverse() const{
    if(rows != columns){
        throw std::invalid_argument("Matrix must be square to calculate inverse");
    }
    size_t n = rows;
    Matrix augmented = *this;
    Matrix result = identity(n);

    for(size_t j = 0; j < n; ++j){
        size_t pivot = j;
        for(size_t i = j + 1; i < n; ++i){
            if(std::abs(augmented(i,j)) > std::abs(augmented(pivot, j))){
                pivot = i;
            }
        }
        if(std::abs(augmented(pivot, j)) < 1e-9){
            throw std::runtime_error("Matrix is singular (non-invertible)");
        }
        if(pivot != j){
            for(size_t k = 0; k < n; ++k){
                std::swap(augmented.data[j * n + k], augmented.data[pivot * n + k]);
                std::swap(result.data[j * n + k], result.data[pivot * n + k]);
            }
        }
        double pivotValue = augmented(j,j);
        double invPivot = 1.0 / pivotValue;
        for(size_t k = 0; k < n; ++k) {
            augmented(j,k) *= invPivot;
            result(j,k) *= invPivot;
        }
        for(size_t i = 0; i < n; ++i){
            if(i != j) {
                double factor = augmented(i,j);
                if(factor == 0.0) continue;
                for(size_t k = 0; k < n; ++k){
                    augmented.data[i * n + k] -= factor * augmented.data[j * n + k];
                    result.data[i * n + k] -= factor * result.data[j * n + k];
                }
            }
        }
    }
    return result;
}
