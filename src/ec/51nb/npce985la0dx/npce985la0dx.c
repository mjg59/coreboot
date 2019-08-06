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

#include <device/pnp.h>

/*
 * This embedded controller looks awfully like a SuperIO chip. LDNs 5 and 6
 * need to be enabled to turn on the keyboard and mouse controller, and LDN
 * 0x11 needs to be enabled to turn on ACPI embedded controller
 * functionality.
 */
static struct pnp_info dev_infos[] = {
	{ NULL, 0x05 }, { NULL, 0x06 }, { NULL, 0x11 }
};

static void ec_51nb_npce985la0dx_ops_enable(struct device *dev)
{
	pnp_enable_devices(dev, &pnp_ops, ARRAY_SIZE(dev_infos), dev_infos);
}

struct chip_operations ec_51nb_npce985la0dx_ops = {
	CHIP_NAME("51NB EC")
	.enable_dev = ec_51nb_npce985la0dx_ops_enable,
};
