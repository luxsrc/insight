// Copyright (C) 2013,2014 Chi-kwan Chan
// Copyright (C) 2013,2014 Steward Observatory
//
// This file is part of GRay.
//
// GRay is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRay is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRay.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HARM_H
#define HARM_H

#include <stddef.h>

typedef struct {
  // size_t i, j, k;
  // double x1, x2, x3;
  // double v1, v2, v3;
  // double gcon[4][4];
  // double gcov[4][4];
  // double gdet;
  // double ck[4];
  // double dxdxp[4][4];
  float theta;
  float gcov [4][4];
  float dxdxp[4][4];
} Coord;

typedef struct {
  float rho;
  float u;
  float jet1; // = -u_t
  float jet2; // = -T^r_t / (rho u^r)
  float ucont;
  float v1, v2, v3;
  float B1, B2, B3;
} Field;

typedef struct {
  Coord *coord;
  Field *field;
  size_t n_r, n_theta, n_phi;
  real   Gamma;
  real   a_spin;
  real   r[N_R];
  real   threshold;
  real   Ti_Te_f, Ti_Te_d;
} Const;

typedef struct {
  real r, theta, phi;
} C;

typedef struct {
  real rho;
  real ur, utheta, uphi;
  real br, btheta, bphi;
  real tgas, te, ibeta;
} F;

namespace harm {
  extern Coord *load_coord(Const &, const char *);
  extern Field *load_field(Const &, const char *);
  extern bool   load      (Const &, const char *);
  extern F      get       (Const &, C);
  extern C      Car2sph   (real, real, real);
}

#endif // HARM_H
