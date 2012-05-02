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

#include <gr_blks2_am_demod.h>
#include <gr_io_signature.h>
#include <gr_optfir.h>

// Shared pointer constructor
gr_blks2_am_demod_sptr
gr_make_blks2_am_demod(int channel_rate, int audio_decim,
				double audio_pass, double audio_stop)
{
	return gnuradio::get_initial_sptr(new gr_blks2_am_demod(channel_rate,
							audio_decim,
							audio_pass,
							audio_stop));
}

// Hierarchical block constructor
gr_blks2_am_demod::gr_blks2_am_demod(
					int channel_rate,   // incoming sample rate of the AM baseband
					int audio_decim,   // input to output decimation rate
					double audio_pass, // audio low pass filter passband frequency
					double audio_stop)   // audio low pass filter stop frequency
	: gr_hier_block2("gr_blks2_am_demod",
		gr_make_io_signature(1, 1, sizeof(gr_complex)), 
		gr_make_io_signature(1, 1, sizeof(float)))
{
	d_mag = gr_make_complex_to_mag();
	d_dcr = gr_make_add_const_ff(-1.0);

	d_audio_taps = gr_optfir::low_pass(0.5,   // Filter gain
					channel_rate,   // Sample rate
					audio_pass,   // Audio passband
					audio_stop,   // Audio stopband
					0.1,   // Passband ripple
					60);   // Stopband attenuation
	d_lpf = gr_make_fir_filter_fff(audio_decim, d_audio_taps);
	
	connect(self(), 0, d_mag, 0);
	connect(d_mag, 0, d_dcr, 0);
	connect(d_dcr, 0, d_lpf, 0);
	connect(d_lpf, 0, self(), 0);
}
