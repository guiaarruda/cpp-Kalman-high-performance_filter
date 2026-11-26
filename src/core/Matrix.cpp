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

Matrix Matrix::multiplyNaive(const Matrix& other) const {
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
Matrix Matrix::operator*(const Matrix& other) const{
    if(columns != other.rows){
        
        throw std::invalid_argument("Matrix dimensions are incompatible");
    }
    const size_t n = rows, m = columns, p = other.columns;
    Matrix result(n, p, 0.0);

    const double* A = data.data();
    const double* B = other.data.data();
    double* C = result.data.data();

    for(size_t k = 0; k < n; ++k){
        double* Ck = C + k * p;
        for(size_t w = 0; w < m; ++w){
            const double a = A[k * m + w];
            const double* Bk = B + w  * p;
            for(size_t z = 0; z < p; ++z){
                Ck[z] += a * Bk[z];
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

Matrix Matrix::indentity(size_t size){
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
    Matrix augumented = *this;
    Matrix result = indentity(n);

    for(size_t j = 0; j < n; ++j){
        size_t pivot = j;
        for(size_t i = j + 1; i < n; ++i){
            if(std::abs(augumented(i,j)) > std::abs(augumented(pivot, j))){
                pivot = i;
            }
        }
        if(std::abs(augumented(pivot, j)) < 1e-9){
            throw std::runtime_error("Matrix is singular (non-invertible)");
        }
        if(pivot != j){
            for(size_t k = 0; k < n; ++k){
                std::swap(augumented.data[j * n + k], augumented.data[pivot * n + k]);
                std::swap(result.data[j * n + k], result.data[pivot * n + k]);
            }
        }
        double pivotValue = augumented(j,j);
        double invPivot = 1.0 / pivotValue;
        for(size_t k = 0; k < n; ++k) {
            augumented(j,k) *= invPivot;
            result(j,k) *= invPivot;
        }
        for(size_t i = 0; i < n; ++i){
            if(i != j) {
                double factor = augumented(i,j);
                for(size_t k = 0; k < n; ++k){
                    augumented(i, k) -= factor *augumented(j,k);
                    result(i,k) -= factor * result(j,k);
                }
            }
        }
    }
    return result;
}
