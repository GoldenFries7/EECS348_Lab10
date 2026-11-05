/*
 * Program: EECS 348 Lab 10 - C++ Programming
 * Description: Custom Matrix class implementing operator overloading, deep copying, and pass-by-value modifications.
 * Author: Iseoluwa Oluwapelumi Adedipe
 * Date: November 5, 2026
 */

#include <iostream>  // Add iostream for standard console input and output
#include <fstream>   // Add fstream so we can read the matrix data from a text file
#include <iomanip>   // Add iomanip to use setw() for perfectly aligned columns
#include <string>    // Add string to store and handle the user-provided filename

class Matrix { // Define my custom Matrix class instead of relying on std::vector directly
private: 
    int** data; // The pointer to a pointer to create a dynamic 2D array structure
    int size;   // Integer to store the N dimension for the NxN matrix

public:
    Matrix(int n) : size(n) { // Constructor that takes the dimension size
        data = new int*[size]; // Make the primary array of row pointers
        for (int i = 0; i < size; ++i) { // Loop through each row index
            data[i] = new int[size]{0}; // Allocate the columns for each row and initialize them to 0
        }
    }

    Matrix(const Matrix& other) : size(other.size) { // Deep copy constructor required for pass-by-value to work safely
        data = new int*[size]; // Create a fresh array of row pointers for the new copy
        for (int i = 0; i < size; ++i) { // Loop through every row
            data[i] = new int[size]; // Make fresh and empty column space
            for (int j = 0; j < size; ++j) { // Loop through every column
                data[i][j] = other.data[i][j]; // Copy the exact value from the original matrix into the new one
            }
        }
    }

    ~Matrix() { // Destructor to clean up our dynamic memory and prevent memory leaks
        for (int i = 0; i < size; ++i) { // Loop through all the row pointers
            delete[] data[i]; // Delete the column arrays first
        }
        delete[] data; // Finally, get rid of the primary row pointer array
    }

    void read(std::ifstream& is) { // Function to populate the matrix from an active file stream
        for (int i = 0; i < size; ++i) { // Loop through the rows
            for (int j = 0; j < size; ++j) { // Loop through the columns
                is >> data[i][j]; // Extract the next integer from the file into the grid
            }
        }
    }

    void print() const { // Function to print the matrix to the console neatly
        for (int i = 0; i < size; ++i) { // Loop through the rows
            for (int j = 0; j < size; ++j) { // Loop through the columns
                std::cout << std::setw(4) << data[i][j]; // Right-align every number with a width of 4 so columns line up
            }
            std::cout << '\n'; // Drop to a new line after finishing a row
        }
    }

    Matrix operator+(const Matrix& other) const { // Overload the addition operator to add two matrices together
        Matrix result(size); // Create a new temporary matrix to hold the sums
        for (int i = 0; i < size; ++i) { // Loop through the rows
            for (int j = 0; j < size; ++j) { // Loop through the columns
                result.data[i][j] = this->data[i][j] + other.data[i][j]; // Add the matching cells and store them
            }
        }
        return result; // Return the final summed matrix
    }

    Matrix operator*(const Matrix& other) const { // Overload the multiplication operator using standard matrix multiplication
        Matrix result(size); // Create a new temporary matrix to hold the products
        for (int i = 0; i < size; ++i) { // Loop through the rows of the first matrix
            for (int j = 0; j < size; ++j) { // Loop through the columns of the second matrix
                result.data[i][j] = 0; // Initialize the target cell to 0
                for (int k = 0; k < size; ++k) { // Loop through the matching dimension to calculate the dot product
                    result.data[i][j] += this->data[i][k] * other.data[k][j]; // Multiply and accumulate into the target cell
                }
            }
        }
        return result; // Return the final multiplied matrix
    }

    void diagonalSums() const { // Function to calculate and print the main and secondary diagonal sums
        int mainSum = 0; // Variable to track the top-left to bottom-right diagonal
        int secSum = 0; // Variable to track the top-right to bottom-left diagonal
        for (int i = 0; i < size; ++i) { // Loop through the grid dimension
            mainSum += data[i][i]; // The main diagonal always has matching row and column indices
            secSum += data[i][size - 1 - i]; // The secondary diagonal steps backward through the columns
        }
        std::cout << "Main diagonal sum: " << mainSum << '\n'; // Print the main diagonal result
        std::cout << "Secondary diagonal sum: " << secSum << '\n'; // Print the secondary diagonal result
    }

    int getSize() const { return size; } // Helper getter to safely expose the matrix size
    
    void swapMatrixRows(int r1, int r2) { // Internal function to swap two rows via pointer reassignment
        int* temp = data[r1]; // Hold the first row pointer temporarily
        data[r1] = data[r2]; // Overwrite the first row pointer with the second row pointer
        data[r2] = temp; // Put the original first row pointer into the second slot
    }
    
    void swapMatrixCols(int c1, int c2) { // Internal function to swap two columns
        for (int i = 0; i < size; ++i) { // Loop through every row vertically
            int temp = data[i][c1]; // Hold the value from the first column temporarily
            data[i][c1] = data[i][c2]; // Overwrite it with the value from the second column
            data[i][c2] = temp; // Put the temporary value into the second column
        }
    }
    
    void setVal(int r, int c, int val) { data[r][c] = val; } // Internal helper to overwrite a specific cell value
};

// Standalone functions requested by the prompt; Matrix m is passed BY VALUE so the original isn't modified
void swapRows(Matrix m, int row1 = 0, int row2 = 1) { // Default indices are 0 and 1
    if (row1 >= 0 && row1 < m.getSize() && row2 >= 0 && row2 < m.getSize()) { // Validate that the bounds are safe
        m.swapMatrixRows(row1, row2); // Call the internal row swapper on the isolated copy
    }
    m.print(); // Print the modified isolated copy
}

void swapCols(Matrix m, int col1 = 0, int col2 = 1) { // Default indices are 0 and 1
    if (col1 >= 0 && col1 < m.getSize() && col2 >= 0 && col2 < m.getSize()) { // Validate that the bounds are safe
        m.swapMatrixCols(col1, col2); // Call the internal column swapper on the isolated copy
    }
    m.print(); // Print the modified isolated copy
}

void updateElement(Matrix m, int row = 0, int col = 0, int value = 100) { // Defaults are 0, 0, and 100
    if (row >= 0 && row < m.getSize() && col >= 0 && col < m.getSize()) { // Validate that the bounds are safe
        m.setVal(row, col, value); // Overwrite the cell in the isolated copy
    }
    m.print(); // Print the modified isolated copy
}

int main() { // Main execution block
    std::string filename; // Variable to hold the filename from the user
    std::cout << "Enter input filename: \n"; // Prompt the user
    std::cin >> filename; // Read the filename from the console

    std::ifstream file(filename); // Open a stream to read the input file
    if (!file.is_open()) { // Check if we failed to find or open it
        std::cerr << "Error opening file.\n"; // Print an error message
        return 1; // Exit with a failure code
    }

    int n; // Variable to hold the N dimension
    file >> n; // Read the very first number from the file

    Matrix A(n); // Create the first matrix object using the dimension we just read
    A.read(file); // Load its data from the file stream

    Matrix B(n); // Create the second matrix object
    B.read(file); // Load the remaining data from the file stream

    file.close(); // Safely close the file stream

    std::cout << "Matrix A:\n"; // Label for Matrix A
    A.print(); // Print the original Matrix A
    std::cout << "\nMatrix B:\n"; // Label for Matrix B
    B.print(); // Print the original Matrix B

    std::cout << "\nA + B:\n"; // Label for the addition operation
    Matrix sum = A + B; // Utilize the overloaded + operator
    sum.print(); // Print the resulting sum matrix

    std::cout << "\nA * B:\n"; // Label for the multiplication operation
    Matrix prod = A * B; // Use the overloaded * operator
    prod.print(); // Print the resulting product matrix

    std::cout << "\nDiagonal sums for Matrix A:\n"; // Label for the diagonal check
    A.diagonalSums(); // Calculate and print the diagonals for Matrix A

    std::cout << "\nProblem 5 - Rows 0 and 2 swapped:\n"; // Label for the row swap test
    swapRows(A, 0, 2); // Pass Matrix A by value and swap rows 0 and 2 per the sample output

    std::cout << "\nProblem 6 - Columns 0 and 2 swapped:\n"; // Label for the col swap test
    swapCols(A, 0, 2); // Pass Matrix A by value and swap cols 0 and 2 per the sample output

    std::cout << "\nProblem 7 - Updated matrix:\n"; // Label for the update test
    updateElement(A, 1, 2, 99); // Pass Matrix A by value and overwrite row 1, col 2 with 99 per the sample output

    return 0; // Exit program successfully
}