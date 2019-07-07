/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2018 Google Inc.
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

Device (IGPU)
{
	Name (_ADR, 0x00020000)
	Method (DTGP, 5, NotSerialized)
	{
		If (LEqual (Arg0, Buffer (0x10)
		{
                 /* 0000 */    0xC6, 0xB7, 0xB5, 0xA0, 0x18, 0x13, 0x1C, 0x44, 
                 /* 0008 */    0xB0, 0xC9, 0xFE, 0x69, 0x5E, 0xAF, 0x94, 0x9B
                }))
		{
			If (LEqual (Arg1, One))
			{
				If (LEqual (Arg2, Zero))
				{
					Store (Buffer (One)
					{
					  0x03
					}, Arg4)

					Return (One)
				}

				If (LEqual (Arg2, One))
				{
					Return (One)
				}
			}
		}

		Store (Buffer (One)
		{
		  0x00
		}, Arg4)

		Return (Zero)
	}

	Method (_DSM, 4, NotSerialized)
	{
		Store (Package (0x04)
		{
			"device-id", 
			Buffer (0x04)
			{
				0x16, 0x59, 0x00, 0x00

			},
/*			"AAPL,ig-platform-id",
			Buffer (0x04)
			{
				0x00, 0x00, 0x16, 0x19
			}, */
			"hda-gfx",
			Buffer (0x0a)
			{
				"onboard-1"
			}
		}, Local0)
		DTGP (Arg0, Arg1, Arg2, Arg3, RefOf (Local0))
		Return (Local0)
	}
	Method (_DOS, 1, NotSerialized)
	{
		/* We never do anything in firmware, so _DOS is a noop */
	}
	Method (_DOD, 0, NotSerialized)
	{
		return (Package (0x03)
		{
			0x80000410, /* LCD */
			0x80000120, /* VGA */
			0x80000330 /* DP */
		})
	}
	Device (LCD)
        {
                Method (_ADR, 0, Serialized)
                {
                        Return (0x800000410)
                }

                Method (_BCL, 0, NotSerialized)
                {
                        Return (Package (0x12)
                        {
                                0x0A,
                                0x0F,
                                0x00,
                                0x01,
                                0x02,
                                0x03,
                                0x04,
                                0x05,
                                0x06,
                                0x07,
                                0x08,
                                0x09,
                                0x0A,
                                0x0B,
                                0x0C,
                                0x0D,
                                0x0E,
                                0x0F
                        })
                }
                Method (_BCM, 1, NotSerialized)
                {
                        \_SB.PCI0.LPCB.EC.BKLG = Arg0
                }
                Method (_BQC, 0, NotSerialized)
                {
                        Return (\_SB.PCI0.LPCB.EC.BKLG)
                }
        }
}
