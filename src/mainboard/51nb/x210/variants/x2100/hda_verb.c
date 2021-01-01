/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <device/azalia_device.h>

const u32 cim_verb_data[] = {
	/* coreboot specific header */
	0x14f15069,	/* Codec Vendor / Device ID: Conexant CX20585 */
	0x14f10101,	/* Subsystem ID */
	11,		/* Number of jacks (NID entries) */
	/* NID 0x01, HDA Codec Subsystem ID */
	AZALIA_SUBVENDOR(0, 0x14f10101),

	/* Pin Widget Verb Table */
	AZALIA_PIN_CFG(0, 0x19, 0x0421401f),
	AZALIA_PIN_CFG(0, 0x1a, 0x04a1902e),
	AZALIA_PIN_CFG(0, 0x1b, 0x21a190f0),
	AZALIA_PIN_CFG(0, 0x1c, 0x610140f0),
	AZALIA_PIN_CFG(0, 0x1d, 0x040150f0),
	AZALIA_PIN_CFG(0, 0x1e, 0x90a70120),
	AZALIA_PIN_CFG(0, 0x1f, 0x90170010),
	AZALIA_PIN_CFG(0, 0x20, 0x044571f0),
	AZALIA_PIN_CFG(0, 0x22, 0x2144e1f0),
	AZALIA_PIN_CFG(0, 0x23, 0x90a700f0),

	/* Intel GPU HDMI */
        0x8086280b, /* Vendor ID */
        0x80860101, /* Subsystem ID */
        2, /* Number of entries */
        AZALIA_SUBVENDOR(2, 0x80860101),
        AZALIA_PIN_CFG(2, 0x03, 0x18560010),
};

const u32 pc_beep_verbs[] = {
};
AZALIA_ARRAY_SIZES;
