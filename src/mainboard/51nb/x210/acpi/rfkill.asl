/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2019 Google Inc.
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

Device (RDIO)
{
	Name (_HID, EisaId ("COR0001"))
	Name (_STA, 0x0F)

	// The set of devices managed by this rfkill interface.
	// Hardcoded to wifi and wwan in this case.
	Method (DEVS, 0, NotSerialized)
	{
		Return (0x11)
	}

	Method (SSTA)
	{
		Local0 = Zero
		If (LEqual (\_SB.PCI0.LPCB.EC.WIRE, One))
		{
			Local0 |= One
		}
		If (LEqual (\_SB.PCI0.LPCB.EC.WWAN, One))
		{
			Local0 |= 0x10
		}
		Return (Local0)
	}

	Method (HSTA)
	{
		Return (0xffffffffffffffff)
	}

	Method (CNTL, 1, NotSerialized)
	{
		If (And (Arg0, 0x01))
		{
			\_SB.PCI0.LPCB.EC.WIRE = One
		} Else {
			\_SB.PCI0.LPCB.EC.WIRE = Zero
		}
		If (And (Arg0, 0x10))
		{
			\_SB.PCI0.LPCB.EC.WWAN = One
		} Else {
			\_SB.PCI0.LPCB.EC.WWAN = Zero
		}
	}
}
