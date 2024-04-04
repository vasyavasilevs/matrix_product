import random
import numpy as np


def generate_matrix(rows: int, cols: int, type: str) -> list:
    matrix = []
    for _ in range(rows * cols): 
        if type == "int":
            matrix.append(random.randint(1, 100))
        else:
            matrix.append(random.uniform(1.0, 100.0))
        
    return matrix


def input_params(matrix_name: str) -> tuple[int, int, str]:
    matrix_input = input(f"Enter matrix {matrix_name}: rows columns type_of_elements ")
    rows, cols, element_type = matrix_input.split()
    rows = int(rows)
    cols = int(cols)
    
    return rows, cols, element_type


def to_compatible_type(matrix_type: str) -> str:
    if matrix_type.startswith("int"):
        return "int"
    return "double"


def print_matrix(matrix: list, matrix_type: str, rows: int, cols: int, name: str, file: str):
    with open(file, "w") as f:
        matrix_header = f"{name} {to_compatible_type(matrix_type)} {rows} {cols}"
        print(matrix_header, file=f)
        for i in range(rows):
            row = matrix[i * cols:(i + 1) * cols]
            print(" ".join(map(str, row)), file=f)
    f.close()
            
            
def get_product(m1: list, r1: int, c1: int, m2: list, r2: int, c2: int ) -> np.ndarray:
    matr1 = np.array(m1).reshape(r1, c1)
    matr2 = np.array(m2).reshape(r2, c2)
    prod = matr1 @ matr2
    return prod


def generate_test_case(rows_A: int, cols_A: int, type_A: str, rows_B: int, cols_B: int, type_B: str) -> np.ndarray:
    matrix_A = generate_matrix(rows_A, cols_A, type_A)
    matrix_B = generate_matrix(rows_B, cols_B, type_B)

    print_matrix(matrix_A, type_A, rows_A, cols_A, "A", "./test/matrix_A.txt")
    print_matrix(matrix_B, type_B, rows_B, cols_B, "B", "./test/matrix_B.txt")

    return get_product(matrix_A, rows_A, cols_A, matrix_B, rows_B, cols_B)


def main():
    rows_A, cols_A, type_A = input_params("A")
    rows_B, cols_B, type_B = input_params("B")

    matrix_AB = generate_test_case(rows_A, cols_A, type_A, rows_B, cols_B, type_B)
    print_matrix(matrix_AB.flatten().tolist(), matrix_AB.dtype.name, rows_A, cols_B, "A @ B", "./test/matrix_AB.txt")


if __name__ == "__main__":
    main()
