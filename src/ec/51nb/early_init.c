/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2018 Google
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <arch/io.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ops.h>
#include <soc/romstage.h>

#include "ec.h"

void ec_51nb_enable(void)
{
	/* Route 0x4e/4f to LPC */
	pci_write_config16(PCI_DEV(0, 0x1f, 0), 0x82, 0x3c03);

	/* Enable function 5 */
	outb(LDN_SEL, SETUP_COMMAND);
	outb(0x05, SETUP_DATA);
	outb(LDN_ENABLE, SETUP_COMMAND);
	outb(0x01, SETUP_DATA);

	/* Enable function 6 */
	outb(LDN_SEL, SETUP_COMMAND);
	outb(0x06, SETUP_DATA);
	outb(LDN_ENABLE, SETUP_COMMAND);
	outb(0x01, SETUP_DATA);

	/* Enable funcion 17 */
	outb(LDN_SEL, SETUP_COMMAND);
	outb(0x11, SETUP_DATA);
	outb(LDN_ENABLE, SETUP_COMMAND);
	outb(0x01, SETUP_DATA);
}
