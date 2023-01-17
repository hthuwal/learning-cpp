#include "common.h"

class Matrix {
    std::vector<std::vector<int>> data;
    int rows;
    int columns;

public:
    Matrix(int _n, int _m): rows(_n), columns(_m) {
        data.resize(rows);
        for(int i = 0; i < rows; i++) {
            data[i].resize(columns);
            std::fill(data[i].begin(), data[i].end(), 0);
        }
    }

    void set_value(int i, int j,  int value) {
        data[i][j] = value;
    }

    void set_all(int i, int j, int value) {
        for(int i=0; i<rows; i++) {
            std::fill(data[i].begin(), data[i].end(), value);
        }
    }

    void updateRowOfProductMatrix(Matrix &result, Matrix &other, int from, int till) {
        int numResultColumns = other.columns;
        for (int i = from; i <= till; i++) {
            for (int j = 0; j < numResultColumns; j++) {
                int value = 0;
                for (int k = 0; k < columns; k++) {
                    value += (data[i][k] * other.data[k][j]);
                }
                result.data[i][j] = value;
            }
        }
    }

    Matrix multiply(Matrix &other) {
        if (columns != other.rows) {
            std::cout<<"Error: Invalid dimensions";
        }

        int numResultRows = rows;
        int numResultColumns = other.columns;
        Matrix result(numResultRows, numResultColumns);

        updateRowOfProductMatrix(result, other, 0, numResultRows-1);

        return std::move(result);
    }

    void print() {
        for(int i=0; i<rows; i++) {
            for(int j=0; j<columns; j++) {
                std::cout<<data[i][j]<<"  ";
            }
            std::cout<<"\n";
        }
    }

    Matrix multiplyParallel(Matrix &other) {
        int numResultRows = rows;
        int numResultColumns = other.columns;
 
        unsigned long const min_per_thread = 1;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        unsigned long const max_threads = numResultRows;

        unsigned long const num_threads = std::min(hardware_threads, max_threads);  
        unsigned long const block_size = numResultRows / num_threads;

        std::vector<std::thread> threads(num_threads);
        Matrix result(numResultRows, other.columns);

        {
            Join_Threads joiner(threads);
            int block_start = 0;
            int block_end = 0;
            for(int i=0; i<num_threads - 1; i++) {
                block_end = block_start + block_size;
                threads[i] = std::thread([this, &other, &result, block_start, block_end]{
                    this->updateRowOfProductMatrix(result, other, block_start, block_end);
                });
                block_start = block_end;
            }

            updateRowOfProductMatrix(result, other, block_start, numResultRows-1);
        }

        return std::move(result);
    }

    bool operator==(const Matrix &other) {
        if (rows != other.rows) {
            return false;
        }

        if (columns != other.columns) {
            return false;
        }

        return data == other.data;
    }
};

int main() {
    int n = 1000;
    int m = 1000;
    Matrix mat1(n, m);
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            mat1.set_value(i, j, 1);
        }
    }

    Matrix mat2(n, m);
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            mat2.set_value(i, j, 1);
        }
    }

    auto startTime = high_resolution_clock::now();
    auto prod1 = mat1.multiply(mat2);
    auto endTime = high_resolution_clock::now();
    print_time_taken("Sequential Matrix Multiplication", startTime, endTime);

    startTime = high_resolution_clock::now();
    auto prod2 = mat1.multiplyParallel(mat2);
    endTime = high_resolution_clock::now();
    print_time_taken("Parallel Matrix Multiplication", startTime, endTime);

    std::cout<<"Results Equal: "<<(prod1 == prod2);
    // prod2.print();
}