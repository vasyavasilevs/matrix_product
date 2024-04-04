#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <limits>

#include "matrix.h"

template <typename T1, typename T2>
matrix::Matrix<std::common_type_t<T1, T2>>
get_matrix_product(const matrix::Matrix<T1> & m1, const matrix::Matrix<T2> & m2) {
    if (m1.get_columns_num() != m2.get_rows_num()) {
        throw std::invalid_argument("Invalid matrix dimensions for multiplication");
    }

    using ResultT = std::common_type_t<T1, T2>;
    constexpr ResultT MAX_T_VALUE = std::numeric_limits<ResultT>::max();

    size_t product_rows = m1.get_rows_num();
    size_t product_cols = m2.get_columns_num();

    matrix::Matrix<ResultT> product(product_rows, product_cols);
    auto m2_t = matrix::transpose(m2);

    for (size_t i = 0; i < product_rows; ++i) {
        for (size_t j = 0; j < product_cols; ++j) {
            ResultT product_value = 0;
            for (size_t k = 0, m1_columns = m1.get_columns_num(); k < m1_columns; ++k) {
                auto m1_value = m1.get_element(i, k);
                auto m2_value = m2_t.get_element(j, k);

                if (m1_value != 0 && MAX_T_VALUE / m1_value < m2_value) {
                    throw std::runtime_error("Overflow at product element " + std::to_string(i) + ", " + std::to_string(j));
                }

                ResultT res_elem = m1_value * m2_value;
                if (MAX_T_VALUE - res_elem < product_value) {
                    throw std::runtime_error("Overflow at product element " + std::to_string(i) + ", " + std::to_string(j));
                }
                product_value += res_elem;
            }
            product.set_element(i, j, product_value);
        }
    }

    return product;
}


template <typename T>
void 
print_matrix(const matrix::Matrix<T> & matrix, const std::string & matrix_name) {
    std::cout << std::setprecision(15);
    std::cout << "Matrix " << matrix_name << " is:" << std::endl;
    for (size_t i = 0; i < matrix.get_rows_num(); ++i) {
        for (size_t j = 0; j < matrix.get_columns_num(); ++j) {
            std::cout << matrix.get_element(i, j) << ' ';
        }
        std::cout << std::endl;
    }
}

template <typename T>
matrix::Matrix<T> load_from_file(std::ifstream & fin) {
    size_t rows = 0;
    if (fin.eof()) {
        throw std::invalid_argument("Dimensions are not provided");
    }
    fin >> rows;

    size_t columns = 0;
    if (fin.eof()) {
        throw std::invalid_argument("Dimensions are not provided");
    }
    fin >> columns;

    matrix::Matrix<T> m(rows, columns);

    for (size_t i = 0; i < m.get_rows_num(); ++i) {
        for (size_t j = 0; j < m.get_columns_num(); ++j) {
            if (fin.eof()) {
                throw std::invalid_argument("Unexpected EOF at element " + std::to_string(i) + ", " + std::to_string(j));
            }
            T elem;
            fin >> elem;
            m.set_element(i, j, elem);
        }
    }
    return m;
}

std::pair<std::string, std::string> read_name_and_type(std::ifstream & fin) {
    if (!fin.is_open()) {
        throw std::invalid_argument("File is not opened");
    }

    std::string name;
    if (fin.eof()) {
        throw std::invalid_argument("Name is not provided");
    }
    fin >> name;

    std::string type;
    if (fin.eof()) {
        throw std::invalid_argument("Type is not provided");
    }
    fin >> type;

    return {name, type};
}

template <typename T1>
void read_second_and_print_product(const matrix::Matrix<T1> & m1, const std::string & type2, std::ifstream & fin2,
                                   const std::string & product_name) {
    if (type2 == "int") {
        auto m2 = load_from_file<int>(fin2);
        print_matrix(get_matrix_product(m1, m2), product_name);
    } else if (type2 == "double") {
        auto m2 = load_from_file<double>(fin2);
        print_matrix(get_matrix_product(m1, m2), product_name);
    } else {
        throw std::invalid_argument("type " + type2 + " is not supported");
    }
}

void read_and_print_product(const std::string & filename1, const std::string & filename2) {
    std::ifstream fin1(filename1);
    auto [name1, type1] = read_name_and_type(fin1);
    std::ifstream fin2(filename2);
    auto [name2, type2] = read_name_and_type(fin2);

    std::string product_name = name1 + " @ " + name2;

    if (type1 == "int") {
        read_second_and_print_product(load_from_file<int>(fin1), type2, fin2, product_name);
    } else if (type1 == "double") {
        read_second_and_print_product(load_from_file<double>(fin1), type2, fin2, product_name);
    } else {
        throw std::invalid_argument("Type " + type1 + " is not supported");
    }
}

int main(int argc, char *argv[ ]) {
    if (argc < 3) {
        std::cerr << "Too few arguments" << std::endl;
        return EXIT_FAILURE;
    }
    const char *file_matrix1 = argv[1];
    const char *file_matrix2 = argv[2];

    try {
        read_and_print_product(file_matrix1, file_matrix2);
    } catch (std::exception & exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown failure" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}