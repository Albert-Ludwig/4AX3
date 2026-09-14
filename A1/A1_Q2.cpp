#include <Eigen/Dense>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

using Vec = Eigen::Vector4d;
using Mat = Eigen::Matrix4d;

Vec rk4(const Vec& x, double h, const Mat& A, const Vec& b) {
    auto f = [&](const Vec& z) { return A * z + b; };

    Vec k1 = f(x);
    Vec k2 = f(x + h * k1 / 2.0);
    Vec k3 = f(x + h * k2 / 2.0);
    Vec k4 = f(x + h * k3);

    return x + h * (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
}

int main() {
    const double g = 9.81;
    const double v0 = 20.0;
    const double deg = 45.0;
    const double h = 0.01;
    const int N = 10000;
    const double a = deg * std::acos(-1.0) / 180.0;

    // Continuous system
    Mat A = Mat::Zero();
    A(0, 1) = 1.0;
    A(2, 3) = 1.0;

    Vec b;
    b << 0.0, 0.0, 0.0, -g;

    // First-order discrete system
    Mat Ad = Mat::Identity() + h * A;
    Vec bd = h * b;

    // Initial condition: [px, vx, py, vy]
    Vec x0;
    x0 << 0.0, v0 * std::cos(a), 0.0, v0 * std::sin(a);

    Vec x = x0;
    Vec xd = x0;

    std::ofstream out("A1_Q2_data.dat");
    if (!out) {
        std::cerr << "Could not create output file.\n";
        return 1;
    }

    out << std::fixed << std::setprecision(8);
    out << "# t x vx y vy xd vxd yd vyd\n";

    auto write = [&](double t) {
        out << t << ' ' << x.transpose() << ' ' << xd.transpose() << '\n';
    };

    // Simulate until both models reach the ground
    write(0.0);
    for (int i = 1; i <= N; ++i) {
        x = rk4(x, h, A, b);
        xd = Ad * xd + bd;
        double t = i * h;
        write(t);

        if (x(2) < 0.0 && xd(2) < 0.0)
            break;
    }

    return 0;
}
