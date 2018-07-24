/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef HDA_VERB_H
#define HDA_VERB_H

#include <device/azalia_device.h>

const u32 cim_verb_data[] = {
	/* coreboot specific header */
	0x14f15069,	/* Codec Vendor / Device ID: Conexant CX20585 */
	0x17aa2155,	/* Subsystem ID */
	0x0000000c,	/* Number of jacks (NID entries) */

	0x0017ff00,	/* Function Reset */
	0x0017ff00,	/* Double Function Reset */
	0x0017ff00,
	0x0017ff00,

	/* Bits 31:28 - Codec Address */
	/* Bits 27:20 - NID */
	/* Bits 19:8 - Verb ID */
	/* Bits 7:0  - Payload */

	/* NID 0x01, HDA Codec Subsystem ID Verb Table: 0x17aa2155 */
	AZALIA_SUBVENDOR(0x0, 0x17aa2155),

	/* Pin Widget Verb Table */

	/* Pin Complex (NID 0x19) */
	AZALIA_PIN_CFG(0x0, 0x19, 0x042140f0),

	/* Pin Complex (NID 0x1A) */
	AZALIA_PIN_CFG(0x0, 0x1a, 0x61a190f0),

	/* Pin Complex (NID 0x1B) */
	AZALIA_PIN_CFG(0x0, 0x1b, 0x04a190f0),

	/* Pin Complex (NID 0x1C) */
	AZALIA_PIN_CFG(0x0, 0x1c, 0x612140f0),

	/* Pin Complex (NID 0x1D) */
	AZALIA_PIN_CFG(0x0, 0x1d, 0x601700f0),

	/* Pin Complex (NID 0x1E) */
	AZALIA_PIN_CFG(0x0, 0x1e, 0x40f001f0),

	/* Pin Complex (NID 0x1F) */
	AZALIA_PIN_CFG(0x0, 0x1f, 0x901701f0),

	/* Pin Complex (NID 0x20) */
	AZALIA_PIN_CFG(0x0, 0x1B, 0x40f001f0),

	/* Pin Complex (NID 0x22) */
	AZALIA_PIN_CFG(0x0, 0x22, 0x40f001f0),

	/* Pin Complex (NID 0x23) */
	AZALIA_PIN_CFG(0x0, 0x23, 0x90a601f0),
};

const u32 pc_beep_verbs[] = {
};
AZALIA_ARRAY_SIZES;
#endif
