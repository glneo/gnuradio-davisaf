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

GR_SWIG_BLOCK_MAGIC(gr,blks2_am_demod)

gr_blks2_am_demod_sptr gr_make_blks2_am_demod(int channel_rate=250000,
						int audio_decim=5,
						double audio_pass=5000,
						double audio_stop=5500);

class gr_blks2_am_demod : public gr_hier_block2
{
private:
	gr_blks2_am_demod(
		int channel_rate,   /* incoming sample rate of the AM baseband */
		int audio_decim,   /* input to output decimation rate */
		double audio_pass, /* audio low pass filter passband frequency */
		double audio_stop); /* audio low pass filter stop frequency */
};
