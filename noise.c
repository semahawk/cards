/*
 *
 * noise.c
 *
 * Created at:  14 Nov 2015 16:45:53 +0100 (CET)
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "noise.h"

static inline double linear_interpolation(double a, double b, double x)
{
  return a * (1 - x) + b * x;
}

static inline double cosine_interpolation(double a, double b, double x)
{
  double ft = x * M_PI;
  double f = (1 - cos(ft)) * 0.5;

  return a * (1 - f) + b * f;
}

/*
 * A function to 'smooth' the value between the two inputs a and b.
 * A standard interpolation function takes three inputs, a and b, the values
 * to be interpolated between, and x which takes a value between 0 and 1.
 * The Interpolation function returns a value between a and b based on the
 * value x. When x equals 0, it returns a, and when x is 1, it returns b.
 * When x is between 0 and 1, it returns some value between a and b.
 */
static inline double interpolate(double a, double b, double x)
{
  /*return cosine_interpolation(a, b, x);*/
  return linear_interpolation(a, b, x);
}

/*
 * Return a pseudo-random value between -1.0 and 1.0 based on the input values
 * Ie. calculate a hash for the given coordinates.
 */
static inline double hash(int x, int y)
{
  int n;

  n = x + y * seed;
  n = (n << 13) ^ n;

  return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)
      / 1073741824.0);
}

/*
 * Like `hash` but also averages the value with it's neighbours' hash values
 */
static double smooth_hash(double x, double y)
{
  double corners = (hash(x-1, y-1) + hash(x+1, y-1) + hash(x-1, y+1) + hash(x+1, y+1)) / 16;
  double sides   = (hash(x-1, y)   + hash(x+1, y)   + hash(x,   y-1) + hash(x,   y+1)) /  8;
  double center  =  hash(x, y) / 4;

  return corners + sides + center;
}

static double interpolated_noise(double x, double y)
{
  int integer_x = (int)x;
  double fractional_x = x - integer_x;

  int integer_y = (int)y;
  double fractional_y = y - integer_y;

  double v1 = smooth_hash(integer_x,     integer_y);
  double v2 = smooth_hash(integer_x + 1, integer_y);
  double v3 = smooth_hash(integer_x,     integer_y + 1);
  double v4 = smooth_hash(integer_x + 1, integer_y + 1);

  double i1 = interpolate(v1 , v2 , fractional_x);
  double i2 = interpolate(v3 , v4 , fractional_x);

  return interpolate(i1 , i2 , fractional_y);
}

/*
 * Return the noise value for the given point (x, y)
 */
double noise(double x, double y)
{
  double noise = 0;
  double p = PERSISTENCE;
  double n = OCTAVES_NUM - 1;
  int i;

  for (i = 0; i < n; i++){
    double frequency = pow(2, i);
    double amplitude = pow(p, i);

    noise = noise + interpolated_noise(x * frequency, y * frequency) * amplitude;
  }

  /* make it more foresty (that's a FIXME all right) */
  noise += 1.05;

  printf("height for point (%-3.1f, %-3.1f) is %-9.3f\n", x, y, noise);

  return noise;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

