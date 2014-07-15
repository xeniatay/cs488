// Xenia Tay
// 20396769
// xzytay

#ifndef CS488_PERLINNOISE_HPP
#define CS488_PERLINNOISE_HPP

// noise functions, basically random number generators
double noise_1(double x);
double smoothed_noise_1(double x, double y);
double interpolated_noise_1(double x, double y);
double perlid_2d(double x, double y);

double interpolate(double a, double b, double x);

#endif
