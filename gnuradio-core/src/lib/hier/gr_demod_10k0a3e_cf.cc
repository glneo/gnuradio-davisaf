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

#include <gr_demod_10k0a3e_cf.h>
#include <gr_io_signature.h>

// Shared pointer constructor
gr_demod_10k0a3e_cf_sptr
gr_make_demod_10k0a3e_cf(int channel_rate,
					int audio_decim)
{
	return gnuradio::get_initial_sptr(new gr_demod_10k0a3e_cf(channel_rate,
							audio_decim));
}

// Hierarchical block constructor
gr_demod_10k0a3e_cf::gr_demod_10k0a3e_cf(
					int channel_rate,   // incoming sample rate of the AM baseband
					int audio_decim)   // input to output decimation rate
	: gr_hier_block2("gr_blks2_am_demod",
		gr_make_io_signature(1, 1, sizeof(gr_complex)), 
		gr_make_io_signature(1, 1, sizeof(float)))
{
	d_am_demod = gr_make_blks2_am_demod( channel_rate, audio_decim, 5000, 5500 );

	connect(self(), 0, d_am_demod, 0);
	connect(d_am_demod, 0, self(), 0);
}
