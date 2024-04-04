import numpy as np
import subprocess
import unittest

from input_matrix import generate_test_case

class TestCppCode(unittest.TestCase):
    DEFAULT_COMPILER = "g++"
    DEFAULT_COMPILE_OPTIONS = ["-Wall", "-Wextra", "-Werror"]
    DEFAULT_EXEC_NAME = "./src/matrix_product_exe"

    MATRIX_A = "./test/matrix_A.txt"
    MATRIX_B = "./test/matrix_B.txt"

    @classmethod
    def setUpClass(cls):
        err = cls.compile(["./src/matrix_product.cpp"], cls.DEFAULT_EXEC_NAME)
        assert err == 0

    @staticmethod
    def compile(source_files: list[str], output_name: str) -> int:
        cmd = [TestCppCode.DEFAULT_COMPILER] + TestCppCode.DEFAULT_COMPILE_OPTIONS + source_files + ["-o", output_name]
        result = subprocess.run(cmd)
        return result.returncode

    def compare_matrices(self, expected: np.ndarray, output: list[str]):
        self.assertEqual(len(output), expected.shape[0])
        for row_idx in range(len(output)):
            row = [float(x) for x in output[row_idx].strip().split(" ")]
            self.assertEqual(len(row), expected.shape[1])
            for col_idx in range(len(row)):
                self.assertAlmostEqual(row[col_idx], expected[row_idx, col_idx])

    def do_results_comparison(self, rows_A: int, cols_A: int, type_A: str, rows_B: int, cols_B: int, type_B: str):
        expected_matrix: np.ndarray = generate_test_case(rows_A, cols_A, type_A, rows_B, cols_B, type_B)

        result = subprocess.run(
            [f"./{self.DEFAULT_EXEC_NAME}", self.MATRIX_A, self.MATRIX_B],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.assertEqual(result.returncode, 0)

        output_cpp = result.stdout.decode()
        output_cpp = output_cpp.strip().split("\n")
        self.compare_matrices(expected_matrix, output_cpp[1:])

    def test_int_int(self):
        self.do_results_comparison(2, 2, "int", 2, 2, "int")

    def test_int_double(self):
        self.do_results_comparison(2, 2, "int", 2, 2, "double")

    def test_double_double(self):
        self.do_results_comparison(2, 3, "double", 3, 2, "double")

    def test_large_matrices(self):
        self.do_results_comparison(100, 100, "double", 100, 100, "double")

    def test_one_matrix(self):
        generate_test_case(2, 2, "int", 2, 2, "int")

        result = subprocess.run(
            [f"./{self.DEFAULT_EXEC_NAME}", self.MATRIX_A],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.assertEqual(result.returncode, 1)
        self.assertTrue("Too few arguments" in result.stderr.decode())

    def test_invalid_dimensions(self):
        generate_test_case(2, 3, "int", 3, 2, "int")

        result = subprocess.run(
            [f"./{self.DEFAULT_EXEC_NAME}", self.MATRIX_A, self.MATRIX_A],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.assertEqual(result.returncode, 1)
        self.assertTrue("Invalid matrix dimensions for multiplication" in result.stderr.decode())


if __name__ == "__main__":
    unittest.main()
