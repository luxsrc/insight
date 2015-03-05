// Copyright (C) 2012--2014 Chi-kwan Chan
// Copyright (C) 2012--2014 Steward Observatory
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

#include "harm2raw.h"
#include "harm.h"
#include <cmath>

#define EPSILON 1e-32
#define R_SCHW  2

#define CONST_c     (2.99792458e+10)
#define CONST_h     (6.62606957e-27)
#define CONST_G     (6.67384800e-08)
#define CONST_kB    (1.38064881e-16)
#define CONST_Ry    (2.17987197e-11)
#define CONST_e     (4.80320425e-10)
#define CONST_me    (9.10938291e-28)
#define CONST_mp_me (1836.152672450)
#define CONST_mSun  (1.98910000e+33)

F harm::get(Const &c, C s)
{
  // Get indices to access HARM data
  real fg, Fg, fG, FG, fgh, Fgh, fGh, FGh, fgH, FgH, fGH, FGH;
  int  ij, Ij, iJ, IJ, ijk, Ijk, iJk, IJk, ijK, IjK, iJK, IJK;
  {
    real F = (real)0.5;
    int  I = c.n_r - N_RS - 1, i = I; // assume c.n_r > N_RS + 1
    if(c.r[i] > s.r) {
      do I = i--; while(i && c.r[i] > s.r); // assume s.r >= c.r[0]
      F = (s.r - c.r[i]) / (c.r[I] - c.r[i]);
    } // else, constant extrapolate

    real G = (real)0.5;
    int  J = c.n_theta-1, j = J;
    if(s.theta < c.coord[i].theta * (1-F) +
                 c.coord[I].theta *    F)
      j = J = 0; // constant extrapolation
    else {
      real theta = c.coord[j*c.n_r + i].theta * (1-F)  +
                   c.coord[j*c.n_r + I].theta *    F;
      if(theta > s.theta) {
	real Theta;
        do {
          J = j--;
          Theta = theta;
          theta = c.coord[j*c.n_r + i].theta * (1-F) +
                  c.coord[j*c.n_r + I].theta *    F;
        } while(j && theta > s.theta);
        G = (s.theta - theta) / (Theta - theta);
      } // else, constant extrapolation
    }

    real H = s.phi / (real)(2*M_PI);
    H -= floor(H);
    H *= c.n_phi;
    int k = H, K = k == c.n_phi-1 ? 0 : k+1;
    H -= k;

    fg = (1-F) * (1-G);
    Fg =    F  * (1-G);
    fG = (1-F) *    G ;
    FG =    F  *    G ;

    ij = j * c.n_r + i;
    Ij = j * c.n_r + I;
    iJ = J * c.n_r + i;
    IJ = J * c.n_r + I;

    fgh = (1-F) * (1-G) * (1-H);
    Fgh =    F  * (1-G) * (1-H);
    fGh = (1-F) *    G  * (1-H);
    FGh =    F  *    G  * (1-H);
    fgH = (1-F) * (1-G) *    H ;
    FgH =    F  * (1-G) *    H ;
    fGH = (1-F) *    G  *    H ;
    FGH =    F  *    G  *    H ;

    ijk = (k * c.n_theta + j) * c.n_r + i;
    Ijk = (k * c.n_theta + j) * c.n_r + I;
    iJk = (k * c.n_theta + J) * c.n_r + i;
    IJk = (k * c.n_theta + J) * c.n_r + I;
    ijK = (K * c.n_theta + j) * c.n_r + i;
    IjK = (K * c.n_theta + j) * c.n_r + I;
    iJK = (K * c.n_theta + J) * c.n_r + i;
    IJK = (K * c.n_theta + J) * c.n_r + I;
  } // 58+ FLOP

  // Construct the four vectors u^\mu and b^\mu in modified KS coordinates
  real ut, ur, utheta, uphi, u; // u is energy
  real bt, br, btheta, bphi, ibeta, ti_te;
  {
    const real gKSP00 = (fg*c.coord[ij].gcov[0][0]+Fg*c.coord[Ij].gcov[0][0]+
                         fG*c.coord[iJ].gcov[0][0]+FG*c.coord[IJ].gcov[0][0]);
    const real gKSP01 = (fg*c.coord[ij].gcov[0][1]+Fg*c.coord[Ij].gcov[0][1]+
                         fG*c.coord[iJ].gcov[0][1]+FG*c.coord[IJ].gcov[0][1]);
    const real gKSP02 = (fg*c.coord[ij].gcov[0][2]+Fg*c.coord[Ij].gcov[0][2]+
                         fG*c.coord[iJ].gcov[0][2]+FG*c.coord[IJ].gcov[0][2]);
    const real gKSP03 = (fg*c.coord[ij].gcov[0][3]+Fg*c.coord[Ij].gcov[0][3]+
                         fG*c.coord[iJ].gcov[0][3]+FG*c.coord[IJ].gcov[0][3]);
    const real gKSP11 = (fg*c.coord[ij].gcov[1][1]+Fg*c.coord[Ij].gcov[1][1]+
                         fG*c.coord[iJ].gcov[1][1]+FG*c.coord[IJ].gcov[1][1]);
    const real gKSP12 = (fg*c.coord[ij].gcov[1][2]+Fg*c.coord[Ij].gcov[1][2]+
                         fG*c.coord[iJ].gcov[1][2]+FG*c.coord[IJ].gcov[1][2]);
    const real gKSP13 = (fg*c.coord[ij].gcov[1][3]+Fg*c.coord[Ij].gcov[1][3]+
                         fG*c.coord[iJ].gcov[1][3]+FG*c.coord[IJ].gcov[1][3]);
    const real gKSP22 = (fg*c.coord[ij].gcov[2][2]+Fg*c.coord[Ij].gcov[2][2]+
                         fG*c.coord[iJ].gcov[2][2]+FG*c.coord[IJ].gcov[2][2]);
    const real gKSP23 = (fg*c.coord[ij].gcov[2][3]+Fg*c.coord[Ij].gcov[2][3]+
                         fG*c.coord[iJ].gcov[2][3]+FG*c.coord[IJ].gcov[2][3]);
    const real gKSP33 = (fg*c.coord[ij].gcov[3][3]+Fg*c.coord[Ij].gcov[3][3]+
                         fG*c.coord[iJ].gcov[3][3]+FG*c.coord[IJ].gcov[3][3]);

    ur     = (fgh * c.field[ijk].v1 + Fgh * c.field[Ijk].v1 +
              fGh * c.field[iJk].v1 + FGh * c.field[IJk].v1 +
              fgH * c.field[ijK].v1 + FgH * c.field[IjK].v1 +
              fGH * c.field[iJK].v1 + FGH * c.field[IJK].v1);
    utheta = (fgh * c.field[ijk].v2 + Fgh * c.field[Ijk].v2 +
              fGh * c.field[iJk].v2 + FGh * c.field[IJk].v2 +
              fgH * c.field[ijK].v2 + FgH * c.field[IjK].v2 +
              fGH * c.field[iJK].v2 + FGH * c.field[IJK].v2);
    uphi   = (fgh * c.field[ijk].v3 + Fgh * c.field[Ijk].v3 +
              fGh * c.field[iJk].v3 + FGh * c.field[IJk].v3 +
              fgH * c.field[ijK].v3 + FgH * c.field[IjK].v3 +
              fGH * c.field[iJK].v3 + FGH * c.field[IJK].v3);
    u      = (fgh * c.field[ijk].u  + Fgh * c.field[Ijk].u  +
              fGh * c.field[iJk].u  + FGh * c.field[IJk].u  +
              fgH * c.field[ijK].u  + FgH * c.field[IjK].u  +
              fGH * c.field[iJK].u  + FGH * c.field[IJK].u );
    br     = (fgh * c.field[ijk].B1 + Fgh * c.field[Ijk].B1 +
              fGh * c.field[iJk].B1 + FGh * c.field[IJk].B1 +
              fgH * c.field[ijK].B1 + FgH * c.field[IjK].B1 +
              fGH * c.field[iJK].B1 + FGH * c.field[IJK].B1);
    btheta = (fgh * c.field[ijk].B2 + Fgh * c.field[Ijk].B2 +
              fGh * c.field[iJk].B2 + FGh * c.field[IJk].B2 +
              fgH * c.field[ijK].B2 + FgH * c.field[IjK].B2 +
              fGH * c.field[iJK].B2 + FGH * c.field[IJK].B2);
    bphi   = (fgh * c.field[ijk].B3 + Fgh * c.field[Ijk].B3 +
              fGh * c.field[iJk].B3 + FGh * c.field[IJk].B3 +
              fgH * c.field[ijK].B3 + FgH * c.field[IjK].B3 +
              fGH * c.field[iJK].B3 + FGH * c.field[IJK].B3);

    // Vector u
    ut     = 1 / sqrt((real)EPSILON
                      -(gKSP00                   +
                        gKSP11 * ur     * ur     +
                        gKSP22 * utheta * utheta +
                        gKSP33 * uphi   * uphi   +
                        2 * (gKSP01 * ur              +
                             gKSP02 * utheta          +
                             gKSP03 * uphi            +
                             gKSP12 * ur     * utheta +
                             gKSP13 * ur     * uphi   +
                             gKSP23 * utheta * uphi)));
    ur     *= ut;
    utheta *= ut;
    uphi   *= ut;

    // Vector B
    bt     = (br     * (gKSP01 * ut     + gKSP11 * ur    +
                        gKSP12 * utheta + gKSP13 * uphi) +
              btheta * (gKSP02 * ut     + gKSP12 * ur    +
                        gKSP22 * utheta + gKSP23 * uphi) +
              bphi   * (gKSP03 * ut     + gKSP13 * ur    +
                        gKSP23 * uphi   + gKSP33 * uphi));
    br     = (br     + bt * ur    ) / ut;
    btheta = (btheta + bt * utheta) / ut;
    bphi   = (bphi   + bt * uphi  ) / ut;

    const real bb = (bt     * (gKSP00 * bt     + gKSP01 * br    +
                               gKSP02 * btheta + gKSP03 * bphi) +
                     br     * (gKSP01 * bt     + gKSP11 * br    +
                               gKSP12 * btheta + gKSP13 * bphi) +
                     btheta * (gKSP02 * bt     + gKSP12 * br    +
                               gKSP22 * btheta + gKSP23 * bphi) +
                     bphi   * (gKSP03 * bt     + gKSP13 * br    +
                               gKSP23 * btheta + gKSP33 * bphi));
    ibeta = bb / (2 * (c.Gamma-1) * u + (real)EPSILON);
    ti_te = (ibeta > c.threshold) ? c.Ti_Te_f : (1 + c.Ti_Te_d * R_SCHW / s.r);
  }

  // Construct the scalars rho and tgas
  real rho, tgas, te;
  {
    const real Gamma = (1 + (ti_te+1) / (ti_te+2) / (real)1.5 + c.Gamma) / 2;

    rho  = (fgh * c.field[ijk].rho + Fgh * c.field[Ijk].rho +
            fGh * c.field[iJk].rho + FGh * c.field[IJk].rho +
            fgH * c.field[ijK].rho + FgH * c.field[IjK].rho +
            fGH * c.field[iJK].rho + FGH * c.field[IJK].rho);
    tgas = (Gamma - 1) * u / (rho + (real)EPSILON);
    te   = ti_te < 0 ? -ti_te : tgas * (real)CONST_mp_me / (ti_te+1);
  }

  // Transform vector u and b from KSP to KS coordinates
  {
    const real dxdxp00=(fg*c.coord[ij].dxdxp[0][0]+Fg*c.coord[Ij].dxdxp[0][0]+
                        fG*c.coord[iJ].dxdxp[0][0]+FG*c.coord[IJ].dxdxp[0][0]);
    const real dxdxp11=(fg*c.coord[ij].dxdxp[1][1]+Fg*c.coord[Ij].dxdxp[1][1]+
                        fG*c.coord[iJ].dxdxp[1][1]+FG*c.coord[IJ].dxdxp[1][1]);
    const real dxdxp12=(fg*c.coord[ij].dxdxp[1][2]+Fg*c.coord[Ij].dxdxp[1][2]+
                        fG*c.coord[iJ].dxdxp[1][2]+FG*c.coord[IJ].dxdxp[1][2]);
    const real dxdxp21=(fg*c.coord[ij].dxdxp[2][1]+Fg*c.coord[Ij].dxdxp[2][1]+
                        fG*c.coord[iJ].dxdxp[2][1]+FG*c.coord[IJ].dxdxp[2][1]);
    const real dxdxp22=(fg*c.coord[ij].dxdxp[2][2]+Fg*c.coord[Ij].dxdxp[2][2]+
                        fG*c.coord[iJ].dxdxp[2][2]+FG*c.coord[IJ].dxdxp[2][2]);
    const real dxdxp33=(fg*c.coord[ij].dxdxp[3][3]+Fg*c.coord[Ij].dxdxp[3][3]+
                        fG*c.coord[iJ].dxdxp[3][3]+FG*c.coord[IJ].dxdxp[3][3]);
    real temp1, temp2;

    temp1  = ur;
    temp2  = utheta;
    ut    *= dxdxp00;
    ur     = (dxdxp11 * temp1 + dxdxp12 * temp2);
    utheta = (dxdxp21 * temp1 + dxdxp22 * temp2);
    uphi  *= dxdxp33;

    temp1  = br;
    temp2  = btheta;
    bt    *= dxdxp00;
    br     = (dxdxp11 * temp1 + dxdxp12 * temp2);
    btheta = (dxdxp21 * temp1 + dxdxp22 * temp2);
    bphi  *= dxdxp33;
  }

  // Transform vector u and b from KS to BL coordinates
  {
    const real Dlt   = s.r * s.r + c.a_spin * c.a_spin - R_SCHW * s.r;
    const real temp0 = -R_SCHW * s.r / Dlt; // Note that s.r and Dlt are
    const real temp3 = -c.a_spin     / Dlt; // evaluated at photon position

    ut   += ur * temp0;
    uphi += ur * temp3;

    bt   += br * temp0;
    bphi += br * temp3;
  }

  return (F){rho, ur, utheta, uphi, br, btheta, bphi, tgas, te, ibeta};
}
