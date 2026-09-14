#include <Eigen/Dense>
#include <stdexcept>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class LTISystem {
private:
    MatrixXd A, B, C;
    VectorXd x;

public:
    LTISystem(const MatrixXd& A, const MatrixXd& B, const MatrixXd& C)
        : A(A), B(B), C(C), x(VectorXd::Zero(A.rows())) {
        if (A.rows() != A.cols() || B.rows() != A.rows() ||
            C.cols() != A.cols()) {
            throw std::invalid_argument("Matrix dimensions do not match");
        }
    }

    void init(const VectorXd& x0) {
        if (x0.size() != A.rows())
            throw std::invalid_argument("Wrong initial state size");
        x = x0;
    }

    void update(const VectorXd& u) {
        if (u.size() != B.cols())
            throw std::invalid_argument("Wrong input size");
        x = A * x + B * u;
    }

    VectorXd state() const {
        return x;
    }

    VectorXd output() const {
        return C * x;
    }
};

int main() {
    // Define the discrete system
    MatrixXd A(2, 2);
    MatrixXd B(2, 1);
    MatrixXd C(1, 2);

    A << 1.0, 1.0,
         0.0, 0.5;
    B << 0.0,
         1.0;
    C << 1.0, 0.0;

    LTISystem sys(A, B, C);

    VectorXd x0(2);
    x0 << 0.0, 0.0;
    sys.init(x0);

    VectorXd u(1);
    u << 1.0;

    // Example update
    for (int k = 0; k < 10; ++k)
        sys.update(u);

    VectorXd x = sys.state();
    VectorXd y = sys.output();
    (void)x;
    (void)y;

    return 0;
}
