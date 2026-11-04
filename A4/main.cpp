#include "DARE.h"
#include "kalman.h"

#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;
using namespace Eigen;

void setup(void)
{
  const double dt = 0.01;   // time step
  const int horizon = 6000; // total step
  const double g = 9.81;    // gravity
  const double accel_sd = 0.1;
  const double sensor_sd = 0.1;

  double h = 100; // initial height
  double v = 0;   // initial velocity

  // observer state: [h, v, g]; only h is measured directly
  MatrixXd A_o(3, 3);
  MatrixXd B_o(3, 1);
  MatrixXd C_o(1, 3);

  A_o << 1.0, dt, 0.0,
      0.0, 1.0, -dt,
      0.0, 0.0, 1.0;

  B_o << 0.0,
      dt,
      0.0;

  C_o << 1.0, 0.0, 0.0;

  MatrixXd Q = MatrixXd::Zero(3, 3);     // noise for dynamic model
  MatrixXd R(1, 1);                      // noise for observation
  MatrixXd P = MatrixXd::Identity(3, 3); // The variance matrix

  Q(1, 1) = dt * dt * accel_sd * accel_sd;
  P(0, 0) = 100.0;
  P(2, 2) = 100.0;
  R << sensor_sd * sensor_sd;

  KalmanFilter observer(dt, A_o, B_o, C_o, Q, R, P);

  VectorXd x_0(3);        // initial state vector
  x_0 << 110.0, 0.0, 5.0; // initial guess
  observer.init(0.0, x_0);
  mt19937 rng(42); // repeat the same noisy landing for the plots
  normal_distribution<double> noise(0.0, sensor_sd);
  normal_distribution<double> accel_noise(0.0, accel_sd);

  MatrixXd A_c(2, 2), B_c(2, 1); // DARE controller matrix, [h,v]
  A_c << 1.0, dt,
      0.0, 1.0;
  B_c << 0.0,
      dt;
  MatrixXd Q_c = MatrixXd::Zero(2, 2); // cost for state deviation
  MatrixXd R_c = MatrixXd(1, 1);       // cost for control input
  Q_c(0, 0) = 10.0;                    // height cost
  Q_c(1, 1) = 80.0;                    // velocity cost
  R_c(0, 0) = 0.5;                     // control cost
  DARE controller(A_c, B_c, Q_c, R_c, horizon);
  controller.init();

  // simulation
  double u = 0.0; // initial control input, which is the upward acceleration
  int steps = 0;  // current step
  VectorXd x = observer.state();
  ofstream data("landing_data.dat");
  if (!data)
    throw runtime_error("Cannot open landing_data.dat");
  data.precision(10);
  data << "# t h v h_est v_est g_est u\n";
  data << 0.0 << ' ' << h << ' ' << v << ' '
       << x(0) << ' ' << x(1) << ' ' << x(2) << ' ' << u << '\n';

  for (int i = 0; i < horizon && h >= 0.0; ++i)
  {
    VectorXd s(2);                                 // controller will use the h and v to do the control
    s << x(0), x(1);                               // from observer
    MatrixXd K = controller.getK(i);               // get the K
    double a = -(K(0, 0) * s(0) + K(0, 1) * s(1)); // the desired acceleration by controller
    u = max(0.0, x(2) + a);                        // this is the input's required accelration, to get the desired a and overcome g. The u should be greater than 0
    h += v * dt;                                   // h_n+1=h_n+v_n*dt
    v += (u - g + accel_noise(rng)) * dt;
    VectorXd y(1);
    y << h + noise(rng); // observed height
    observer.update(y, u);
    x = observer.state(); // directly get posterior
    // print & update
    steps += 1;
    data << steps * dt << ' ' << h << ' ' << v << ' '
         << x(0) << ' ' << x(1) << ' ' << x(2) << ' ' << u << '\n';
    cout << "Time: " << steps * dt
         << ", height: " << h
         << ", velocity: " << v
         << ", estimated v: " << x(1)
         << ", estimated g: " << x(2)
         << ", u: " << u << endl;
  }
}

int main(void)
{

  setup();
  return (0);
}
