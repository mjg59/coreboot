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

#include <arch/io.h>
#include <device/device.h>

#include "chip.h"

static void ec_51nb_npce985la0dx_ops_enable(struct device *dev)
{
	const struct ec_51nb_npce985la0dx_config *config = dev->chip_info;
	int setup_command = config->setup_command;
	int setup_data = config->setup_data;

	/* Enable function 5 (PS/2 AUX) */
	outb(LDN_SEL, setup_command);
	outb(0x05, setup_data);
	outb(LDN_ENABLE, setup_command);
	outb(0x01, setup_data);

	/* Enable function 6 (PS/2 KB) */
	outb(LDN_SEL, setup_command);
	outb(0x06, setup_data);
	outb(LDN_ENABLE, setup_command);
	outb(0x01, setup_data);

	/* Enable function 17 (EC) */
	outb(LDN_SEL, setup_command);
	outb(0x11, setup_data);
	outb(LDN_ENABLE, setup_command);
	outb(0x01, setup_data);
}

struct chip_operations ec_51nb_npce985la0dx_ops = {
	CHIP_NAME("51NB EC")
	.enable_dev = ec_51nb_npce985la0dx_ops_enable,
};
