#pragma once

#include <vector>
#include <iostream>


namespace task {

    const double EPS = 1e-6;


    class OutOfBoundsException : public std::exception {
    };

    class SizeMismatchException : public std::exception {
    };


    class Matrix {
        double **matrix;
        size_t rows;
        size_t columns;

        void allocate_memory();

        void check_bounds(size_t, size_t) const;

        void check_size(size_t, size_t) const;

    public:

        Matrix();

        Matrix(size_t rows, size_t cols);

        Matrix(const Matrix &copy);

        Matrix &operator=(const Matrix &a);

        double &get(size_t row, size_t col);

        const double &get(size_t row, size_t col) const;

        void set(size_t row, size_t col, const double &value);

        void resize(size_t new_rows, size_t new_cols);

        double *operator[](size_t row);

        double const *operator[](size_t row) const;

        Matrix &operator+=(const Matrix &a);

        Matrix &operator-=(const Matrix &a);

        Matrix &operator*=(const Matrix &a);

        Matrix &operator*=(const double &number);

        Matrix operator+(const Matrix &a) const;

        Matrix operator-(const Matrix &a) const;

        Matrix operator*(const Matrix &a) const;

        Matrix operator*(const double &a) const;

        Matrix operator-() const;

        Matrix operator+() const;

        double det() const;

        void transpose();

        Matrix transposed() const;

        double trace() const;

        std::vector<double> getRow(size_t row);

        std::vector<double> getColumn(size_t column);

        bool operator==(const Matrix &a) const;

        bool operator!=(const Matrix &a) const;

        size_t getRowsNum() const;

        size_t getColumnsNum() const;

        ~Matrix();
    };

    Matrix operator*(const double &a, const Matrix &b);

    std::ostream &operator<<(std::ostream &output, const Matrix &matrix);

    std::istream &operator>>(std::istream &input, Matrix &matrix);


}  // namespace task
