#pragma once

#include <iostream>
#include <limits>
#include <vector>

namespace matrix {

template <typename T>
class Matrix final {
public:
    explicit Matrix(size_t rows, size_t columns);

    const T & get_element(size_t row, size_t column) const;
    void set_element(size_t row, size_t column, const T & value);
    size_t get_rows_num() const;
    size_t get_columns_num() const;

private:
    std::vector<T> buffer_;
    size_t rows_;
    size_t columns_;
};

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t columns) : rows_(rows), columns_(columns) {
    constexpr T MAX_T_VALUE = std::numeric_limits<T>::max();
    if (rows == 0 || columns == 0 || MAX_T_VALUE / rows < columns) {
        throw std::invalid_argument("Invalid matrix dimensions");
    }
    buffer_ = std::vector<T>(rows * columns, 0);
}

template <typename T>
const T & Matrix<T>::get_element(size_t row, size_t column) const {
    if (row >= rows_ || column >= columns_) {
        throw std::out_of_range("Matrix index out of range");
    }
    return buffer_[row * columns_ + column];
}

template <typename T>
void Matrix<T>::set_element(size_t row, size_t column, const T & value) {
    if (row >= rows_ || column >= columns_) {
        throw std::out_of_range("Matrix index out of range");
    }
    buffer_[row * columns_ + column] = value;
}

template <typename T>
size_t Matrix<T>::get_rows_num() const {
    return rows_;
}

template <typename T>
size_t Matrix<T>::get_columns_num() const {
    return columns_;
}

template <typename T>
Matrix<T> transpose(const Matrix<T> & m) {
    Matrix<T> result(m.get_columns_num(), m.get_rows_num());
    for (size_t i = 0; i < m.get_rows_num(); ++i) {
        for (size_t j = 0; j < m.get_columns_num(); ++j) {
            result.set_element(j, i, m.get_element(i, j));
        }
    }
    return result;
}

} //end of namespace matrix
