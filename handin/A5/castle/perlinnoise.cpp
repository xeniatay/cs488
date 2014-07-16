// Xenia Tay
// 20396769
// xzytay

#include "perlinnoise.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// from: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
double noise_1(double x, double y) {
  int n = x + y * 57;
  n = ( n << 13 ) ^ n;
  return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double smoothed_noise_1(double x, double y) {
  double corners = ( noise_1(x - 1, y - 1) + noise_1(x + 1, y - 1) + noise_1(x + 1, y + 1) ) / 16;
  double sides = ( noise_1(x - 1, y) + noise_1( x + 1, y) + noise_1(x, y - 1) + noise_1(x, y + 1) ) / 8;
  double center = noise_1(x, y) / 4;

  return corners + sides + center;
}

// cubic interpolation
double interpolate(double a, double b, double x) {
  double ft = x * M_PI;
  double f = (1 - cos(ft) * 0.5);
  return a * (1 - f) + b * f;
}

double interpolated_noise_1(double x, double y) {
  int int_x = (int)x;
  double frac_x = x - int_x;

  int int_y = (int)y;
  double frac_y = y - int_y;

  double v1 = smoothed_noise_1(int_x, int_y);
  double v2 = smoothed_noise_1(int_x + 1, int_y);
  double v3 = smoothed_noise_1(int_x, int_y + 1);
  double v4 = smoothed_noise_1(int_x + 1, int_y + 1);

  double i1 = interpolate(v1, v2, frac_x);
  double i2 = interpolate(v3, v4, frac_x);

  return interpolate(i1, i2, frac_y);
}

double perlin_2d(double x, double y) {
  double total = 0;
  //todo
  double p = 1; // persistence
  int num_octaves = 100;
  double n = num_octaves - 1;
  double freq = 0, amp = 0;


  for(int i = 0; i < n; i++) {
    freq = 2 * i;
    amp = pow(p, i);

    total = total + interpolated_noise_1(x * freq, y * freq) * amp;
  }

  return total;
}

