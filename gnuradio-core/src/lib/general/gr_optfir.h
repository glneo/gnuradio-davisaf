/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
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

#ifndef _GR_OPTFIR_H_
#define _GR_OPTFIR_H_

#include <gr_core_api.h>
#include <vector>
#include <cmath>

/*!
 * \brief Routines for designing optimal FIR filters.
 *
 * For a great intro to how all this stuff works, see section 6.6 of
 * "Digital Signal Processing: A Practical Approach", Emmanuael C. Ifeachor
 * and Barrie W. Jervis, Adison-Wesley, 1993.  ISBN 0-201-54413-X.
 * \ingroup filter_design
 */

class GR_CORE_API gr_optfir 
{
public:
	/*!
	* \brief Builds a low pass filter.
	*
	* \p gain:	Filter gain in the passband (linear)
	* \p Fs:	Sampling rate (sps)
	* \p freq1:	End of pass band (in Hz)
	* \p freq2:	Start of stop band (in Hz)
	* \p passband_ripple_db:	Pass band ripple in dB (should be small, < 1)
	* \p stopband_atten_db:	Stop band attenuation in dB (should be large, >= 60)
	* \p nextra_taps:	Extra taps to use in the filter (default=2)
	*/
	static std::vector<float>
	low_pass (double gain, double Fs,
	            double freq1, double freq2,
	            double passband_ripple_db, double stopband_atten_db,
	            int nextra_taps = 2);

	/*!
	* \brief Builds a high pass filter.
	* \p gain:  Filter gain in the passband (linear)
	* \p Fs:    Sampling rate (sps)
	* \p freq1: End of stop band (in Hz)
	* \p freq2: Start of pass band (in Hz)
	* \p passband_ripple_db: Pass band ripple in dB (should be small, < 1)
	* \p stopband_atten_db:  Stop band attenuation in dB (should be large, >= 60)
	* \p nextra_taps:  Extra taps to use in the filter (default=2)
	*/
	static std::vector<float>
	high_pass (double gain, double Fs,
	            double freq1, double freq2,
	            double passband_ripple_db, double stopband_atten_db,
	            int nextra_taps = 2);

	/*!
	* \brief Builds a band pass filter.
	*
	* \p gain:  Filter gain in the passband (linear)
	* \p Fs:    Sampling rate (sps)
	* \p freq_sb1: End of stop band (in Hz)
	* \p freq_pb1: Start of pass band (in Hz)
	* \p freq_pb2: End of pass band (in Hz)
	* \p freq_sb2: Start of stop band (in Hz)
	* \p passband_ripple_db: Pass band ripple in dB (should be small, < 1)
	* \p stopband_atten_db:  Stop band attenuation in dB (should be large, >= 60)
	* \p nextra_taps:  Extra taps to use in the filter (default=2)
	*/
	static std::vector<float>
	band_pass(double gain, double Fs,
	            double freq_sb1, double freq_pb1, 
	            double freq_pb2, double freq_sb2,
	            double passband_ripple_db, double stopband_atten_db,
	            int nextra_taps = 2);

	/*!
	* \brief Builds a band reject filter
	* \p gain:  Filter gain in the passband (linear)
	* \p Fs:    Sampling rate (sps)
	* \p freq_pb1: End of pass band (in Hz)
	* \p freq_sb1: Start of stop band (in Hz)
	* \p freq_sb2: End of stop band (in Hz)
	* \p freq_pb2: Start of pass band (in Hz)
	* \p passband_ripple_db: Pass band ripple in dB (should be small, < 1)
	* \p stopband_atten_db:  Stop band attenuation in dB (should be large, >= 60)
	* \p nextra_taps:  Extra taps to use in the filter (default=2)
	*/
	static std::vector<float>
	band_reject (double gain, double Fs,
	                double freq_pb1, double freq_sb1,
	                double freq_sb2, double freq_pb2,
	                double passband_ripple_db, double stopband_atten_db,
	                int nextra_taps = 2);

private:
	static double lporder (double freq1, double freq2, double delta_p, double delta_s);
	static double stopband_atten_to_dev( double atten_db );
	static double passband_ripple_to_dev( double ripple_db );
};

#endif
