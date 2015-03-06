// Copyright (C) 2015 Chi-kwan Chan
// Copyright (C) 2015 Steward Observatory
//
// This file is part of insight.
//
// Insight is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Insight is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with insight.  If not, see <http://www.gnu.org/licenses/>.

#include "harm2raw.h"
#include "harm.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstdio>

#define R 111 // 64  sqrt(3)
#define N 222 // 128 sqrt(3)
#define X 2

static inline real i2c(real i, real n) { return (R) * (i - 0.5 * (n-1)) / n; }
static inline uint8_t clam(real x) { return x > 0 ? x > 255 ? 255 : x : 0; }

real max0 = 0, max1 = 0, max2 = 0, max3 = 0;

void process(int n, int m, int l, Const &c, uint8_t *d)
{
	int  h  = ((n * N + m) * N + l) * 4;
	real f0 = 0, f1 = 0, f2 = 0, f3 = 0;

	for(int i = X * n; i < X * (n+1); ++i)
		for(int j = X * m; j < X * (m+1); ++j)
			for(int k = X * l; k < X * (l+1); ++k) {
				real x = i2c(i, X*N);
				real y = i2c(j, X*N);
				real z = i2c(k, X*N);
				C    s = harm::Car2sph(x, y, z);

				if(s.r > 1 + sqrt(1 - c.a_spin * c.a_spin)
				           + 1e-3) {
					F f = harm::get(c, s);
					f0 += 1e4 * f.tgas;
					f1 += 1e4 * f.ur;
					f2 += 1e3 * f.uphi * s.r;
					f3 += 5e1 * f.ibeta;
				}
			}

	f0 /= X * X * X; if(max0 < f0) max0 = f0; d[h+0] = clam(f0);
	f1 /= X * X * X; if(max1 < f1) max1 = f1; d[h+1] = clam(f1);
	f2 /= X * X * X; if(max2 < f2) max2 = f2; d[h+2] = clam(f2);
	f3 /= X * X * X; if(max3 < f3) max3 = f3; d[h+3] = clam(f3);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
		error("usage: harm2raw input output\n");

	Const c = {.threshold=5, .Ti_Te_d=3, .Ti_Te_f=3};
	if(!harm::load(c, argv[1]))
		error("Make sure that the input file is valid\n");

	FILE *file = fopen(argv[2], "wb");
	if(!file)
		error("Failed to create output file \"%s\"\n", argv[2]);

	uint8_t *data = (uint8_t *)malloc(N * N * N * 4);
	if(!data)
		error("Failed to allocate memory\n");

	for(int i = 0; i < N; ++i)
		for(int j = 0; j < N; ++j)
			for(int k = 0; k < N; ++k)
				process(i, j, k, c, data);
	print("maxima: %g %g %g %g\n", max0, max1, max2, max3);
	int n = N;
	fwrite(&n, sizeof(int), 1, file);
	fwrite(data, 1, N * N * N * 4, file);
	free(data);
	fclose(file);

	return 0;
}
