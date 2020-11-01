#include "matrix.h"
#include <cmath>

using namespace task;

void Matrix::allocate_memory() {
    this->matrix = new double *[this->rows];
    for (size_t i = 0; i < this->rows; i++) {
        this->matrix[i] = new double[this->columns];
    }
}

void Matrix::check_bounds(size_t rows, size_t columns) const {
    if (rows >= this->rows || columns >= this->columns) {
        throw OutOfBoundsException();
    }
}

void Matrix::check_size(size_t rows, size_t columns) const {
    if (this->rows != rows || this->columns != columns) {
        throw SizeMismatchException();
    }
}

Matrix::Matrix() {
    this->rows = 1;
    this->columns = 1;

    allocate_memory();

    this->matrix = new double *[rows];
    this->matrix[0] = new double[columns];

    this->matrix[0][0] = 1.0;
}

Matrix::Matrix(size_t rows, size_t columns) {
    this->rows = rows;
    this->columns = columns;

    allocate_memory();

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            this->matrix[i][j] = i == j ? 1.0 : 0.0;
        }
    }
}

Matrix::Matrix(const Matrix &copy) {
    this->rows = copy.rows;
    this->columns = copy.columns;

    allocate_memory();

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            this->matrix[i][j] = copy.matrix[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (size_t i = 0; i < this->columns; i++) {
        delete[] this->matrix[i];
    }
    delete[] this->matrix;
}

Matrix &Matrix::operator=(const Matrix &copy) {
    if (this == &copy) {
        return *this;
    }

    if (this->rows != copy.rows || this->columns != copy.columns) {
        this->~Matrix();

        this->rows = copy.rows;
        this->columns = copy.columns;

        allocate_memory();
    }

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            this->matrix[i][j] = copy.matrix[i][j];
        }
    }

    return *this;
}

double &Matrix::get(size_t row, size_t col) {
    check_bounds(row, col);
    return this->matrix[row][col];
}

const double &Matrix::get(size_t row, size_t col) const {
    return get(row, col);
}

void Matrix::set(size_t row, size_t col, const double &value) {
    check_bounds(row, col);
    this->matrix[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
    Matrix new_matrix(new_rows, new_cols);

    for (size_t i = 0; i < new_rows; i++) {
        for (size_t j = 0; j < new_cols; j++) {
            if (i < this->rows && j < this->columns) {
                new_matrix.matrix[i][j] = this->matrix[i][j];
            } else {
                new_matrix.matrix[i][j] = 0.0;
            }
        }
    }
    *this = new_matrix;
}

double *Matrix::operator[](size_t row) {
    check_bounds(row, 0);
    return matrix[row];
}

double const *Matrix::operator[](size_t row) const {
    check_bounds(row, 0);
    return matrix[row];
}

Matrix &Matrix::operator+=(const Matrix &a) {
    check_size(a.rows, a.columns);

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            matrix[i][j] += a.matrix[i][j];
        }
    }

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &a) {
    check_size(a.rows, a.columns);

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            matrix[i][j] -= a.matrix[i][j];
        }
    }

    return *this;
}

Matrix &Matrix::operator*=(const Matrix &a) {
    check_size(a.rows, a.columns);

    Matrix new_matrix(this->rows, a.columns);

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < a.columns; j++) {
            new_matrix.matrix[i][j] = 0;

            for (size_t k = 0; k < this->columns; k++) {
                new_matrix.matrix[i][j] += matrix[i][k] * a.matrix[k][j];
            }
        }
    }

    *this = new_matrix;
    return *this;
}

Matrix &Matrix::operator*=(const double &number) {
    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            matrix[i][j] *= number;
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
    check_size(a.rows, a.columns);

    Matrix new_matrix(*this);
    new_matrix += a;

    return new_matrix;
}

Matrix Matrix::operator-(const Matrix &a) const {
    check_size(a.rows, a.columns);

    Matrix new_matrix(*this);
    new_matrix -= a;

    return new_matrix;
}

Matrix Matrix::operator*(const Matrix &a) const {
    check_size(this->rows, a.columns);

    Matrix new_matrix(this->rows, a.columns);
    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < a.columns; j++) {
            new_matrix.matrix[i][j] = 0;
            for (size_t s = 0; s < this->columns; s++) {
                new_matrix.matrix[i][j] += this->matrix[i][s] * a.matrix[s][j];
            }
        }
    }

    return new_matrix;
}

Matrix Matrix::operator*(const double &a) const {
    Matrix new_matrix(*this);

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            new_matrix.matrix[i][j] *= a;
        }
    }
    return new_matrix;
}

Matrix Matrix::operator-() const {
    Matrix new_matrix(*this);

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            new_matrix.matrix[i][j] *= -1;
        }
    }
    return new_matrix;
}

Matrix Matrix::operator+() const {
    Matrix new_matrix(*this);
    return new_matrix;
}

double Matrix::det() const {
    if (this->rows != this->columns) {
        throw SizeMismatchException();
    }

    double det = 0;

    if (this->rows == 1) {
        det = matrix[0][0];
    } else if (this->rows == 2) {
        det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    } else {
        for (size_t k = 0; k < this->rows; k++) {
            Matrix submatrix(this->rows - 1, this->rows - 1);

            for (size_t i = 1; i < this->rows; i++) {
                size_t l = 0;
                for (size_t j = 0; j < this->rows; j++) {
                    if (j == k) {
                        continue;
                    } else {
                        submatrix.matrix[i - 1][l] = matrix[i][j];
                        l++;
                    }
                }
            }
            det += std::pow(-1, k) * matrix[0][k] * submatrix.det();
        }
    }

    return det;
}

void Matrix::transpose() {
    Matrix new_matrix(this->columns, this->rows);

    for (size_t i = 0; i < this->columns; i++) {
        for (size_t j = 0; j < this->rows; j++) {
            new_matrix.matrix[i][j] = this->matrix[j][i];
        }
    }

    *this = new_matrix;
}

Matrix Matrix::transposed() const {
    Matrix new_matrix(this->columns, this->rows);

    for (size_t i = 0; i < this->columns; i++) {
        for (size_t j = 0; j < this->rows; j++) {
            new_matrix.matrix[i][j] = matrix[j][i];
        }
    }

    return new_matrix;
}

double Matrix::trace() const {
    if (this->rows != this->columns) {
        throw SizeMismatchException();
    }

    double result = 0;
    for (size_t i = 0; i < this->rows; i++) {
        result += matrix[i][i];
    }

    return result;
}

bool Matrix::operator==(const Matrix &a) const {
    if (this->rows != a.rows || this->columns != a.columns) {
        return false;
    }

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            if (this->matrix[i][j] - a.matrix[i][j] > EPS || a.matrix[i][j] - this->matrix[i][j] > EPS) {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix &a) const {
    if (this->rows != a.rows || this->columns != a.columns) {
        return true;
    }

    for (size_t i = 0; i < this->rows; i++) {
        for (size_t j = 0; j < this->columns; j++) {
            if (matrix[i][j] - a.matrix[i][j] > EPS || a.matrix[i][j] - matrix[i][j] > EPS) {
                return true;
            }
        }
    }

    return false;
}

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
    for (size_t i = 0; i < matrix.getRowsNum(); i++) {
        for (size_t j = 0; j < matrix.getColumnsNum(); j++) {
            output << matrix[i][j] << ' ';
        }
        output << '\n';
    }

    return output;
}

std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
    size_t n, m;
    input >> n >> m;

    matrix.resize(n, m);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            input >> matrix[i][j];
        }
    }

    return input;
}

Matrix task::operator*(const double &a, const Matrix &b) {
    Matrix new_matrix(b);

    for (size_t i = 0; i < b.getRowsNum(); i++) {
        for (size_t j = 0; j < b.getColumnsNum(); j++) {
            new_matrix[i][j] *= a;
        }
    }
    return new_matrix;
}

std::vector<double> Matrix::getRow(size_t row) {
    if (row >= this->rows) {
        throw SizeMismatchException();
    }

    std::vector<double> row_vec;
    row_vec.reserve(this->columns);
    for (size_t i = 0; i < this->columns; i++) {
        row_vec.push_back(matrix[row][i]);
    }

    return row_vec;
}

std::vector<double> Matrix::getColumn(size_t column) {
    if (column >= this->columns) {
        throw SizeMismatchException();
    }
    std::vector<double> column_vec;
    column_vec.reserve(this->rows);

    for (size_t i = 0; i < this->rows; i++) {
        column_vec.push_back(matrix[i][column]);
    }

    return column_vec;
}

size_t Matrix::getRowsNum() const {
    return this->rows;
}

size_t Matrix::getColumnsNum() const {
    return this->columns;
}