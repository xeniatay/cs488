// Xenia Tay
// 20396769
// xzytay

#include "perlinnoise.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// from: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
// noise 1
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

// noise 2
double noise_2(double x, double y) {
  int n = x + y * 57;
  n = ( n << 13 ) ^ n;
  return ( 1.0 - ( (n * (n * n * 15733 + 789227) + 1376312627) & 0x7fffffff) / 1073741824.0);
}

double smoothed_noise_2(double x, double y) {
  double corners = ( noise_2(x - 1, y - 1) + noise_2(x + 1, y - 1) + noise_2(x + 1, y + 1) ) / 16;
  double sides = ( noise_2(x - 1, y) + noise_2( x + 1, y) + noise_2(x, y - 1) + noise_2(x, y + 1) ) / 8;
  double center = noise_2(x, y) / 4;

  return corners + sides + center;
}

// noise 3
double noise_3(double x, double y) {
  int n = x + y * 57;
  n = ( n << 13 ) ^ n;
  return ( 1.0 - ( (n * (n * n * 15737 + 789251) + 1376312629) & 0x7fffffff) / 1073741824.0);
}

double smoothed_noise_3(double x, double y) {
  double corners = ( noise_3(x - 1, y - 1) + noise_3(x + 1, y - 1) + noise_3(x + 1, y + 1) ) / 16;
  double sides = ( noise_3(x - 1, y) + noise_3( x + 1, y) + noise_3(x, y - 1) + noise_3(x, y + 1) ) / 8;
  double center = noise_3(x, y) / 4;

  return corners + sides + center;
}

// noise 4
double noise_4(double x, double y) {
  int n = x + y * 57;
  n = ( n << 13 ) ^ n;
  return ( 1.0 - ( (n * (n * n * 15739 + 789311) + 1376312657) & 0x7fffffff) / 1073741824.0);
}

double smoothed_noise_4(double x, double y) {
  double corners = ( noise_4(x - 1, y - 1) + noise_4(x + 1, y - 1) + noise_4(x + 1, y + 1) ) / 16;
  double sides = ( noise_4(x - 1, y) + noise_4( x + 1, y) + noise_4(x, y - 1) + noise_4(x, y + 1) ) / 8;
  double center = noise_4(x, y) / 4;

  return corners + sides + center;
}

// noise 5
double noise_5(double x, double y) {
  int n = x + y * 57;
  n = ( n << 13 ) ^ n;
  return ( 1.0 - ( (n * (n * n * 15761 + 789323) + 1376312689) & 0x7fffffff) / 1073741824.0);
}

double smoothed_noise_5(double x, double y) {
  double corners = ( noise_5(x - 1, y - 1) + noise_5(x + 1, y - 1) + noise_5(x + 1, y + 1) ) / 16;
  double sides = ( noise_5(x - 1, y) + noise_5( x + 1, y) + noise_5(x, y - 1) + noise_5(x, y + 1) ) / 8;
  double center = noise_5(x, y) / 4;

  return corners + sides + center;
}

// interpolate things
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

double interpolated_noise_2(double x, double y) {
  int int_x = (int)x;
  double frac_x = x - int_x;

  int int_y = (int)y;
  double frac_y = y - int_y;

  double v1 = smoothed_noise_2(int_x, int_y);
  double v2 = smoothed_noise_2(int_x + 1, int_y);
  double v3 = smoothed_noise_2(int_x, int_y + 1);
  double v4 = smoothed_noise_2(int_x + 1, int_y + 1);

  double i1 = interpolate(v1, v2, frac_x);
  double i2 = interpolate(v3, v4, frac_x);

  return interpolate(i1, i2, frac_y);
}

double interpolated_noise_3(double x, double y) {
  int int_x = (int)x;
  double frac_x = x - int_x;

  int int_y = (int)y;
  double frac_y = y - int_y;

  double v1 = smoothed_noise_3(int_x, int_y);
  double v2 = smoothed_noise_3(int_x + 1, int_y);
  double v3 = smoothed_noise_3(int_x, int_y + 1);
  double v4 = smoothed_noise_3(int_x + 1, int_y + 1);

  double i1 = interpolate(v1, v2, frac_x);
  double i2 = interpolate(v3, v4, frac_x);

  return interpolate(i1, i2, frac_y);
}


double interpolated_noise_4(double x, double y) {
  int int_x = (int)x;
  double frac_x = x - int_x;

  int int_y = (int)y;
  double frac_y = y - int_y;

  double v1 = smoothed_noise_4(int_x, int_y);
  double v2 = smoothed_noise_4(int_x + 1, int_y);
  double v3 = smoothed_noise_4(int_x, int_y + 1);
  double v4 = smoothed_noise_4(int_x + 1, int_y + 1);

  double i1 = interpolate(v1, v2, frac_x);
  double i2 = interpolate(v3, v4, frac_x);

  return interpolate(i1, i2, frac_y);
}


double interpolated_noise_5(double x, double y) {
  int int_x = (int)x;
  double frac_x = x - int_x;

  int int_y = (int)y;
  double frac_y = y - int_y;

  double v1 = smoothed_noise_5(int_x, int_y);
  double v2 = smoothed_noise_5(int_x + 1, int_y);
  double v3 = smoothed_noise_5(int_x, int_y + 1);
  double v4 = smoothed_noise_5(int_x + 1, int_y + 1);

  double i1 = interpolate(v1, v2, frac_x);
  double i2 = interpolate(v3, v4, frac_x);

  return interpolate(i1, i2, frac_y);
}

// cubic interpolation
double interpolate(double a, double b, double x) {
  double ft = x * M_PI;
  double f = (1 - cos(ft) * 0.5);
  return a * (1 - f) + b * f;
}

// actual perlin
double perlin2d(double x, double y) {
  int num_octaves = 1, j = 0;
  double n = 10, amp = 0, total = 0, freq = 0;
  double freq1 = 2, p1 = 0.25; // persistence
  double freq2 = 8, p2 = 0.25; // persistence
  double freq3 = 3, p3 = 0.25; // persistence
  double freq4 = 4, p4 = 0.5; // persistence
  double freq5 = 2, p5 = 0.030; // persistence

  for(int i = 1; i <= n; i++) {
    j = i % num_octaves;

    switch (j) {
      case 1:
        freq = pow(freq1, i);
        amp = pow(p1, i);
        total = total + interpolated_noise_1(x * freq, y * freq) * amp;
        break;
      case 2:
        freq = pow(freq2, i);
        amp = pow(p2, i);
        total = total + interpolated_noise_2(x * freq, y * freq) * amp;
        break;
      case 3:
        freq = pow(freq3, i);
        amp = pow(p3, i);
        total = total + interpolated_noise_3(x * freq, y * freq) * amp;
        break;
      case 4:
        freq = pow(freq4, i);
        amp = pow(p4, i);
        total = total + interpolated_noise_4(x * freq, y * freq) * amp;
        break;
      case 5:
        freq = pow(freq5, i);
        amp = pow(p5, i);
        total = total + interpolated_noise_5(x * freq, y * freq) * amp;
        break;
      default:
        break;
    }
  }

  //cerr << total << " " ;
  return total;
}


/// 8pn (x, y, z)
// + 4pn(3x, 3y, 3z);