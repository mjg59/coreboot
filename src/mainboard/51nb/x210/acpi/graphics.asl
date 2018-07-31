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

Scope (\_SB.PCI0.GFX0)
{
        Device (LCD)
        {
                Method (_ADR, 0, Serialized)
                {
                        Return (0x1F)
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
