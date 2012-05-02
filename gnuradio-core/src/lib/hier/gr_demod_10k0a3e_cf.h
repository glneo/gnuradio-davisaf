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

#pragma once

#include <gr_core_api.h>
#include <gr_top_block.h>
#include <gr_blks2_am_demod.h>

class gr_demod_10k0a3e_cf;
typedef boost::shared_ptr<gr_demod_10k0a3e_cf> gr_demod_10k0a3e_cf_sptr;

GR_CORE_API gr_demod_10k0a3e_cf_sptr gr_make_demod_10k0a3e_cf(int channel_rate=250000,
					int audio_decim=5);

/*!
 * \brief 10K0A3E Emission Standard Demodulator
 *    
 * This block demodulates an AM channel conformant to 10K0A3E emission
 * standards, such as broadcast band AM transmissions.
 * \ingroup misc_blk
 */
class GR_CORE_API gr_demod_10k0a3e_cf : public gr_hier_block2
{
private:
	gr_demod_10k0a3e_cf(
					int channel_rate,   /* incoming sample rate of the AM baseband */
					int audio_decim);   /* input to output decimation rate */

	friend GR_CORE_API gr_demod_10k0a3e_cf_sptr gr_make_demod_10k0a3e_cf(int channel_rate,
					int audio_decim);

	gr_blks2_am_demod_sptr d_am_demod;
};
