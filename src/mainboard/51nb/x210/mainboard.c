/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <soc/ramstage.h>

static void mainboard_enable(struct device *dev)
{
	/* Route 0x4e/4f to LPC */
	lpc_enable_fixed_io_ranges(LPC_IOE_EC_4E_4F);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
