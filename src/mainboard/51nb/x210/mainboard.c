/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2019 Google LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <soc/ramstage.h>
#include <intelblocks/pcr.h>
#include <soc/pcr_ids.h>
#include "gpio.h"

void mainboard_silicon_init_params(FSP_SIL_UPD *params)
{
	/* Configure pads prior to SiliconInit() in case there's any
	 * dependencies during hardware initialization. */
	gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));
}

static void mainboard_enable(struct device *dev)
{
	/* Route 0x4e/4f to LPC */
	lpc_enable_fixed_io_ranges(LPC_IOE_EC_4E_4F);

	/* Enable Apple SMC emulation */

	pcr_write32(PID_PSTH, 0x1e90, 0x001c0301); // Decode 0x300-0x31f
	pcr_write32(PID_PSTH, 0x1e94, 0x000200f0); // R/W access, any width

	/*
	 * New Intel platforms have two documented sets of I/O trap registers,
	 * one in PSTH space, one in DMI space. The former is where the status
	 * registers are, so naively you'd think they'd be the ones that
	 * matter - but in reality, nothing works unless you enable the ones
	 * in DMI space around the other LPC decode registers.
	 */

	/*
	 * TODO: are these sufficient, or do the PSTH registers need to be
	 * set as well?
	 */
	pcr_write32(PID_DMI, 0x2760, 0x001c0301);
	pcr_write32(PID_DMI, 0x2764, 0x000200f0);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
