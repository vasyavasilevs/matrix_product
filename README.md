# Matrix Product

This repository contains programs for generating matrices, computing their product, and testing the functionality.

## Getting Started

To get a local copy of this repository, run the following command in your terminal:

```
git clone https://github.com/vasyavasilevs/matrix_product.git
```

Navigate to the `matrix_product` directory to execute further commands.

## Generating Matrices

The `input_matrix.txt` file located in the `test/` directory is responsible for generating matrix data based on input parameters. It creates two files, `matrix_A.txt` and `matrix_B.txt`, containing the generated matrices. Additionally, the program computes the product of these matrices using Python's built-in functionality and writes the result to the `matrix_AB.txt` file.

To run `input_matrix.py`:

```
python3 test/input_matrix.py
```


## Computing Matrix Product

The main program resides in the `src/` directory -- `matrix_product.cpp` (and `matrix.h`). It takes two files containing matrices as input and computes their product, displaying the result on the screen.

To run `matrix_product.cpp`:

```
g++ src/matrix_product.cpp
src/a.out test/matrix_A.txt test/matrix_B.txt
```

## Testing

The `test/` directory includes the `python_test.py` file, which compiles the `.cpp` file, invokes the generation function from `input_matrix.py`, feeds the generated data into the executable file, and tests various input scenarios along with the resulting program output in C++.

To run `python_test.py`:

```
python3 test/python_test.py
```

