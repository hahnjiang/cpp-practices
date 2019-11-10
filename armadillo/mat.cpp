#include <armadillo>
#include <iostream>

using namespace std;
using namespace arma;

int main() {
    mat A = randu<mat>(4, 5);
    A.print("A:");

    cout << A.n_rows << endl;
    cout << A.n_cols << endl;
    cout << A.n_elem << endl;
    cout << A(1) << endl;
    cout << A.at(10) << endl;
    cout << A[20] << endl;
    cout << A.at(1, 2) << endl;
    cout << A.at(1, 9) << endl;  // 超出边界，随机值，接近0

    A.col(0).print("A:");
    A.row(1).print("A:");
    A.cols(0, 2).print("A:");
    A.rows(2, 2).print("A:");

    A.set_size(4, 4);
    A << 0 << 1 << 2 << 3 << endr << 4 << 5 << 6 << 7 << endr << 8 << 9 << 10
      << 11 << endr << 12 << 13 << 14 << 15 << endr;
    A.print("A:");

    A.set_size(3, 3);
    A.load("./sample.csv", csv_ascii);
    A.print("A:");

    A.save("./output", raw_ascii);

    return 0;
}
