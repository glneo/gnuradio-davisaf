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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gr_optfir.h"
#include "gr_remez.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <string>

using std::vector;
using std::max;
using std::string;

/* Convert a stopband attenuation in dB to an absolute value */
double gr_optfir::stopband_atten_to_dev( double atten_db )
{
    return pow( 10, ( -atten_db / 20 ) );
}

/* Convert passband ripple spec expressed in dB to an absolute value */
double gr_optfir::passband_ripple_to_dev( double ripple_db )
{
    return (pow( 10, ( ripple_db / 20 ) ) - 1) / (pow( 10, ( ripple_db / 20 ) ) + 1);
}

/*
	FIR lowpass filter length estimator.  freq1 and freq2 are
    normalized to the sampling frequency.  delta_p is the passband
    deviation (ripple), delta_s is the stopband deviation (ripple).

    Note, this works for high pass filters too (freq1 > freq2), but
    doesnt work well if the transition is near f == 0 or f == fs/2

    From Herrmann et al (1973), Practical design rules for optimum
    finite impulse response filters.  Bell System Technical J., 52, 769-99
*/
double gr_optfir::lporder (double freq1, double freq2, double delta_p, double delta_s)
{
	string sti;
	std::stringstream mystrm;

    double df = fabs (freq2 - freq1);
    double ddp = log10 (delta_p);
    double dds = log10 (delta_s);

    double a1 = 5.309e-3;
    double a2 = 7.114e-2;
    double a3 = -4.761e-1;
    double a4 = -2.66e-3;
    double a5 = -5.941e-1;
    double a6 = -4.278e-1;

    double b1 = 11.01217;
    double b2 = 0.5124401;

    double t1 = a1 * ddp * ddp;
    double t2 = a2 * ddp;
    double t3 = a4 * ddp * ddp;
    double t4 = a5 * ddp;

    double dinf = ( ( t1 + t2 + a3 ) * dds ) + ( t3 + t4 + a6 );
    double ff = b1 + b2 * ( ddp - dds );

    return (dinf / df - ff * df + 1);
}

/*
	FIR bandpass filter length estimator.  freq1 and freq2 are
    normalized to the sampling frequency.  delta_p is the passband
    deviation (ripple), delta_s is the stopband deviation (ripple).

    From Mintzer and Liu (1979)
*/
//static double bporder (double freq1, double freq2, double delta_p, double delta_s)
//{
//    double df = abs (freq2 - freq1);
//    double ddp = log10 (delta_p);
//    double dds = log10 (delta_s);
//
//    double a1 = 0.01201;
//    double a2 = 0.09664;
//    double a3 = -0.51325;
//    double a4 = 0.00203;
//    double a5 = -0.57054;
//    double a6 = -0.44314;
//
//    double t1 = a1 * ddp * ddp;
//    double t2 = a2 * ddp;
//    double t3 = a4 * ddp * ddp;
//    double t4 = a5 * ddp;
//
//    double cinf = dds * (t1 + t2 + a3) + t3 + t4 + a6;
//    double ginf = -14.6 * log10 (delta_p / delta_s) - 16.9;
//    return (cinf / df + ginf * df + 1);
//}

/*  Builds a low pass filter. */
vector<float>
gr_optfir::low_pass(double gain,	/* Filter gain in the passband (linear) */
                    double Fs,			/*Sampling rate (sps)  */
                    double freq1,	/* End of pass band (in Hz) */
                    double freq2,	/* Start of stop band (in Hz) */
                    double passband_ripple_db,	/* Pass band ripple in dB (should be small, < 1) */
                    double stopband_atten_db,	/* Stop band attenuation in dB (should be large, >= 60) */
                    int nextra_taps)	/* Extra taps to use in the filter (default=2) */
{
	vector<double> frequencies; /* build the frequencys vector */
	frequencies.push_back(0);
	frequencies.push_back((freq1/Fs)*2); /* Normalize to sampling frequency */
	frequencies.push_back((freq2/Fs)*2); /* and double to nyquist = 1 */
	frequencies.push_back(1);

	vector<double> desired_ampls; 
	desired_ampls.push_back(gain);
	desired_ampls.push_back(gain);
	desired_ampls.push_back(0);
	desired_ampls.push_back(0);
	
	vector<double> deviations;
	deviations.push_back(passband_ripple_to_dev (passband_ripple_db)/gain); /* remez compatable devs */
	deviations.push_back(stopband_atten_to_dev (stopband_atten_db));

	double l = lporder( (freq1/Fs), (freq2/Fs), deviations[0], deviations[1] ); /* estimate taps */
	int number_of_taps = (int)ceil(l) - 1; /* need order, not length for remez */
	
	vector<double>::const_iterator max_dev = max_element( deviations.begin(), deviations.end() );
	vector<double> wts;
	for( unsigned int i = 0; i < deviations.size(); i++ )
		wts.push_back( (*max_dev / deviations[i]) );

	/* Add 2 taps by default */
	vector<double> double_taps = gr_remez (number_of_taps + nextra_taps, frequencies, desired_ampls, wts, string("bandpass"));

	/* I hate losing the precision after calculating in double, but gnuradio only has float tap fir filters */
	vector<float> float_taps ( double_taps.begin(), double_taps.end() );
	return float_taps;
}

/*  Builds a high pass filter. */
vector<float>
gr_optfir::high_pass(double gain,	/* Filter gain in the passband (linear) */
                    double Fs,	/* Sampling rate (sps) */
                    double freq1,	/* End of stop band (in Hz) */
                    double freq2,	/* Start of pass band (in Hz) */
                    double passband_ripple_db,	/* Pass band ripple in dB (should be small, < 1) */
                    double stopband_atten_db,	/* Stop band attenuation in dB (should be large, >= 60) */
                    int nextra_taps)	/* Extra taps to use in the filter (default=2) */
{
	vector<double> frequencies; /* build the frequencys vector */
	frequencies.push_back(0);
	frequencies.push_back((freq1/Fs)*2); /* Normalize to sampling frequency */
	frequencies.push_back((freq2/Fs)*2); /* and double to nyquist = 1 */
	frequencies.push_back(1);

	vector<double> desired_ampls; 
	desired_ampls.push_back(0);
	desired_ampls.push_back(0);
	desired_ampls.push_back(gain);
	desired_ampls.push_back(gain);
	
	vector<double> deviations;
	deviations.push_back(stopband_atten_to_dev (stopband_atten_db)); /* remez compatable devs */
	deviations.push_back(passband_ripple_to_dev (passband_ripple_db)/gain);

	double l = lporder( (freq1/Fs), (freq2/Fs), deviations[0], deviations[1] ); /* estimate taps */
	int number_of_taps = (int)ceil(l) - 1; /* need order, not length for remez */

	/* For a HPF, we need to use an odd number of taps
	In gr_remez, ntaps = n+1, so n must be even */
	if( (number_of_taps + nextra_taps) % 2 )
		number_of_taps++;
	
	vector<double>::const_iterator max_dev = max_element( deviations.begin(), deviations.end() );
	vector<double> wts;
	for( unsigned int i = 0; i < deviations.size(); i++ )
		wts.push_back( (*max_dev / deviations[i]) );

	/* Add 2 taps by default */
	vector<double> double_taps = gr_remez (number_of_taps + nextra_taps, frequencies, desired_ampls, wts, string("bandpass"));

	/* I hate losing the precision after calculating in double, but gnuradio only has float tap fir filters */
	vector<float> float_taps ( double_taps.begin(), double_taps.end() );
	return float_taps;
}

/*  Builds a band pass filter. */
vector<float>
gr_optfir::band_pass(double gain,	/* Filter gain in the passband (linear) */
                    double Fs,	/* Sampling rate (sps) */
                    double freq_sb1,	/* End of stop band (in Hz) */
                    double freq_pb1,	/* Start of pass band (in Hz) */
                    double freq_pb2,	/* End of pass band (in Hz) */
                    double freq_sb2,	/* Start of stop band (in Hz) */
                    double passband_ripple_db,	/* Pass band ripple in dB (should be small, < 1) */
                    double stopband_atten_db,	/* Stop band attenuation in dB (should be large, >= 60) */
                    int nextra_taps)	/* Extra taps to use in the filter (default=2) */
{
	vector<double> frequencies; /* build the frequencys vector */
	frequencies.push_back(0);
	frequencies.push_back((freq_sb1/Fs)*2); /* Normalize to sampling frequency */
	frequencies.push_back((freq_pb1/Fs)*2); /* and double to nyquist = 1 */
	frequencies.push_back((freq_pb2/Fs)*2);
	frequencies.push_back((freq_sb2/Fs)*2);
	frequencies.push_back(1);

	vector<double> desired_ampls;
	desired_ampls.push_back(0);
	desired_ampls.push_back(0);
	desired_ampls.push_back(gain);
	desired_ampls.push_back(gain);
	desired_ampls.push_back(0);
	desired_ampls.push_back(0);

	vector<double> deviations;
	deviations.push_back(stopband_atten_to_dev (stopband_atten_db)); /* remez compatable devs */
	deviations.push_back(passband_ripple_to_dev (passband_ripple_db)/gain);
	deviations.push_back(stopband_atten_to_dev (stopband_atten_db));

	double l = max(lporder( (freq_pb2/Fs), (freq_sb2/Fs), deviations[1], deviations[2] ), 
		lporder( (freq_sb1/Fs), (freq_pb1/Fs), deviations[1], deviations[0] )); /* find side with most taps needed */
	int number_of_taps = (int)ceil(l) - 1; /* need order, not length for remez */

	vector<double>::const_iterator max_dev = max_element( deviations.begin(), deviations.end() );
	vector<double> wts;
	for( unsigned int i = 0; i < deviations.size(); i++ )
		wts.push_back( (*max_dev / deviations[i]) );

	/* Add 2 taps by default */
	vector<double> double_taps = gr_remez (number_of_taps + nextra_taps, frequencies, desired_ampls, wts, string("bandpass"));

	/* I hate losing the precision after calculating in double, but gnuradio only has float tap fir filters */
	vector<float> float_taps ( double_taps.begin(), double_taps.end() );
	return float_taps;
}

/*  Builds a band reject filter */
vector<float>
gr_optfir::band_reject(double gain,	/* Filter gain in the passband (linear) */
                        double Fs,	/* Sampling rate (sps) */
                        double freq_pb1,	/* End of pass band (in Hz) */
                        double freq_sb1,	/* Start of stop band (in Hz) */
                        double freq_sb2,	/* End of stop band (in Hz) */
                        double freq_pb2,	/* Start of pass band (in Hz) */
                        double passband_ripple_db,	/* Pass band ripple in dB (should be small, < 1) */
                        double stopband_atten_db,	/* Stop band attenuation in dB (should be large, >= 60) */
                        int nextra_taps)	/* Extra taps to use in the filter (default=2) */
{
	vector<double> frequencies; /* build the frequencys vector */
	frequencies.push_back(0);
	frequencies.push_back((freq_pb1/Fs)*2); /* Normalize to sampling frequency */
	frequencies.push_back((freq_sb1/Fs)*2); /* and double to nyquist = 1 */
	frequencies.push_back((freq_sb2/Fs)*2);
	frequencies.push_back((freq_pb2/Fs)*2);
	frequencies.push_back(1);

	vector<double> desired_ampls;
	desired_ampls.push_back(gain);
	desired_ampls.push_back(gain);
	desired_ampls.push_back(0);
	desired_ampls.push_back(0);
	desired_ampls.push_back(gain);
	desired_ampls.push_back(gain);

	vector<double> deviations;
	deviations.push_back(passband_ripple_to_dev (passband_ripple_db)/gain); /* remez compatable devs */
	deviations.push_back(stopband_atten_to_dev (stopband_atten_db));
	deviations.push_back(passband_ripple_to_dev (passband_ripple_db)/gain);

	double l = max(lporder( (freq_pb2/Fs), (freq_sb2/Fs), deviations[1], deviations[2] ), 
		lporder( (freq_sb1/Fs), (freq_pb1/Fs), deviations[1], deviations[0] )); /* find side with most taps needed */
	int number_of_taps = (int)ceil(l) - 1; /* need order, not length for remez */

	/* For a BRF, we need to use an odd number of taps
	In gr_remez, ntaps = n+1, so n must be even */
	if( (number_of_taps + nextra_taps) % 2 )
		number_of_taps++;

	vector<double>::const_iterator max_dev = max_element( deviations.begin(), deviations.end() );
	vector<double> wts;
	for( unsigned int i = 0; i < deviations.size(); i++ )
		wts.push_back( (*max_dev / deviations[i]) );

	/* Add 2 taps by default */
	vector<double> double_taps = gr_remez (number_of_taps + nextra_taps, frequencies, desired_ampls, wts, string("bandpass"));

	/* I hate losing the precision after calculating in double, but gnuradio only has float tap fir filters */
	vector<float> float_taps ( double_taps.begin(), double_taps.end() );
	return float_taps;
}
