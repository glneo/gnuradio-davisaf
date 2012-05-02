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

GR_SWIG_BLOCK_MAGIC(gr,demod_10k0a3e_cf)

gr_demod_10k0a3e_cf_sptr gr_make_demod_10k0a3e_cf(int channel_rate=250000,
					int audio_decim=5);

class gr_demod_10k0a3e_cf : public gr_hier_block2
{
private:
	gr_demod_10k0a3e_cf(
					int channel_rate,   /* incoming sample rate of the AM baseband */
					int audio_rate);   /* input to output decimation rate */
};