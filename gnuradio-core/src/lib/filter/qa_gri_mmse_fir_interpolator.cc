/* -*- c++ -*- */
/*
 * Copyright 2002 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cppunit/TestAssert.h>
#include <qa_gri_mmse_fir_interpolator.h>
#include <gri_mmse_fir_interpolator.h>
#include <stdio.h>
#include <cmath>

#define	NELEM(x) (sizeof (x) / sizeof (x[0]))


static float
test_fcn (double index)
{
  return (2 * sin (index * 0.25 * 2 * M_PI + 0.125 * M_PI)
	  + 3 * sin (index * 0.077 * 2 * M_PI + 0.3 * M_PI));
}


void
qa_gri_mmse_fir_interpolator::t1 ()
{
  static const unsigned	N = 100;
  float input[N + 10];

  for (unsigned i = 0; i < NELEM(input); i++)
    input[i] = test_fcn ((double) i);

  gri_mmse_fir_interpolator	intr;
  float inv_nsteps = 1.0 / intr.nsteps ();

  for (unsigned i = 0; i < N; i++){
    for (unsigned imu = 0; imu <= intr.nsteps (); imu += 1){
      float expected = test_fcn ((i + 3) + imu * inv_nsteps);
      float actual = intr.interpolate (&input[i], imu * inv_nsteps);

      CPPUNIT_ASSERT_DOUBLES_EQUAL (expected, actual, 0.004);
      // printf ("%9.6f  %9.6f  %9.6f\n", expected, actual, expected - actual);
    }
  }
}

